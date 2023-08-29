import math
import sys
import pandas as pd
import numpy as np
import symmnfAPI


SYMNMF = "symnmf"
SYM = "sym"
DDG = "ddg"
NORM = "norm"


def prepare_args_to_c(points):
    """Convert points dataframes into list of lists in order to pass it to C program"""
    points_list = []
    for point in points.values:
        points_list.append(point.tolist())

    return points_list


def get_m(W):
    avgs = [np.average(row) for row in W]
    return sum(avgs) / len(avgs)


def init_h(W, k, n):
    m = get_m(W)
    x = 2 * math.sqrt(m/k)
    return np.random.uniform(0, x, size=(N, K))


args = sys.argv
np.random.seed(0)

K = int(args[1])
goal = args[2]
file_name = args[3]

X = pd.read_csv(file_name, header=None)

N = X.shape[0]
D = int(X.shape[1])

X_lst = prepare_args_to_c(X)
c_output = None
try:
    if goal == SYM:
        c_output = symmnfAPI.sym(X_lst, D)
    elif goal == DDG:
        c_output = symmnfAPI.ddg(X_lst, D)
    elif goal == NORM:
        c_output = symmnfAPI.norm(X_lst, D)
    elif goal == SYMNMF:
        W = symmnfAPI.norm(X_lst, D)
        H = init_h(W, K, N).tolist()
        c_output = symmnfAPI.symnmf(H, W, K)
    for centroid in c_output:
        print(','.join('{:.4f}'.format(num) for num in centroid))
except ValueError as e:
    print ("An Error Has Occurred")
    exit(1)