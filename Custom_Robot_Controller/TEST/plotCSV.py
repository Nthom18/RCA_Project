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


# for i in range(19):
#     xd.append(xt[i] - xh[i])
#     yd.append(yt[i] - yh[i])
#     rd.append(rt[i] - rh[i])

for i in range(19):
    xd.append(100 * abs(xt[i] - xh[i]) / (xt[i] + 0.0001))
    yd.append(100 * abs(yt[i] - yh[i]) / (yt[i] + 0.0001))
    rd.append(100 * abs(rt[i] - rh[i]) / (rt[i] + 0.0001))

    # print(xd[i])

print('\n')

# Average error
av_x = 0
av_y = 0
av_r = 0

for i in range(12):
    av_x += xd[i+2]
    av_y += yd[i+2]
    av_r += rd[i+2]

    # print(av_x)

av_x /= 12
av_y /= 12
av_r /= 12

print(av_x)
print(av_y)
print(av_r)


# plt.subplot(3,1,1)
# plt.plot(t,xd, label = 'x')
# plt.plot(t,yd, label = 'y')

# plt.xlabel('Distance to marble')
# plt.ylabel('ground truth - measured value')
# plt.title('Error percentage of center')
# plt.legend()


# plt.subplot(3,1,3)
# plt.plot(t,rd, label = 'r')
# plt.xlabel('Distance to marble')
# plt.ylabel('ground truth - measured value')
# plt.title('Error percentage of radius')
# plt.legend()

# plt.show()

