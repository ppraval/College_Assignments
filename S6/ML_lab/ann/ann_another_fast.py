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
    def __init__(self, layers):
        self.layers = layers
        self.weights = []
        self.biases = []

        for i in range(len(layers) - 1):
            self.weights.append(np.random.randn(layers[i], layers[i + 1]) * 0.01)
            self.biases.append(np.zeros((1, layers[i + 1])))

    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def softmax(self, x):
        exp_x = np.exp(x - np.max(x, axis=1, keepdims=True))
        return exp_x / np.sum(exp_x, axis=1, keepdims=True)

    def forward(self, X):
        self.activations = [X]
        self.z_values = []

        for i in range(len(self.weights)):
            z = np.dot(self.activations[-1], self.weights[i]) + self.biases[i]
            self.z_values.append(z)

            if i == len(self.weights) - 1:
                a = self.softmax(z)
            else: 
                a = self.sigmoid(z)

            self.activations.append(a)

        return self.activations[-1]

    def backward(self, X, y, output):
        m = X.shape[0]
        deltas = [output - y]

        for i in range(len(self.weights) - 1, 0, -1):
            delta = np.dot(deltas[0], self.weights[i].T) * self.sigmoid_derivative(self.activations[i])
            deltas.insert(0, delta)

        self.dw = []
        self.db = []

        for i in range(len(self.weights)):
            self.dw.append(np.dot(self.activations[i].T, deltas[i]) / m)
            self.db.append(np.sum(deltas[i], axis=0, keepdims=True) / m)

    def update_parameters(self, learning_rate):
        for i in range(len(self.weights)):
            self.weights[i] -= learning_rate * self.dw[i]
            self.biases[i] -= learning_rate * self.db[i]

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

# layers = [784, 160, 100, 10]
layers = [784, 160, 10]
ann = ANN(layers)

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