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
#     print(d[i])  

# print('\n')

# for i in range(19):
#     dd.append(100 * abs(t[i] - d[i]) / (t[i] + 0.0001))

#     print(dd[i])

for i in range(19):
    dd.append(t[i] - d[i])

# print('\n')

# Average error
av_d = 0

for i in range(12):
    av_d += dd[i+2]

av_d /= 12

# print('\n')

print(av_d)



plt.subplot(3,1,1)
plt.plot(t,t, 'r--', linewidth=0.5, label = 'Ideal line')
plt.plot(t,d, label = 'calculated distance')

plt.xlabel('True distance to marble')
plt.ylabel('calculated distance')
plt.title('Relation between true distance and calculated distance')
plt.legend()

plt.subplot(3,1,3)
plt.plot(t,[0]*19, 'r--', linewidth=0.5, label = 'x-axis')
plt.plot(t,dd, label = 'difference in distance [meters]')
plt.xlabel('True distance to marble')
plt.ylabel('ground truth - calculated distance')
plt.title('Error of calculated distance in meters')
plt.legend()

plt.show()

