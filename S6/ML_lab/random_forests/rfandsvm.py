import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split 
from sklearn.metrics import accuracy_score,precision_score,f1_score,roc_auc_score,matthews_corrcoef,recall_score
from sklearn.ensemble import RandomForestClassifier
import matplotlib.pyplot as plt

data=pd.read_csv(r"C:\Users\Theenesh\Downloads\Heart_Disease .csv")

data.head()

data.tail()

data.isnull().sum()

data['ca'].unique()

data.isnull().sum()

data.replace('?',np.nan,inplace=True)

modethal=data['thal'].mode()[0]
modeca=data['ca'].mode()[0]
data['thal'].fillna(modethal,inplace=True)
data['ca'].fillna(modeca,inplace=True)


data.isnull().sum()
data['thal'] = data['thal'].astype('int64')
data['ca'] = data['ca'].astype('int64')


data.info()
data.describe()


x=data.iloc[:,:-1].values
y=data.iloc[:,-1].values


x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.20,random_state=42,shuffle=True)

rfclassifier=RandomForestClassifier(n_estimators=10000,random_state=42)
rfclassifier.fit(x_train,y_train)
y_pred=rfclassifier.predict(x_test)
y_pred_proba = rfclassifier.predict_proba(x_test)[:, 1]


accuracy = accuracy_score(y_test, y_pred)
precision = precision_score(y_test, y_pred)
recall = recall_score(y_test, y_pred)
f1 = f1_score(y_test, y_pred)
auc = roc_auc_score(y_test, y_pred_proba)
mcc = matthews_corrcoef(y_test, y_pred)


print(f"Accuracy: {accuracy:.4f}")
print(f"Precision: {precision:.4f}")
print(f"Recall: {recall:.4f}")
print(f"F1-Score: {f1:.4f}")
print(f"AUC: {auc:.4f}")
print(f"MCC: {mcc:.4f}")

from sklearn.model_selection import RandomizedSearchCV
paramgrid={
    'n_estimators':np.arange(10,200,10)    
}


rfclassifier=RandomForestClassifier(random_state=42)
randomsearch=RandomizedSearchCV(estimator=rfclassifier, param_distributions=paramgrid,n_iter=10,cv=5)
randomsearch.fit(x_train,y_train)
print(f"Best n_estimators: {randomsearch.best_params_['n_estimators']}")
print(f"Best Accuracy: {randomsearch.best_score_:.4f}")
rfclassifier.fit(x_train, y_train)
importance = rfclassifier.feature_importances_
feature_names = data.columns[:-1]  
feature_importances = pd.DataFrame({
    'Features': feature_names,
    'Importance': importance
})


feature_importances = feature_importances.sort_values(by='Importance', ascending=False)
print(feature_importances)
feature_importances.head()
plt.figure(figsize=(15,6))
plt.bar(feature_importances['Features'],feature_importances['Importance'])
plt.show()
xx = data.iloc[:, :-1].values 
yy = data.iloc[:, -1].values
rf_classifier = RandomForestClassifier(n_estimators=10000, oob_score=True,random_state=42)
rf_classifier.fit(xx, yy)
oob_score = rf_classifier.oob_score_
print(f"OOB Score:{oob_score:.4f}")


n_estimators_range = [10, 50, 100, 150, 200]
results = {}
for n in n_estimators_range:
    rf_classifier = RandomForestClassifier(n_estimators=n, random_state=42)
    rf_classifier.fit(x_train, y_train)
    y_pred = rf_classifier.predict(x_test)
    accuracy = accuracy_score(y_test, y_pred)
    results[n] = accuracy

for n, accuracy in results.items():
    print(f"n_estimators: {n}, Accuracy: {accuracy:.4f}")

plt.figure(figsize=(10, 6))
plt.plot(list(results.keys()), list(results.values()),marker='o')
plt.xlabel('n_estimators')
plt.ylabel('Accuracy')
plt.title('Random Forest Performance vs Number of Trees')
#plt.grid(True)
plt.show()



############################SVM###############################################################

from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
scaler=StandardScaler()
cor=data.corr()
corr=cor['Diseased'].sort_values(ascending=False)
print(corr)


