import sys
import pandas as pd
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


args = sys.argv

K = args[1]
goal = args[2]
file_name = "input.txt"

X = pd.read_csv(file_name, header=None)

N = X.shape[0]
D = X.shape[1]

X_lst = prepare_args_to_c(X)
c_output = None
try:
    if goal == SYM:
        c_output = symmnfAPI.sym(X_lst, D)
    elif goal == DDG:
        c_output = symmnfAPI.ddg(X_lst, D)
    elif goal == NORM:
        c_output = symmnfAPI.norm(X_lst, D)
    for centroid in c_output:
        print(','.join('{:.4f}'.format(num) for num in centroid))
except ValueError as e:
    print ("An Error Has Occurred")
    exit(1)