import mnist
from autograd.BaseGraph import Graph
import autograd.BaseNode as BaseNode
import numpy as np
import pickle
from autograd.utils import PermIterator
from util import setseed
from autograd.Init import * 
#import time
#import pdb

# Hyperparameters
# TODO: You can change the hyperparameters here
lr = 3e-4   # Learning rate
wd1 = 1e-4  # L1 regularization
wd2 = 7e-4  # L2 regularization
batchsize = 128


def buildGraph(Y):
    """
    建图
    @param Y: n 样本的label
    @return: Graph类的实例, 建好的图
    """
    
    # TODO: design your architecture here
    nodes = [
        BaseNode.Conv2D(input_channels=1, output_channels=32, kernel_size=3),
        BaseNode.MyBatchNorm(indim=32),
        BaseNode.relu(),
        BaseNode.MaxPool2D(pool_size=2),
        
        BaseNode.Conv2D(input_channels=32, output_channels=64, kernel_size=3),
        BaseNode.MyBatchNorm(indim=64),
        BaseNode.relu(),
        BaseNode.MaxPool2D(pool_size=2),
        
        BaseNode.Flatten(),
        BaseNode.Linear(indim=1600, outdim=128),
        BaseNode.relu(),
        BaseNode.Dropout(p=0.1),
        
        BaseNode.Linear(indim=128, outdim=mnist.num_class),
        BaseNode.LogSoftmax(),
        BaseNode.NLLLoss(Y)
    ]
    graph = Graph(nodes)
    return graph



"""
    model:
"""

setseed(0) # 固定随机数种子以提高可复现性

save_path = "model/lxy.npy"


val_X = mnist.val_X
val_Y = mnist.val_Y

val_split_ratio = 0.05
num_val_samples = int(val_X.shape[0] * val_split_ratio)

shuffle_indices = np.arange(val_X.shape[0])
np.random.seed(42)
np.random.shuffle(shuffle_indices)
val_X = val_X[shuffle_indices]
val_Y = val_Y[shuffle_indices]

val_X_to_train = val_X[:num_val_samples]
val_Y_to_train = val_Y[:num_val_samples]

val_X = val_X[num_val_samples:]
val_Y = val_Y[num_val_samples:]

trn_X = np.concatenate((mnist.trn_X, val_X_to_train), axis=0)
trn_Y = np.concatenate((mnist.trn_Y, val_Y_to_train), axis=0)

X = trn_X.reshape(-1, 1, 28, 28)
Y = trn_Y

if __name__ == "__main__":
    #print(mnist.val_X.shape)
    #pdb.set_trace()
    graph = buildGraph(Y)
    # 训练
    best_train_acc = 0
    dataloader = PermIterator(X.shape[0], batchsize)
    for i in range(1, 30+1):
        #start_time = time.time()
        hatys = []
        ys = []
        losss = []
        graph.train()
        for perm in dataloader:
            #print(f"this mini-batch used time: {time.time() - start_time}")
            tX = X[perm]
            tY = Y[perm]
            graph[-1].y = tY
            graph.flush()
            pred, loss = graph.forward(tX)[-2:]
            hatys.append(np.argmax(pred, axis=1))
            ys.append(tY)
            graph.backward()
            graph.optimstep(lr, wd1, wd2)
            losss.append(loss)
        loss = np.average(losss)
        acc = np.average(np.concatenate(hatys)==np.concatenate(ys))
        print(f"epoch {i} loss {loss:.3e} acc {acc:.4f}")
        if acc > best_train_acc:
            best_train_acc = acc
            with open(save_path, "wb") as f:
                pickle.dump(graph, f)


    '''# 验证
    with open(save_path, "rb") as f:
        graph = pickle.load(f)
    graph.eval()

    data_X, data_Y = val_X, val_Y
    
    batch_size = 7700
    num_data = data_X.shape[0]
    num_batches = num_data // batch_size + int(num_data % batch_size > 0)

    correct_predictions = 0

    for i in range(num_batches):
        start_idx = i * batch_size
        end_idx = min((i + 1) * batch_size, num_data)

        batch_X = data_X[start_idx:end_idx].reshape(-1, 1, 28, 28)
        batch_Y = data_Y[start_idx:end_idx]

        graph.flush()
        pred = graph.forward(batch_X, removelossnode=1)[-1]
        haty = np.argmax(pred, axis=1)
        correct_predictions += np.sum(haty == batch_Y)

    val_accuracy = correct_predictions / num_data
    print("valid acc", val_accuracy)
    
    
    # 测试
    with open(save_path, "rb") as f:
        graph = pickle.load(f)
    graph.eval()

    data_X, data_Y = mnist.data, mnist.targets
    
    batch_size = 1000
    num_data = data_X.shape[0]
    num_batches = num_data // batch_size + int(num_data % batch_size > 0)

    correct_predictions = 0

    for i in range(num_batches):
        start_idx = i * batch_size
        end_idx = min((i + 1) * batch_size, num_data)

        batch_X = data_X[start_idx:end_idx].reshape(-1, 1, 28, 28)
        batch_Y = data_Y[start_idx:end_idx]

        graph.flush()
        pred = graph.forward(batch_X, removelossnode=1)[-1]
        haty = np.argmax(pred, axis=1)
        correct_predictions += np.sum(haty == batch_Y)

    val_accuracy = correct_predictions / num_data
    print("test acc", val_accuracy)'''