columns_to_remove = ['thalach']
data_reduced = data.drop(columns=columns_to_remove)
x = data_reduced.iloc[:, :-1].values  
y = data_reduced.iloc[:, -1].values 


x=scaler.fit_transform(x)
data_reduced.head()
xtrain, xtest, ytrain, ytest = train_test_split(x, y, test_size=0.2, random_state=42)
svmclassifier=SVC(kernel='rbf',probability=True,random_state=42)
svmclassifier.fit(xtrain,ytrain)
ypred=svmclassifier.predict(xtest)
yprob=svmclassifier.predict_proba(xtest)[:, 1] 
print(f"accuracy_score: {accuracy_score(ytest,ypred): 0.2f}")
print(f"precision_score: {precision_score(ytest,ypred): 0.2f}")
print(f"f1_score: {f1_score(ytest,ypred): 0.2f}")
print(f"recall_score: {recall_score(ytest,ypred): 0.2f}")
print(f"matthews_corrcoef: {matthews_corrcoef(ytest,ypred): 0.2f}")
print(f"roc_auc_score: {roc_auc_score(ytest,yprob): 0.2f}")



data_reduced['thal'] = data_reduced['thal'].astype('int64')
data_reduced['ca'] = data_reduced['ca'].astype('int64')


data.info()

X_train_selected = xtrain[:, [11, 10]]  # Select 'thal' and 'ca'
X_test_selected = xtest[:, [11, 10]]    
g=SVC(kernel='rbf',random_state=12345)
g.fit(X_train_selected, ytrain)

x1 = np.linspace(X_train_selected[:, 0].min(), X_train_selected[:, 0].max(), 50)
x2 = np.linspace(X_train_selected[:, 1].min(), X_train_selected[:, 1].max(), 50)
xxx, yyy = np.meshgrid(x1, x2)
z = g.predict(np.c_[xxx.ravel(), yyy.ravel()])
z = z.reshape(xxx.shape)
plt.contourf(xxx, yyy, z, cmap=plt.cm.coolwarm, alpha=0.7)
plt.scatter(X_train_selected[:, 0], X_train_selected[:, 1], c=y_train, cmap=plt.cm.coolwarm, edgecolors="k")
plt.xlabel("Thal")
plt.ylabel("CA")
plt.title("SVM Decision Boundary for Thal vs CA")
plt.show()

from sklearn.model_selection import GridSearchCV

# Define the parameter grid
param_grid = {
    'C': [0.1, 1, 10, 100], 
    'gamma': [0.01, 0.1, 1, 10]  
}

grid_search = GridSearchCV(SVC(kernel='rbf', random_state=42), param_grid, cv=5, scoring='accuracy')
grid_search.fit(xtrain, ytrain)
print(f"Best Parameters: {grid_search.best_params_}")
print(f"Best Accuracy: {grid_search.best_score_:.4f}")



svm_linear = SVC(kernel='linear', C=grid_search.best_params_['C'], random_state=42)
svm_linear.fit(xtrain, ytrain)
y_pred_linear = svm_linear.predict(xtest) 
svm_rbf = SVC(kernel='rbf', C=grid_search.best_params_['C'], gamma=grid_search.best_params_['gamma'], random_state=42)
svm_rbf.fit(xtrain, ytrain)
y_pred_rbf = svm_rbf.predict(xtest)
def evaluate_model(y_true, y_pred):
    return {
        'Accuracy': accuracy_score(y_true, y_pred),
        'Precision': precision_score(y_true, y_pred),
        'Recall': recall_score(y_true, y_pred),
        'F1-Score': f1_score(y_true, y_pred),
        'AUC': roc_auc_score(y_true, y_pred),
        'MCC': matthews_corrcoef(y_true, y_pred)
    }
results_linear = evaluate_model(ytest, y_pred_linear)
results_rbf = evaluate_model(ytest, y_pred_rbf)
print("Linear Kernel Results:", results_linear)
print("RBF Kernel Results:", results_rbf)


from sklearn.metrics import classification_report

report = classification_report(y_test, y_pred_rbf, target_names=['No Heart Disease', 'Heart Disease'])
print("Classification Report:\n", report)