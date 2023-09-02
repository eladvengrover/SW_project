import sys
import numpy as np
import pandas as pd
import kmeans
import symmnfAPI
import math
from sklearn import metrics as sk


def prepare_args_to_c(points):
    """Convert points dataframes into list of lists in order to pass it to C program"""
    points_list = []
    for point in points.values:
        points_list.append(point.tolist())

    return points_list


def get_m(W):
    avgs = [np.average(row) for row in W]
    return sum(avgs) / len(avgs)


def init_h(W, N):
    m = get_m(W)
    x = 2 * math.sqrt(m / K)
    return np.random.uniform(0, x, size=(N, K))


def calc_symnmf_clusters(matrix, n):
    clusters = [-1 for _ in range(n)]
    for point_index in range(len(matrix)):
        max_value = max(matrix[point_index])
        cluster_index = matrix[point_index].index(max_value)
        clusters[point_index] = cluster_index
    return clusters


def get_symnmf_clusters(X):
    N = int(X.shape[0])
    D = int(X.shape[1])

    X_lst = prepare_args_to_c(X)
    W = symmnfAPI.norm(X_lst, D)
    H = init_h(W, N).tolist()
    return calc_symnmf_clusters(symmnfAPI.symnmf(H, W, K), N)


args = sys.argv

K = int(args[1])
file_name = args[2]
np.random.seed(0)

kmeans_clusters = kmeans.get_clusters(K, file_name)

X = pd.read_csv(file_name, header=None)

symnmf_clusters = get_symnmf_clusters(X)

print("nmf: {:.4f}".format(sk.silhouette_score(X, symnmf_clusters)))
print("kmeans: {:.4f}".format(sk.silhouette_score(X, kmeans_clusters)))



