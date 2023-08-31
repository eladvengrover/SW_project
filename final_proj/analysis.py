import sys
import numpy as np
import pandas as pd
import kmeans
import symmnfAPI
import math



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
    x = 2 * math.sqrt(m/K)
    return np.random.uniform(0, x, size=(N, K))


def calc_symnmf_clusters(matrix):
    clusters = [[] for _ in range(K)]
    for point in matrix:
        max_value = max(point)
        cluster_index = point.index(max_value)
        clusters[cluster_index].append(point)
    return clusters


def get_symnmf_clusters():
    X = pd.read_csv(file_name, header=None)
    N = int(X.shape[0])
    D = int(X.shape[1])

    X_lst = prepare_args_to_c(X)
    W = symmnfAPI.norm(X_lst, D)
    H = init_h(W, N).tolist()
    return calc_symnmf_clusters(symmnfAPI.symnmf(H, W, K))


args = sys.argv

K = int(args[1])
file_name = args[2]

kmeans_clusters = kmeans.get_clusters(K, file_name)

symnmf_clusters = get_symnmf_clusters()


