import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_curve, auc, accuracy_score, precision_score, recall_score, f1_score
from matplotlib.colors import ListedColormap

# Load the dataset
df = pd.read_csv("logistic_regression/test/WineQT.csv")

# Select features and target variable
X = df[['volatile acidity', 'alcohol']]
y = (df['quality'] > 5).astype(int)

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Define sigmoid function
def sigmoid(z):
    return 1 / (1 + np.exp(-z))

# Define log loss function
def logloss(y_hat_sigmoid, y):
    m = len(y)
    cost = (-1 / m) * (np.dot(y, np.log(y_hat_sigmoid)) + np.dot((1 - y), np.log(1 - y_hat_sigmoid)))
    return cost

# Logistic Regression class
class LogisticRegression:
    def __init__(self, lr, num_iterations):
        self.w = None
        self.lr = lr
        self.num_iterations = num_iterations
        self.costs_list = []

    def add_bias(self, X):
        X_with_bias = np.c_[X, np.ones(X.shape[0])]
        return X_with_bias

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y
        self.w = np.zeros(self.X_train.shape[1])
        self.w, self.costs_list = self.gradient_descent(self.X_train, self.y_train, self.w)
        return self.w, self.costs_list

    def gradient_descent(self, X, y, w):
        costs = []
        m = len(y)
        for i in range(self.num_iterations):
            y_hat = X.dot(w)
            y_hat_sigmoid = sigmoid(y_hat)
            error = y_hat_sigmoid - y
            w = w - (self.lr / m) * error.dot(X).T
            costs.append(logloss(y_hat_sigmoid, y))
        return w, costs
    
    def predict(self, X):
        z = sigmoid(X.dot(self.w))
        return z > 0.5
    
# Initialize and train the logistic regression model
logistic_regression = LogisticRegression(0.1, 10000)
X_train = logistic_regression.add_bias(X_train)
w, costs_list = logistic_regression.fit(X_train, y_train)

# Plotting the cost function
plt.plot(range(logistic_regression.num_iterations), costs_list)
plt.xlabel('Iterations')
plt.ylabel('Costs')
plt.title('Cost Function')
plt.show()

# Predict on training and testing data
y_train_preds = logistic_regression.predict(X_train)
X_test = logistic_regression.add_bias(X_test)
y_test_preds = logistic_regression.predict(X_test)

# Print evaluation metrics
print("Training Accuracy: ", accuracy_score(y_train, y_train_preds))
print("Testing Accuracy: ", accuracy_score(y_test, y_test_preds))

print("Training Precision: ", precision_score(y_train, y_train_preds))
print("Testing Precision: ", precision_score(y_test, y_test_preds))

print("Training Recall: ", recall_score(y_train, y_train_preds))
print("Testing Recall: ", recall_score(y_test, y_test_preds))

print("Training F1 Score: ", f1_score(y_train, y_train_preds))
print("Testing F1 Score: ", f1_score(y_test, y_test_preds))

# Plot ROC curve
fpr, tpr, _ = roc_curve(y_test, logistic_regression.predict(X_test))
roc_auc = auc(fpr, tpr)

plt.figure()
plt.plot(fpr, tpr, color='darkorange', lw=2, label='ROC curve (area = %0.2f)' % roc_auc)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver Operating Characteristic')
plt.legend(loc="lower right")
plt.show()

# Generate two sample test cases and predict
sample_test_cases = np.array([
    [0.4, 9.0],  # Sample 1
    [1.0, 12.5]  # Sample 2
])

sample_test_cases = logistic_regression.add_bias(sample_test_cases)
sample_predictions = logistic_regression.predict(sample_test_cases)
print("Sample Predictions: ", sample_predictions)

# Plot decision boundary for classification
def plot_decision_boundary(X, y, model):
    X_set = X[:, :2]
    
    x_min, x_max = X_set[:, 0].min() - 1, X_set[:, 0].max() + 1
    y_min, y_max = X_set[:, 1].min() - 1, X_set[:, 1].max() + 1
    
    xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.01),
                         np.arange(y_min, y_max, 0.01))
    

    grid = np.c_[xx.ravel(), yy.ravel()]
    grid = model.add_bias(grid)

    Z = model.predict(grid)
    Z = Z.reshape(xx.shape)

    plt.contourf(xx, yy, Z, alpha=0.75, cmap=ListedColormap(('red', 'green')))
    plt.scatter(X_set[:, 0], X_set[:, 1], c=y, cmap=ListedColormap(('red', 'green')), edgecolors='k')
    
    plt.title('Logistic Regression Decision Boundary')
    plt.xlabel('Volatile Acidity')
    plt.ylabel('Alcohol')
    plt.legend()
    plt.show()

# Plotting decision boundary using only the first two features for visualization
plot_decision_boundary(X_test, y_test, logistic_regression)