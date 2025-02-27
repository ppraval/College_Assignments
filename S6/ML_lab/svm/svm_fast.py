import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from sklearn.model_selection import GridSearchCV, train_test_split
from sklearn.metrics import classification_report, accuracy_score, precision_score, recall_score, f1_score, roc_auc_score, matthews_corrcoef
from sklearn.decomposition import PCA

# Load the dataset
df = pd.read_csv("heart_data.csv")
print(df.head())

# Check for missing values
print("Missing values:\n", df.isnull().sum())

# Select features and target
X = df[['cp', 'thalach', 'exang', 'oldpeak']]
y = df['target']

# Split the dataset
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Function to plot decision boundary
def plot_decision_boundary(model, X, y, title):
    """
    Plots the decision boundary of a model trained on 2D data.
    """
    # Create a meshgrid to plot the decision boundary
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.01), np.arange(y_min, y_max, 0.01))

    # Predict for each point in the meshgrid
    Z = model.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)

    # Plot the decision boundary
    plt.figure(figsize=(8, 6))
    plt.contourf(xx, yy, Z, alpha=0.8, cmap=plt.cm.coolwarm)
    plt.scatter(X[:, 0], X[:, 1], c=y, edgecolors='k', marker='o', cmap=plt.cm.coolwarm)
    plt.xlabel('Principal Component 1')
    plt.ylabel('Principal Component 2')
    plt.title(title)
    plt.show()

# 8. Hyperparameter tuning for RBF kernel using Grid Search
param_grid_rbf = {'C': [0.1, 1, 10, 100], 'gamma': [0.01, 0.1, 1, 10]}
grid_search_rbf = GridSearchCV(SVC(kernel='rbf', probability=True, random_state=42), param_grid_rbf, cv=3)
grid_search_rbf.fit(X_train, y_train)
print("Best parameters for RBF kernel:", grid_search_rbf.best_params_)

# 9. Hyperparameter tuning for Linear kernel using Grid Search
param_grid_linear = {'C': [0.1, 1, 10, 100]}
grid_search_linear = GridSearchCV(SVC(kernel='linear', probability=True, random_state=42), param_grid_linear, cv=3)
grid_search_linear.fit(X_train, y_train)
print("Best parameters for Linear kernel:", grid_search_linear.best_params_)

# Train the best RBF kernel model
best_svm_rbf = grid_search_rbf.best_estimator_
best_svm_rbf.fit(X_train, y_train)

# Train the best Linear kernel model
best_svm_linear = grid_search_linear.best_estimator_
best_svm_linear.fit(X_train, y_train)

# Evaluate the best RBF kernel model
y_pred_best_rbf = best_svm_rbf.predict(X_test)
y_pred_proba_best_rbf = best_svm_rbf.predict_proba(X_test)[:, 1]

accuracy_best_rbf = accuracy_score(y_test, y_pred_best_rbf)
precision_best_rbf = precision_score(y_test, y_pred_best_rbf)
recall_best_rbf = recall_score(y_test, y_pred_best_rbf)
f1_best_rbf = f1_score(y_test, y_pred_best_rbf)
auc_best_rbf = roc_auc_score(y_test, y_pred_proba_best_rbf)
mcc_best_rbf = matthews_corrcoef(y_test, y_pred_best_rbf)

print("Best SVM (RBF Kernel) Performance:")
print(f"Accuracy: {accuracy_best_rbf:.4f}")
print(f"Precision: {precision_best_rbf:.4f}")
print(f"Recall: {recall_best_rbf:.4f}")
print(f"F1-Score: {f1_best_rbf:.4f}")
print(f"AUC: {auc_best_rbf:.4f}")
print(f"MCC: {mcc_best_rbf:.4f}")

# Evaluate the best Linear kernel model
y_pred_best_linear = best_svm_linear.predict(X_test)
y_pred_proba_best_linear = best_svm_linear.predict_proba(X_test)[:, 1]

accuracy_best_linear = accuracy_score(y_test, y_pred_best_linear)
precision_best_linear = precision_score(y_test, y_pred_best_linear)
recall_best_linear = recall_score(y_test, y_pred_best_linear)
f1_best_linear = f1_score(y_test, y_pred_best_linear)
auc_best_linear = roc_auc_score(y_test, y_pred_proba_best_linear)
mcc_best_linear = matthews_corrcoef(y_test, y_pred_best_linear)

print("Best SVM (Linear Kernel) Performance:")
print(f"Accuracy: {accuracy_best_linear:.4f}")
print(f"Precision: {precision_best_linear:.4f}")
print(f"Recall: {recall_best_linear:.4f}")
print(f"F1-Score: {f1_best_linear:.4f}")
print(f"AUC: {auc_best_linear:.4f}")
print(f"MCC: {mcc_best_linear:.4f}")

# 7. Visualize decision boundary for the best RBF kernel model using PCA
pca = PCA(n_components=2)
X_train_pca = pca.fit_transform(X_train)
best_svm_rbf_pca = SVC(kernel='rbf', probability=True, random_state=42)
best_svm_rbf_pca.fit(X_train_pca, y_train)

# Plot decision boundary
plot_decision_boundary(best_svm_rbf_pca, X_train_pca, y_train, 'Decision Boundary of Best SVM (RBF Kernel)')

# 10. Classification report for the best RBF kernel model
print("Classification Report for Best SVM (RBF Kernel):")
print(classification_report(y_test, y_pred_best_rbf))

# Classification report for the best Linear kernel model
print("Classification Report for Best SVM (Linear Kernel):")
print(classification_report(y_test, y_pred_best_linear))