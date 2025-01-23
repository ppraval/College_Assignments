import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import StandardScaler

df = pd.read_csv("D:\College_Assignments\S6\ML_lab\linear_regression\ecommerce_customers.csv")
df.head()

df = df.drop(["Email", "Address", "Avatar"], axis=1)
df.head()

scaler = StandardScaler()

features = ['Avg. Session Length', 'Time on App', 'Time on Website', 'Length of Membership']
df[features] = scaler.fit_transform(df[features])

df.head()

df.corr()

def cost_mse(X, y, w):
    m = len(y)
    y_hat = X.dot(w)
    j = (1 / (2 * m)) * np.sum(np.square(y_hat - y))
    return j

class LinearRegression():
    def __init__(self, lr, num_iterations):
        self.w = None
        self.lr = lr
        self.num_iterations = num_iterations
        self.costs_list = []

    def add_bias(self, X):
        # X_with_bais = np.concatenate((X, np.ones((X.shape[0], 1))), axis=1)
        X_with_bais = np.c_[X, np.ones(X.shape[0])]
        return X_with_bais

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y
        self.w = np.zeros(self.X_train.shape[1])
        self.w, self.costs_list = self.gradient_descent(self.X_train, self.y_train, self.w)
        return self.w, self.costs_list
    
    def predict(self, X):
        return X.dot(self.w)
    
    def gradient_descent(self, X, y, w):
        costs = []
        m = len(y)
        for i in range(self.num_iterations):
            y_hat = X.dot(w)
            error = y_hat - y
            w = w - (self.lr / m) * error.dot(X).T
            # w = w - (lr / m) * X.T.dot(error)
            # print(w)
            # print(X)
            costs.append(cost_mse(X, y, w))
        return w, costs
    
X_single = df['Length of Membership']
y_single = df['Yearly Amount Spent']

lin_reg_single = LinearRegression(0.01, 1000)
X_single = lin_reg_single.add_bias(X_single)
X_single_train, X_single_test, y_single_train, y_single_test = train_test_split(X_single, y_single, test_size=0.2, random_state=42)
single_train_costs = []
w_single, single_train_costs = lin_reg_single.fit(X_single_train, y_single_train)
w_single

plt.plot(range(lin_reg_single.num_iterations), single_train_costs)
plt.xlabel('Iterations')
plt.ylabel('Cost Function Value')
plt.show()

plt.scatter(X_single_train[:, 0], y_single_train, color='blue', label='Training Data')

y_pred_train_single = lin_reg_single.predict(X_single_train)
y_pred_test_single = lin_reg_single.predict(X_single_test)

plt.plot(X_single_train[:, 0], y_pred_train_single, color='red', label='Regression Line')
plt.xlabel('Length of Membership')
plt.ylabel('Yearly Amount Spent')
plt.title('Linear Regression Line')
plt.legend()
plt.show()

X_double = df[['Time on App', 'Length of Membership']]
y_double = df['Yearly Amount Spent']

lin_reg_double = LinearRegression(0.01, 1000)
X_double = lin_reg_double.add_bias(X_double)
X_double_train, X_double_test, y_double_train, y_double_test = train_test_split(X_double, y_double, test_size=0.2, random_state=42)
double_train_costs = []
w_double, double_train_costs = lin_reg_double.fit(X_double_train, y_double_train)
w_double

plt.plot(range(lin_reg_double.num_iterations), double_train_costs)
plt.xlabel('Iterations')
plt.ylabel('Cost Function Value')
plt.show()

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(X_double_train[:, 0], X_double_train[:, 1], y_double_train, color='blue')

y_pred_train_double = lin_reg_double.predict(X_double_train)
y_pred_test_double = lin_reg_double.predict(X_double_test)

ax.plot_trisurf(X_double_train[:, 0], X_double_train[:, 1], y_pred_train_double, color='red', alpha=0.5)
ax.set_xlabel('Time on App')
ax.set_ylabel('Length of Membership')
ax.set_zlabel('Yearly Amount Spent')
plt.show()

mse_train_single = mean_squared_error(y_single_train, y_pred_train_single)
mse_test_single = mean_squared_error(y_single_test, y_pred_test_single)

mse_train_double = mean_squared_error(y_double_train, y_pred_train_double)
mse_test_double = mean_squared_error(y_double_test, y_pred_test_double)

print('Single Feature (Length of Membership) - Training Mean Squared Error:', mse_train_single)
print('Single Feature (Length of Membership) - Test Mean Squared Error:', mse_test_single)

print('Double Features (Length of Membership, Time on App) - Training Mean Squared Error:', mse_train_double)
print('Double Features (Length of Membership, Time on App) - Test Mean Squared Error:', mse_test_double)

# sample_data_single = np.array([4])
sample_data_single = np.array([0.550107])
sample_data_single = lin_reg_single.add_bias(sample_data_single)

# sample_data_single = np.array([12, 4])
sample_data_double = np.array([[0.607280, 0.550107]])
sample_data_double = lin_reg_double.add_bias(sample_data_double)

predictions_single = lin_reg_single.predict(sample_data_single)
predictions_double = lin_reg_double.predict(sample_data_double)

print('Single Feature Predictions:', predictions_single)
print('double Features Predictions:', predictions_double)