import numpy as np
from sklearn.datasets import fetch_openml
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder

mnist = fetch_openml('mnist_784', version=1)
X, y = mnist["data"], mnist["target"]

y = y.astype(int)
X = X / 255.0

encoder = OneHotEncoder()
y = encoder.fit_transform(y.to_numpy().reshape(-1, 1)).toarray()

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

class ANN:
    def __init__(self, input_size, hidden_size, output_size):
        self.w1 = np.random.randn(input_size, hidden_size) * 0.01
        self.b1 = np.zeros((1, hidden_size))
        self.w2 = np.random.randn(hidden_size, output_size) * 0.01
        self.b2 = np.zeros((1, output_size))

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def softmax(self, x):
        exp_x = np.exp(x - np.max(x, axis=1, keepdims=True))
        return exp_x / np.sum(exp_x, axis=1, keepdims=True)

    def forward(self, X):
        self.z1 = np.dot(X, self.w1) + self.b1
        self.a1 = self.sigmoid(self.z1)
        self.z2 = np.dot(self.a1, self.w2) + self.b2
        self.a2 = self.softmax(self.z2)
        return self.a2

    def backward(self, X, y, output):
        m = X.shape[0]

        self.dz2 = output - y
        self.dw2 = np.dot(self.a1.T, self.dz2) / m
        self.db2 = np.sum(self.dz2, axis=0, keepdims=True) / m

        self.dz1 = np.dot(self.dz2, self.w2.T) * self.sigmoid_derivative(self.a1)
        self.dw1 = np.dot(X.T, self.dz1) / m
        self.db1 = np.sum(self.dz1, axis=0, keepdims=True) / m

    def update_parameters(self, learning_rate):
        self.w1 -= learning_rate * self.dw1
        self.b1 -= learning_rate * self.db1
        self.w2 -= learning_rate * self.dw2
        self.b2 -= learning_rate * self.db2

    def train(self, X, y, epochs, learning_rate, batch_size):
        m = X.shape[0]
        for epoch in range(epochs):
            for i in range(0, m, batch_size):
                X_batch = X[i:i + batch_size]
                y_batch = y[i:i + batch_size]

                output = self.forward(X_batch)
                self.backward(X_batch, y_batch, output)
                self.update_parameters(learning_rate)

            if epoch % 10 == 0:
                output = self.forward(X)
                loss = -np.sum(y * np.log(output + 1e-10)) / m
                print(f"Epoch {epoch}, Loss: {loss}")

    def predict(self, X):
        output = self.forward(X)
        return np.argmax(output, axis=1)

    def accuracy(self, X, y):
        predictions = self.predict(X)
        true_labels = np.argmax(y, axis=1)
        return np.mean(predictions == true_labels)
    
input_size = X_train.shape[1]
hidden_size = 128
output_size = y_train.shape[1]
ann = ANN(input_size, hidden_size, output_size)

epochs = 100
learning_rate = 0.1
batch_size = 32
ann.train(X_train, y_train, epochs, learning_rate, batch_size)

train_accuracy = ann.accuracy(X_train, y_train)
test_accuracy = ann.accuracy(X_test, y_test)
print(f"Training Accuracy: {train_accuracy:.4f}")
print(f"Test Accuracy: {test_accuracy:.4f}")

y_pred = ann.predict(X_test)
print("Sample Predictions:", y_pred[:10])