# PPT vs Report — Exhaustive Difference Analysis
> **Source of Truth: `final_report/ERGLM-Report/main.tex`**
> All discrepancies are framed as "what the PPT says / does differently from the report."
> Review each item, decide: keep the report version, update the report, or update the PPT.
> Generated: May 17, 2026

---

## Table of Contents
1. [Numbers & Metrics Discrepancies](#1-numbers--metrics-discrepancies)
2. [Dataset Size Discrepancies](#2-dataset-size-discrepancies)
3. [Architecture Claims Discrepancies](#3-architecture-claims-discrepancies)
4. [Training Configuration Discrepancies](#4-training-configuration-discrepancies)
5. [Content in PPT Missing from Report](#5-content-in-ppt-missing-from-report)
6. [Content in Report Missing from PPT](#6-content-in-report-missing-from-ppt)
7. [Figures — PPT vs Report](#7-figures--ppt-vs-report)
8. [References — PPT vs Report](#8-references--ppt-vs-report)
9. [Wording & Framing Differences](#9-wording--framing-differences)
10. [Structural / Organisational Differences](#10-structural--organisational-differences)
11. [Ethical & Legal Compliance Differences](#11-ethical--legal-compliance-differences)
12. [Future Work Differences](#12-future-work-differences)
13. [Limitations Differences](#13-limitations-differences)
14. [Summary Table — Quick Review](#14-summary-table--quick-review)

---

## 1. Numbers & Metrics Discrepancies

### 1.1 BLEU-4 — MVLMERG is NOT best
| | Value | Notes |
|---|---|---|
| **Report (truth)** | MVLMERG BLEU-4 = **0.0006** | Listed in Table 3 honestly |
| **PPT** | MVLMERG BLEU-4 = **0.0006** | Same value — consistent |
| **Discrepancy** | BLEU-4 for Qwen = **0.0009**, so MVLMERG is the **WORST** on BLEU-4 | Report Table 3 is accurate. PPT Conclusion slide says "Achieved state-of-the-art" and lists BLEU-4 0.0006 without noting it is below Qwen. PPT "Our Objective" slide claims "outperforms all baselines" — this is factually wrong for BLEU-4. |
| **Action** | **PPT only.** Add caveat: "Note: BLEU-4 lower than Qwen (0.0006 vs 0.0009); all 4 other metrics exceed all baselines." Report Table 3 is fine as-is. |

### 1.2 Documentation burden statistics
| Statistic | Report | PPT | Action |
|---|---|---|---|
| Hours/day on documentation (average) | **1.77 hrs/day** (Ch. 2) | Not stated | Report is more rigorous — keep |
| EHR users specifically | **1.84 hrs/day** | Not mentioned | No action needed |
| Non-EHR users | **1.10 hrs/day** | Not mentioned | No action needed |
| 3/4 of providers: burden harms quality | Present (Ch. 2) | Absent | No action needed (PPT is a summary) |
| 58.1% disagree load is reasonable | Present (Ch. 2) | Absent | No action needed |
| 84.7% billing-driven paperwork | Present (Ch. 2) | Absent | No action needed |
| Per-procedure time (3–5 min) | **ABSENT from report** | Present ("3–5 minutes per procedure") | **Add to report** Section 1.1 or 2.1 |
| Unit-performing-40-procedures calc | **ABSENT from report** | "~3.3 hours/day at 40 procedures" | **Add to report** |

### 1.3 Inference latency
| | Report | PPT |
|---|---|---|
| Time per generated report | **ABSENT** | **~7 seconds per report** |
| **Action** | ⚠️ This is a key clinical usability metric. **Add "~7 seconds per report" to report Section 5.1.2 (Implementation Details) or the Conclusion.** |

### 1.4 GPU memory during training
| | Report | PPT |
|---|---|---|
| Training VRAM used | **ABSENT** | **~43 GB** |
| Plain LoRA VRAM comparison | **ABSENT** | **~44 GB** |
| **Action** | **Add to report Section 5.1.2.** |

### 1.5 P-LoRA vs plain LoRA ablation gains
| | Report | PPT |
|---|---|---|
| CIDEr gain P-LoRA over LoRA | **ABSENT** | **+5%** |
| ROUGE-L gain P-LoRA over LoRA | **ABSENT** | **+4.4%** |
| **Action** | **Add these ablation numbers to report Section 5.1.4** (possibly a new sub-section 5.1.4.2). These justify the P-LoRA design choice quantitatively. |

### 1.6 AdamW 8-bit memory reduction
| | Report | PPT |
|---|---|---|
| Memory reduction stat | **ABSENT** | **~75% vs standard AdamW** |
| **Action** | **Add to report Section 5.1.2.** |

---

## 2. Dataset Size Discrepancies

### 2.1 Phase 1 / Stage 1 dataset total size — CRITICAL
| | Report | PPT |
|---|---|---|
| Table 1 value | **60k samples** (PubMed + MedTrinity-25M + VQA-Med combined) | **"~160K Medical records"** (MedTrinity-25M) on "Our Objective" slide and Stage 1 pipeline slide |
| **Discrepancy** | 60k (report) vs ~160k (PPT). These are contradictory figures for Stage 1. |
| **Likely explanation** | The 60k may be the GI-filtered subset used. The 160k may be a broader MedTrinity-25M sample. The report's Table 1 says "60k" total for Phase 1 across all three datasets. |
| **Action** | 🔴 **CRITICAL.** Reconcile with actual training logs. State the canonical number in the report with scope clearly defined (e.g., "60k GI-relevant samples drawn from the combined PubMed / MedTrinity-25M / VQA-Med corpora"). Update PPT to match. |

### 2.2 Phase 2 final augmented count
| | Report | PPT |
|---|---|---|
| Augmented dataset size | **~31,200** image-report pairs (Table 1, Section 4.1.5.3) | Two conflicting numbers appear: **"~31,200 samples"** (Stage 2 methodology) AND **"~36,000 image-report pairs"** / **"34,150 train / 1,850 validation"** (Figure 1 slide / Dataset Creation slide) |
| **Discrepancy** | 31,200 (consistent with report) vs 36,000 (inconsistent) both appear in the same PPT. |
| **Likely explanation** | 36,000 may count each image individually (~4 images × 9,000 procedures ≈ 36,000 images) vs 31,200 at the report-pair level (6,500 base × 4 rotations + 20% translation). |
| **Action** | 🔴 **CRITICAL.** Report's 31,200 is canonical. Remove the 36,000 figure from PPT or add a clarifying note that 36,000 = raw image count, 31,200 = image-report pair count. |

### 2.3 Train/Validation split
| | Report | PPT |
|---|---|---|
| Training samples | **~29,640** (95% of 31,200) | **34,150** |
| Validation samples | **~1,560** (5% of 31,200) | **1,850** |
| **Discrepancy** | Both the absolute numbers and implied total differ. |
| **Action** | 🔴 **CRITICAL.** Verify the actual split used in training. If 31,200 pairs is correct and 5% held out, the report's 29,640/1,560 is right. Update PPT to match report. |

### 2.4 Base images count
| | Report | PPT |
|---|---|---|
| Raw base image count | Not stated explicitly | **~9,000 base images** |
| **Expected if 6,500 × 4 images each** | 26,000 images | ~9,000 images implies not all reports had 4 images |
| **Action** | Report should state actual base image count. The PPT's ~9,000 implies ~1.4 images per procedure on average — verify this is correct or whether ~9,000 refers to a different subset. |

### 2.5 Effective batch size — explicit statement
| | Report | PPT |
|---|---|---|
| Global batch size | Implicitly 8 (micro 2 × grad accum 4) — Table 2 shows both components | PPT explicitly states **global batch size = 8** |
| **Action** | Consider adding "effective batch size = 8" as an explicit row in report Table 2 for clarity. |

---

## 3. Architecture Claims Discrepancies

### 3.1 Soft prompt token count
| | Report | PPT |
|---|---|---|
| m (soft prompt tokens) | **Not stated in report body** | **m = 64** |
| Soft prompt parameter count | **Not stated** | **~262K parameters** (64 × 4096) |
| **Action** | **Add m=64 and ~262K to report Section 4.1.3.** This is a concrete architectural detail. |

### 3.2 LoRA application — which projection
| | Report | PPT |
|---|---|---|
| Report | "LoRA applied to feed-forward layers' first linear projection (d_model → d_ff)" | PPT does not specify |
| **Action** | Report is more precise. PPT is acceptable as-is. No action needed. |

### 3.3 Architecture consistency
| Component | Report | PPT | Status |
|---|---|---|---|
| Base model | LLaVA-4 | LLaVA-4 | ✅ |
| Vision encoder | CLIP-ViT-L/14 | CLIP-ViT-L/14 | ✅ |
| Pre-training data | 400M image-text pairs | 400M image-text pairs | ✅ |
| Image resolution | 336×336 | 336×336 | ✅ |
| Patch size | 14×14 | 14×14 | ✅ |
| Vision token grid | 24×24 | 24×24 | ✅ |
| Total vision tokens | 577 (576+CLS) | 577 (576+CLS) | ✅ |
| LLM backbone | Vicuna-7B-v1.5 | Vicuna-7B-v1.5 | ✅ |
| Trainable params % | 0.343% | 0.343% | ✅ |
| LoRA rank | 64 | 64 | ✅ |
| LoRA alpha | 32 | 32 | ✅ |
| LoRA dropout | 0.05 | 0.05 | ✅ |

---

## 4. Training Configuration Discrepancies

### 4.1 Learning rate scheduler
| | Report | PPT |
|---|---|---|
| Scheduler | **Not in Table 2** | **Cosine scheduler with warmup=0.1** |
| **Action** | **Add "cosine schedule" to report Table 2.** Warmup ratio is already there (0.1). Just label it as the scheduler type. |

### 4.2 Precision / dtype
| | Report | PPT |
|---|---|---|
| Precision | bfloat16 (BF16) — mentioned in Implementation Details | BF16 | ✅ Consistent. Report could add this to Table 2 |

### 4.3 All other hyperparameters
| Parameter | Report Table 2 | PPT | Status |
|---|---|---|---|
| Learning rate | 2e-4 (both phases) | 2e-4 | ✅ |
| Optimizer | AdamW-8bit | AdamW with 8-bit quantization | ✅ |
| Warm-up ratio | 0.1 | 0.1 | ✅ |
| Epochs | 4 | 4 | ✅ |
| Random seed | 42 | 42 | ✅ |
| Batch size (micro) | 2 | 2 | ✅ |
| Gradient accumulation | 4 | 4 | ✅ |
| Max sequence length | 4096 | 4096 | ✅ |

### 4.4 Hardware
| Component | Report | PPT | Status |
|---|---|---|---|
| GPU | NVIDIA H200 NVL 143 GB HBM3e, CUDA 13.0 | Same | ✅ |
| CPU | Dual Intel Xeon 6505P, 12 cores/socket, 48 threads | Same | ✅ |
| RAM | 251 GiB | Same | ✅ |
| Python | 3.12.3 | Same | ✅ |
| PyTorch | 2.11.0+cu130 | Same | ✅ |

---

## 5. Content in PPT Missing from Report

Topics present in PPT but **absent or underdeveloped** in the report. Evaluate each for inclusion.

### 5.1 Chain-of-Thought verbatim example
- **PPT has:** Full worked example — input image + "step-by-step" query → structured output:
  ```
  Observation 1: I observe the lower third of the oesophagus.
  Observation 2: There are swollen, tortuous submucosal veins present.
  Observation 3: The veins occupy less than one third of the lumen, with no red colour signs.
  Conclusion: Therefore, the diagnosis is Grade II Esophageal Varices.
  ```
  Also shows "Before CoT" baseline: plain "Grade II Esophageal Varices."
- **Report has:** Section 4.1.6 describes CoT conceptually, no concrete example.
- **Recommendation:** ✅ **ADD to report** Section 4.1.6.2 — the verbatim before/after example is strong clinical evidence.

### 5.2 System prompt verbatim
- **PPT has:**
  ```
  "You are a medical endoscopy report assistant.
  Respond with only the clinical finding using correct medical terminology. Be concise."
  ```
- **Report has:** Mentions system prompts exist but never quotes the actual text.
- **Recommendation:** ✅ **ADD to report** Section 5.1.2 as a code listing.

### 5.3 Verbose vs. Concise report comparison (qualitative figure)
- **PPT has:** Side-by-side PDF renders of old verbose generation vs. new concise generation (figures `old_report.pdf` / `new_report.pdf`).
- **Report has:** No qualitative figure showing actual model output.
- **Recommendation:** ✅ **ADD to report** Section 5.1.4 as a qualitative results figure. This is compelling evidence of system prompt impact.

### 5.4 A full generated diagnostic report sample
- **PPT has:** `generated_report.pdf` shown as a side-by-side 2-page result.
- **Report has:** No generated report example figure anywhere.
- **Recommendation:** ✅ **ADD to report** Section 5 — show at least one full generated report as a figure.

### 5.5 Quantization format table (GGUF)
- **PPT has:**
  | Format | Use Case | Size Reduction |
  |---|---|---|
  | Q8_0 | High-end workstations | ~50% |
  | Q5_K_M | Standard clinical computers | ~69% |
  | Q4_K_M | Basic clinical terminals | ~75% (3.25 GB) |
  | Q3_K_L | Mobile deployments | Ultra-compact |
- **Report has:** Section 6.1 mentions quantisation in future work but no format details.
- **Recommendation:** ✅ **ADD quantization table to report** Section 6.1.

### 5.6 Deployment frameworks (vLLM and Ollama)
- **PPT has:** vLLM (PagedAttention, high-throughput hospital server) and Ollama (individual workstations).
- **Report has:** No specific deployment framework.
- **Recommendation:** ✅ **ADD to report** Section 6.1.

### 5.7 Expert physician evaluation protocol details
- **PPT has:** 2 senior endoscopists, 10+ years experience, GMC Calicut; disagreements resolved by consensus.
- **Report conclusion:** Only "Expert physician review at GMC Calicut confirmed most outputs meet professional standards."
- **Recommendation:** ✅ **ADD evaluator details** to report Section 5.1.3 or 5.1.4 — credentials, number of evaluators, consensus protocol.

### 5.8 "First model for Indian GI endoscopic setting" claim
- **PPT has:** Explicit claim: "First model targeting professional Indian GI endoscopic report generation in a real hospital deployment setting."
- **Report has:** Does not make this "first" claim explicitly.
- **Recommendation:** ✅ **ADD to report** Introduction contributions list or Conclusion. Strong positioning.

### 5.9 Per-procedure documentation time context
- **PPT has:** 3–5 minutes per procedure; ~3.3 hours/day at 40 procedures.
- **Report:** Uses aggregate statistics only (1.77 hrs/day across all physicians) — no per-endoscopy number.
- **Recommendation:** ✅ **ADD to report** Section 1.1 or 2.1 — per-procedure time is more directly relevant to the specific clinical workflow.

### 5.10 "Why P-LoRA outperforms LoRA alone" — quantitative justification
- **PPT has:** Dedicated slide + numbers: +5% CIDEr, +4.4% ROUGE-L over plain LoRA.
- **Report has:** Explains P-LoRA design but does not compare to plain LoRA numerically.
- **Recommendation:** ✅ **ADD the +5%/+4.4% ablation numbers** to report Section 5.1.4.

### 5.11 QA Generation 4-step pipeline (explicit)
- **PPT has:**
  1. Medical entity extraction (pathological findings, diagnoses, procedural techniques)
  2. Question template design (diagnostic, anatomical, pathological, procedural, comparative)
  3. Automated QA generation (multiple variations per concept)
  4. Answer synthesis from reports + medical knowledge
- **Report has:** Section 4.1.4 describes GPT-5 processing but not this 4-step enumerated pipeline.
- **Recommendation:** ✅ **ADD the 4-step QA pipeline** to report Section 4.1.4 or a new subsection.

### 5.12 Inference latency
- **PPT:** ~7 seconds per report.
- **Report:** Absent.
- **Recommendation:** 🔴 **MUST ADD** to report Section 5.1.2. Clinically critical: directly contrasts with 3–5 minutes manual.

---

## 6. Content in Report Missing from PPT

In report, absent from PPT — report needs no change; PPT changes are optional.

| # | Report Content | PPT Status | Recommendation |
|---|---|---|---|
| 6.1 | Full literature survey stats (1.77 hrs, EHR breakdown) | Summarised in PPT | Acceptable — PPT is condensed |
| 6.2 | All 10 limitations (Section 6.1) | No limitations slide | ⚠️ Add compressed 3-bullet limitations slide to PPT for credibility |
| 6.3 | Formal mathematical notation for visual encoding (V, T, R vectors) | Absent | Not needed in PPT |
| 6.4 | Anatomical coverage: Esophagus, GE Junction, Stomach (Fundus/Body/Antrum), Pylorus, Duodenum | Absent | Consider adding to PPT Dataset slide |
| 6.5 | Pathological categories: varices, polyps, GI bleeding, ulcers, early/advanced gastric malignancy | Absent | Add to PPT Dataset slide for clinical richness |
| 6.6 | Native image resolution: 1920×1080 before downsampling | Absent | Minor — add if space allows |
| 6.7 | GPT-5 3-step QC (format → gastro review → reject if <0.85) | Partially present | Report is more complete |
| 6.8 | CDSCO SaMD regulatory compliance pathway | Absent from PPT | Add to PPT Deployment Planning slide |
| 6.9 | MoU date: 27 April 2022 | Not in PPT | Minor administrative detail |
| 6.10 | Token-level count (~29.95M tokens total) | Absent | Not critical for PPT |
| 6.11 | Autoregressive decoding formula (Formula 2) | Absent | Not needed in PPT |
| 6.12 | 10-item detailed limitations list | Absent | Add brief version to PPT |

---

## 7. Figures — PPT vs Report

### 7.1 Figures in PPT but NOT in Report

| Figure file | Description | Recommendation |
|---|---|---|
| `figures/figure1_data_workflow.png` | Dataset workflow (~36,000 pair generation) | ✅ Add to report Section 4.1.5.3 |
| `figures/system_architecture.png` | High-level system architecture | ✅ Add to report Section 4.1.1 (Overview) |
| `figures/llava_architecture.png` | LLaVA architecture diagram | ✅ Add to report Section 4.1.1 |
| `figures/clip_vit_architecture.png` | CLIP-ViT encoder architecture | ✅ Add to report Section 4.1.1 |
| `figures/llama_architecture.png` | Vicuna/LLaMA decoder architecture | ✅ Add to report Section 4.1.1 |
| `figures/pipeline_part1_data.png` | Stage 1 pre-training pipeline | Optional — report's Fig-1 covers this |
| `figures/pipeline_part2_training.png` | Stage 2 fine-tuning pipeline | Optional — report's Fig-1 covers this |
| `figures/old_report.pdf` + `figures/new_report.pdf` | Verbose vs. concise generation | ✅ **Add to report** as qualitative figure |
| `generated_report.pdf` | Full generated diagnostic report | ✅ **Add to report** — critical qualitative result |

### 7.2 Figures in Report but NOT in PPT

| Figure | Report Location | PPT Equivalent | Notes |
|---|---|---|---|
| `Fig-1.png` | Overall two-phase architecture | Split into pipeline_part1 + pipeline_part2 slides | Report's single figure is cleaner |
| `Fig-2.png` | P-LoRA schematic (2 panels) | Covered across Soft Prompt + Selective LoRA slides | Effectively same content |

### 7.3 Consistent Figures in Both

| Figure | Report | PPT | Status |
|---|---|---|---|
| `aug_original.jpg`, `aug_rotated_90.jpg`, `aug_translated.png` | Fig. 3 | Data Augmentation Examples slide | ✅ |
| `deidentification_diagram.jpeg` | Fig. 4 | De-identification slide | ✅ |
| `Fig-10.png` (deployment workflow) | Fig. 5 | Deployment Planning slide | ✅ |

---

## 8. References — PPT vs Report

### 8.1 Critical bibliography numbering mismatch in REPORT

| Issue | Detail |
|---|---|
| **Body text** | Both documents attribute reference [42] to **Zhang et al.** — "A fine-tuning multimodal large language model for endoscopic report generation" (Biomed. Signal Process. Control 118, 2026) |
| **Bib entry at position 42** | Report's `\bibitem{ref42}` physically corresponds to **CIDEr (Vedantam et al. 2015)** in the bibliography list |
| **Effect** | Compiled PDF will render "CIDEr" where "Zhang 2026" is intended, breaking the key methodological citation |
| **Action** | 🔴 **URGENT: Fix report bibliography.** Verify in compiled PDF which paper actually renders for [42]. Re-order or re-number to ensure Zhang 2026 appears where [42] is cited in the body. |

### 8.2 References in PPT bib NOT in Report bib

| PPT key | Title | Year | In Report? |
|---|---|---|---|
| `openai2023gpt4` | GPT-4 Technical Report | 2023 | Not cited in report |
| `arxiv2024kvasir` | Kvasir-VQA-x1 | 2024 | Unclear |
| `dettmers2023qlora` | QLoRA | 2023 | Not as a standalone citation |
| `frantar2023gptq` | GPTQ | 2023 | Not cited |
| `kwon2023vllm` | vLLM/PagedAttention | 2023 | Not cited |

### 8.3 References in Report bib NOT in PPT bib
The report has a 46+ entry bibliography. The PPT's `references.bib` covers ~22 entries — all a subset of the report. The report is more complete on references.

### 8.4 PPT bib contains non-academic / blog-post sources
The previous `ppt_vs_report_diff.md` (pre-existing) identified blog-post citations in the PPT bib.
The current PPT `references.bib` now contains cleaned academic sources only (see Section 7 of old diff file).
Verify the PPT bib is clean before submission.

---

## 9. Wording & Framing Differences

### 9.1 "Outperforms all baselines across all metrics" — BLEU-4 exception
| | Report | PPT |
|---|---|---|
| Report Conclusion | "MVLMERG outperforms every evaluated baseline" — does NOT specify "all metrics" | Accurate |
| Report Table 3 | Shows MVLMERG BLEU-4 (0.0006) < Qwen BLEU-4 (0.0009) | Honest |
| PPT Conclusion | "Achieved state-of-the-art: BLEU-4 0.0006" — **without** noting it is below Qwen | Misleading |
| PPT Objective slide | "outperforms all baselines" | Factually wrong for BLEU-4 |
| **Action** | Fix PPT Conclusion and Objective slides to add caveat about BLEU-4. |

### 9.2 "Dataset, MVLMERG" vs "model, MVLMERG" — report abstract error
| | Report | PPT |
|---|---|---|
| Report Abstract text | "our curated endoscopic dataset, **MVLMERG**, achieves state-of-the-art performance" | Uses MVLMERG as **model name** throughout |
| Rest of report | MVLMERG = the **model** | Consistent |
| **Discrepancy** | The abstract accidentally calls MVLMERG the "dataset" when it is the model. |
| **Action** | 🔴 **Fix report abstract.** Change to: "our fine-tuned model, MVLMERG, trained on a curated endoscopic dataset, achieves state-of-the-art performance." |

### 9.3 "Minor clinically consequential inaccuracies" — contradictory phrasing
| | Report Conclusion | PPT |
|---|---|---|
| Exact phrasing | "errors limited to minor clinically consequential inaccuracies" | Does not use this phrasing |
| **Problem** | "minor" and "clinically consequential" are contradictory. If errors are clinically consequential, they are NOT minor. |
| **Action** | 🔴 **Fix report conclusion.** Use: "errors limited to minor, **non-**clinically-consequential inaccuracies" OR "rare clinically relevant errors do occur, making mandatory physician review essential." |

### 9.4 P-LoRA full form consistency
| | Report | PPT | Status |
|---|---|---|---|
| Full form | "Prompt-based Low Rank Adaptation (P-LoRA)" | "P-LoRA — soft prompt tuning + selective LoRA" | ✅ Consistent in substance |

### 9.5 "AI gap" framing
| | Report | PPT |
|---|---|---|
| Report | General statement about clinical vocabulary gaps | |
| PPT | Detailed "The AI Gap" slide listing specific failures: inability to identify erythema/granularity/friability; unfamiliarity with Paris classification, Barrett's Prague criteria, ESGE grades; produces generic descriptions not structured clinical reports | |
| **Action** | These specific failure modes strengthen the problem motivation. **Add to report** Introduction Section 1.1. |

---

## 10. Structural / Organisational Differences

| Aspect | Report | PPT | Notes |
|---|---|---|---|
| Literature survey | Chapter 2 standalone | Slides 8–11 integrated within Introduction section | PPT approach is fine for presentation; no change needed |
| Problem definition | Chapter 3 standalone | Slide 7 within Introduction | Acceptable format difference |
| Architecture detail | Chapter 4 (Methodology) covers all | PPT splits into "Methodology" (slides 12–23) + "Architecture" (slides 24–33) | PPT provides more architectural depth: dedicated LLaVA, CLIP-ViT, Vicuna slides |
| Implementation rationale | Section 5.1.2 brief | PPT has 4 dedicated slides: system prompts, verbose vs concise, CoT blackbox, training config rationale, AdamW/FA2 justification | **Report missing these justifications — add them** |
| Qualitative results | ABSENT from report | PPT slide 43: generated report side-by-side | **Critical gap in report — add qualitative section** |
| Future work | Section 6.1 combined with limitations | Two dedicated future work slides with specific frameworks, timelines, and quantization formats | **PPT future work is more detailed — report should match this specificity** |
| Post-mid badge | Not applicable | Slides tagged [Post-Mid] mark contributions after mid-evaluation | Useful internal tracking; not needed in report |

---

## 11. Ethical & Legal Compliance Differences

| Aspect | Report | PPT | Status / Action |
|---|---|---|---|
| DPDP Act compliance | Mentioned in Conclusion + Ethical Considerations | Mentioned in De-identification slide | ✅ Consistent |
| PHI fields removed | Patient ID, full name, age/gender, encounter dates, UGI-OGD numbers | Same list | ✅ |
| MoU date | **27 April 2022** explicitly stated | Date not mentioned | Minor — PPT could add date |
| IEC approval status | Implies ongoing | "pending IEC approval" — more explicit | PPT phrasing is clearer; adopt in report too |
| CDSCO SaMD compliance | Section 6.1 (future work) | NOT in PPT | Add to PPT Deployment Planning slide |
| Oversight committee for discrepant cases | Mentioned in Ethical Considerations | Not in PPT | Not needed in PPT |
| Data labelled as decision-support only | Ethical Considerations | Stated | ✅ |
| Clinician mandatory review | Conclusion | Stated | ✅ |

---

## 12. Future Work Differences

| Future Work Item | Report (Sec 6.1) | PPT (Slides 50–51) | Action |
|---|---|---|---|
| Multi-centre evaluation (≥2 hospitals, India) | ✅ | ✅ | Consistent |
| Sequential image input / temporal modelling | ✅ | ✅ | Consistent |
| Enlarge dataset (2,000–3,000 more GMC cases) | ✅ | ✅ | Consistent |
| INT8 / INT4 quantisation | ✅ (brief) | ✅ with full GGUF format table | **Add quantization table to report** |
| Inference API + EMR integration | ✅ | ✅ | Consistent |
| Open dataset / model release post-IEC | ✅ | ✅ | Consistent |
| Brief site-specific P-LoRA fine-tuning | ✅ (in report) | ❌ NOT in PPT | Add to PPT Future Work slide |
| vLLM (PagedAttention) + Ollama frameworks | ❌ NOT in report | ✅ in PPT | **Add to report** Section 6.1 |
| Annotation bias correction / second annotator | ✅ (in limitations) | Not explicit in future work | Consider adding to PPT |
| CDSCO SaMD deployment compliance | ✅ | ❌ | Add to PPT deployment slide |

---

## 13. Limitations Differences

| | Report (Sec 6.1) | PPT |
|---|---|---|
| Dedicated limitations section | ✅ Full 10-item enumerated list | ❌ No limitations slide at all |
| Item 1: Not error-free; physician review mandatory | ✅ | Implied in conclusion slide only |
| Item 2: Single-institution data | ✅ | Not mentioned |
| Item 3: Clinician annotation bias | ✅ | Not mentioned |
| Item 4: Single-frame input | ✅ | Implied in future work |
| Item 5: No temporal modelling | ✅ | Implied in future work |
| Item 6: Regional disease variation | ✅ | Not mentioned |
| Item 7: Ambiguous visuals → conservative output | ✅ | Not mentioned |
| Item 8: No uncertainty quantification | ✅ | Not mentioned |
| Item 9: Under-represented categories (Barrett's, polyps, post-ESD) | ✅ | Not mentioned |
| Item 10: Open problems (boundary delineation, malignancy grading) | ✅ | Not mentioned |
| BLEU-4 below Qwen | Not explicitly called a limitation | Table shows it plainly |
| IEC approval pending | Implied | Explicitly stated |
| **Action** | **Add a compressed limitations slide to PPT** (3–4 bullets). Standard for final evaluation; builds credibility with the review panel. |

---

## 14. Summary Table — Quick Review

Priority: 🔴 Critical (factual error / must fix) / 🟡 Important (significant gap) / 🟢 Minor (nice to have)

| # | Issue | Priority | Target | Recommended Action |
|---|---|---|---|---|
| 1 | PPT Conclusion/Objective slides: "outperforms all metrics" is wrong for BLEU-4 | 🔴 | PPT | Add BLEU-4 caveat |
| 2 | Report Abstract: calls MVLMERG the "dataset" instead of "model" | 🔴 | Report | Fix "dataset, MVLMERG" → "model, MVLMERG" |
| 3 | Report Conclusion: "minor clinically consequential" — contradictory phrasing | 🔴 | Report | Fix phrasing |
| 4 | Report bib: ref [42] body text ≠ bib entry at position 42 (Zhang 2026 vs CIDEr) | 🔴 | Report | Audit compiled PDF; fix bibliography |
| 5 | Stage 1 dataset: 60k (report) vs ~160k (PPT) contradiction | 🔴 | Both | Reconcile with actual training data; state canonical number |
| 6 | Phase 2 augmented count: 31,200 (report) vs 36,000 (PPT) contradiction | 🔴 | Both | Clarify image vs report-pair unit; unify to 31,200 report pairs |
| 7 | Train/val split: 29,640/1,560 (report) vs 34,150/1,850 (PPT) | 🔴 | Both | Verify actual split used; update both to match |
| 8 | Inference latency (~7 seconds) absent from report | 🟡 | Report | Add to Section 5.1.2 |
| 9 | GPU training memory (~43 GB) absent from report | 🟡 | Report | Add to Section 5.1.2 |
| 10 | P-LoRA vs plain LoRA ablation (+5% CIDEr, +4.4% ROUGE-L) absent from report | 🟡 | Report | Add to Section 5.1.4 |
| 11 | Soft prompt count (m=64, ~262K params) not in report body | 🟡 | Report | Add to Section 4.1.3 |
| 12 | Cosine scheduler not in report Table 2 | 🟡 | Report | Add scheduler row |
| 13 | AdamW 8-bit ~75% memory reduction not in report | 🟡 | Report | Add to Section 5.1.2 |
| 14 | CoT verbatim before/after example absent from report | 🟡 | Report | Add to Section 4.1.6.2 |
| 15 | System prompt verbatim not in report | 🟡 | Report | Add to Section 5.1.2 |
| 16 | QA generation 4-step pipeline not enumerated in report | 🟡 | Report | Add to Section 4.1.4 |
| 17 | Expert physician eval protocol details vague in report | 🟡 | Report | Add credentials + consensus method to Section 5.1.3 |
| 18 | "First model for Indian GI setting" claim absent from report | 🟡 | Report | Add to Introduction or Conclusion |
| 19 | Per-procedure time (3–5 min) not in report | 🟡 | Report | Add to Section 1.1 or 2.1 |
| 20 | Verbose vs. concise report qualitative figure missing from report | 🟡 | Report | Add to Section 5.1.4 |
| 21 | Full generated report example figure missing from report | 🟡 | Report | Add to Section 5 |
| 22 | System/CLIP/Vicuna architecture figures absent from report | 🟡 | Report | Add to Section 4.1.1 |
| 23 | Data workflow figure missing from report | 🟡 | Report | Add to Section 4.1.5 |
| 24 | Quantization format table (GGUF) missing from report | 🟡 | Report | Add to Section 6.1 |
| 25 | vLLM / Ollama deployment frameworks not in report | 🟡 | Report | Add to Section 6.1 |
| 26 | AI gap specific failure modes not in report intro | 🟡 | Report | Add to Section 1.1 |
| 27 | PPT has no Limitations slide | 🟡 | PPT | Add compressed 3-4 bullet limitations frame |
| 28 | Base image count ~9,000 (PPT) vs expected 26,000 — needs clarification | 🟡 | Both | Verify and clarify base image count |
| 29 | CDSCO SaMD not in PPT deployment slide | 🟢 | PPT | Add to deployment planning |
| 30 | Site-specific P-LoRA fine-tuning absent from PPT future work | 🟢 | PPT | Add to Future Work slide |
| 31 | MoU date (27 April 2022) not in PPT | 🟢 | PPT | Minor — add if desired |
| 32 | Pathological categories list absent from PPT | 🟢 | PPT | Add to Dataset slide |
| 33 | GPT-5 data template verbatim not in report | 🟢 | Report | Consider adding to Section 4.1.4 |
