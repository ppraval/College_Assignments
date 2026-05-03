# PPT vs Report — Gap Analysis

> **Documents compared**
> - **Report**: `final_report/ERGLM-Report/main.tex` (58-reference IEEE-style report, ~700 lines)
> - **PPT**: `final_ppt/main.tex` + `references.bib` (Beamer slides, Madrid theme, natbib author-year)

---

## 1. Project Title & Authors

### Report says
- Title: *Medical Diagnosis and Report Generation for Endoscopic Procedures Using Vision-Language Models*
- Authors: Praval Pattam (B220057CS), Theenesh Potluri (B221121CS), Pranav Sai Sarvepalli (B220055CS)
- Affiliation: Department of CSE, **National Institute of Technology Calicut**, Calicut, Kerala, India – 673 601
- Guides: Jayaraj PB, Srinivasa TM
- Date: May 13, 2026
- HoD: Dr. Subashini R

### PPT says
- Title: identical
- Authors: Praval Pattam, Theenesh Potluri, Pranav Sai Sarvepalli
- Affiliation listed as "**National Institute of Technology, Calicut**" (extra comma)
- Date field: "B.Tech CSE Semester 8 Final Evaluation" (no calendar date)
- No guide names on title slide

### Discrepancies / What needs syncing
- **Minor**: PPT affiliation has an extra comma ("NIT, Calicut" vs "NIT Calicut"). Fix to match report.
- **Missing**: Guide names (Jayaraj PB, Srinivasa TM) do not appear anywhere in the PPT. Add to title or acknowledgements slide.
- **Missing**: Actual submission date (May 13, 2026) is absent from the PPT.

---

## 2. Abstract / Overview Slide

### Report
Has a full formal abstract (≈120 words): Background (MLLM limitations in endoscopy), Methods (MVLMERG, two-stage P-LoRA), Results (CIDEr 0.2667, ROUGE-L 0.4598, METEOR 0.4070, BLEU-4 0.3863, BERTScore 0.8216).

### PPT
**No abstract slide exists.** The second slide is an `\tableofcontents` Agenda slide. The project is never introduced in summary form before diving into Methodology.

### Missing points
- No project motivation or problem statement at the start
- Key results (five specific metric scores) never appear in a dedicated summary slide
- The acronym **MVLMERG** is **never used anywhere in the PPT** — the model has no named identity in the presentation

---

## 3. Introduction / Motivation

### Topics in report intro not covered in PPT
- AI advancement in multimodal large language models and cross-modal understanding
- Specific bottleneck: general MLLMs lack specialised endoscopy terminology and pathological reasoning
- Prior work landscape: LoRA, Prefix Tuning, adapter-based fine-tuning
- Problem of limited annotated endoscopic data (PubMed too general; high-quality endoscopic annotations scarce)
- Specific limitations of VisualGLM-6B and DeepSeek-VL-7B in medical applications
- Formal list of two numbered contributions of the paper
- Chapter roadmap (Chapters 2–6 description)

### Topics in PPT intro not in report
- **PPT has no introduction section at all.** The first content section is "Methodology".

### Slides needed / slides to remove
- **Add to PPT**: An "Introduction & Motivation" slide covering: (1) time burden of manual endoscopy report writing, (2) MLLM capabilities and domain gap, (3) project objective and named system MVLMERG, (4) two contributions bullet points.

---

## 4. Literature Survey / Related Work

### Papers/models mentioned in report but completely missing from PPT
| Ref | Model / Paper | Report section |
|-----|--------------|----------------|
| [17] Wang et al. | **CogVLM** | Sec 2.1 MLLMs |
| [18] Bai et al. | **Qwen-VL series** | Sec 2.1 MLLMs |
| [19] Liu et al. | **LLaVA** (original) | Sec 2.1 MLLMs |
| [21] Chen et al. | **HuatuoGPT-Vision** (PubMedVision) | Sec 2.2 Medical MLLMs |
| [22] Seenivasan et al. | **SurgicalGPT** (3–5% accuracy gain) | Sec 2.2 Medical MLLMs |
| [23] Ji et al. | **ColonINST + ColonGPT** | Sec 2.2 Medical MLLMs |
| [24] Schmidgall et al. | **GP-VLS** (general surgical VLM) | Sec 2.2 Medical MLLMs |
| [8] Li et al. | **LLaVA-Med** (PMC-15M, GPT-4 fine-tuning) | Sec 2.3 Fine-tuning |
| [27] He et al. | **PeFoMed** (parameter-efficient MLLM fine-tuning) | Sec 2.3 Fine-tuning |
| [28] Chen et al. | **MISS** (generative pre-training for Med-VQA) | Sec 2.3 Fine-tuning |
| [29] Lu et al. | Two-stage fine-tuning for radiology reports | Sec 2.3 Fine-tuning |
| [30] Wu et al. | **PathInsight** (PathEnhanceDS, 45,000 cases) | Sec 2.3 Fine-tuning |
| [26] Gema et al. | Parameter-efficient fine-tuning of LLaMA for clinical domain | Sec 2.3 Fine-tuning |

