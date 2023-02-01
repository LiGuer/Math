import numpy as np 

def LinearRegression(X, y):
    w = np.matmul(np.linalg.pinv(X), y)
    return w

def GaussProcessRegression(x, train_x, train_y, kernel):
    x = np.asarray(x).reshape(-1, 1)
    train_x = np.asarray(train_x).reshape(-1, 1)
    train_y = np.asarray(train_y).reshape(-1, 1)
    
    Kff = kernel(train_x, train_x)  # (N, N)
    Kyy = kernel(x, x)  # (k, k)
    Kfy = kernel(train_x, x)  # (N, k)
    Kff_inv = np.linalg.inv(Kff + 1e-8 * np.eye(len(train_x)))  # (N, N)
    
    mu  = Kfy.T.dot(Kff_inv).dot(train_y)
    cov = Kyy - Kfy.T.dot(Kff_inv).dot(Kfy)
    return mu, cov