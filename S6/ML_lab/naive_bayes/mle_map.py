import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix, precision_score, recall_score, f1_score, accuracy_score

# Load and preprocess
def load_data(path):
    df = pd.read_csv(path)
    df.drop(columns=['id'], inplace=True)
    df = df.drop(columns=[col for col in df.columns if 'Unnamed' in col])  # Drop NaN column
    df['diagnosis'] = df['diagnosis'].map({'M': 1, 'B': 0})
    return df

# Drop highly correlated features and visualize
def drop_highly_correlated(df, threshold=0.9):
    corr_matrix = df.drop(columns=['diagnosis']).corr()
    plt.figure(figsize=(12, 10))
    plt.title("Feature Correlation Heatmap")
    plt.imshow(corr_matrix, cmap='coolwarm', interpolation='none')
    plt.colorbar()
    plt.xticks(range(len(corr_matrix.columns)), corr_matrix.columns, rotation=90)
    plt.yticks(range(len(corr_matrix.columns)), corr_matrix.columns)
    plt.tight_layout()
    plt.show()

    upper = corr_matrix.where(np.triu(np.ones(corr_matrix.shape), k=1).astype(bool))
    to_drop = [column for column in upper.columns if any(upper[column].abs() > threshold)]
    print("Dropped features due to high correlation:", to_drop)
    return df.drop(columns=to_drop)

# Drop weakly correlated features with target
def drop_weak_correlation(df, target='diagnosis', threshold=0.5):
    corr_matrix = df.corr()
    corr_with_target = corr_matrix[target].drop(target)
    strong_features = corr_with_target[abs(corr_with_target) >= threshold].index.tolist()
    print("\nRetained features with |correlation| ≥ 0.5:")
    for feature in strong_features:
        print(f"  {feature}: corr = {corr_with_target[feature]:.4f}")
    return df[[target] + strong_features]

# Prior probabilities
def compute_priors(df):
    total = len(df)
    priors = {}
    print("\n📊 Prior Probabilities:")
    for label in [0, 1]:
        prior = len(df[df['diagnosis'] == label]) / total
        priors[label] = prior
        print(f"  Class {label} → Prior: {prior:.4f}")
    return priors

# MLE estimation
def estimate_mle(df):
    features = df.columns.drop('diagnosis')
    stats = {0: {}, 1: {}}
    print("\n📈 MLE Estimates (Mean & Variance):")
    for label in [0, 1]:
        subset = df[df['diagnosis'] == label]
        print(f"\nClass {label}:")
        for feature in features:
            mean = np.mean(subset[feature])
            var = np.var(subset[feature])
            stats[label][feature] = (mean, var)
            print(f"  {feature}: mean = {mean:.4f}, variance = {var:.4f}")
    return stats

# Gaussian likelihood
def gaussian_likelihood(x, mean, var):
    eps = 1e-6
    coeff = 1 / np.sqrt(2 * np.pi * (var + eps))
    exponent = np.exp(-((x - mean) ** 2) / (2 * (var + eps)))
    return coeff * exponent

# MLE classification
def classify_mle(sample, stats):
    likelihoods = {}
    for label in [0, 1]:
        likelihood = 1
        for feature in sample.index:
            mean, var = stats[label][feature]
            likelihood *= gaussian_likelihood(sample[feature], mean, var)
        likelihoods[label] = likelihood
    return max(likelihoods, key=likelihoods.get)

# MAP classification
def classify_map(sample, stats, priors):
    posteriors = {}
    for label in [0, 1]:
        likelihood = 1
        for feature in sample.index:
            mean, var = stats[label][feature]
            likelihood *= gaussian_likelihood(sample[feature], mean, var)
        posteriors[label] = likelihood * priors[label]
    return max(posteriors, key=posteriors.get)

# Evaluation
def evaluate_classifier(df, stats, priors=None, mode='MLE'):
    X = df.drop(columns=['diagnosis'])
    y_true = df['diagnosis'].values
    y_pred = []

    for _, row in X.iterrows():
        if mode == 'MLE':
            y_pred.append(classify_mle(row, stats))
        elif mode == 'MAP':
            y_pred.append(classify_map(row, stats, priors))

    cm = confusion_matrix(y_true, y_pred)
    acc = accuracy_score(y_true, y_pred)
    prec = precision_score(y_true, y_pred, zero_division=0)
    rec = recall_score(y_true, y_pred, zero_division=0)
    f1 = f1_score(y_true, y_pred, zero_division=0)

    print(f"\n📊 Evaluation ({mode}):")
    print("Confusion Matrix:\n", cm)
    print(f"Accuracy: {acc:.4f}")
    print(f"Precision: {prec:.4f}")
    print(f"Recall: {rec:.4f}")
    print(f"F1 Score: {f1:.4f}")

# Plot Gaussian fits
def plot_feature_distribution(df, feature):
    plt.figure(figsize=(8, 5))
    for label in [0, 1]:
        subset = df[df['diagnosis'] == label][feature]
        mean = np.mean(subset)
        var = np.var(subset)
        x = np.linspace(min(subset), max(subset), 100)
        y = gaussian_likelihood(x, mean, var)
        label_name = 'Malignant' if label == 1 else 'Benign'
        plt.plot(x, y, label=f"{label_name} Fit")
        plt.hist(subset, bins=20, alpha=0.3, density=True, label=f"{label_name} Data")
    plt.title(f"Gaussian Fit for {feature}")
    plt.xlabel(feature)
    plt.ylabel("Density")
    plt.legend()
    plt.tight_layout()
    plt.show()

# Run the full pipeline
if __name__ == "__main__":
    df = load_data("naive_bayes\data.csv")
    df = drop_highly_correlated(df, threshold=0.9)
    df = drop_weak_correlation(df, threshold=0.5)
    priors = compute_priors(df)
    stats = estimate_mle(df)

    evaluate_classifier(df, stats, mode='MLE')
    evaluate_classifier(df, stats, priors, mode='MAP')

    # Visualize selected feature
    plot_feature_distribution(df, 'radius_mean')