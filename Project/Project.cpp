#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

template <typename T>
const T INF = std::numeric_limits<T>::max();

template <typename T, typename U = int64_t>
struct Edge {
    U from;
    U to;
    T cost;
    Edge() = default;
    Edge(U from, U to, T cost) : from(from), to(to), cost(cost) {}
};

template <typename T, typename U = int64_t>
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

template <typename T, typename U = int64_t>
struct Graph {
    std::vector<std::vector<std::pair<U, T>>> neig;
    U n, m;
    Graph(U n, U m): n(n), m(m) {
        neig.resize(n + 1, std::vector<std::pair<U, T>> {});
    }
    Graph(Edges<T, U>& edges): n(edges.vertexCount), m(edges.edgeCount) {
        neig.resize(n + 1, std::vector<std::pair<U, T>> {});
        for (auto v : edges.edges) {
            neig[v[0]].emplace_back(std::make_pair(v[1], v[2]));
            neig[v[1]].emplace_back(std::make_pair(v[0], v[2]));
        }
    }
    void input() {
        for (U i = 0; i < m; ++i) {
            U from, to;
            T cost;
            std::cin >> from >> to >> cost;
            neig[from].emplace_back(std::make_pair(to, cost));
            neig[to].emplace_back(std::make_pair(from, cost));
        }
    }
    void show() const {
        for (U i = 0; i < neig.size(); ++i) {
            std::cout << i << ": ";
            for (U j = 0; j < neig[i].size(); ++j) {
                std::cout << "(to " << neig[i][j].first << ", for " << neig[i][j].second << ") ";
            }
            std::cout << "\n";
        }
    }
};

// TODO: IMPLEMENT BINARY HEAP INSTEAD LINEAR SEARCH
int64_t find_min(std::vector<int64_t>& dist, std::vector<int64_t>& used) {
    int64_t min_value = INF<int64_t>;
    int64_t min_ind = -1;
    for (int64_t i = 0; i < dist.size(); ++i) {
        if (min_value >= dist[i] && !used[i]) {
            min_value = dist[i];
            min_ind = i;
        }
    }
    used[min_ind] = 1;
    return min_ind;
}

template <typename T, typename U = int64_t>
Graph<T, U> Prim(Graph<T, U>& graph) {
    Graph<T, U> new_graph(graph.n, 0);
    std::vector<std::pair<T, U>> parent(graph.n + 1, std::make_pair(-1, -1));
    std::vector<U> dist(graph.n + 1, INF<U>);
    std::vector<U> used(graph.n + 1, 0);
    dist[1] = 0;
    for (U i = 0; i < graph.neig.size(); ++i) {
        U min_ind = find_min(dist, used);
        for (U j = 0; j < graph.neig[min_ind].size(); ++j) {
            if (dist[graph.neig[min_ind][j].first] > graph.neig[min_ind][j].second && !used[graph.neig[min_ind][j].first]) {
                dist[graph.neig[min_ind][j].first] = graph.neig[min_ind][j].second;
                parent[graph.neig[min_ind][j].first].second = graph.neig[min_ind][j].second;
                parent[graph.neig[min_ind][j].first].first = min_ind;
            }
        }
    }
    for (U i = 0; i < parent.size(); ++i) {
        if (parent[i].first != -1) {
            new_graph.neig[i].emplace_back(std::make_pair(parent[i].first, parent[i].second));
            new_graph.neig[parent[i].first].emplace_back(std::make_pair(i, parent[i].second));
        }
    }
    return new_graph;
}

struct Dsu {
    std::vector<int64_t> parent;
    std::vector<int64_t> rank;
    Dsu(int64_t n) {
        rank.resize(n, 1);
        parent.resize(n, 0);
        for (int64_t i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }
    int64_t find_parent(int64_t x) {
        std::vector<int64_t> way;
        while (parent[x] != x) {
            way.push_back(x);
            x = parent[x];
        }
        for (auto i : way) {
            parent[i] = x;
        }
        return x;
    }
    void unite(int64_t x, int64_t y) {
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

bool comp(std::vector<int64_t> a, std::vector<int64_t> b) {
    return a[2] < b[2];
}

template <typename T, typename U = int64_t>
Graph<T, U> Kruskal(Edges<T, U>& edges) {
    Edges new_edges(edges.n, 0);
    sort(edges.edges.begin(), edges.edges.end(), comp);
    Dsu dsu(edges.n + 1);
    for (auto edge : edges.edges) {
        if (dsu.find_parent(edge[0]) != dsu.find_parent(edge[1])) {
            dsu.unite(edge[0], edge[1]);
            new_edges.edges.emplace_back(edge);
        }
    }
    Graph<T, U> graph(new_edges);
    return graph;
}

int main() {
    int64_t n, m;
    std::cin >> n >> m;
    Edges<int64_t> edges(n, m);
    edges.input();
    Graph<int64_t> kruskal_graph = Kruskal(edges);
    std::cout << "KRUSKAL:\n";
    kruskal_graph.show();

    Graph<int64_t> graph(edges);
    Graph<int64_t> prim_graph = Prim(graph);
    std::cout << "PRIM:\n";
    prim_graph.show();
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