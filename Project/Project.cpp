#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <limits>
#include <random>
#include <chrono>
#include <cmath>

template <typename T>
const T INF = std::numeric_limits<T>::max();

template<typename U>
struct std::hash<std::pair<U, U>> {
    size_t operator()(const std::pair<U, U>& p) const {
        auto hash1 = std::hash<U>{}(p.first);
        auto hash2 = std::hash<U>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

template <typename T, typename U>
struct Edge {
    U from;
    U to;
    T cost;
    Edge() = default;
    Edge(U from, U to, T cost) : from(from), to(to), cost(cost) {}
};

template <typename T, typename U>
struct Edges {
    std::vector<Edge<T, U>> edges;
    U vertexCount, edgeCount;
    Edges(U vertexCount, U edgeCount): vertexCount(vertexCount), edgeCount(edgeCount) {
        if (edgeCount > (vertexCount * (vertexCount - 1)) / 2) {
            throw std::exception("Invalid edge count");
        }
    }
    void input() {
        for (U i = 0; i < edgeCount; ++i) {
            U from, to;
            T cost;
            std::cin >> from >> to >> cost;
            edges[i] = { from, to, cost };
        }
    }
    void fill() {
        std::mt19937 gen(42);
        std::uniform_int_distribution<> vertexGenerator(1, static_cast<int>(vertexCount));
        std::uniform_real_distribution<T> distGenerator(0.0, 1.0);

        std::unordered_set<std::pair<U, U>> edges_uset;
        for (U i = 1; i < vertexCount + 1; ++i) {
            U new_from = i;
            U new_to = i % vertexCount + 1;
            if (new_from > new_to) {
                std::swap(new_from, new_to);
            }
            T new_cost = distGenerator(gen);
            edges.push_back(Edge<T, U>(new_from, new_to, new_cost));
            edges_uset.insert(std::make_pair(new_from, new_to));
        }
        for (U i = 1; i < edgeCount - vertexCount + 1; ++i) {
            U new_from = vertexGenerator(gen);
            U new_to = vertexGenerator(gen);
            if (new_from > new_to) {
                std::swap(new_from, new_to);
            }
            T new_cost = distGenerator(gen);
            Edge<T, U> new_edge = Edge<T, U>(new_from, new_to, new_cost);
            if (new_from == new_to || edges_uset.count(std::pair<U, U>(new_from, new_to))) {
                --i;
                continue;
            }
            edges.push_back(new_edge);
            edges_uset.insert(std::pair<U, U>(new_from, new_to));
        }
    }
    void show() const {
        for (const auto& edge : edges) {
            std::cout << "from " << edge.from << ", to " << edge.to << ", for " << edge.cost << "\n";
        }
    }
};

template <typename T, typename U>
struct Graph {
    std::vector<std::vector<std::pair<U, T>>> neig;
    U vertexCount, edgeCount;
    Graph(U vertexCount, U edgeCount): vertexCount(vertexCount), edgeCount(edgeCount) {
        neig.resize(vertexCount + 1, std::vector<std::pair<U, T>> {});
    }
    Graph(Edges<T, U>& edges): vertexCount(edges.vertexCount), edgeCount(edges.edgeCount) {
        neig.resize(vertexCount + 1, std::vector<std::pair<U, T>> {});
        for (auto edge : edges.edges) {
            neig[edge.from].emplace_back(std::make_pair(edge.to, edge.cost));
            neig[edge.to].emplace_back(std::make_pair(edge.from, edge.cost));
        }
    }
    bool operator==(const Graph<T, U>& another_graph) const {
        if (vertexCount != another_graph.vertexCount || edgeCount != another_graph.edgeCount) {
            return false;
        }
        for (U i = 1; i < static_cast<U>(neig.size()); ++i) {
            if (neig[i].size() != another_graph.neig[i].size()) {
                return false;
            }
            for (U j = 1; j < static_cast<U>(neig[i].size()); ++j) {
                if (neig[i][j] != another_graph.neig[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    void input() {
        for (U i = 0; i < edgeCount; ++i) {
            U from, to;
            T cost;
            std::cin >> from >> to >> cost;
            neig[from].emplace_back(std::make_pair(to, cost));
            neig[to].emplace_back(std::make_pair(from, cost));
        }
    }
    void show() const {
        for (U i = 0; i < static_cast<U>(neig.size()); ++i) {
            std::cout << i << ": ";
            for (U j = 0; j < static_cast<U>(neig[i].size()); ++j) {
                std::cout << "(to " << neig[i][j].first << ", for " << neig[i][j].second << ") ";
            }
            std::cout << "\n";
        }
    }
};

template <typename T, typename U = int64_t>
class BinaryHeap {
private:
    std::vector<T> heap;

    U parent(U i) const { return (i - 1) / 2; }
    U left_child(U i) const { return 2 * i + 1; }
    U right_child(U i) const { return 2 * i + 2; }

    void sift_up(U i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    void sift_down(U i) {
        U min_index = i;
        U left = left_child(i);
        U right = right_child(i);

        if (left < static_cast<U>(heap.size()) && heap[left] < heap[min_index]) {
            min_index = left;
        }
        if (right < static_cast<U>(heap.size()) && heap[right] < heap[min_index]) {
            min_index = right;
        }
        if (i != min_index) {
            std::swap(heap[i], heap[min_index]);
            sift_down(min_index);
        }
    }

public:
    BinaryHeap() = default;

    bool empty() const { return heap.empty(); }

    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        return heap.front();
    }

    void push(const T& value) {
        heap.push_back(value);
        sift_up(heap.size() - 1);
    }

    void pop() {
        if (empty()) {
            throw std::out_of_range("Heap is empty");
        }
        heap[0] = heap.back();
        heap.pop_back();
        if (!empty()) {
            sift_down(0);
        }
    }
};

template <typename T, typename U>
U find_min(BinaryHeap<std::pair<T, U>>& dist, std::vector<U>& used) {
    while (used[dist.top().second]) {
        dist.pop();
    }
    U min_dist_ind = dist.top().second;
    used[min_dist_ind] = 1;
    return min_dist_ind;
}

template <typename T, typename U>
Graph<T, U> Prim(Graph<T, U>& graph) {
    Graph<T, U> new_graph(graph.vertexCount, 0);
    std::vector<std::pair<U, T>> parent(graph.vertexCount + 1, std::pair<U, T>(-1, -1));
    std::vector<U> used(graph.vertexCount + 1, 0);
    BinaryHeap<std::pair<T, U>> dist;
    dist.push({ 0, 1 }); 
    for (U i = 0; i < static_cast<U>(graph.neig.size()); ++i) {
        U min_dist_ind = 0;
        try {
            min_dist_ind = find_min(dist, used);
        }
        catch (...) {}
        for (U j = 0; j < static_cast<U>(graph.neig[min_dist_ind].size()); ++j) {
            if (!used[graph.neig[min_dist_ind][j].first]) {
                dist.push({ graph.neig[min_dist_ind][j].second, graph.neig[min_dist_ind][j].first });
                parent[graph.neig[min_dist_ind][j].first].second = graph.neig[min_dist_ind][j].second;
                parent[graph.neig[min_dist_ind][j].first].first = min_dist_ind;
            }
        }
    }
    for (U i = 0; i < static_cast<U>(parent.size()); ++i) {
        if (parent[i].first != -1) {
            new_graph.neig[i].emplace_back(std::pair<U, T>(parent[i].first, parent[i].second));
            new_graph.neig[parent[i].first].emplace_back(std::pair<U, T>(i, parent[i].second));
        }
    }
    return new_graph;
}

template <typename U>
struct Dsu {
    std::vector<U> parent;
    std::vector<U> rank;
    Dsu(U n) {
        rank.resize(n, 1);
        parent.resize(n, 0);
        for (U i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }
    U find_parent(U x) {
        std::vector<U> way;
        while (parent[x] != x) {
            way.push_back(x);
            x = parent[x];
        }
        for (auto i : way) {
            parent[i] = x;
        }
        return x;
    }
    void unite(U x, U y) {
        x = find_parent(x);
        y = find_parent(y);
        if (rank[x] == rank[y]) {
            parent[x] = y;
            ++rank[y];
        } else if (rank[x] < rank[y]) {
            parent[x] = y;
        } else {
            parent[y] = x;
        }
    }
};

template <typename T, typename U>
bool comp(const Edge<T, U>& edge1, const Edge<T, U>& edge2) {
    return edge1.cost < edge2.cost;
}

template <typename T, typename U>
Graph<T, U> Kruskal(Edges<T, U>& edges) {
    Edges<T, U> new_edges(edges.vertexCount, 0);
    std::sort(edges.edges.begin(), edges.edges.end(), comp<T, U>);
    Dsu<U> dsu(edges.vertexCount + 1);
    for (auto edge : edges.edges) {
        if (dsu.find_parent(edge.from) != dsu.find_parent(edge.to)) {
            dsu.unite(edge.from, edge.to);
            new_edges.edges.emplace_back(edge);
        }
    }
    Graph<T, U> graph(new_edges);
    return graph;
}

int test() {
    int64_t n, m;
    std::cin >> n >> m;
    Edges<int64_t, int64_t> edges(n, m);
    edges.input();
    Graph<int64_t, int64_t> kruskal_graph = Kruskal(edges);
    std::cout << "KRUSKAL:\n";
    kruskal_graph.show();

    Graph<int64_t, int64_t> graph(edges);
    Graph<int64_t, int64_t> prim_graph = Prim(graph);
    std::cout << "PRIM:\n";
    prim_graph.show();

    std::cout << (kruskal_graph == prim_graph) << "\n";
    /*
    6 7
    1 4 1
    2 5 1
    3 6 3
    1 2 5
    2 3 8
    4 5 2
    5 6 4
    */
    return 0;
}

int main() {
    std::ofstream kruskalFile1("../kruskalFile1.txt");
    std::ofstream kruskalFile2("../kruskalFile2.txt");
    std::ofstream kruskalFile3("../kruskalFile3.txt");

    std::ofstream primFile1("../primFile1.txt");
    std::ofstream primFile2("../primFile2.txt");
    std::ofstream primFile3("../primFile3.txt");

    int64_t maxVertexCount = 3000;
    int64_t vertexStep = 50;

    for (int64_t n = 10; n < maxVertexCount; n += vertexStep) {
        int64_t m = 2 * n;
        Edges<double, int64_t> edges(n, m);
        edges.fill();

        auto startKruskal = std::chrono::high_resolution_clock::now();
        Graph<double, int64_t> kruskal_graph = Kruskal(edges);
        auto endKruskal = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationKruskal = endKruskal - startKruskal;
        kruskalFile1 << n << " " << durationKruskal.count() << "\n";

        Graph<double, int64_t> graph(edges);

        auto startPrim = std::chrono::high_resolution_clock::now();
        Graph<double, int64_t> prim_graph = Prim(graph);
        auto endPrim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationPrim = endPrim - startPrim;
        primFile1 << n << " " << durationPrim.count() << "\n";
    }
    for (int64_t n = 10; n < maxVertexCount; n += vertexStep) {
        int64_t m = n * static_cast<int64_t>(std::sqrt(n));
        Edges<double, int64_t> edges(n, m);
        edges.fill();

        auto startKruskal = std::chrono::high_resolution_clock::now();
        Graph<double, int64_t> kruskal_graph = Kruskal(edges);
        auto endKruskal = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationKruskal = endKruskal - startKruskal;
        kruskalFile2 << n << " " << durationKruskal.count() << "\n";

        Graph<double, int64_t> graph(edges);

        auto startPrim = std::chrono::high_resolution_clock::now();
        Graph<double, int64_t> prim_graph = Prim(graph);
        auto endPrim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationPrim = endPrim - startPrim;
        primFile2 << n << " " << durationPrim.count() << "\n";
    }
    for (int64_t n = 10; n < maxVertexCount; n += vertexStep) {
        int64_t m = n * n / 4;
        Edges<double, int64_t> edges(n, m);
        edges.fill();

        auto startKruskal = std::chrono::high_resolution_clock::now();
        Graph<double, int64_t> kruskal_graph = Kruskal(edges);
        auto endKruskal = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationKruskal = endKruskal - startKruskal;
        kruskalFile3 << n << " " << durationKruskal.count() << "\n";

        Graph<double, int64_t> graph(edges);

        auto startPrim = std::chrono::high_resolution_clock::now();
        Graph<double, int64_t> prim_graph = Prim(graph);
        auto endPrim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationPrim = endPrim - startPrim;
        primFile3 << n << " " << durationPrim.count() << "\n";
    }
}