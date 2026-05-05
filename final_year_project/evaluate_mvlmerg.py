#!/usr/bin/env python3
"""
Evaluation script for MVLMERG endoscopy report generation.
Replicates: CIDEr, ROUGE-L, METEOR, BLEU-4, BERTScore
"""

import sys, math, subprocess
import numpy as np
from collections import Counter, defaultdict

# ── auto-install dependencies ─────────────────────────────────────────────────
for pkg in ["rouge-score", "bert-score", "nltk"]:
    subprocess.check_call([sys.executable, "-m", "pip", "install", pkg, "-q"])

import nltk
nltk.download("punkt",     quiet=True)
nltk.download("punkt_tab", quiet=True)
nltk.download("wordnet",   quiet=True)
nltk.download("omw-1.4",   quiet=True)

from rouge_score import rouge_scorer
from nltk.translate.bleu_score   import corpus_bleu, SmoothingFunction
from nltk.translate.meteor_score import meteor_score
from bert_score import score as bert_score_fn

# ── test data ─────────────────────────────────────────────────────────────────
# Add more (reference, hypothesis) pairs here to get corpus-level averages.
# Currently one representative sample is included.

REFERENCES = [
    (
        "Indication : UGI Bleed\n"
        "Scope passed upto : D2\n"
        "Premedication : Lignocaine spray\n"
        "Esophagus : Grade IIX3 Esophageal varices without RCS. EVL Done-5 bands deployed\n"
        "GE Junction : Normal\n"
        "Stomach :\n"
        "Fundus : Erythematous, edematous mucosa with mosaic pattern and hemorrhagic spots. GOV1F1 without RCS\n"
        "Body : Erythematous, edematous mucosa with mosaic pattern and hemorrhagic spots\n"
        "Antrum : Erythematous, edematous mucosa with mosaic pattern and hemorrhagic spots\n"
        "Pylorus : Normal\n"
        "Duodenum :\n"
        "D1 : Erythematous edematous mucosa noted\n"
        "D2 : Normal\n"
        "RUT : Not taken\n"
        "Biopsy : Not taken\n"
        "Impression : GRADE IIX3 ESOPHAGEAL VARICES WITH RCS EVL DONE SEVERE PHG PORTAL DUODENOPATHY"
    ),
]

HYPOTHESES = [
    (
        "Indication: CLD-UGIB\n"
        "Scope Passed Upto: D2\n"
        "Premedication: Lignocaine spray\n"
        "Esophagus: Normal\n"
        "Ge Junction: Lax at 38cm\n"
        "Stomach:\n"
        "Fundus: Edematous mucosa noted\n"
        "Body: Edematous mucosa noted\n"
        "Antrum: Erythematous mucosa noted\n"
        "Pylorus: Normal\n"
        "Duodenum: D1: Normal | D2: Normal\n"
        "Rut: Not taken\n"
        "Biopsy: Not taken\n"
        "Impression: GRADE II ESOPHAGEAL VARICES WITH RCS EVL DONE SEVERE PHG"
    ),
]

# ── helpers ───────────────────────────────────────────────────────────────────
def tokenize(text: str):
    return nltk.word_tokenize(text.lower())


# ── ROUGE-L ───────────────────────────────────────────────────────────────────
def compute_rouge_l(refs, hyps):
    scorer = rouge_scorer.RougeScorer(["rougeL"], use_stemmer=True)
    scores = [scorer.score(r, h)["rougeL"].fmeasure for r, h in zip(refs, hyps)]
    return float(np.mean(scores))


# ── METEOR ────────────────────────────────────────────────────────────────────
def compute_meteor(refs, hyps):
    scores = [
        meteor_score([tokenize(r)], tokenize(h))
        for r, h in zip(refs, hyps)
    ]
    return float(np.mean(scores))


# ── BLEU-4 ────────────────────────────────────────────────────────────────────
def compute_bleu4(refs, hyps):
    ref_list = [[tokenize(r)] for r in refs]
    hyp_list = [tokenize(h)  for h in hyps]
    sf = SmoothingFunction().method1
    return float(corpus_bleu(ref_list, hyp_list,
                             weights=(0.25, 0.25, 0.25, 0.25),
                             smoothing_function=sf))


