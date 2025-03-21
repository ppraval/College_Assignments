import numpy as np
import pandas as pd
from sklearn.metrics import accuracy_score
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, confusion_matrix, roc_curve, auc, ConfusionMatrixDisplay

data=pd.read_csv(r"breast_cancer_data.csv")
data.head()
data.tail()
data.describe()
data.info()
data.isna().sum()
cor=data.corr()
corr=cor['diagnosis'].sort_values(ascending=False)
print(corr)
feat = data.drop(columns='diagnosis')

for feature in feat:
    sns.scatterplot(x=feature, y='diagnosis', data=data)
    plt.title(f'Scatter Plot: {feature} vs Diagnosis')
    plt.xlabel(feature)
    plt.ylabel('Diagnosis')
    plt.show()

features = data.columns

for feature in features:
    plt.figure(figsize=(6, 4))
    sns.histplot(data[feature], kde=True) 
    plt.title(f'Histogram: {feature}')  
    plt.xlabel(feature)  
    plt.ylabel('Frequency')
    plt.show() 

x = data.iloc[ : , :-1].values
y = data.iloc[ : , -1].values

x_train,x_test,y_train,y_test=train_test_split(x,y,test_size=0.20,random_state=42,shuffle=True)

print(np.unique(data['diagnosis'],return_counts=True))

def gini(y):
    cls,counts=np.unique(y,return_counts=True)
    gimpurity=1.0
    for count in counts:
        gimpurity-=(count/len(y))**2
    return gimpurity

def split(x,y,limit,feature):
    left=x[ :, feature]<=limit
    right=x[:, feature]>limit
    return x[left],x[right],y[left],y[right]

def bestsplit(x,y):
    bestgini=float('inf')
    bestsplit=None

    nooffeatures=x.shape[1]
    for features in range(nooffeatures):
        limits=np.unique(x[: ,features])
        for limit in limits:
            xleft,xright,yleft,yright=split(x,y,limit,features)

            if len(yleft)==0 or len(yright)==0:
                continue

            gleft=gini(yleft)
            gright=gini(yright)

            numerator=len(yleft)*gleft+len(yright)*gright
            terminator=len(yleft)+len(yright)
            weighted=numerator/terminator
            if weighted<bestgini:
                bestgini=weighted
                bestsplit={
                    'feature_index':features,
                    'limit':limit,
                    'xleft':xleft,
                    'xright':xright,
                    'yleft':yleft,
                    'yright':yright

                }

    return bestsplit



def build_tree(x, y, cur_depth = 0, max_depth = None, min_splits = 2):

    n, m = x.shape

    if cur_depth >= max_depth or len(np.unique(y)) == 1 or n < min_splits:
        return np.bincount(y).argmax()

    best_split = bestsplit(x, y)
    if best_split is None:
        return np.bincount(y).argmax()

    left_subtree = build_tree(best_split['xleft'], best_split['yleft'], cur_depth + 1, max_depth, min_splits)
    right_subtree = build_tree(best_split['xright'], best_split['yright'], cur_depth + 1, max_depth, min_splits)

    return {
        'feature_index' : best_split['feature_index'],
        'limit' : best_split['limit'],
        'left' : left_subtree,
        'right' : right_subtree
    }

def predict_sample(X, tree):
    if isinstance(tree, dict):
        feature_value = X[tree['feature_index']]
        if feature_value <= tree['limit']:
            return predict_sample(X, tree['left'])
        else:
            return predict_sample(X, tree['right'])
    else:
        return tree

def predict(X, tree):
    return [predict_sample(x, tree) for x in X]


tree = build_tree(x_train, y_train, 0, max_depth = 5, min_splits = 2)
y_pred = predict(x_test, tree)

accuracy = accuracy_score(y_test, y_pred)
precision = precision_score(y_test, y_pred)
recall = recall_score(y_test, y_pred)
conf_matrix = confusion_matrix(y_test, y_pred)
print(f'Accuracy : {accuracy : .4f}')
print(f'Precision : {precision : .4f}')
print(f'Recall : {recall : .4f}')
print(f'Confusion Matrix :\n{conf_matrix}')

sns.heatmap(conf_matrix,annot = True)
plt.title('Confusion Matrix')
plt.xlabel('Predicted')
plt.ylabel('Actual')
plt.show()

fpr, tpr, _ = roc_curve(y_test, y_pred)
roc_area = auc(fpr, tpr)
plt.figure(figsize = (4, 4))
plt.plot(fpr, tpr, label = f"ROC-CURVE-AREA : {roc_area : .4f}")
plt.plot([0, 1], [0, 1])
plt.title('ROC-CURVE')
plt.xlabel('False Positive Rates')
plt.ylabel('True Positive Rates')
plt.legend()
plt.show()

import numpy as np

new_sample = np.array([21.56, 22.39, 142.00, 1479.0, 0.11100])
new_sample = new_sample.reshape(1, -1)

predicted_class = predict(new_sample, tree) 
print(f"Predicted class for new input: {predicted_class[0]}")