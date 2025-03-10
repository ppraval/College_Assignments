import numpy as np
import pandas as pd

X_train = pd.read_csv(r"D:\Sem - 5\ML- LAB\Assg-7 - ANN\train_X.csv")
X_test = pd.read_csv(r'D:\Sem - 5\ML- LAB\Assg-7 - ANN\test_X.csv')
Y_train = pd.read_csv(r"D:\Sem - 5\ML- LAB\Assg-7 - ANN\train_label.csv")
Y_test = pd.read_csv(r"D:\Sem - 5\ML- LAB\Assg-7 - ANN\test_label.csv")

x_random = X_test.iloc[0:1, :]
y_random_test = Y_test.iloc[0:1, :]

x_random = x_random.to_numpy()
y_random_test = y_random_test.astype(int).to_numpy()

X_train = X_train.to_numpy()
X_test = X_test.to_numpy()

Y_train = Y_train.astype(int).to_numpy()
Y_test = Y_test.astype(int).to_numpy()


def initialize_parameters(layer_dims):
    np.random.seed(1)
    parameters = {}
    for l in range(1, len(layer_dims)):
        parameters['W' + str(l)] = np.random.randn(layer_dims[l], layer_dims[l-1]) * 0.01
        parameters['b' + str(l)] = np.zeros((layer_dims[l], 1))
    return parameters


def relu(x):
    return np.maximum(0, x)

def softmax(x):
    num = np.exp(x - np.max(x, axis=0, keepdims=True))
    den = num.sum(axis=0, keepdims=True)
    return num / den 


def forward_propagation(X, parameters):
    forward_cache = {}
    A = X.T
    L = len(parameters) // 2

    for l in range(1, L):
        Z = parameters['W' + str(l)].dot(A) + parameters['b' + str(l)]
        A = relu(Z)
        forward_cache['A' + str(l)] = A
        forward_cache['Z' + str(l)] = Z

    ZL = parameters['W' + str(L)].dot(A) + parameters['b' + str(L)]
    AL = softmax(ZL)
    forward_cache['A' + str(L)] = AL
    forward_cache['Z' + str(L)] = ZL

    return AL, forward_cache


def compute_cost(AL, Y):
    m = Y.shape[0]
    cost = -np.sum(Y.T * np.log(AL + 1e-8)) / m
    return np.squeeze(cost)

def relu_derivative(x):
    return x > 0

def backward_propagation(X, Y, caches, parameters):
    grads = {}
    L = len(parameters) // 2
    m = X.shape[0]
    Y = Y.T
    A_prev = X.T

    dZL = caches['A' + str(L)] - Y
    grads['dW' + str(L)] = dZL.dot(caches['A' + str(L-1)].T) / m
    grads['db' + str(L)] = np.sum(dZL, axis = 1, keepdims = True) / m

    for l in reversed(range(1, L)):
        dA = parameters['W' + str(l+1)].T.dot(dZL)
        dZ = dA * relu_derivative(caches['Z' + str(l)])
        A_prev = X.T if l == 1 else caches['A' + str(l-1)]
        grads['dW' + str(l)] = dZ.dot(A_prev.T) / m
        grads['db' + str(l)] = np.sum(dZ, axis = 1, keepdims = True) / m
        dZL = dZ

    return grads

def update_parameters(parameters, grads, learning_rate):
    L = len(parameters) // 2
    for l in range(1, L + 1):
        parameters['W' + str(l)] -= learning_rate * grads['dW' + str(l)]
        parameters['b' + str(l)] -= learning_rate * grads['db' + str(l)]
    return parameters


def model(X, Y, layer_dims, learning_rate=0.01, num_epochs=100):
    parameters = initialize_parameters(layer_dims)
    for epoch in range(num_epochs):
        AL, caches = forward_propagation(X, parameters)

        cost = compute_cost(AL, Y)

        grads = backward_propagation(X, Y, caches, parameters)

        parameters = update_parameters(parameters, grads, learning_rate)

        if epoch % 10 == 0:
            print(f"Cost after epoch {epoch}: {cost:.4f}")
    return parameters


def predict(X, parameters):
    AL, _ = forward_propagation(X, parameters)
    predictions = np.argmax(AL, axis=0)
    return predictions

def accuracy(predictions, Y):
    true_labels = np.argmax(Y, axis=1)
    return np.mean(predictions == true_labels)

layer_dims = [784, 64, 10]

parameters = model(X_train, Y_train, layer_dims, learning_rate=0.001, num_epochs=100)

train_preds = predict(X_train, parameters)
test_preds = predict(X_test, parameters)

print(f"Train accuracy: {accuracy(train_preds, Y_train) * 100:.2f}")
print(f"Test accuracy: {accuracy(test_preds, Y_test) * 100:.2f}")


pred = test_preds[25]
true_class = np.argmax(Y_test[25])
print(pred)
print(true_class)

random_test_pred = predict(x_random, parameters)
print(random_test_pred)
print("")
print(y_random_test)

