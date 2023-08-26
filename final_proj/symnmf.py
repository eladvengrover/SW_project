import sys
import pandas as pd
import symmnfAPI


SYMNMF = "symnmf"
SYM = "sym"
DDG = "ddg"
NORM = "norm"



args = sys.argv

K = args[1]
goal = args[2]
file_name = args[3]

X = pd.read_csv(file_name, header=None)

if goal == SYM:
    # symnmfAPI.symnmf()
    pass


