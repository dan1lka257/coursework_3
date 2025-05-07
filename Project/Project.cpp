#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

template <typename T>
const T INF = std::numeric_limits<T>::max();

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
    T vertexCount, edgeCount;
    Edges(T vertexCount, T edgeCount): vertexCount(vertexCount), edgeCount(edgeCount) {
        edges.resize(edgeCount, Edge<T, U>{});
    }
    void input() {
        for (T i = 0; i < edgeCount; ++i) {
            T from, to;
            U cost;
            std::cin >> from >> to >> cost;
            edges[i] = { from, to, cost };
        }
    }
    void show() const {
        for (auto edge : edges) {
            std::cout << "from " << edge.from << ", to " << edge.to << ", for" << edge.cost << "\n";
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

template <typename T>
class BinaryHeap {
private:
    std::vector<T> heap;

    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t left_child(size_t i) const { return 2 * i + 1; }
    size_t right_child(size_t i) const { return 2 * i + 2; }

    void sift_up(size_t i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    void sift_down(size_t i) {
        size_t min_index = i;
        size_t left = left_child(i);
        size_t right = right_child(i);

        if (left < heap.size() && heap[left] < heap[min_index]) {
            min_index = left;
        }
        if (right < heap.size() && heap[right] < heap[min_index]) {
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
    std::vector<std::pair<T, U>> parent(graph.vertexCount + 1, std::make_pair(-1, -1));
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
            new_graph.neig[i].emplace_back(std::make_pair(parent[i].first, parent[i].second));
            new_graph.neig[parent[i].first].emplace_back(std::make_pair(i, parent[i].second));
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

int main() {
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
}