import numpy as np

# 超参数
# TODO: You can change the hyperparameters here
lr = 1 # 学习率
wd = 1e-5  # l2正则化项系数


def predict(X, weight, bias):
    """
    使用输入的weight和bias预测样本X是否为数字0
    @param X: n*d 每行是一个输入样本。n: 样本数量, d: 样本的维度
    @param weight: d*1
    @param bias: 1*1
    @return: wx+b
    """
    # TODO: YOUR CODE HERE
    return np.dot(X, weight) + bias # (n, )
        # dot: 矩阵乘列向量
        # +: element-wise(broadcast)

def sigmoid(x):
    x = np.clip(x, -709, 709)
        # 避免overflow
    return 1 / (np.exp(-x) + 1)
        # numpy的函数可以以矩阵为参数,进行element-wise mapping

def step(X, weight, bias, Y):
    """
    单步训练, 进行一次forward、backward和参数更新
    @param X: n*d 每行是一个训练样本。 n: 样本数量， d: 样本的维度
    @param weight: d*1
    @param bias: 1*1
    @param Y: n 样本的label, 1表示为数字0, -1表示不为数字0
    @return:
        haty: n 模型的输出, 为正表示数字为0, 为负表示数字不为0
        loss: 1*1 由交叉熵损失函数计算得到
        weight: d*1 更新后的weight参数
        bias: 1*1 更新后的bias参数
    """
    # TODO: YOUR CODE HERE   
    haty = predict(X, weight, bias) # (n, )
    p = sigmoid(haty * Y) # (n, )
        # 这里的乘法*是element-wise multiplication
    loss = -np.mean( np.log(p + 1e-15) ) # scalar
        #加一个小常数避免取到log0
    l2_regularization = wd * np.sum( np.square(weight) ) # scalar
    loss += l2_regularization
    
    temp = (1 - p) * Y # (n, )
    grad = -np.mean(temp.reshape(-1, 1) * X, axis=0) # (d, )
    b_grad = -np.mean(temp) # scalar
    
    weight -= lr * (grad + 2 * wd * weight)  # (d, )
    bias -= lr * b_grad  # scalar
    
    return haty, loss, weight, bias