### Papers/models in PPT not in report
- **EndoVis 2017/2018** and **Endo700k (700,000+ images)** are cited in the PPT as Stage 1 training datasets — these do **not appear anywhere** in the report (see §6.2 below for major discrepancy)
- PPT cites **pmc2024endovit (EndoViT)**, **zenodo2024endovis**, **arxiv2024endovision** — none are in the report's reference list

### What needs to be added/removed
- **Add to PPT**: A "Related Work" or "Background" slide with 3–4 key models (e.g., LLaVA-Med, HuatuoGPT-Vision, SurgicalGPT) and their limitations, motivating MVLMERG.
- **Reconcile**: Either add EndoVis/Endo700k to the report, or remove them from the PPT and replace with PubMed/MedTrinity-25M/VQA-Med.

---

## 5. Problem Definition

### PPT
**No dedicated problem definition slide exists.** The PPT's Implementation section includes a slide titled "The 'Blackbox' Problem & Our Solution" which addresses a narrow sub-problem (opacity of reasoning), not the overall problem statement of the project.

### Report
Chapter 3 ("Problem Definition") explicitly states: writing endoscopic image reports requires domain expertise; MLLMs struggle with specialised terminology, pathological features, and complex medical logic; existing fine-tuning methods suffer from limited adaptability and insufficient domain-specific knowledge. Goal: develop MVLMERG.

### Gaps
- **Add to PPT**: A "Problem Definition" slide with the 3-sentence problem statement from Chapter 3 of the report.

---

## 6. Methodology

### 6.1 Overall Framework / Architecture

| | Report | PPT |
|---|--------|-----|
| **Content** | Fig-1 shows two-stage P-LoRA framework with frozen CLIP-ViT-L/14 + Vicuna-7B-v1.5; stage 1 and stage 2 both freeze visual encoder and connector, fine-tune LLM via P-LoRA | "System Architecture" slide (system_architecture.png); "Two-Stage Training Framework" slide; plus separate slides for LLaVA, CLIP-ViT, and LLaMA architectures |
| **Missing from PPT** | Report's Fig-1 (exact P-LoRA two-stage architecture diagram) never appears | PPT's system_architecture.png is not Fig-1 and its exact contents are unknown |
| **Missing from Report** | Separate architectural breakdown into CLIP-ViT, LLaMA, and LLaVA sub-diagrams | — |

**CRITICAL DISCREPANCY — Stage 1 trainable components:**
- **Report** (Sec 4.1.2): Stage 1 keeps visual encoder AND connector frozen; fine-tunes only the LLM via P-LoRA.
- **PPT** ("Two-Stage Training Framework"): Stage 1 trains **"Only multimodal projection layer"**; Stage 2 unfreezes projection layer + LLM.
These descriptions are **mutually contradictory**. One must be corrected.

### 6.2 Two-Stage Fine-Tuning

| | Report | PPT |
|---|--------|-----|
| **Stage 1 purpose** | Domain knowledge injection — gastroenterology concepts, lesion localisation, anatomy | Medical domain pre-training — establish medical concept alignment |
| **Stage 1 datasets** | **PubMed, MedTrinity-25M (25M image-text pairs), VQA-Med** → filtered to ~60K gastrointestinal records | **EndoVis 2017/2018 + Endo700k (700,000+ images)** |
| **Stage 2 purpose** | Generate templated language matching professional physician style (GMC Calicut data) | Instruction tuning for diagnosis and report generation |
| **Stage 2 dataset** | GMC Calicut physician-annotated data (6.5K base, ~31K after augmentation) | Custom GMC-derived Q&A pairs (32,545 samples) |

**MAJOR DISCREPANCY**: Stage 1 datasets are **completely different** between report and PPT. The report cites publicly available text/image datasets (PubMed, MedTrinity-25M, VQA-Med); the PPT cites surgical-video endoscopy challenge datasets (EndoVis, Endo700k). This is the most serious factual inconsistency.

