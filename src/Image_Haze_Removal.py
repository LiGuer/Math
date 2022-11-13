import cv2
import math
import numpy as np
import sys

# 计算区域最暗通道值
#   \min_{y\in \Omega(x)} ( \min_{c\in \{R,G,B\}} I_c(y)
def DarkChannel(img, size):
    [b, g, r]   = cv2.split(img)
    darkChannel = cv2.min(cv2.min(r, g), b)     # 取RGB最暗值
    kernel  = cv2.getStructuringElement(cv2.MORPH_RECT, (size, size))   # 局部核
    darkImg = cv2.erode(darkChannel, kernel)    # 求局部最小值
    return darkImg

# 估计大气光强度 A
def AtmLight(img, darkImg):
    [h, w]  = img.shape[:2]
    imgSize = h * w
    
    numpx   = int(max(math.floor(imgSize / 1000), 1))
    darkvec = darkImg.reshape(imgSize)
    imgvec  = img.reshape(imgSize, 3)
    indices = darkvec.argsort()
    indices = indices[imgSize - numpx::]

    atmSum = np.zeros([1,3])
    for ind in range(1, numpx):
       atmSum = atmSum + imgvec[indices[ind]]

    A = atmSum / numpx
    return A

# 估计折射率 
#   t = 1 - \min_{y\in \Omega(x)} ( \min_{c\in \{R,G,B\}} \frac{I_c(y)}{A_c} )
def TransmissionEstimate(img, A, size):
    transmission = np.empty(img.shape, img.dtype)

    for ind in range(0, 3):
        transmission[:, :, ind] = img[:, :, ind] / A[0, ind]

    transmission = 1 - DarkChannel(transmission, size)
    return transmission

# 导向滤波
def Guidedfilter(img, p, r, eps):
    mean_I  = cv2.boxFilter(img, cv2.CV_64F, (r,r))
    mean_p  = cv2.boxFilter(p, cv2.CV_64F, (r,r))
    mean_Ip = cv2.boxFilter(img * p, cv2.CV_64F, (r,r))
    cov_Ip  = mean_Ip - mean_I * mean_p

    mean_II = cv2.boxFilter(img * img, cv2.CV_64F, (r,r))
    var_I   = mean_II - mean_I*mean_I

    a = cov_Ip / (var_I + eps)
    b = mean_p - a * mean_I

    mean_a = cv2.boxFilter(a,cv2.CV_64F,(r,r))
    mean_b = cv2.boxFilter(b,cv2.CV_64F,(r,r))

    q = mean_a * img + mean_b
    return q

# 折射率柔化
def TransmissionRefine(img, t):
    return Guidedfilter(
        np.float64(cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)) / 0xFF, 
        t, r = 60, eps = 0.0001
    )

# 图像去雾
#   $J(x) = \frac{I(x) - A}{t(x)} + A$
def HazeRemoval(img, t, A):
    res = np.empty(img.shape, img.dtype)

    for c in range(0, 3):
        res[:, :, c] = (img[:, :, c] - A[0, c]) / t + A[0, c]

    return res

if __name__ == '__main__':
    # input
    src = cv2.imread(sys.argv[1])
    size = int(sys.argv[2])

    # HazeRemoval
    I = src.astype('float64') / 0xFF
    dark = DarkChannel(I, size)
    A = AtmLight(I, dark)
    t = TransmissionEstimate(I, A, size)
    t = TransmissionRefine(src, t)
    #t = np.full(np.shape(t), 0.5)
    J = HazeRemoval(I, t, A)

    #output
    cv2.imwrite("dark.png",dark * 0xFF)
    cv2.imwrite("t.png",t * 0xFF)
    cv2.imwrite("J.png",J * 0xFF)
    cv2.waitKey()