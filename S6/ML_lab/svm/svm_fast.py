import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import GridSearchCV, train_test_split
from sklearn.metrics import classification_report, accuracy_score, precision_score, recall_score, f1_score, roc_auc_score, matthews_corrcoef
from sklearn.decomposition import PCA

df = pd.read_csv("heart_disease.csv")
df.head()

df.replace('?', np.nan, inplace=True)
df = df.apply(pd.to_numeric, errors='coerce')

df.corr()

df.isnull().sum()

df['ca'].unique()
df['thal'].unique()

mode_thal = df['thal'].mode()[0]
mode_ca = df['ca'].mode()[0]

df['thal'].fillna(mode_thal,inplace=True)
df['ca'].fillna(mode_ca,inplace=True)

df['ca'].unique()
df['thal'].unique()

X = df[['cp', 'thalach', 'exang', 'oldpeak', 'ca', 'thal']]
y = df['Diseased']

scaler = StandardScaler()
X = scaler.fit_transform(X)
X = pd.DataFrame(X)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

param_grid_rbf = {'C': [0.1, 1, 10, 100], 'gamma': [0.01, 0.1, 1, 10]}
grid_search_rbf = GridSearchCV(SVC(kernel='rbf', probability=True, random_state=42), param_grid_rbf, cv=3)
grid_search_rbf.fit(X_train, y_train)
print("Best parameters for RBF kernel:", grid_search_rbf.best_params_)

best_svm_rbf = grid_search_rbf.best_estimator_
best_svm_rbf.fit(X_train, y_train)

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

pca = PCA(n_components=2)
X_train_pca = pca.fit_transform(X_train)

svm_rbf_pca = SVC(kernel='rbf', probability=True, random_state=42)
svm_rbf_pca.fit(X_train_pca, y_train)

def plot_decision_boundary(model, X, y, title):
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.01), np.arange(y_min, y_max, 0.01))

    Z = model.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)

    plt.figure(figsize=(8, 6))
    plt.contourf(xx, yy, Z, alpha=0.8, cmap=plt.cm.coolwarm)
    plt.scatter(X[:, 0], X[:, 1], c=y, edgecolors='k', cmap=plt.cm.coolwarm)
    plt.xlabel('Principal Component 1')
    plt.ylabel('Principal Component 2')
    plt.title(title)
    plt.show()

plot_decision_boundary(svm_rbf_pca, X_train_pca, y_train, 'Decision Boundary of SVM (RBF Kernel)')

param_grid_linear = {'C': [0.1, 1, 10, 100], 'gamma': [0.01, 0.1, 1, 10]}
grid_search_linear = GridSearchCV(SVC(kernel='linear', probability=True, random_state=42), param_grid_linear, cv=3)
grid_search_linear.fit(X_train, y_train)

print("Best parameters for Linear kernel:", grid_search_linear.best_params_)

best_svm_linear = grid_search_linear.best_estimator_
best_svm_linear.fit(X_train, y_train)

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

print("Classification Report for Best SVM (RBF Kernel):")
print(classification_report(y_test, y_pred_best_rbf))

print("Classification Report for Best SVM (Linear Kernel):")
print(classification_report(y_test, y_pred_best_linear))