# ── CIDEr-D ───────────────────────────────────────────────────────────────────
def _get_ngrams(toks, n):
    return [tuple(toks[i:i+n]) for i in range(len(toks) - n + 1)]


def compute_cider(refs, hyps, n_max=4):
    """
    CIDEr-D: TF-IDF weighted cosine similarity averaged over n=1..4.
    IDF is computed over the provided reference corpus with Laplace smoothing.
    """
    ref_toks = [tokenize(r) for r in refs]
    hyp_toks = [tokenize(h) for h in hyps]
    N = len(refs)

    # corpus document frequency (from references)
    doc_freq = defaultdict(int)
    for rtoks in ref_toks:
        for n in range(1, n_max + 1):
            for ng in set(_get_ngrams(rtoks, n)):
                doc_freq[ng] += 1

    def tfidf_vec(toks, n):
        ngrams = _get_ngrams(toks, n)
        counts = Counter(ngrams)
        length = max(len(ngrams), 1)
        vec = {}
        for ng, cnt in counts.items():
            if N == 1:
                # With a single sample IDF is always 0; fall back to TF-only.
                idf = 1.0
            else:
                # Laplace-smoothed IDF: log((N+1)/(df+1))
                idf = math.log((N + 1.0) / (doc_freq.get(ng, 0) + 1.0))
            vec[ng] = (cnt / length) * idf
        return vec

    def cosine(va, vb):
        common = set(va) & set(vb)
        num = sum(va[k] * vb[k] for k in common)
        den = math.sqrt(
            sum(v**2 for v in va.values()) *
            sum(v**2 for v in vb.values())
        )
        return num / den if den > 1e-12 else 0.0

    scores = []
    for rt, ht in zip(ref_toks, hyp_toks):
        per_n = [cosine(tfidf_vec(rt, n), tfidf_vec(ht, n)) for n in range(1, n_max + 1)]
        scores.append(float(np.mean(per_n)))

    return float(np.mean(scores))


# ── BERTScore ─────────────────────────────────────────────────────────────────
def compute_bertscore(refs, hyps):
    """Raw BERTScore F1 using roberta-large (no baseline rescaling)."""
    _, _, F1 = bert_score_fn(hyps, refs, lang="en",
                             rescale_with_baseline=False,
                             verbose=False)
    return float(F1.mean())


# ── main ──────────────────────────────────────────────────────────────────────
if __name__ == "__main__":
    print("=" * 65)
    print("  MVLMERG – Endoscopy Report Generation Metrics")
    print("=" * 65)

    results = {}

    print("\n[1/5] ROUGE-L     ...", end=" ", flush=True)
    results["ROUGE-L"]   = compute_rouge_l(REFERENCES, HYPOTHESES)
    print(f"{results['ROUGE-L']:.4f}")

    print("[2/5] METEOR      ...", end=" ", flush=True)
    results["METEOR"]    = compute_meteor(REFERENCES, HYPOTHESES)
    print(f"{results['METEOR']:.4f}")

    print("[3/5] BLEU-4      ...", end=" ", flush=True)
    results["BLEU-4"]    = compute_bleu4(REFERENCES, HYPOTHESES)
    print(f"{results['BLEU-4']:.4f}")

    print("[4/5] CIDEr       ...", end=" ", flush=True)
    results["CIDEr"]     = compute_cider(REFERENCES, HYPOTHESES)
    print(f"{results['CIDEr']:.4f}")

    print("[5/5] BERTScore   ... (downloading roberta-large on first run)", flush=True)
    results["BERTScore"] = compute_bertscore(REFERENCES, HYPOTHESES)
    print(f"      BERTScore = {results['BERTScore']:.4f}")

    print("\n" + "=" * 40)
    print(f"  {'Metric':<12}  {'Score':>10}")
    print("  " + "-" * 26)
    for m in ["CIDEr", "ROUGE-L", "METEOR", "BLEU-4", "BERTScore"]:
        print(f"  {m:<12}  {results[m]:>10.4f}")
    print("=" * 40)
