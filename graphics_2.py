import matplotlib.pyplot as plt
import numpy as np

def count_edges_1_prim(vertex_count):
    return 2 * float(vertex_count) * np.log2(float(vertex_count))

def count_edges_2_prim(vertex_count):
    return np.sqrt(float(vertex_count)) * float(vertex_count) * np.log2(float(vertex_count))

def count_edges_3_prim(vertex_count):
    return float(vertex_count) * float(vertex_count) / 4 * np.log2(float(vertex_count))

def count_edges_1_kruskal(vertex_count):
    return 2 * float(vertex_count)

def count_edges_2_kruskal(vertex_count):
    return np.sqrt(float(vertex_count)) * float(vertex_count)

def count_edges_3_kruskal(vertex_count):
    return float(vertex_count) * float(vertex_count) / 4

def read_data(filename, func):
    x, y = [], []
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            x.append(int(parts[0]))
            y.append(float(parts[1]) / func(parts[0]))
    return np.array(x), np.array(y)

x_kruskal_1, y_kruskal_1 = read_data('kruskalFile1.txt', count_edges_1_kruskal)
x_kruskal_2, y_kruskal_2 = read_data('kruskalFile2.txt', count_edges_2_kruskal)
x_kruskal_3, y_kruskal_3 = read_data('kruskalFile3.txt', count_edges_3_kruskal)
x_prim_1, y_prim_1 = read_data('primFile1.txt', count_edges_1_prim)
x_prim_2, y_prim_2 = read_data('primFile2.txt', count_edges_2_prim)
x_prim_3, y_prim_3 = read_data('primFile3.txt', count_edges_3_prim)

plt.figure(figsize=(14, 6))

plt.subplot(1, 2, 1)
plt.plot(x_kruskal_1, y_kruskal_1, label='m = n * 2')
plt.plot(x_kruskal_2, y_kruskal_2, label='m = n * sqrt(n)')
plt.plot(x_kruskal_3, y_kruskal_3, label='m = n * n / 4')
plt.title('Алгоритм Краскалла')
plt.xlabel('Количество вершин графа')
plt.ylabel('Отношение к оценке сложности')
plt.ylim(0, 1e-5)
plt.legend(loc='upper left')

plt.subplot(1, 2, 2)
plt.plot(x_prim_1, y_prim_1, label='m = n * 2')
plt.plot(x_prim_2, y_prim_2, label='m = n * sqrt(n)')
plt.plot(x_prim_3, y_prim_3, label='m = n * n / 4')
plt.title('Алгоритм Прима')
plt.xlabel('Количество вершин графа')
plt.ylabel('Отношение к оценке сложности')
plt.ylim(0, 1e-6)
plt.legend(loc='upper left')

plt.suptitle('Отношение времени выполнения к оценке сложности')

plt.show()