### 6.3 P-LoRA Method

| | Report | PPT |
|---|--------|-----|
| **Coverage** | Full section (Sec 4.1.3) with Fig-2, Equations 1–2, mathematical derivation of ΔW=AB low-rank decomposition, prompt token insertion, selective LoRA on first/middle/last layers | **No P-LoRA slide exists.** LoRA is only mentioned in passing as "Parameter-Efficient Fine-Tuning (LoRA)" on the Two-Stage Training Framework slide |
| **Missing from PPT** | P-LoRA definition, selective layer strategy, Equations 1–2, Fig-2 | — |
| **Missing from Report** | — | — |

**Add to PPT**: A dedicated "P-LoRA Method" slide covering: (1) soft prompt tokens prepended to attention input, (2) low-rank ΔW=AB on feedforward layers of first/middle/last layers, (3) why selective application — lower layers capture general features, middle layers integrate modalities, upper layers are task-specific.

### 6.4 Dataset / Data Collection

| | Report | PPT |
|---|--------|-----|
| **GMC data** | 6,500 physician-annotated endoscopic reports; images at 1920×1080, resized to 336×336; age 20–80 (85% aged 40–80); ~1:1 gender ratio; urban+rural; 40% normal / 60% abnormal; diseases: gastric polyps, bleeding, ulcers, early/advanced gastric cancer | Dataset Creation slide: "6,500 patient reports (semi-structured PDFs + endoscopic images), 4 images per report" |
| **Q&A pairs** | Final dataset: 32,545 unique samples; split: 30,917 training / 1,627 validation | Same numbers stated |
| **Missing from PPT** | Patient demographics (age, gender, region), image resolution pipeline (1920×1080 → 336×336), disease type breakdown (polyps/bleeding/ulcers/cancer), 40/60 normal/abnormal split rationale, expert majority-voting for quality labelling | — |
| **Missing from Report** | De-identification diagram (DPDP Act compliance, automated redaction of Patient ID/Name/Age-Gender/Visit Date/UGI-OGD numbers) shown in PPT slide | — |

### 6.5 Data Augmentation

| | Report | PPT |
|---|--------|-----|
| **Rotation** | 0°/90°/180°/270°, 4× expansion, 6,500 → 26,000 reports, 24.96M tokens | Same — well covered |
| **Stochastic translation** | Bernoulli p=0.2, truncated Gaussian (σ=5%W, clamped ±15%W), 72.25% overlap guarantee, adds ~5,200 reports, 4.99M tokens | Same math in separate slide |
| **Combined token count** | ~29.95M tokens total | Not stated |
| **Training set after augmentation** | ~31,200 image-report pairs, 29,640 train / 1,560 validation after 5% split | "32,545 samples" stated but the train/val split from augmentation is not shown |
| **Missing from PPT** | Combined token count (29.95M), full augmented training/validation split numbers | — |
| **Missing from Report** | Actual augmentation example images (aug_original.jpg, aug_rotated_90.jpg, aug_translated.png) | — |

### 6.6 Chain-of-Thought Integration

Both documents cover CoT well. Both use the identical "Grade II Esophageal Varices" example (Observation 1/2/3 → Conclusion). No significant gap. 

**Minor gap**: The report (Sec 4.1.6) has three sub-sections: "The Interpretability Problem", "CoT Implementation", and "Impact on Clinical Utility". The PPT covers this over two slides ("The Blackbox Problem" and "Chain-of-Thought in Endoscopy") — coverage is adequate.

---

## 7. Experimental Setup

### Datasets (public + private)

| Item | Report | PPT |
|------|--------|-----|
| Stage 1 public datasets | PubMed, MedTrinity-25M, VQA-Med → 60K records (Table 5.1) | EndoVis 2017/2018 + Endo700k → no record count given (**MISMATCH**) |
| Stage 2 private | GMC Calicut, 6,500 base / ~31K augmented | GMC Q&A pairs, 32,545 samples |
| Data distribution table | Table 5.1 (`tab:data-distribution`) | **Not shown** |

### System specs / hardware

