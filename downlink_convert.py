import pandas as pd
import tkinter as tk
import math
from tkinter import filedialog
from os.path import exists

R1 = 10000
c1 = 1.009249522e-03
c2 = 2.378405444e-04
c3 = 2.019202697e-07
i = 0
j = 0
num = 1
col = 3

tk.Tk().withdraw()
file_path = filedialog.askopenfilename()
old_df = pd.read_csv(file_path, sep=",", header=None)
df = pd.DataFrame()

#delete rows with issues
no_na = old_df.dropna(axis=0)
df = df.append(no_na, ignore_index=True)

#convert to kelvin
size = len(df)

while col <= 4:
    t_col = df[col]    #internal = 3, external = 4
    while(i < size):
        Vo = float(t_col[i])
        R2 = R1 * (1023.0 / (Vo - 1.0))
        logR2 = math.log(R2)
        t_kelvin = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2))
        df.at[i, col] = t_kelvin
        i += 1
    col += 1
    i = 0

#create new column for difference between ext and int temp
while j < size:
    ext_temp = df[4][j]
    int_temp = df[3][j]
    diff = ext_temp - int_temp
    df.loc[j, 6] = diff
    j += 1

#convert to csv file
filename = 'downlink_data.csv'
files_present = exists(filename)

while files_present:
    filename = 'downlink_data['+ str(num) + '].csv'
    files_present = exists(filename)
    num += 1

if not files_present:
    df.to_csv(filename, index=False)

#check
print(df)
print("done")
