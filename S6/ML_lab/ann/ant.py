import pandas as pd
import numpy as np
from sklearn.preprocessing import OneHotEncoder
from sklearn.metrics import accuracy_score

train_data = pd.read_csv("C:\\Users\\SIDHARTHA\\Downloads\\archive (3)\\mnist_train.csv")
test_data = pd.read_csv("C:\\Users\\SIDHARTHA\\Downloads\\archive (3)\\mnist_test.csv")

X_train = train_data.drop('label', axis=1).values
y_train = train_data['label'].values.reshape(-1, 1)
X_test = test_data.drop('label', axis=1).values
y_test = test_data['label'].values.reshape(-1, 1)

encoder = OneHotEncoder(sparse_output=False)
y_train = encoder.fit_transform(y_train)
y_test = encoder.transform(y_test)

def softmax(X):
    exp_X = np.exp(X - np.max(X, axis=1, keepdims=True))
    return exp_X / np.sum(exp_X, axis=1, keepdims=True)

def relu(X):
    return np.maximum(0, X)

def relu_derivative(X):
    return (X > 0).astype(int)

def forward(X, weights, biases):
    act = [X]
    Zval = []
    for i in range(len(weights) - 1):
        Z = np.dot(act[-1], weights[i]) + biases[i]
        X = relu(Z)
        act.append(X)
        Zval.append(Z)
    Z1 = np.dot(act[-1], weights[-1]) + biases[-1]
    X = softmax(Z1)
    act.append(X)
    Zval.append(Z1)
    return act, Zval

def ann(layers, lr, epochs, X_train, y_train, X_test, y_test):
    weights = []
    biases = []
    for i in range(len(layers) - 1):
        w = np.random.randn(layers[i], layers[i + 1]) * 0.01
        weights.append(w)
        b = np.zeros((1, layers[i + 1]))
        biases.append(b)

    def backward(act, Z):
        m = X_train.shape[0]
        zder = []
        zder.append(act[-1] - y_train)
        for i in range(len(Z) - 1, 0, -1):
            dz = np.dot(zder[-1], weights[i].T) * relu_derivative(Z[i - 1])
            zder.append(dz)
        zder.reverse()

        for i in range(len(weights)):
            dw = np.dot(act[i].T, zder[i]) / m
            db = np.sum(zder[i], axis=0, keepdims=True) / m
            weights[i] -= lr * dw
            biases[i] -= lr * db

    for epoch in range(epochs):
        activations, Z_values = forward(X_train, weights, biases)
        backward(activations, Z_values)
        if epoch % 10 == 0:
            loss = -np.mean(y_train * np.log(activations[-1] + 1e-8))
            print(f"Epoch {epoch}, Loss: {loss}")

    activ, _ = forward(X_test, weights, biases)
    y_pred = np.argmax(activ[-1], axis=1)
    y_true = np.argmax(y_test, axis=1)
    print("Test Accuracy:", accuracy_score(y_true, y_pred))

    return weights, biases

layers = [784, 160, 100, 10]
lr = 0.01
epochs = 100

weights, biases = ann(layers, lr, epochs, X_train, y_train, X_test, y_test)

def predict_single_input(input_data, weights, biases):
    input_data = input_data.reshape(1, -1)
    activations, _ = forward(input_data, weights, biases)
    predictions = activations[-1]
    predicted_class = np.argmax(predictions, axis=1)
    return predicted_class[0]

input_index = 0
single_input = X_test[input_index]

predicted_class = predict_single_input(single_input, weights, biases)
true_class = np.argmax(y_test[input_index])

print(f"Predicted Class: {predicted_class}")
print(f"True Class: {true_class}")