| Item | Report | PPT |
|------|--------|-----|
| CPU | Intel Xeon Silver 4310 @ 2.10 GHz, 24 cores, 48 threads, 2 sockets | **Not stated** |
| RAM | 125 GiB | **Not stated** |
| GPU | NVIDIA A100 80GB PCIe, CUDA 12.9 | **Not stated** |
| OS / Python | Linux, Python 3.10.12, PyTorch | **Not stated** |
| GPU memory used | ~43 GB during training | **Not stated** |
| Stage 1 training time | ~70 h | **Not stated** |
| Stage 2 training time | ~15 h | **Not stated** |
| Inference speed | ~7 s / report on A100; ~7 s on RTX 3090 | **Not stated** |
| Deployment minimum spec | 16 GB RAM, Python 3.8+, CUDA 11.2+, PyTorch 2.0+ | **Not stated** |

**PPT has no hardware specification slide.** This is a gap for an academic final evaluation.

### Hyperparameters

| Parameter | Report (Table 5.2) | PPT (Fine-Tuning Configuration Details slide) |
|-----------|-------------------|----------------------------------------------|
| LoRA rank | 64 | **Not stated** |
| α (alpha) | 32 | **Not stated** |
| Dropout | 0.05 | **Not stated** |
| Prompt tokens | 64 | **Not stated** |
| Learning rate | 2e-4, cosine scheduler, warmup 0.1 | 2e-4 with cosine scheduler, warmup 0.1 ✓ |
| Batch size | 2 micro / 4 accumulation = 8 global | 2 micro / 4 accumulation = 8 global ✓ |
| Precision | BF16 | BF16 ✓ |
| Optimizer | AdamW-8bit | AdamW with 8-bit quantization ✓ |
| Max sequence length | 4096 | **Not stated** |
| Random seed | 42 | **Not stated** |
| **Epochs** | **4 epochs** | **"2 epochs, 7,730 steps, 253M tokens"** — **MISMATCH** |
| Attention | Flash Attention 2 | Flash Attention 2 ✓ |

**CRITICAL MISMATCH**: The report states **4 epochs** for both stages; the PPT states **2 epochs**. This must be reconciled.

---

## 8. Results & Evaluation

### Table 5.3 (model comparison) — in PPT?

The PPT has a slide titled **"Quantitative Comparison"** which shows `figures/comparison_table.png` — a PNG image of a table. However:
- The report's Table 5.3 (`tab:model-comparison`) compares **9 models**: Qwen-VL-7B, VisualGLM-6B, GPT-4V, HuatuoGPT-Vision-7B, LLaVA-Med, BiomedGPT, Uni-Med, EndoChat, **MVLMERG (Ours)** across 5 metrics.
- The PPT's comparison_table.png caption says "Performance comparison of our fine-tuned model against **baseline LLaVA-4** on key clinical metrics" — only a **2-model comparison**, not the full 9-model table.
- The full Table 5.3 with all 9 baseline models **does not appear in the PPT**.

### Accuracy table (Table 5.4) — in PPT?

The report's Table 5.4 (`tab:accuracy-quality`) compares accuracy across 7 models: HuatuoGPT-Vision-7B (0.7240), LLaVA-Med (0.7013), BiomedGPT (0.6426), Qwen-VL-7B (0.5659), EndoChat (0.6734), GPT-4V (0.7833), MVLMERG (0.8403).

**This table does not appear in the PPT at all.**

### Metrics discussed

