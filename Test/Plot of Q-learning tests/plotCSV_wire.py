import numpy as np
import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')
# ax = fig.gca(projection='3d')

x = []
y = []
z = []

with open('testDataAlphaGamma1.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(float(row[0]))
        y.append(float(row[1]))
        z.append(float(row[2]))

X = np.reshape(x, (4, 4))
Y = np.reshape(y, (4, 4))
Z = np.reshape(z, (4, 4))


# # Plot the surface.
surf = ax.plot_wireframe(X, Y, Z)

# # Customize the z axis.
# ax.set_zlim(-1.01, 1.01)
# ax.zaxis.set_major_locator(LinearLocator(10))
# ax.zaxis.set_major_formatter(FormatStrFormatter('%.02f'))

# # Add a color bar which maps values to colors.
# fig.colorbar(surf, shrink=0.5, aspect=5)

# ax.plot_surface(X, Y, Z)

plt.xlabel('alpha')
plt.ylabel('gamma')
plt.title('Average reward with different values of alpha and gamma')
# plt.legend()
plt.show()
