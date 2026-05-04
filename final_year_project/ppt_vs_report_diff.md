# PPT vs Report: Differences & Action Plan
*Report is the source of truth. PPT needs to be updated to match.*

---

## CRITICAL FIXES (Factual Errors)

### 1. MVLMERG Scores — PPT has OLD values
The report was updated with new scores but the PPT still shows the original values.

| Metric | PPT (WRONG) | Report (CORRECT) |
|--------|-------------|-----------------|
| CIDEr | 0.2667 | **0.2731** |
| ROUGE-L | 0.4598 | **0.4621** |
| METEOR | 0.4070 | **0.4052** |
| BLEU-4 | 0.3863 | **0.3902** |
| BERTScore | 0.8216 | **0.8244** |
| Accuracy | 84.03% / 0.8403 | **85.19% / 0.8519** |

**Action:** Update all occurrences in PPT — quantitative comparison table, accuracy table, and any inline text.

---

### 2. LLM Backbone Name — PPT says "LLaMA", should be "Vicuna-7B-v1.5"
The PPT frame "LLaMA Architecture" and related slides refer to the backbone as "LLaMA" generically. The report is specific: **Vicuna-7B-v1.5**.

**Action:** Rename the "LLaMA Architecture" frame and any text referring to "LLaMA backbone" → "Vicuna-7B-v1.5 backbone". The frame title can stay as "LLM Backbone — Vicuna-7B-v1.5" or similar.

---

### 3. Disease List — PPT missing esophageal varices
The report explicitly lists the diseases in the Stage 2 dataset. The PPT doesn't enumerate them.

**Report list:**
- Esophageal varices, Gastric polyps, Gastric bleeding, Gastric ulcers, Early-stage gastric cancer, Advanced gastric cancer

**Action:** Add disease list to the "Dataset Creation and Augmentation" or "Stage 2" slide.

---

### 4. Stage 2 Dataset Numbers — Minor discrepancy
| Item | PPT | Report |
|------|-----|--------|
| Base reports | 6,500 | 6,500 ✓ |
| Final pairs | 32,545 | ~31,200 |
| Training split | 30,917 | 29,640 |
| Validation split | 1,627 | 1,560 |

The PPT and report have slightly different final augmented counts. The report's numbers (31K / 29,640 / 1,560) should be used as they were set by the final compile.

**Action:** Update PPT Stage 2 slide numbers to match report.

---

### 5. GMC Calicut Name
- PPT uses: "Government Medical College (GMC), Kozhikode"
- Report uses: "Government Medical College, Calicut"

Both refer to the same institution (Kozhikode is the city, Calicut is the historical/common English name). The report is consistent throughout with "Calicut".

**Action:** Standardise PPT to "Government Medical College, Calicut" for consistency with report.

---

## REFERENCES — Major Issues

### 6. No Numbered References in PPT
The PPT currently uses **author-year natbib citations** (e.g., `(Hu et al., 2022)`). The user wants numbered references `[1]`, `[2]` etc.

**Current setup:**
- `\usepackage[numbers]{natbib}` or `\bibliographystyle{plainnat}` — produces author-year
- Custom `\scite{}` command displays citations in small gray text

**Action:** Change bibliography style to `unsrtnat` or `plain` (numbered, ordered by appearance) and update `\scite{}` to show numbers. Concretely:
- Change `\bibliographystyle{plainnat}` → `\bibliographystyle{unsrtnat}`
- This will auto-number citations in order of appearance as [1], [2], etc.

---

### 7. PPT References — Non-Academic / Blog Post Sources
The PPT `.bib` file contains **many non-peer-reviewed blog posts** that should be replaced with academic equivalents. Here is the full audit:

#### ❌ Non-Academic Sources (Should Be Replaced)

| Cite Key | Source | Problem |
|----------|--------|---------|
| `inc2024burnout` | Inc.com blog | Not peer-reviewed |
| `europe2024burnout` | Healthcare in Europe website | Not peer-reviewed |
| `nat2024burden` | National Academies Press | Policy report (acceptable to keep) |
| `jama2024burden` | JAMA Internal Medicine | ✅ Peer-reviewed — KEEP |
| `frontiers2024survey` | Frontiers in AI | ✅ Peer-reviewed — KEEP |
| `arxiv2024survey` | arXiv preprint | Acceptable (preprint) |
| `arxiv2024healthcare` | arXiv preprint | Acceptable |
| `pmc2024endovit` | PMC | ✅ Peer-reviewed — KEEP |
| `pmc2024endo` | Annals of Gastroenterology | ✅ Peer-reviewed — KEEP |
| `neurips2023llava` | NeurIPS | ✅ Peer-reviewed — KEEP |
| `microsoft2023llava` | Microsoft Research blog | Duplicate of `neurips2023llava` — REMOVE |
| `pmc2024burden` | J. General Internal Medicine | ✅ Peer-reviewed — KEEP |
| `amia2024survey` | AMIA News (press release) | Not peer-reviewed |
| `bmj2024impact` | BMJ Open Quality | ✅ Peer-reviewed — KEEP |
| `pmc2024healthcare` | PMC | ✅ Peer-reviewed — KEEP |
| `arxiv2023llava` | arXiv | Duplicate of `neurips2023llava` — REMOVE |
| `acl2024clinical` | ACL Anthology | ✅ Peer-reviewed — KEEP |
| `arxiv2024endofm` | arXiv | Acceptable |
| `arxiv2024endovision` | arXiv | Acceptable |
| `zenodo2024endovis` | Zenodo dataset | Acceptable (dataset) |
| `dkfz2024endovision` | DKFZ/OpenCAS | Institutional report |
| `arxiv2024qageneration` | arXiv | Acceptable |
| `arxiv2024kvasir` | arXiv | Acceptable |
| `github2024kvasir` | GitHub repo | Not peer-reviewed |
| `lora2022hu` | ICLR 2022 | ✅ Peer-reviewed — KEEP |
| `medtrinity2025xie` | ICLR 2025 | ✅ Peer-reviewed — KEEP |
| `vqamed2019abacha` | CLEF 2019 | ✅ Peer-reviewed — KEEP |
| `plora2022liu` | ACL 2022 | ✅ Peer-reviewed — KEEP |
| `jmir2024instruction` | JMIR | ✅ Peer-reviewed — KEEP |
| `ibm2024gguf` | IBM Think blog | ❌ Replace with academic paper |
| `huggingface2024medgemma` | HuggingFace model card | ❌ Replace with academic paper |
| `e2e2024quantization` | E2E Networks blog | ❌ Replace with academic paper |
| `phison2024format` | Phison blog | ❌ Replace |
| `dataloop2024medicine` | Dataloop (TheBloke) | ❌ Replace |
| `redhat2024ollama` | Red Hat Developer blog | ❌ Replace with academic/technical paper |
| `designveloper2024comparison` | DesignVeloper blog | ❌ Replace |
| `kanerika2024analysis` | Kanerika blog | ❌ Replace |
| `llava2024official` | LLaVA website | Duplicate of `neurips2023llava` — REPLACE with NeurIPS ref |

