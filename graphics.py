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

plt.figure(figsize=(14, 6))

plt.subplot(1, 3, 1)
plt.plot(x_kruskal_1, y_kruskal_1, label='Алгоритм Краскала')
plt.plot(x_prim_1, y_prim_1, label='Алгоритм Прима')
plt.title('m = n * 2')
plt.xlabel('Количество вершин графа')
plt.ylabel('Время (секунды)')
plt.legend()

plt.subplot(1, 3, 2)
plt.plot(x_kruskal_2, y_kruskal_2, label='Алгоритм Краскала')
plt.plot(x_prim_2, y_prim_2, label='Алгоритм Прима')
plt.title('m = n * sqrt(n)')
plt.xlabel('Количество вершин графа')
plt.ylabel('Время (секунды)')
plt.legend()

plt.subplot(1, 3, 3)
plt.plot(x_kruskal_3, y_kruskal_3, label='Алгоритм Краскала')
plt.plot(x_prim_3, y_prim_3, label='Алгоритм Прима')
plt.title('m = n * n / 4')
plt.xlabel('Количество вершин графа')
plt.ylabel('Время (секунды)')
plt.legend()

plt.suptitle('Сравнение алгоритмов Краскала и Прима')

plt.show()