import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split, RandomizedSearchCV
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_auc_score, matthews_corrcoef
from sklearn.inspection import permutation_importance

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

rf = RandomForestClassifier(random_state=42, oob_score=True)
rf.fit(X_train, y_train)

y_pred = rf.predict(X_test)
y_pred_proba = rf.predict_proba(X_test)[:, 1]

accuracy = accuracy_score(y_test, y_pred)
precision = precision_score(y_test, y_pred)
recall = recall_score(y_test, y_pred)
f1 = f1_score(y_test, y_pred)
auc = roc_auc_score(y_test, y_pred_proba)
mcc = matthews_corrcoef(y_test, y_pred)

print("Random Forest Performance:")
print(f"Accuracy: {accuracy:.4f}")
print(f"Precision: {precision:.4f}")
print(f"Recall: {recall:.4f}")
print(f"F1-Score: {f1:.4f}")
print(f"AUC: {auc:.4f}")
print(f"MCC: {mcc:.4f}")

param_dist = {'n_estimators': np.arange(50, 500, 50)}
random_search = RandomizedSearchCV(rf, param_distributions=param_dist, n_iter=5, cv=3, random_state=42)
random_search.fit(X_train, y_train)
print(f"Best n_estimators: {random_search.best_params_['n_estimators']}")

importances = rf.feature_importances_
feature_names = ['cp', 'thalach', 'exang', 'oldpeak', 'ca', 'thal']
feature_importance_df = pd.DataFrame({'Feature': feature_names, 'Importance': importances})
feature_importance_df = feature_importance_df.sort_values(by='Importance', ascending=False)
print("Feature Importance:")
print(feature_importance_df)

oob_score = rf.oob_score_
print(f"OOB Score: {oob_score:.4f}")

n_estimators_list = [50, 100, 200, 300, 400]
for n in n_estimators_list:
    rf = RandomForestClassifier(n_estimators=n, random_state=42)
    rf.fit(X_train, y_train)
    y_pred = rf.predict(X_test)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"n_estimators={n}, Accuracy: {accuracy:.4f}")