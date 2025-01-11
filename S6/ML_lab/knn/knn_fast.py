import math
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

df = pd.read_table("knn/fruits_dataset.txt")

X = df[['width', 'height', 'mass']].values
y = df['fruit_label'].values

plt.scatter(df['width'], df['height'], c=df['fruit_label'], alpha=0.8)
plt.title("Fruit's Width vs Fruit's Height")
plt.xlabel("Width")
plt.ylabel("Height")
plt.show()

def distance(x, y):
    return math.sqrt(sum((x1 - y1) ** 2 for x1, y1 in zip(x, y)))

class KNN:
    def __init__(self, k):
        self.k = k

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y

    def pred(self, X):
        y_preds = [self.compute(x) for x in X]
        return y_preds
        
    def compute(self, x):
        distances = [distance(x, x_train) for x_train in self.X_train]
        first_k = sorted(range(len(distances)), key=lambda i : distances[i])[:self.k]
        first_k_labels = [self.y_train[i] for i in first_k]

        freq = {}
        for i in first_k_labels:
            freq.setdefault(i, 0)
            freq[i] += 1

        most_freq = max(freq.values())
        for i, j in freq.items():
            if j == most_freq:
                mode = i
        return mode
    
def accuracy(y_target, y_pred):
    return sum(y1 == y2 for y1, y2 in zip(y_pred, y_target)) / len(y_target)

knn = KNN(5)
knn.fit(X, y)
y_preds = knn.pred(X)
print(accuracy(y_preds, y))