import csv
import matplotlib.pyplot as plt
import numpy as np

xh = []
yh = []
rh = []

xt = []
yt = []
rt = []

xd = []
yd = []
rd = []

t = np.arange(1, 10.5, 0.5)

with open('houghGroundTruth.csv','r') as csvTruth:
    plots = csv.reader(csvTruth, delimiter=',')
    for row in plots:
        xt.append(float(row[0]))
        yt.append(float(row[1]))
        rt.append(float(row[2]))

with open('houghData.csv','r') as csvHough:
    plots = csv.reader(csvHough, delimiter=',')
    for row in plots:
        xh.append(float(row[0]))
        yh.append(float(row[1]))
        rh.append(float(row[2]))


for i in range(19):
    xd.append(xt[i] - xh[i])
    yd.append(yt[i] - yh[i])
    rd.append(rt[i] - rh[i])

plt.subplot(3,1,1)
plt.plot(t,xd, label = 'x')
plt.plot(t,yd, label = 'y')

plt.xlabel('Distance to marble')
plt.ylabel('ground truth - measured value')
plt.title('Error of center')
plt.legend()


plt.subplot(3,1,3)
plt.plot(t,rd, label = 'r')
plt.xlabel('Distance to marble')
plt.ylabel('ground truth - measured value')
plt.title('Error of radius')
plt.legend()

plt.show()

