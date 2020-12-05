import csv
import matplotlib.pyplot as plt
import numpy as np

d = []
dd = []

t = np.arange(1, 10.5, 0.5)


with open('distanceData.csv','r') as csvDistance:
    plots = csv.reader(csvDistance, delimiter=',')
    for row in plots:
        d.append(float(row[0]))


# for i in range(19):
#     xd.append(xt[i] - xh[i])
#     yd.append(yt[i] - yh[i])
#     rd.append(rt[i] - rh[i])

for i in range(19):
    dd.append(100 * abs(t[i] - d[i]) / (t[i] + 0.0001))

    print(dd[i])

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

