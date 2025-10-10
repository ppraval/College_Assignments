# plots.py
import os
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

sns.set(context="talk", style="whitegrid", font_scale=1.0)
PALETTE = {"LRU": "#1f77b4", "MRU": "#ff7f0e"}

# -----------------------------
# Raw data from your simulations
# -----------------------------
metrics = [
    {"benchmark": "Alpha_test_fmath", "policy": "LRU", "cycles": 96116, "ipc": 0.1883,
     "l1d_miss": 0.0324, "l1i_miss": 0.0155, "l2_miss": 0.9914},
    {"benchmark": "Alpha_test_fmath", "policy": "MRU", "cycles": 138368, "ipc": 0.1308,
     "l1d_miss": 0.0492, "l1i_miss": 0.0367, "l2_miss": 0.6658},
    {"benchmark": "PISA_test_lswlr", "policy": "LRU", "cycles": 59237, "ipc": 0.1222,
     "l1d_miss": 0.0547, "l1i_miss": 0.0282, "l2_miss": 1.0000},
    {"benchmark": "PISA_test_lswlr", "policy": "MRU", "cycles": 70065, "ipc": 0.1033,
     "l1d_miss": 0.0601, "l1i_miss": 0.0384, "l2_miss": 0.9099},
]

outdir = "plots"
os.makedirs(outdir, exist_ok=True)

# -----------------------------
# Normalization helpers
# -----------------------------
def normalize(bm, key):
    bm_entries = [m for m in metrics if m["benchmark"] == bm]
    lru = next(m for m in bm_entries if m["policy"] == "LRU")[key]
    mru = next(m for m in bm_entries if m["policy"] == "MRU")[key]
    return {"benchmark": bm, "LRU": 1.0, "MRU": mru / lru}

norm_cycles = [normalize("Alpha_test_fmath", "cycles"),
               normalize("PISA_test_lswlr", "cycles")]
norm_ipc = [normalize("Alpha_test_fmath", "ipc"),
            normalize("PISA_test_lswlr", "ipc")]

def abs_rates(key):
    out = []
    for bm in ["Alpha_test_fmath", "PISA_test_lswlr"]:
        lru = next(m for m in metrics if m["benchmark"] == bm and m["policy"] == "LRU")[key]
        mru = next(m for m in metrics if m["benchmark"] == bm and m["policy"] == "MRU")[key]
        out.append({"benchmark": bm, "LRU": lru, "MRU": mru})
    return out

abs_l1d = abs_rates("l1d_miss")
abs_l1i = abs_rates("l1i_miss")
abs_l2 = abs_rates("l2_miss")

# -----------------------------
# Plotting functions
# -----------------------------
def bar_normalized(title, norm_list, ylabel, filename):
    labels = [d["benchmark"] for d in norm_list]
    lru_vals = [d["LRU"] for d in norm_list]
    mru_vals = [d["MRU"] for d in norm_list]

    x = np.arange(len(labels))
    width = 0.35

    plt.figure(figsize=(8, 5))
    plt.bar(x - width/2, lru_vals, width, label="LRU", color=PALETTE["LRU"])
    plt.bar(x + width/2, mru_vals, width, label="MRU", color=PALETTE["MRU"])
    plt.xticks(x, labels)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(outdir, filename), dpi=200)
    plt.close()

def grouped_abs(title, series_list, ylabel, filename):
    labels = [d["benchmark"] for d in series_list]
    lru_vals = [d["LRU"] for d in series_list]
    mru_vals = [d["MRU"] for d in series_list]

    x = np.arange(len(labels))
    width = 0.35

    plt.figure(figsize=(8, 5))
    plt.bar(x - width/2, lru_vals, width, label="LRU", color=PALETTE["LRU"])
    plt.bar(x + width/2, mru_vals, width, label="MRU", color=PALETTE["MRU"])
    plt.xticks(x, labels)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(outdir, filename), dpi=200)
    plt.close()

def line_abs(title, series_list, ylabel, filename):
    labels = [d["benchmark"] for d in series_list]
    lru_vals = [d["LRU"] for d in series_list]
    mru_vals = [d["MRU"] for d in series_list]

    plt.figure(figsize=(8, 5))
    plt.plot(labels, lru_vals, marker="o", label="LRU", color=PALETTE["LRU"])
    plt.plot(labels, mru_vals, marker="o", label="MRU", color=PALETTE["MRU"])
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.tight_layout()
    plt.savefig(os.path.join(outdir, filename), dpi=200)
    plt.close()

def radar_chart(benchmark, filename):
    categories = ["Cycles(norm)", "IPC(norm)", "L1D miss(norm)", "L1I miss(norm)", "L2 miss(norm)"]
    N = len(categories)

    # Angles
    angles = np.linspace(0, 2*np.pi, N, endpoint=False).tolist()
    angles += angles[:1]

    # Normalized values (LRU baseline = 1.0)
    bm_entries = [m for m in metrics if m["benchmark"] == benchmark]
    lru = next(m for m in bm_entries if m["policy"] == "LRU")
    mru = next(m for m in bm_entries if m["policy"] == "MRU")

    lru_vals = [1.0, 1.0, 1.0, 1.0, 1.0]
    mru_vals = [
        mru["cycles"] / lru["cycles"],
        mru["ipc"] / lru["ipc"],
        mru["l1d_miss"] / lru["l1d_miss"],
        mru["l1i_miss"] / lru["l1i_miss"],
        mru["l2_miss"] / lru["l2_miss"],
    ]

    # Close the loop
    lru_vals += lru_vals[:1]
    mru_vals += mru_vals[:1]

    fig, ax = plt.subplots(figsize=(6, 6), subplot_kw=dict(polar=True))
    ax.plot(angles, lru_vals, 'o-', linewidth=2, label=f'{benchmark} LRU', color=PALETTE["LRU"])
    ax.fill(angles, lru_vals, alpha=0.25, color=PALETTE["LRU"])
    ax.plot(angles, mru_vals, 'o-', linewidth=2, label=f'{benchmark} MRU', color=PALETTE["MRU"])
    ax.fill(angles, mru_vals, alpha=0.25, color=PALETTE["MRU"])

    ax.set_xticks(angles[:-1])
    ax.set_xticklabels(categories)
    ax.set_title(f"Radar chart: {benchmark} (normalized)")
    ax.legend(loc='upper right', bbox_to_anchor=(1.3, 1.1))

    plt.tight_layout()
    plt.savefig(os.path.join(outdir, filename), dpi=200)
    plt.close()

# -----------------------------
# Generate all plots
# -----------------------------
bar_normalized("Normalized execution cycles (LRU=1.0)", norm_cycles, "Normalized cycles", "norm_cycles.png")
bar_normalized("Normalized IPC (LRU=1.0)", norm_ipc, "Normalized IPC", "norm_ipc.png")
grouped_abs("Absolute L1D miss rate", abs_l1d, "Miss rate", "abs_l1d.png")
grouped_abs("Absolute L1I miss rate", abs_l1i, "Miss rate", "abs_l1i.png")
line_abs("L2 miss rate comparison", abs_l2, "Miss rate", "l2_miss.png")
radar_chart("Alpha_test_fmath", "radar_alpha.png")
radar_chart("PISA_test_lswlr", "radar_pisa.png")

print(f"Saved plots to ./{outdir}/")