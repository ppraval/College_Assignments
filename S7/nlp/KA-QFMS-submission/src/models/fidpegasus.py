"""
FiD-Pegasus: Fusion-in-Decoder Pegasus for Multi-Document Summarization
Updated for latest Transformers API
Based on FiD approach from Salesforce
"""
import torch
import torch.nn.functional as F
from transformers import PegasusForConditionalGeneration, PegasusConfig
from transformers.modeling_outputs import BaseModelOutput
from torch import nn


class PegasusForMultiConditionalGeneration(PegasusForConditionalGeneration):
    """
    Pegasus model extended to handle multiple input documents using Fusion-in-Decoder approach
    """
    
    def __init__(self, config, args):
        super().__init__(config)
        self.args = args
        
        # Knowledge-aware components
        if self.args.knowledge_aware == 'multihead':
            self.multi_head_attn = nn.MultiheadAttention(
                embed_dim=config.d_model,
                num_heads=config.encoder_attention_heads,
                batch_first=True,
                dropout=config.attention_dropout
            )
        elif self.args.knowledge_aware == 'concat':
            # Reserved for concat mode - can be extended
            self.resweight = nn.Parameter(torch.Tensor([0]))

    def multi_encode(
        self,
        input_ids=None,
        attention_mask=None,
        return_dict=None
    ):
        """
        Encode multiple documents and fuse them
        
        Args:
            input_ids: Can be either:
                - Tensor of shape (B*N, L) for basic mode
                - List containing [src_ids, knowledge_inputs, query_inputs, doc_inputs]
            attention_mask: Attention mask tensor
            return_dict: Whether to return a dict or tuple
            
        Returns:
            Tuple of (encoder_outputs, attention_mask)
        """
        original_input_ids = input_ids
        
        # Handle knowledge-aware mode
        if self.args.knowledge_aware != '' and isinstance(input_ids, list):
            input_ids = original_input_ids[1]['input_ids']
            attention_mask = original_input_ids[1]['attention_mask']
        elif isinstance(input_ids, list):
            input_ids = original_input_ids[0]
        
        # Get dimensions
        B = 1  # Batch size (assumed to be 1 for multi-doc)
        N = input_ids.size(0)  # Number of documents
        L = input_ids.size(1)  # Sequence length
        
        # Encode all documents
        encoder_outputs = self.model.encoder(
            input_ids=input_ids,
            attention_mask=attention_mask,
            return_dict=True
        )
        
        hidden_states = encoder_outputs.last_hidden_state  # (B*N, L, D)
        D = hidden_states.size(2)
        
        # Stack representations from all documents
        stacked_source_reps = hidden_states.contiguous().view(B, N * L, D)  # [B, N*L, D]
        stacked_source_mask = attention_mask.contiguous().view(B, N * L)
        
        # Knowledge-aware attention
        if self.args.knowledge_aware == 'multihead' and isinstance(original_input_ids, list):
            if original_input_ids[2] is not None:
                # Encode query representations
                query_outputs = self.model.encoder(
                    **original_input_ids[2],
                    return_dict=True
                )
                knowledge_outputs = query_outputs.last_hidden_state  # [B*N, L, D]
                
                # Apply multi-head attention
                attn_output, _ = self.multi_head_attn(
                    stacked_source_reps,
                    knowledge_outputs,
                    knowledge_outputs
                )
                stacked_source_reps = stacked_source_reps + attn_output
        
        # Return encoder outputs
        if return_dict:
            encoder_outputs = BaseModelOutput(last_hidden_state=stacked_source_reps)
        else:
            encoder_outputs = (stacked_source_reps,)
        
        return encoder_outputs, stacked_source_mask

    @torch.no_grad()
    def generate(
        self,
        input_ids=None,
        attention_mask=None,
        **kwargs,
    ):
        """
        Generate summaries from multiple input documents
        
        Args:
            input_ids: Input document IDs
            attention_mask: Attention mask
            **kwargs: Additional generation arguments
            
        Returns:
            Generated token IDs
        """
        # Encode and fuse documents
        encoder_outputs, attention_mask = self.multi_encode(
            input_ids=input_ids,
            attention_mask=attention_mask,
            return_dict=True
        )
        
        # Generate using parent class method
        return super().generate(
            input_ids=None,
            attention_mask=attention_mask,
            encoder_outputs=encoder_outputs,
            **kwargs
        )

    def forward(
        self,
        input_ids=None,
        attention_mask=None,
        decoder_input_ids=None,
        decoder_attention_mask=None,
        head_mask=None,
        decoder_head_mask=None,
        cross_attn_head_mask=None,
        encoder_outputs=None,
        past_key_values=None,
        inputs_embeds=None,
        decoder_inputs_embeds=None,
        labels=None,
        use_cache=None,
        output_attentions=None,
        output_hidden_states=None,
        return_dict=None
    ):
        """
        Forward pass through the model
        
        Args:
            Standard Pegasus forward arguments
            
        Returns:
            Model outputs including loss, logits, etc.
        """
        # Encode documents if input_ids provided
        if input_ids is not None:
            encoder_outputs, attention_mask = self.multi_encode(
                input_ids=input_ids,
                attention_mask=attention_mask,
                return_dict=True
            )
        elif encoder_outputs is None:
            raise ValueError("Either input_ids or encoder_outputs must be provided")
        
        # Call parent forward with encoded representations
        output = super().forward(
            input_ids=None,
            attention_mask=attention_mask,
            decoder_input_ids=decoder_input_ids,
            decoder_attention_mask=decoder_attention_mask,
            head_mask=head_mask,
            decoder_head_mask=decoder_head_mask,
            cross_attn_head_mask=cross_attn_head_mask,
            encoder_outputs=encoder_outputs,
            past_key_values=past_key_values,
            inputs_embeds=inputs_embeds,
            decoder_inputs_embeds=decoder_inputs_embeds,
            labels=labels,
            use_cache=use_cache,
            output_attentions=output_attentions,
            output_hidden_states=output_hidden_states,
            return_dict=return_dict
        )
        
        return output
