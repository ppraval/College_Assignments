import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix, roc_curve, auc

# Load and preprocess
def load_data(path):
    df = pd.read_csv(path)
    df.drop(columns=['id'], inplace=True)
    df['diagnosis'] = df['diagnosis'].map({'M': 1, 'B': 0})
    return df

# Drop features with absolute correlation < threshold
def drop_weak_correlation(df, target='diagnosis', threshold=0.5):
    corr_matrix = df.corr()
    corr_with_target = corr_matrix[target].drop(target)
    strong_features = corr_with_target[abs(corr_with_target) >= threshold].index.tolist()
    print("Retained features with |correlation| >= 0.5:", strong_features)
    return df[[target] + strong_features]

# Compute prior probabilities
def compute_priors(df):
    total = len(df)
    return {label: len(df[df['diagnosis'] == label]) / total for label in [0, 1]}

# Estimate mean and variance for each feature per class
def estimate_gaussian_params(df):
    features = df.columns.drop('diagnosis')
    stats = {0: {}, 1: {}}
    for label in [0, 1]:
        subset = df[df['diagnosis'] == label]
        for feature in features:
            vals = subset[feature].values
            mean = np.mean(vals)
            var = np.var(vals)
            stats[label][feature] = (mean, var)
    return stats

# Gaussian likelihood function
def gaussian(x, mean, var):
    eps = 1e-6  # to avoid division by zero
    coeff = 1 / np.sqrt(2 * np.pi * (var + eps))
    exponent = np.exp(-((x - mean) ** 2) / (2 * (var + eps)))
    return coeff * exponent

# Naive Bayes prediction
def predict(sample, stats, priors):
    posteriors = {}
    for label in [0, 1]:
        likelihood = 1
        for feature in sample.index:
            mean, var = stats[label][feature]
            likelihood *= gaussian(sample[feature], mean, var)
        posteriors[label] = likelihood * priors[label]
    return max(posteriors, key=posteriors.get)

# Evaluate classifier
def evaluate(df, stats, priors):
    y_true = df['diagnosis'].values
    X = df.drop(columns=['diagnosis'])
    y_pred = [predict(row, stats, priors) for _, row in X.iterrows()]
    cm = confusion_matrix(y_true, y_pred)
    TP, TN = cm[1, 1], cm[0, 0]
    FP, FN = cm[0, 1], cm[1, 0]
    acc = (TP + TN) / np.sum(cm)
    prec = TP / (TP + FP + 1e-6)
    rec = TP / (TP + FN + 1e-6)
    f1 = 2 * prec * rec / (prec + rec + 1e-6)
    print("Confusion Matrix:\n", cm)
    print(f"Accuracy: {acc:.4f}, Precision: {prec:.4f}, Recall: {rec:.4f}, F1: {f1:.4f}")
    return y_true, y_pred

# Plot ROC curve and calculate AUC
def plot_roc(df, stats, priors):
    y_true = df['diagnosis'].values
    X = df.drop(columns=['diagnosis'])
    scores = []
    for _, row in X.iterrows():
        prob = priors[1]
        for feature in X.columns:
            mean, var = stats[1][feature]
            prob *= gaussian(row[feature], mean, var)
        scores.append(prob)
    scores = np.array(scores)
    scores[np.isnan(scores)] = 0  # Replace NaNs with 0
    fpr, tpr, _ = roc_curve(y_true, scores)
    roc_auc = auc(fpr, tpr)
    plt.plot(fpr, tpr, label=f"AUC = {roc_auc:.4f}")
    plt.plot([0, 1], [0, 1], 'k--')
    plt.xlabel("False Positive Rate")
    plt.ylabel("True Positive Rate")
    plt.title("Naive Bayes ROC Curve")
    plt.legend()
    plt.show()

# Run the full pipeline
if __name__ == "__main__":
    df = load_data("naive_bayes/data.csv")
    df = drop_weak_correlation(df, threshold=0.5)
    priors = compute_priors(df)
    stats = estimate_gaussian_params(df)
    y_true, y_pred = evaluate(df, stats, priors)
    plot_roc(df, stats, priors)