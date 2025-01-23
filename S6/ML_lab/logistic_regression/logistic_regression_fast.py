import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sklearn as sk

iris = sk.datasets.load_iris()

X = iris.data
y = (iris.target == 2).astype(int)

df = pd.DataFrame(data=np.c_[X, y], columns=iris.feature_names + ['target'])
print(df.describe())

df.head()

df.isnull().sum()

X = df[['sepal length (cm)', 'sepal width (cm)', 'petal length (cm)', 'petal width (cm)']]
y = df['target']
X, y

target_true = y.sum()
target_false = len(y) - target_true

targets = ['True', 'False']
counts = [target_true, target_false]

plt.bar(targets, counts)
plt.title('Distribution of Target Variable')
plt.xlabel('Target')
plt.ylabel('Count')
plt.show()

X_train, X_test, y_train, y_test = sk.model_selection.train_test_split(X, y, test_size=0.2, random_state=42)

def sigmoid(z):
        return 1 / (1 + np.exp(-z))

def logloss(y_hat_sigmoid, y):
    m = len(y)
    cost = (-1 / m) * (np.dot(y, np.log(y_hat_sigmoid)) + np.dot((1 - y), np.log(1 - y_hat_sigmoid)))
    return cost

class LogisticRegression:
    def __init__(self, lr, num_iterations):
        self.w = None
        self.lr = lr
        self.num_iterations = num_iterations
        self.costs_list = []

    def add_bias(self, X):
        X_with_bais = np.c_[X, np.ones(X.shape[0])]
        return X_with_bais

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y
        self.w = np.zeros(self.X_train.shape[1])
        print("w = ", self.w)
        self.w, self.costs_list = self.gradient_descent(self.X_train, self.y_train, self.w)
        return self.w, self.costs_list

    def gradient_descent(self, X, y, w):
        costs = []
        m = len(y)
        for i in range(self.num_iterations):
            y_hat = X.dot(w)
            y_hat_sigmoid = sigmoid(y_hat)
            print("y hat sig = ", y_hat_sigmoid)
            error = y_hat_sigmoid - y
            w = w - (self.lr / m) * error.dot(X).T
            print("W at every iteration = ", w)
            costs.append(logloss(y_hat_sigmoid, y))
        return w, costs
    
    def predict(self, X):
        z = sigmoid(X.dot(self.w))
        # return [1 if i > 0.5 else 0 for i in z]
        return z > 0.5
    
logistic_regression = LogisticRegression(0.005, 20000)
# logistic_regression = LogisticRegression(0.01, 1000)

X_train = logistic_regression.add_bias(X_train)
X_train

costs_list = []
w, costs_list = logistic_regression.fit(X_train, y_train)
w

plt.plot(range(logistic_regression.num_iterations), costs_list)
plt.xlabel('Iterations')
plt.ylabel('Costs')
plt.show()

y_train_preds = logistic_regression.predict(X_train)

X_test = logistic_regression.add_bias(X_test)
y_test_preds = logistic_regression.predict(X_test)

def accuracy(y, y_pred):
    return np.mean(y == y_pred)

def precision(y, y_pred):
    tp = np.sum((y == 1) & (y_pred == 1))
    fp = np.sum((y == 0) & (y_pred == 1))
    return tp / (tp + fp)

def recall(y_true, y_pred):
    tp = np.sum((y_true == 1) & (y_pred == 1))
    fn = np.sum((y_true == 1) & (y_pred == 0))
    return tp / (tp + fn)

def f1_score(y_true, y_pred):
    p = precision(y_true, y_pred)
    r = recall(y_true, y_pred)
    return 2 * (p * r) / (p + r)

print("Training Accuracy: ", accuracy(y_train, y_train_preds))
print("Training Accuracy: ", accuracy(y_test, y_test_preds))

print("Training Precision: ", precision(y_train, y_train_preds))
print("Testing Precision: ", precision(y_test, y_test_preds))

print("Training Recall: ", recall(y_train, y_train_preds))
print("Testing Recall: ", recall(y_test, y_test_preds))

print("Training F1 Score: ", f1_score(y_train, y_train_preds))
print("Testing F1 Score: ", f1_score(y_test, y_test_preds))