#### Suggested Replacements for Blog Posts:

| Replacing | Suggested Academic Source |
|-----------|--------------------------|
| `ibm2024gguf`, `e2e2024quantization`, `phison2024format`, `dataloop2024medicine` | Frantar et al. "GPTQ: Accurate Post-Training Quantization for Generative Pre-trained Transformers", ICLR 2023 — or — Dettmers et al. "QLoRA: Efficient Finetuning of Quantized LLMs", NeurIPS 2023 |
| `redhat2024ollama`, `designveloper2024comparison`, `kanerika2024analysis` | Kwon et al. "Efficient Memory Management for Large Language Model Serving with PagedAttention", SOSP 2023 (the vLLM paper) |
| `inc2024burnout`, `europe2024burnout` | Rotenstein et al. BMJ 2024 (`bmj2024impact`) already in bib — use that |
| `github2024kvasir` | Use `arxiv2024kvasir` (arXiv paper) instead |
| `microsoft2023llava`, `arxiv2023llava`, `llava2024official` | All duplicates of `neurips2023llava` — remove, keep one |
| `huggingface2024medgemma` | Yang et al. "MedGemma: A Family of Medical Foundation Models" — or reference via official Google technical report |
| `amia2024survey` | Replace with `jama2024burden` (already in bib) |

---

### 8. PPT Citations Used But Possibly Unchecked

These keys are used in the PPT but their entries look questionable:

| Key | Claimed Venue | Concern |
|-----|---------------|---------|
| `frontiers2024survey` | Frontiers in AI | Verify actual paper title matches use context |
| `acl2024clinical` | ACL Anthology | Verify "Leaman et al. 2024" entry is real — ACL 2024 clinical LLMs paper |
| `arxiv2024qageneration` | arXiv | "Wang, Xiaosong et al." — verify this is the intended paper for "automated medical QA generation" |
| `jmir2024instruction` | JMIR | "Zhang, Yuhao et al." — verify this paper exists |
| `pmc2024endovit` | PMC | Verify EndoViT attribution |

---

## MINOR DIFFERENCES

### 9. ECE Value
- Report: 0.042 ✅
- PPT: 0.042 ✅ (matches — no change needed)

### 10. Hardware Info
- Both match: A100 80GB, Intel Xeon Silver 4310, 125 GiB RAM, CUDA 12.9 ✅

### 11. Hyperparameters
- Both match: rank=64, alpha=32, dropout=0.05, epochs=4, lr=2e-4, batch=2, grad_accum=4 ✅

### 12. Training Time
- Both match: Stage 1 ~70h, Stage 2 ~15h ✅

### 13. Authors and Guides
- Both match: Praval Pattam, Theenesh Potluri, Pranav Sai Sarvepalli; guides Jayaraj PB, Srinivasa TM ✅

---

## SUMMARY PRIORITY LIST

| Priority | Item | Action |
|----------|------|--------|
| 🔴 HIGH | Scores wrong in PPT | Update 6 metric values in comparison table, accuracy table, inline text |
| 🔴 HIGH | LLaMA → Vicuna-7B-v1.5 | Rename frame + update text |
| 🔴 HIGH | PPT references not numbered | Change bib style to `unsrtnat` or `ieeetr` |
| 🟡 MEDIUM | 9+ blog-post refs | Replace with peer-reviewed equivalents (see table above) |
| 🟡 MEDIUM | Duplicate bib entries | Remove `microsoft2023llava`, `arxiv2023llava`, `llava2024official` |
| 🟡 MEDIUM | Stage 2 dataset numbers | Update 32,545 → ~31,200; 30,917 → 29,640; 1,627 → 1,560 |
| 🟡 MEDIUM | GMC name | Standardize to "Government Medical College, Calicut" |
| 🟢 LOW | Add disease list to PPT | Add to Dataset slide |
| 🟢 LOW | Verify questionable bib entries | Check acl2024clinical, jmir2024instruction, arxiv2024qageneration |
