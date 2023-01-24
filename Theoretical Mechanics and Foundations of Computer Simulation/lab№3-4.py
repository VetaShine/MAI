import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import math
from scipy.integrate import odeint

def odesys(y, t, m1, m2, l, g, alpha):
    dy = np.zeros(4)
    dy[0] = y[2]
    dy[1] = y[3]

    a11 = m1 + m2
    a12 = -m2 * l * np.cos(y[1] - alpha)
    a21 = -np.cos(y[1] - alpha)
    a22 = l

    b1 = (m1 + m2) * g * np.sin(alpha) - m2 * l * (y[3]) ** 2 * np.sin(y[1] - alpha)
    b2 = -g * np.sin(y[1])

    dy[2] = (b1 * a22 - a12 * b2) / (a11 * a22 - a21 * a12)
    dy[3] = (b2 * a11 - b1 * a21) / (a11 * a22 - a21 * a12)
    return dy

t = np.linspace(0, 5, 500)

m1 = 5 #масса тела
m2 = 2 #масса точечного груза
l = 2 #длина стержня
g = 9.81
alpha = math.pi / 8
a_side = 2 #длина тела
b_side = 1 #высота тела
dia = (a_side ** 2 + b_side ** 2) ** (1 / 2)
betta = np.arctan(b_side / a_side)
proc = (dia / 2) * np.cos(alpha + betta)
gec = (dia / 2) * np.sin(alpha + betta)

x0 = 0
phi0 = math.pi
dx0 = 0
dphi0 = 0
y0 = [x0, phi0, dx0, dphi0]

Y = odeint(odesys, y0, t, (m1, m2, l, g, alpha))

x = Y[:, 0]
phi = Y[:, 1]
dx = Y[:, 2]
dphi = Y[:, 3]
ddx = [odesys(y, t, m1, m2, l, g, alpha)[2] for y, t in zip(Y, t)]
ddphi = [odesys(y, t, m1, m2, l, g, alpha)[3] for y, t in zip(Y, t)]

X_A = -x * np.cos(alpha)
Y_A = -x * np.sin(alpha)
X_B = X_A + l * np.sin(phi)
Y_B = Y_A - l * np.cos(phi)
X_Box = np.array([-proc, proc + (b_side * np.sin(alpha)), proc, -proc - (b_side * np.sin(alpha)), -proc])
Y_Box = np.array([-gec, gec - (b_side * np.cos(alpha)), gec, -gec + (b_side * np.cos(alpha)), -gec])

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
ax.axis('equal')

ax.plot(X_A - proc, Y_A - gec, color='grey') #построение линии наклонной плоскости
Drawed_Box = ax.plot(X_A[0] + X_Box, Y_A[0] + Y_Box, color='blue')[0] #построение прямоугольного тела
Line_AB = ax.plot([X_A[0], X_B[0]], [Y_A[0], Y_B[0]], color='black')[0] #построение стержня
Point_A = ax.plot(X_A[0], Y_A[0], marker='o', markersize=5, color='blue')[0] #построение точки центра масс тела
Point_B = ax.plot(X_B[0], Y_B[0], marker='o', markersize=8, color='red')[0] #построение точечного груза

fig_for_graphs = plt.figure(figsize=[13, 7])

#Построение графика зависимости абциссы от параметра t
ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 1)
ax_for_graphs.plot(t, -x, color='blue')
ax_for_graphs.set_title('x(t)')
ax_for_graphs.grid()

#Построение графика зависимости угла phi от параметра t
ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 2)
ax_for_graphs.plot(t, phi, color='red')
ax_for_graphs.set_title('phi(t)')
ax_for_graphs.grid()

#Построение графика производной функции от x(t)
ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 3)
ax_for_graphs.plot(t, dx, color='green')
ax_for_graphs.set_title("x'(t)")
ax_for_graphs.grid()

#Построение графика производной функции от phi(t)
ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 4)
ax_for_graphs.plot(t, dphi, color='black')
ax_for_graphs.set_title("phi'(t)")
ax_for_graphs.grid()

#Построение графика второй производной функции от x(t)
ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 5)
ax_for_graphs.plot(t, ddx, color='green')
ax_for_graphs.set_title("x''(t)")
ax_for_graphs.grid()

#Построение графика второй производной функции от phi(t)
ax_for_graphs = fig_for_graphs.add_subplot(2, 3, 6)
ax_for_graphs.plot(t, ddphi, color='black')
ax_for_graphs.set_title("phi''(t)")
ax_for_graphs.grid()

#Функция анимации
def anima(i):
    Point_A.set_data(X_A[i], Y_A[i])
    Point_B.set_data(X_B[i], Y_B[i])
    Line_AB.set_data([X_A[i], X_B[i]], [Y_A[i], Y_B[i]])
    Drawed_Box.set_data(X_A[i] + X_Box, Y_A[i] + Y_Box)
    return [Point_A, Point_B, Line_AB, Drawed_Box]

anim = FuncAnimation(fig, anima, frames=len(t), interval=10)
plt.show()
