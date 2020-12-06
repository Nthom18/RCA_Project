import numpy as np
import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')

x = []
y = []
z = []

with open('testDataAlphaGamma1.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(float(row[0]))
        y.append(float(row[1]))
        z.append(float(row[2]))

ax.scatter(x,y,z)

plt.xlabel('alpha')
plt.ylabel('gamma')
plt.title('Average reward with different values of alpha and gamma')
plt.legend()
plt.show()
