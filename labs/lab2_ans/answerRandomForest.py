from numpy.random import rand
import mnist
from answerTree import *
import numpy as np
#import pdb

# 超参数
# TODO: You can change the hyperparameters here
num_tree = 128     # 树的数量
ratio_data = 1 - 1e-2   # 采样的数据比例
ratio_feat = 0.1 # 采样的特征比例
hyperparams = {"depth": 17, "purity_bound": 1 - 1.7e-1, "gainfunc": negginiDA} # 每颗树的超参数


def buildtrees(X, Y):
    """
    构建随机森林
    @param X: n*d, 每行是一个输入样本。 n: 样本数量， d: 样本的维度
    @param Y: n, 样本的label
    @return: List of DecisionTrees, 随机森林
    """
    # TODO: YOUR CODE HERE
    # 提示：整体流程包括样本扰动、属性扰动和预测输出
    trees = []

    n, d = X.shape
    num_samples = int(n * ratio_data)
    num_features = int(d * ratio_feat)
    
    for _ in range(num_tree):
        sample_indices = np.random.choice(n, num_samples, replace=True)
        X_sample = X[sample_indices]
        Y_sample = Y[sample_indices]

        feature_indices = np.random.choice(d, num_features, replace=False)
        X_sample = X_sample[:, feature_indices]

        tree = buildTree(X_sample, Y_sample, list(range(num_features)), hyperparams["depth"], hyperparams["purity_bound"], hyperparams["gainfunc"])
        tree.feature_indices = feature_indices # 要记录顺序，否则特征乱了，训练和测试不匹配就出问题了
        trees.append(tree)
    
    return trees

def infertrees(trees, X):
    """
    随机森林预测
    @param trees: 随机森林
    @param X: n*d, 每行是一个输入样本。 n: 样本数量， d: 样本的维度
    @return: n, 预测的label
    """
    pred = []
    for tree in trees:
        X_selected_features = X[tree.feature_indices]
        pred.append(inferTree(tree, X_selected_features))
    pred = list(filter(lambda x: not np.isnan(x), pred))
    upred, ucnt = np.unique(pred, return_counts=True)
    return upred[np.argmax(ucnt)]