| Metric | Report | PPT |
|--------|--------|-----|
| CIDEr = 0.2667 | ✓ (Table 5.3, abstract, conclusion) | Not shown numerically |
| ROUGE-L = 0.4598 | ✓ | Not shown numerically |
| METEOR = 0.4070 | ✓ | Not shown numerically |
| BLEU-4 = 0.3863 | ✓ | Not shown numerically |
| BERTScore = 0.8216 | ✓ | Not shown numerically |
| Accuracy = 0.8403 | ✓ (Table 5.4) | Not shown numerically |
| ECE = 0.042 | ✓ (calibration analysis) | **Not mentioned at all** |
| Statistical significance (paired t-test, p<0.01, Cohen's d>0.8) | ✓ | **Not mentioned at all** |

### Missing tables/figures in PPT
- Table 5.1 (data distribution of Stage 1 and Stage 2)
- Table 5.2 (hyperparameter settings) — only partial coverage as bullet-point slides
- Table 5.3 (full 9-model comparison) — only a 2-model comparison PNG shown
- Table 5.4 (accuracy comparison across 7 models) — entirely absent
- Report Fig-1 (P-LoRA architecture two-stage diagram)
- Report Fig-2 (P-LoRA method detail: attention prompt tokens + feedforward LoRA)
- Report Fig-3 (sentence selection cosine similarity pipeline)
- Report Fig-4 (cosine similarity score distribution; 78.6% of data >0.9 similarity)
- Report Fig-10 (clinical workflow: image → MVLMERG draft → physician review → EMR)

### Missing content in report (that PPT has)
- Actual generated report pages (generated_report.pdf) — two-page side-by-side
- Before/after system prompt quality comparison (verbose old_report.pdf vs concise new_report.pdf)
- Data augmentation example images side-by-side
- De-identification diagram (deidentification_diagram.jpeg)
- Model quantization strategy table (Q8_0/Q5_K_M/Q4_K_M/Q3_K_L)
- Deployment strategy section (vLLM vs Ollama, RESTful API / EHR integration)

---

## 9. Figures

### Report figures (Fig-1 through Fig-10) — status in PPT

| Figure | Report caption | PPT equivalent | In PPT? |
|--------|---------------|----------------|---------|
| **Fig-1** (Fig-1.png) | "Architecture of fine-tuning LLM. Stage 1 and Stage 2, right side shows P-LoRA." | system_architecture.png appears to be different | **No** — P-LoRA framework diagram absent |
| **Fig-2** (Fig-2.png) | "P-LoRA method: (b) first/middle/final layers; (a) all other layers" | No equivalent | **No** |
| **Fig-3** (Fig-3.png) | "Sentence selection: original vs GPT-4 optimised, cosine similarity >0.85 check" | No equivalent | **No** |
| **Fig-4** | "Distribution of data across different similarity levels (78.6% >0.9)" | No equivalent | **No** |
| **Fig-5 to Fig-9** | Not identified in text (likely qualitative results/ablation figures) | No equivalent | **Unclear — need to verify these files exist** |
| **Fig-10** (Fig-10.png) | "Clinical workflow: image → MVLMERG draft → physician review → EMR" | No equivalent | **No** |

### PPT images — status in report

| PPT image file | PPT caption/slide | In report? |
|----------------|------------------|-----------|
| figures/figure1_data_workflow.png | "Data Workflow: From 6,500 patient reports to 32,545 Q&A pairs" | **No** — this is a different figure from report's Fig-1 |
| figures/aug_original.jpg | "Left: Original" | **No** |
| figures/aug_rotated_90.jpg | "Middle: Rotated (180°)" | **No** (also: caption says 180° but filename says 90° — internal inconsistency) |
| figures/aug_translated.png | "Right: Randomly translated" | **No** |
| figures/deidentification_diagram.jpeg | "Irreversible de-identification diagram" | **No** |
| figures/pipeline_part1_data.png | "Stage 1: Pre-training" | **No** |
| figures/pipeline_part2_training.png | "Stage 2: Fine-tuning" | **No** |
| figures/system_architecture.png | "High-level architecture" | Partially — report's Fig-1 covers architecture, different diagram |
| figures/llava_architecture.png | "LLaVA architecture" | **No** separate figure |
| figures/clip_vit_architecture.png | "CLIP-ViT-L/14 vision encoder" | **No** separate figure |
| figures/llama_architecture.png | "LLaMA language model backbone" | **No** separate figure |
| figures/old_report.pdf | "Previous generation (verbose)" | **No** |
| figures/new_report.pdf | "Current generation (concise via system prompt)" | **No** |
| figures/comparison_table.png | "Performance comparison vs baseline LLaVA-4" | Partially — report has full Table 5.3 with 9 models, not just 2 |
| generated_report.pdf | "Final diagnostic report – Pages 1 and 2" | **No** |

**Action needed summary for figures:**
| Figure | Action |
|--------|--------|
| Fig-1 (P-LoRA architecture) | Add to PPT |
| Fig-2 (P-LoRA method detail) | Add to PPT |
| Fig-3 (sentence selection) | Add to PPT |
| Fig-4 (similarity distribution) | Add to PPT |
| Fig-10 (clinical workflow) | Add to PPT |
| aug_rotated_90.jpg caption error | Fix: "Rotated (90°)" or rename file to aug_rotated_180.jpg |
| comparison_table.png | Replace with or supplement by full Table 5.3 (9-model) |

---

## 10. Conclusion & Future Work

### Report conclusion (Chapter 6, Section 6.2)
- Two-stage strategy: Stage 1 aligns domain knowledge from public endoscopy data; Stage 2 fine-tunes on GMC Calicut physician-annotated data
- Specific metrics: CIDEr 0.2667, ROUGE-L 0.4598, METEOR 0.4070, BLEU-4 0.3863, BERTScore 0.8216, Accuracy 84.03%
- Reliability mechanisms: **constrained decoding (temperature=0)**, **image-text alignment verification**, **structured report format**
- Extension to other modalities: colonoscopy, bronchoscopy via adapted prompts
- Inference speed: ~7 s/image vs 3–5 min manual; comparable to intermediate-level physicians
- Comparison: "significantly outperforms GPT-4V and HuatuoGPT-Vision-7B"
- Limitations of automatic metrics (CIDEr/ROUGE etc.); trend consistency with human evaluation noted

### PPT conclusion slide
- "Built a vision-language model for automated endoscopic report generation, fine-tuned on a custom 32,545-sample Q&A dataset"
- "Implemented a DPDP-compliant de-identification pipeline"
- "Integrated system prompts and Chain-of-Thought reasoning"
- "Achieved high-quality diagnostic outputs rivaling manually written reports"
- Impact: "Drastically reduces documentation time; provides explainable AI assistance"
- Path Forward: "Model quantization and clinical deployment trials (pending IEC approval)"

### Gaps
- **Missing from PPT conclusion**: All five specific metric scores; 84.03% accuracy figure; comparison claim vs GPT-4V; constrained decoding / reliability mechanisms; extension to colonoscopy/bronchoscopy.
- **Missing from report**: IEC (Institutional Ethics Committee) approval mention for deployment trials; DPDP Act compliance framing in the conclusion; GGUF quantization and vLLM/Ollama deployment roadmap.

### Future Work

| Future work item | Report | PPT |
|-----------------|--------|-----|
| Multi-center validation (hospitals across India) | ✓ detailed | Not mentioned |
| Multi-image / patient-level report generation | ✓ | Not mentioned |
| Video-based endoscopy with temporal modeling | ✓ | Not mentioned |
| Synthetic data generation using LLMs | ✓ (ref [54,55]) | Not mentioned |
| Efficient multimodal feature fusion architecture | ✓ (ref [56]) | Not mentioned |
| Dataset public release (pending ethics approval) | ✓ | Not mentioned |
| GGUF quantization (Q8_0, Q5_K_M, Q4_K_M, Q3_K_L) | Not mentioned | ✓ |
| vLLM / Ollama deployment | Not mentioned | ✓ |
| RESTful API + EHR integration | Not mentioned | ✓ |
| IEC approval for clinical trials | Not mentioned | ✓ |

---

## 11. References / Citations

### Report references
58 references in IEEE numbered format ([1]–[58]). Key ones for the PPT:
- [8] LLaVA-Med, [14] LoRA (Hu et al.), [18] Qwen-VL, [19] LLaVA, [21] HuatuoGPT-Vision, [33] MedTrinity-25M, [34] VQA-Med, [35] P-Tuning, [38] Sentence-BERT, [42] CIDEr, [46] LLaVA-4 (improved baselines), [47] GPT-4V, [49] EndoChat, [50] BiomedGPT

### PPT references (references.bib)
Uses natbib `\citet{}` author-year format. Includes: pmc2024tuning, jmir2024instruction, pmc2024endovit, zenodo2024endovis, neurips2023llava, arxiv2023llava, acl2024clinical, llava2024official, plainenglish2024finetuning, ibm2024gguf, huggingface2024medgemma, redhat2024ollama, etc. (≈30 entries).

**PPT references are blog posts, tutorials, and preprints** (IBM Think, Plain English, Hugging Face model cards, Red Hat Developer). None of the formal peer-reviewed citations from the report (CIDEr paper, Sentence-BERT, MedTrinity-25M ICLR, etc.) are in the PPT bibliography.

### Citation style inconsistency
- **Report**: IEEE numbered, `[14]`, full venue info (journal, DOI, pages)
- **PPT**: Author-year `\citet{hu2024lora}` natbib plainnat style; many citations are web URLs

### What needs syncing
- The PPT should cite report refs [14] (LoRA), [19] (LLaVA), [46] (LLaVA-4), [18] (Qwen-VL), [21] (HuatuoGPT) when making claims about baselines.
- The PPT's Stage 1 dataset citations (pmc2024endovit, zenodo2024endovis) need to either be added to the report or removed from the PPT.
- PPT references to blog posts for core technical claims (LLaVA fine-tuning, AdamW) should be replaced with or supplemented by the academic sources already in the report.

---

## 12. Content in Report NOT in PPT (needs to be added to PPT)

- **Introduction/motivation section** — why endoscopy reporting is hard, why MLLMs need domain adaptation
- **Literature survey section** — CogVLM, Qwen-VL, LLaVA, HuatuoGPT-Vision, SurgicalGPT, ColonGPT, GP-VLS, LLaVA-Med, PeFoMed
- **Problem definition slide** — formal statement from Chapter 3
- **System name MVLMERG** — never appears in PPT; the model needs a named identity
- **Fig-1: P-LoRA two-stage architecture diagram** — the actual framework figure
- **Fig-2: P-LoRA method detail diagram** — attention prompt tokens + FFN LoRA
- **Fig-3: Sentence selection / GPT-4 templatization pipeline**
- **Fig-4: Cosine similarity score distribution** (78.6% of data above 0.9)
- **Fig-10: Clinical workflow diagram** (image → draft → review → EMR)
- **P-LoRA method slide** — ΔW=AB low-rank decomposition, Equations 1–2, selective layer strategy
- **Data optimization (GPT-4 templatization)** — structured positive prompt, Sentence-BERT cosine similarity filtering (threshold 0.85), physician review loop
- **Full Table 5.3** (9-model comparison: Qwen-VL-7B, VisualGLM-6B, GPT-4V, HuatuoGPT, LLaVA-Med, BiomedGPT, Uni-Med, EndoChat, MVLMERG) with all 5 metrics
- **Table 5.4** (accuracy comparison: 7 models with numerical scores including MVLMERG at 0.8403)
- **ECE = 0.042** calibration analysis
- **Statistical significance** (paired t-test p<0.01, Cohen's d>0.8)
- **All specific metric values** in the conclusion slide (CIDEr/ROUGE-L/METEOR/BLEU-4/BERTScore/Accuracy)
- **Hardware spec slide** (A100 80GB, Xeon Silver 4310, 125 GiB RAM, CUDA 12.9)
- **Training time** (Stage 1: ~70 h; Stage 2: ~15 h)
- **Inference latency** (~7 s/report vs 3–5 min manual)
- **LoRA hyperparameters** (rank=64, α=32, dropout=0.05, prompt tokens=64)
- **Reliability mechanisms** (temperature=0, image-text alignment verification, structured format)
- **Patient demographics** (20–80 age, 85% aged 40–80, near 1:1 gender ratio, 40%/60% normal/abnormal)
- **Multi-center validation plan** and future extension to colonoscopy/bronchoscopy
- **Ethics section reference** — de-identified data, HIPAA-like PHI removal

---

## 13. Content in PPT NOT in Report (needs to be added to report or removed from PPT)

- **GGUF quantization strategy** (Q8_0 ~50% reduction, Q5_K_M ~69%, Q4_K_M ~75% = 3.25 GB, Q3_K_L ultra-compact) — entire section missing from report
- **Deployment planning** (vLLM for high-throughput, Ollama for individual workstations, RESTful API, real-time EHR sync) — missing from report
- **IEC (Institutional Ethics Committee) approval** for clinical deployment trials — not mentioned in report's ethics sections
- **EndoVis 2017/2018 and Endo700k** as Stage 1 training datasets — not in report (**resolve the dataset discrepancy**)
- **De-identification diagram** (deidentification_diagram.jpeg) showing automated redaction fields — not a figure in report
- **Data augmentation example images** (aug_original, aug_rotated, aug_translated side-by-side) — report describes this but has no illustrative figure
- **LLaVA Architecture diagram** (llava_architecture.png) — report uses LLaVA-4 but has no separate LLaVA figure
- **CLIP-ViT Architecture diagram** (clip_vit_architecture.png) — report describes CLIP-ViT-L/14 but has no sub-component figure
- **LLaMA Architecture diagram** (llama_architecture.png) — report mentions Vicuna-7B-v1.5 but has no figure
- **System Prompts for report formatting** — described briefly in report (Sec 4.1.4 templatization) but the PPT's "Refining Generation: Implementing System Prompts" slide gives it much more prominence as a key contribution
- **Verbose vs concise report quality comparison** (old_report.pdf vs new_report.pdf pages side-by-side) — not in report
- **Generated report sample** (generated_report.pdf, 2 pages) — no qualitative example in report
- **Quantitative comparison PNG vs baseline LLaVA-4 only** — PPT only shows 2-model comparison, while report has 9-model table

---

## 14. Recommended Actions

### Add to PPT:

| Slide topic | Reason |
|-------------|--------|
| Introduction & Motivation slide | PPT jumps directly to Methodology; audience has no context for why the project exists |
| System MVLMERG — name and contributions | Model name is never stated in PPT; without it, the presentation has no identity |
| Related Work slide (CogVLM / LLaVA / HuatuoGPT / LLaVA-Med / SurgicalGPT with limitations) | Situates MVLMERG against prior work; required for academic evaluation |
| Problem Definition slide | Chapter 3 content is entirely absent from PPT |
| P-LoRA Method slide (with Equations 1–2 and Fig-2) | P-LoRA is the core technical novelty; it is inexplicably absent from the PPT |
| Full Table 5.3 (9-model comparison) replacing comparison_table.png | Current PPT only compares vs baseline LLaVA-4, which dramatically understates the result |
| Table 5.4 (accuracy comparison, 7 models, MVLMERG=0.8403) | Clinical accuracy result entirely missing from PPT |
| Hardware specification slide (A100 80GB, training times, inference speed) | Required for reproducibility claims in academic evaluation |
| All 6 metric values in the Conclusion slide | Conclusion currently has no numbers; the strongest result (84.03% accuracy) is absent |
| ECE = 0.042 calibration result | Supports trustworthiness of the model; missing |
| Fig-3 (sentence selection/cosine similarity pipeline) | Explains data quality control mechanism; currently missing |
| Fig-4 (similarity distribution chart, 78.6% > 0.9) | Validates data quality claim |
| Fig-10 (clinical workflow diagram) | Needed for the "Path Forward" conclusion slide |

### Add to Report:

| Section/content | Reason |
|-----------------|--------|
| GGUF quantization strategy (Section 6.x or Sec 5.1.2) | PPT presents a concrete deployment-ready quantization plan; report has none |
| Deployment architecture (vLLM/Ollama/RESTful API) | PPT has detailed deployment slides not reflected anywhere in report |
| De-identification figure (deidentification_diagram.jpeg as Fig-X) | Report discusses de-identification but has no figure illustrating the automated redaction |
| Fig showing verbose vs concise report output (before/after system prompt) | Validates system prompt contribution; report mentions it in Sec 4.1.4 but has no figure |
| Generated report sample (qualitative example) | Report has no inline qualitative example of a generated report |
| IEC approval mention | Report's ethics section omits pending IEC approval for clinical deployment |
| Data augmentation example figure (3 image side-by-side) | Report describes rotation/translation but lacks a visual illustration |

### Remove/Fix in PPT:

| Item | Reason |
|------|--------|
| Stage 1 dataset list (EndoVis 2017/2018, Endo700k) | Directly contradicts the report which uses PubMed/MedTrinity-25M/VQA-Med; one document must be authoritative |
| "Trainable: Only multimodal projection layer" in Stage 1 | Contradicts report which states LLM is fine-tuned (not just projection layer) in Stage 1 |
| Epochs: "2 epochs" | Report says 4 epochs; fix to match |
| aug_rotated_90.jpg caption saying "Rotated (180°)" | Caption-to-filename mismatch; fix caption to "Rotated (90°)" or rename file |
| comparison_table.png showing only 2 models | Replace with full 9-model Table 5.3 from the report |
| Blog-post/tutorial citations for technical claims | Replace pmc2024tuning, learnopencv2024tutorial, plainenglish2024finetuning, ubiai2024finetuning with the peer-reviewed references from the report ([14] LoRA, [46] LLaVA-4, [33] MedTrinity-25M, etc.) |
| Missing guide names on title slide | Add Jayaraj PB and Srinivasa TM |
| NIT affiliation comma ("NIT, Calicut") | Fix to "NIT Calicut" to match report |

### Remove/Fix in Report:

| Item | Reason |
|------|--------|
| Stage 1 training dataset list (if PPT's EndoVis/Endo700k is the correct list) | If PPT is correct, report must be updated to cite EndoVis + Endo700k instead of PubMed/MedTrinity-25M/VQA-Med |
| Epoch count (if 2 epochs is correct) | Fix report's "4 epochs" to match PPT if 2 epochs is the actual experiment |
| Table 5.2 note "trained for 4 epochs" | Sync with actual training run |
| Stage 1 "Trainable component" description | Sync with PPT's clarification on what is frozen vs trained in Stage 1 |
| Figures 5–9 (unaccounted) | Verify these figure files (Fig-5.png through Fig-9.png) exist and are referenced correctly in the report; they are listed in `\listoffigures` but not found in the text excerpts reviewed |

---

*Generated by gap analysis comparing `final_report/ERGLM-Report/main.tex` (58 refs, 6 chapters) and `final_ppt/main.tex` + `references.bib` (Beamer, ~30 refs). Analysis date: May 2026.*
