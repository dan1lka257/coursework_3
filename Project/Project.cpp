#include <iostream>
#include <algorithm>
#include <vector>

const int64_t INF = 1e9;

struct Edges {
    std::vector<std::vector<int64_t>> edges;
    int64_t n, m;
    Edges(int64_t n, int64_t m): n(n), m(m) {
        edges.resize(m, std::vector<int64_t> {});
    }
    void input() {
        for (int64_t i = 0; i < m; ++i) {
            int64_t from, to, cost;
            std::cin >> from >> to >> cost;
            edges[i] = { from, to, cost };
        }
    }
    void show() {
        for (auto v : edges) {
            std::cout << "from " << v[0] << ", to " << v[1] << ", for" << v[2] << "\n";
        }
    }
};

struct Graph {
    std::vector<std::vector<std::pair<int64_t, int64_t>>> neig;
    int64_t n, m;
    Graph(int64_t n, int64_t m): n(n), m(m) {
        neig.resize(n + 1, std::vector<std::pair<int64_t, int64_t>> {});
    }
    Graph(Edges& edges): n(edges.n), m(edges.m) {
        neig.resize(n + 1, std::vector<std::pair<int64_t, int64_t>> {});
        for (auto v : edges.edges) {
            neig[v[0]].emplace_back(std::make_pair(v[1], v[2]));
            neig[v[1]].emplace_back(std::make_pair(v[0], v[2]));
        }

    }
    void input() {
        for (int64_t i = 0; i < m; ++i) {
            int64_t from, to, cost;
            std::cin >> from >> to >> cost;
            neig[from].emplace_back(std::make_pair(to, cost));
            neig[to].emplace_back(std::make_pair(from, cost));
        }
    }
    void show() const {
        for (int64_t i = 0; i < neig.size(); ++i) {
            std::cout << i << ": ";
            for (int64_t j = 0; j < neig[i].size(); ++j) {
                std::cout << "(to " << neig[i][j].first << ", for " << neig[i][j].second << ") ";
            }
            std::cout << "\n";
        }
    }
};

// TODO: IMPLEMENT BINARY HEAP INSTEAD LINEAR SEARCH
int64_t find_min(std::vector<int64_t>& dist, std::vector<int64_t>& used) {
    int64_t min_value = INF;
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

Graph Prim(Graph& graph) {
    Graph new_graph(graph.n, 0);
    std::vector<std::pair<int64_t, int64_t>> parent(graph.n + 1, std::make_pair(-1, -1));
    std::vector<int64_t> dist(graph.n + 1, INF);
    std::vector<int64_t> used(graph.n + 1, 0);
    dist[1] = 0;
    for (int64_t i = 0; i < graph.neig.size(); ++i) {
        int64_t min_ind = find_min(dist, used);
        for (int64_t j = 0; j < graph.neig[min_ind].size(); ++j) {
            if (dist[graph.neig[min_ind][j].first] > graph.neig[min_ind][j].second && !used[graph.neig[min_ind][j].first]) {
                dist[graph.neig[min_ind][j].first] = graph.neig[min_ind][j].second;
                parent[graph.neig[min_ind][j].first].second = graph.neig[min_ind][j].second;
                parent[graph.neig[min_ind][j].first].first = min_ind;
            }
        }
    }
    for (int64_t i = 0; i < parent.size(); ++i) {
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

Graph Kruskal(Edges& edges) {
    Edges new_edges(edges.n, 0);
    sort(edges.edges.begin(), edges.edges.end(), comp);
    Dsu dsu(edges.n + 1);
    for (auto edge : edges.edges) {
        if (dsu.find_parent(edge[0]) != dsu.find_parent(edge[1])) {
            dsu.unite(edge[0], edge[1]);
            new_edges.edges.emplace_back(edge);
        }
    }
    Graph graph(new_edges);
    return graph;
}

int main() {
    int64_t n, m;
    std::cin >> n >> m;
    Edges edges(n, m);
    edges.input();
    Graph kruskal_graph = Kruskal(edges);
    std::cout << "KRUSKAL:\n";
    kruskal_graph.show();

    Graph graph(edges);
    Graph prim_graph = Prim(graph);
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