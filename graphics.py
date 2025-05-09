import matplotlib.pyplot as plt
import numpy as np

def read_data(filename):
    x, y = [], []
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            x.append(int(parts[0]))
            y.append(float(parts[1]))
    return np.array(x), np.array(y)

x_kruskal_1, y_kruskal_1 = read_data('kruskalFile1.txt')
x_kruskal_2, y_kruskal_2 = read_data('kruskalFile2.txt')
x_kruskal_3, y_kruskal_3 = read_data('kruskalFile3.txt')
x_prim_1, y_prim_1 = read_data('primFile1.txt')
x_prim_2, y_prim_2 = read_data('primFile2.txt')
x_prim_3, y_prim_3 = read_data('primFile3.txt')

x_min = min(min(x_kruskal_1), min(x_kruskal_2), min(x_kruskal_3), min(x_prim_1), min(x_prim_2), min(x_prim_3))
x_max = max(max(x_kruskal_1), max(x_kruskal_2), max(x_kruskal_3), max(x_prim_1), max(x_prim_2), max(x_prim_3))
y_min = min(min(y_kruskal_1), min(y_kruskal_2), min(y_kruskal_3), min(y_prim_1), min(y_prim_2), min(y_prim_3))
y_max = max(max(y_kruskal_1), max(y_kruskal_2), max(y_kruskal_3), max(y_prim_1), max(y_prim_2), max(y_prim_3))

plt.figure(figsize=(14, 6))

plt.subplot(1, 2, 1)
plt.plot(x_kruskal_1, y_kruskal_1, label='m = n * 2')
plt.plot(x_kruskal_2, y_kruskal_2, label='m = n * sqrt(n)')
plt.plot(x_kruskal_3, y_kruskal_3, label='m = n * n / 4')
plt.title('Время выполнения алгоритма Краскала')
plt.xlabel('Количество вершин графа')
plt.ylabel('Время (секунды)')
plt.xlim(x_min, x_max)
plt.ylim(y_min, y_max)
plt.legend()

plt.subplot(1, 2, 2)
plt.plot(x_prim_1, y_prim_1, label='m = n * 2')
plt.plot(x_prim_2, y_prim_2, label='m = n * sqrt(n)')
plt.plot(x_prim_3, y_prim_3, label='m = n * n / 4')
plt.title('Время выполнения алгоритма Прима')
plt.xlabel('Количество вершин графа')
plt.ylabel('Время (секунды)')
plt.xlim(x_min, x_max)
plt.ylim(y_min, y_max)
plt.legend()

plt.suptitle('Сравнение алгоритмов Краскала и Прима')

plt.show()