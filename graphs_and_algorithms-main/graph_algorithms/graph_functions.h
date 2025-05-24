/*#pragma once*/
// Pragma shall be on when there will be prototyps of functions

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

const int INF = 1000000000;

void print(vector<int> mas) {
    // prints vector into console
    for (int i = 0; i < mas.size(); i++) {
        cout << mas[i] << " ";
    }
    cout << endl;
}

vector<int> bfs(int s, vector<vector<pair<int, char>>>& graph) {
    // BFS which find distance to other verticles
    int n = graph.size();
    vector<int> dist(n, INF);
    dist[s] = 0;
    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (pair<int, char> u : graph[v]) {
            if (dist[u.first] > dist[v] + 1) {
                dist[u.first] = dist[v] + 1;
                q.push(u.first);
            }
        }
    }

    return dist;
}

vector<int> bfs(int s, vector<vector<int>>& graph) {
    // BFS which find distance to other verticles
    int n = graph.size();
    vector<int> dist(n, INF);
    dist[s] = 0;
    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int u : graph[v]) {
            if (dist[u] > dist[v] + 1) {
                dist[u] = dist[v] + 1;
                q.push(u);
            }
        }
    }

    return dist;
}

bool is_3_colored_and_non_oriented(vector<vector<pair<int, char>>>& graph) {
    // checks graph on tricolor and orientedness
    // 1 edge of each of 3 colors in one verticle
    for (int i = 0; i < graph.size(); i++) {
        // tricolor check
        int color_u = 0, color_s = 0, color_t = 0;
        for (int j = 0; j < graph[i].size(); j++) {
            if (graph[i][j].second == 'u') color_u += 1;
            if (graph[i][j].second == 's') color_s += 1;
            if (graph[i][j].second == 't') color_t += 1;
        }
        // orientedness check
        int oriented = 1;
        for (int j = 0; j < graph[i].size(); j++) {
            if (i != graph[graph[i][j].first][j].first) {
                oriented = 0;
            }
        }
        // no cycles with len = 1
        int has_no_cycles_with_len_1 = 1;
        for (int j = 0; j < graph[i].size(); j++) {
            if (i != graph[i][j].first) {
                has_no_cycles_with_len_1 = 0;
            }
        }
        if (!(color_u == 1 && color_s == 1 && color_t == 1)) {
            // Graph is not 3 colored
            return false;
        }
        if (!(oriented)) {
            // Graph is not oriented
            return false;
        }

        if (!(has_no_cycles_with_len_1)) {
            // Graph has cycles with len = 1
            return false;
        }
    }
    return true;
}

bool is_connected(vector<vector<pair<int, char>>>& graph) {
    // checks graph is connected or not
    vector<int> dist = bfs(0, graph);
    int max_dist = 0;
    for (int j = 0; j < dist.size(); j++) {
        max_dist = max(max_dist, dist[j]);
    }
    if (max_dist == INF) {
        return false;
    }
    return true;
}

vector<int> find_cycle(char color1, char color2, char prev_color, int cur_v, int prev_v, vector<vector<pair<int, char>>>& graph, vector<string>& color, vector<int>& parent) {
    // Standard cycle finding taking into account cycle's bicolor
    color[cur_v] = "grey";
    for (int i = 0; i < graph[cur_v].size(); i++) {
        if (graph[cur_v][i].second != prev_color && (graph[cur_v][i].second == color1 || graph[cur_v][i].second == color2)) {
            if (color[graph[cur_v][i].first] == "white") {
                parent.push_back(cur_v);
                return find_cycle(color1, color2, graph[cur_v][i].second, graph[cur_v][i].first, cur_v, graph, color, parent);
            }
            if (color[graph[cur_v][i].first] == "grey" && graph[cur_v][i].first != prev_v && cur_v != prev_v) {
                parent.push_back(cur_v);
                /*
                THIS CODE MUST DESCREASE PARENT TREE TO CYCLE BUT WE DONT NEED IT BECAUSE OF GRAPH BUILDING
                int k = 0;
                for (int j = 0; j < parent.size(); j++) {
                    if (parent[j] == parent[parent.size() - 1]) {
                        k = j + 1;
                    } else {
                        break;
                    }
                }
                vector<int> cycle(parent.size() - k, 0);
                for (int i = k; i < parent.size(); i++) {
                    cycle[i - k] = parent[k];
                }*/
                return parent;
            }
        }
    }
    parent.push_back(cur_v);
    color[cur_v] = "black";
    return parent;
}

vector<vector<int>> find_cycles(vector<vector<pair<int, char>>>& graph, char color1, char color2) {
    // Find bicolored cycles in graph
    int n = graph.size();
    vector<int> used_v(n, 0);
    vector<vector<int>> cycles;
    for (int i = 0; i < n; i++) {
        if (!used_v[i]) {
            vector<string> color(n, "white");
            vector<int> parent;
            vector<int> cycle = find_cycle(color1, color2, 'o', i, i, graph, color, parent);
            if (cycle.empty()) {
                continue;
            }
            for (int j = 0; j < cycle.size(); j++) {
                used_v[cycle[j]] = 1;
            }
            cycles.push_back(cycle);
        }
    }
    return cycles;
}

bool is_acceptable(vector<vector<pair<int, char>>>& graph) {
    // Lenght of su-cycle is equal 4, graph is connected, non_oriented and tricolored
    vector<vector<int>> cycles = find_cycles(graph, 's', 'u');
    bool su_cycle_is_4 = 1;
    for (int i = 0; i < cycles.size(); i++) {
        if (cycles[i].size() != 4) {
            su_cycle_is_4 = 0;
        }
    }
    return su_cycle_is_4 && is_connected(graph) && is_3_colored_and_non_oriented(graph);
}

int count_euler_number(vector<vector<pair<int, char>>>& graph) {
    // Euler number = tu - su + st
    return find_cycles(graph, 'u', 't').size() - find_cycles(graph, 's', 'u').size() + find_cycles(graph, 's', 't').size();
}

bool is_oriented_surface(vector<vector<pair<int, char>>>& graph) {
    // Graph is on oriented surface <=> length of all cycles is even
    // All cycles is even <=> any cycle from base of cycles is even
    vector<vector<int>> spanning_tree(graph.size(), vector<int> ());
    vector<int> visited(graph.size(), 0);
    // Creating spanning tree for future cycle finding
    for (int i = 0; i < graph.size(); i++) {
        vector<int> i_neighbors;
        for (int j = 0; j < graph[i].size(); j++) {
            if (visited[graph[i][j].first] == 0) {
                spanning_tree[i].push_back(graph[i][j].first);
                spanning_tree[graph[i][j].first].push_back(i);
                i_neighbors.push_back(j);
                visited[graph[i][j].first] = 1;
                visited[i] = 1;
            }
        }
    }
    vector<int> way_from_root = bfs(0, spanning_tree);
    // Finding base of cycles & checking if cycles is even
    vector<vector<int>> base;
    bool is_even_base = true;
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {
            bool is_in = false;
            for (int k = 0; k < spanning_tree[i].size(); k++) {
                if (graph[i][j].first == spanning_tree[i][k]) {
                    is_in = true;
                    break;
                }
            }
            if (!is_in && (way_from_root[i] + way_from_root[graph[i][j].first] + 1) % 2 == 1) {
                is_even_base = false;
                cout << "!!!" << i << ":" << way_from_root[i] << " " << graph[i][j].first << ":" << way_from_root[graph[i][j].first] << "!!!";
                break;
            }
        }
        if (!is_even_base) {
            break;
        }
    }
    return is_even_base;
}

vector<int> print_dynamical_system_info(vector<vector<pair<int, char>>>& graph) {
    // Prints dynamical system info
    // result[0] - Is graph acceptable?
    // result[1] - Is surface oriented?
    // result[2] - Euler Number
    // result[3] - Number of sources
    // result[4] - Number of saddles
    // result[5] - Number of drains
    vector<int> result = {(int)is_acceptable(graph), (int)is_oriented_surface(graph), count_euler_number(graph), find_cycles(graph, 's', 't').size(), find_cycles(graph, 'u', 's').size(), find_cycles(graph, 'u', 't').size()};
    return result;
}

vector<vector<vector<pair<int, char>>>> graph_generator(int euler_number, int saddles) {
    vector<vector<vector<pair<int, char>>>> graphs(0);
    // Generating primal graph
    vector<vector<pair<int, char>>> graph(0);
    for (int i = 0; i < saddles * 4; i++) {
        vector<pair<int, char>> neighbours;
        if (i == 0) { // first verticle
            neighbours = { {i + 1, 't'}, {i + 2, 's'}, {i + 1, 'u'} };
        } else if (i == 1) { // second verticle
            neighbours = { {i - 1, 't'}, {i + 2, 's'}, {i - 1, 'u'} };
        } else if (i == saddles * 4 - 1) { // last verticle
            neighbours = { {i - 1, 't'}, {i - 2, 's'}, {i - 1, 'u'} };
        } else if (i == saddles * 4 - 2) { // penultimate verticle
            neighbours = { {i + 1, 't'}, {i - 2, 's'}, {i + 1, 'u'} };
        } else if (i % 4 == 0) {
            neighbours = { {i - 2, 't'}, {i + 2, 's'}, {i + 1, 'u'} };
        } else if (i % 4 == 1) {
            neighbours = { {i - 2, 't'}, {i + 2, 's'}, {i - 1, 'u'} };
        } else if (i % 4 == 2) {
            neighbours = { {i + 2, 't'}, {i - 2, 's'}, {i + 1, 'u'} };
        } else if (i % 4 == 3) {
            neighbours = { {i + 2, 't'}, {i - 2, 's'}, {i - 1, 'u'} };
        }
        graph.push_back(neighbours);
    }
    // operating with euler number
    if (2 - euler_number > saddles) {
        cout << "Uncorrect input: number of saddles must be lower or equal than (2 - euler_number)\n";
        return graphs;
    }
    for (int i = euler_number; i < 2; i += 2) {
        int j = 8 * (i - euler_number) / 2;
        if (euler_number == 0 && saddles == 2) { // usual torus
            graph[j][0].first = j + 1;
            graph[j + 1][0].first = (j + 1) - 1;
            graph[j + 2][0].first = (j + 2) + 4;
            graph[j + 3][0].first = (j + 3) + 1;
            graph[j + 4][0].first = (j + 4) - 1;
            graph[j + 5][0].first = (j + 5) + 2;
            graph[j + 6][0].first = (j + 6) - 4;
            graph[j + 7][0].first = (j + 7) - 2;
        } else if (i == euler_number) { // First step
            if (2 - euler_number < saddles) {
                graph[j][0].first = 8 * (2 - euler_number) / 2 + 1;
                graph[8 * (2 - euler_number) / 2 + 1][0].first = j;
            } else {
                graph[j][0].first = 8 * (2 - euler_number) / 2 - 1;
                graph[8 * (2 - euler_number) / 2 - 1][0].first = j;
            }
            graph[j + 1][0].first = (j + 1) + 4;
            graph[j + 2][0].first = (j + 2) + 4;
            graph[j + 3][0].first = (j + 3) + 1;
            graph[j + 4][0].first = (j + 4) - 1;
            graph[j + 5][0].first = (j + 5) - 4;
            graph[j + 6][0].first = (j + 6) - 4;
            graph[j + 7][0].first = (j + 7) + 1; // non-oriented graph
            graph[(j + 7) + 1][0].first = j + 7; // non-oriented graph
        } else if (i == 2 - 2 && 2 - euler_number == saddles) { // Last step
            graph[j][0].first = j - 1;
            graph[j + 1][0].first = (j + 1) + 4;
            graph[j + 2][0].first = (j + 2) + 4;
            graph[j + 3][0].first = (j + 3) + 1;
            graph[j + 4][0].first = (j + 4) - 1;
            graph[j + 5][0].first = (j + 5) - 4;
            graph[j + 6][0].first = (j + 6) - 4;
            graph[j + 7][0].first = 0;
        } else {
            graph[j][0].first = j - 1;
            graph[j + 1][0].first = (j + 1) + 4;
            graph[j + 2][0].first = (j + 2) + 4;
            graph[j + 3][0].first = (j + 3) + 1;
            graph[j + 4][0].first = (j + 4) - 1;
            graph[j + 5][0].first = (j + 5) - 4;
            graph[j + 6][0].first = (j + 6) - 4;
            graph[j + 7][0].first = (j + 7) + 1; // non-oriented graph
            graph[(j + 7) + 1][0].first = j + 7; // non-oriented graph
        }
    }
    graphs.push_back(graph);
    for (int i = (2 - euler_number) + 1; i < saddles; i += 2) {
        int j = i * 4;
        if (i == saddles - 1) {
            graph[j][0].first = j - 1;
            graph[j - 1][0].first = j;

            graph[j + 1][0].first = (j + 1) + 2; //equal
            graph[(j + 1) + 2][0].first = j + 1;


            graph[j + 2][0].first = (j + 2) - 4;
            graph[(j + 2) - 4][0].first = j + 2;

            graph[j + 3][0].first = (j + 3) - 2; // equal
            graph[(j + 3) - 2][0].first = j + 3;
        } else {
            graph[j][0].first = j - 1;
            graph[j - 1][0].first = j;

            graph[j + 1][0].first = (j + 1) + 4;
            graph[(j + 1) + 4][0].first = j + 1;


            graph[j + 2][0].first = (j + 2) - 4;
            graph[(j + 2) - 4][0].first = j + 2;

            graph[j + 3][0].first = (j + 3) + 1;
            graph[(j + 3) + 1][0].first = j + 3;
        }
        graphs.push_back(graph);
    }
    int fixed_size = graphs.size();
    for (int i = 0; i < fixed_size; i++) {
        vector<vector<pair<int, char>>> graph_reverse(0);
        graph_reverse.assign(graphs[i].begin(), graphs[i].end());
        for (int j = 0; j < graph_reverse.size(); j++) {
            int c = graph_reverse[j][1].first;
            graph_reverse[j][1].first = graph_reverse[j][2].first;
            graph_reverse[j][2].first = c;
        }
        graphs.push_back(graph_reverse);
    }
    return graphs;
}

vector<pair<char, vector<int>>> find_neighbors(vector<vector<pair<int, char>>>& graph) {
    vector<pair<char, vector<int>>> cycles(0);
    // Sourses
    vector<vector<int>> cycles_st = find_cycles(graph, 's', 't');
    vector<pair<char, vector<int>>> a_cycles(0);
    for (int i = 0; i < cycles_st.size(); i++) {
        a_cycles.push_back(make_pair('a', cycles_st[i]));
    }
    // Drains
    vector<vector<int>> cycles_ut = find_cycles(graph, 'u', 't');
    vector<pair<char, vector<int>>> o_cycles(0);
    for (int i = 0; i < cycles_ut.size(); i++) {
        a_cycles.push_back(make_pair('o', cycles_ut[i]));
    }
    // Saddles
    vector<vector<int>> cycles_su = find_cycles(graph, 's', 'u');
    vector<pair<char, vector<int>>> s_cycles(0);
    for (int i = 0; i < cycles_su.size(); i++) {
        a_cycles.push_back(make_pair('s', cycles_su[i]));
    }
    // Uniting
    cycles.insert(cycles.end(), a_cycles.begin(), a_cycles.end());
    cycles.insert(cycles.end(), o_cycles.begin(), o_cycles.end());
    cycles.insert(cycles.end(), s_cycles.begin(), s_cycles.end());
    // Cycles-neighbors finding
    int alphas = cycles_st.size();
    int omegas = cycles_ut.size();
    int sigmas = cycles_su.size();
    vector<pair<char, vector<int>>> neighbors(0);
    for (int i = 0; i < cycles.size(); i++) { // O(n^2)
        vector<int> empty_vector(0);
        neighbors.push_back(make_pair(cycles[i].first, empty_vector));
        char i_cycle_type = cycles[i].first;
        for (int j = 0; j < cycles[i].second.size(); j++) {
            int a1 = cycles[i].second[j];
            int a2 = cycles[i].second[(j + 1) % cycles[i].second.size()];
            bool is_found = false;
            for (int k = 0; k < cycles.size(); k++) {
                char k_cycle_type = cycles[k].first;
                if (((i_cycle_type == 'a' && k_cycle_type == 's' && graph[a1][1].first == a2)) || (i_cycle_type == 'o' && k_cycle_type == 's' && graph[a1][2].first == a2) || (i_cycle_type == 's' && ((k_cycle_type == 'a' && graph[a1][1].first == a2) || (k_cycle_type == 'o' && graph[a1][2].first == a2)))) {
                    if (k == i) {
                        continue;
                    }
                    for (int t = 0; t < cycles[k].second.size(); t++) {
                        int b1 = cycles[k].second[t];
                        int b2 = cycles[k].second[(t + 1) % cycles[k].second.size()];
                        if ((a1 == b1 && a2 == b2) || (a1 == b2 && a2 == b1)) {
                            neighbors[i].second.push_back(k);
                            is_found == true;
                            break;
                        }
                    }
                    if (is_found) {
                        break;
                    }
                }
            }
            if (cycles[i].second.size() == 2) {
                break;
            }
        }
    }
    return neighbors;
}

bool is_separatres_cross(vector<float> sep1, vector<float> sep2) {
    float a = sep1[0], a0 = sep1[1], b = sep1[2], b0 = sep1[3], t1 = sep1[4], t2 = sep1[5];
    float c = sep2[0], c0 = sep2[1], d = sep2[2], d0 = sep2[3], k1 = sep2[4], k2 = sep2[5];
    float max_t = max(t1, t2);
    float min_t = min(t1, t2);
    t1 = min_t;
    t2 = max_t;
    float max_k = max(k1, k2);
    float min_k = min(k1, k2);
    k1 = min_k;
    k2 = max_k;
    if ((a0 != c0 || b0 != d0) && a*d == b*c) {
        return false; // parallel
    }
    if (a0 == c0 && b0 == d0 && a*d == b*c && t2 > k1 && k2 > t1) {
        return true; // coincide
    } else if (a0 == c0 && b0 == d0 && a*d == b*c) {
        return false; // lying on one line
    }
    float y_cross = (b * d * c0 - b * c * d0 - b * d * a0 + d * a * b0) / (d * a - b * c); // calculated before (linear algebra)
    float x_cross;
    float t_cross, k_cross;
    if (b != 0) {
        t_cross = (y_cross - b0) / b; // calculated before (linear algebra)
    } else {
        x_cross = (c * y_cross - c * d0) / d + c0; // calculated before (linear algebra)
        t_cross = (x_cross - a0) / a; // calculated before (linear algebra)
    }
    if (d != 0) {
        k_cross = (y_cross - d0) / d; // calculated before (linear algebra)
    } else {
        x_cross = (a * y_cross - a * b0) / b + a0; // calculated before (linear algebra)
    }
    bool is_crossing = (t1 < t_cross && t_cross < t2) && (k1 < k_cross && k_cross < k2);
    return is_crossing;
}

vector<pair<char, vector<float>>> find_sep(int side_len, int center_len, vector<pair<char, int>> sourse_order, vector<pair<char, int>> drain_order, vector<pair<char, vector<int>>> neighbors) {
    vector<pair<int, pair<float, float>>> side(side_len);
    vector<pair<int, pair<float, float>>> center(center_len);
    for (int i = 1; i < side_len + 1; i++) {
        float y = -180 * i / (side_len + 1) + 90;
        side[i - 1] = (make_pair(sourse_order[i - 1].second, make_pair(0, y)));
    }
    for (int i = 1; i < center_len + 1; i++) {
        float y = -180 * i / (center_len + 1) + 90;
        center[i - 1] = (make_pair(drain_order[i - 1].second, make_pair(180, y)));
    }

    vector<pair<char, vector<float>>> left;
    vector<pair<char, vector<float>>> right;
    for (int i = 0; i < side.size(); i++) {
        if (i % 2) {
            vector<float> coords = {side[i - 1].second.first - side[i].second.first, side[i].second.first, side[i - 1].second.second - side[i].second.second, side[i].second.second, 0, 1};
            right.push_back(make_pair('s', coords));
            coords = {side[i + 1].second.first - side[i].second.first, side[i].second.first, side[i + 1].second.second - side[i].second.second, side[i].second.second, 0, 1};
            right.push_back(make_pair('s', coords));
        }
    }
    for (int i = 0; i < center.size(); i++) {
        if (i % 2) {
            vector<float> coords = {center[i - 1].second.first - center[i].second.first, center[i].second.first, center[i - 1].second.second - center[i].second.second, center[i].second.second, 0, 1};
            right.push_back(make_pair('u', coords));
            coords = {center[i + 1].second.first - center[i].second.first, center[i].second.first, center[i + 1].second.second - center[i].second.second, center[i].second.second, 0, 1};
            right.push_back(make_pair('u', coords));
        }
    }

    for (int i = 0; i < sourse_order.size(); i++) {
        if (true /*sourse_order[i].first == 's'*/) {
            for (int j = 0; j < neighbors[sourse_order[i].second].second.size(); j++) {
                if (neighbors[sourse_order[i].second].second[j] == sourse_order[(i - 1 + sourse_order.size()) % sourse_order.size()].second || neighbors[sourse_order[i].second].second[j] == sourse_order[(i + 1 + sourse_order.size()) % sourse_order.size()].second) {
                    continue;
                }
                float xi, yi, xj, yj;
                for (int k = 0; k < side.size(); k++) {
                    if (side[k].first == sourse_order[i].second) {
                        xi = side[k].second.first;
                        yi = side[k].second.second;
                        break;
                    }
                }
                for (int k = 0; k < center.size(); k++) {
                    if (center[k].first == neighbors[sourse_order[i].second].second[j]) {
                        xj = center[k].second.first;
                        yj = center[k].second.second;
                        break;
                    }
                }
                vector<float> sep = {xj - xi, xi, yj - yi, yi, 0, 1};
                bool is_crossing = false;
                for (int k = 0; k < left.size(); k++) {
                    if (is_separatres_cross(sep, left[k].second)) {
                        is_crossing = true;
                        break;
                    }
                }
                if (is_crossing) {
                    sep = {xj - (xi + 360), (xi + 360), yj - yi, yi, 0, 1};
                    if (sourse_order[i].first == 's') {
                        right.push_back(make_pair('u', sep));
                    } else if (sourse_order[i].first == 'a') {
                        sep = {xi - xj, xj, yi - yj, yj, 0, 1};
                        right.push_back(make_pair('s', sep));
                    }
                } else {
                    if (sourse_order[i].first == 's') {
                        left.push_back(make_pair('u', sep));
                    } else if (sourse_order[i].first == 'a') {
                        sep = {xi - xj, xj, yi - yj, yj, 0, 1};
                        left.push_back(make_pair('s', sep));
                    }
                }
            }
        }
    }

    for (int i = 0; i < drain_order.size(); i++) {
        if (drain_order[i].first == 's') {
            for (int j = 0; j < neighbors[drain_order[i].second].second.size(); j++) {
                if (neighbors[drain_order[i].second].second[j] == drain_order[(i - 1 + drain_order.size()) % drain_order.size()].second || neighbors[drain_order[i].second].second[j] == drain_order[(i + 1 + drain_order.size()) % drain_order.size()].second) {
                    continue;
                }
                float xi, yi, xj, yj;
                for (int k = 0; k < center.size(); k++) {
                    if (center[k].first == drain_order[i].second) {
                        xi = center[k].second.first;
                        yi = center[k].second.second;
                        break;
                    }
                }
                for (int k = 0; k < side.size(); k++) {
                    if (side[k].first == neighbors[drain_order[i].second].second[j]) {
                        xj = side[k].second.first;
                        yj = side[k].second.second;
                        break;
                    }
                }
                vector<float> sep = {xj - xi, xi, yj - yi, yi, 0, 1};
                bool is_crossing = false;
                for (int k = 0; k < left.size(); k++) {
                    if (is_separatres_cross(sep, left[k].second)) {
                        is_crossing = true;
                    }
                }
                if (is_crossing) {
                    sep = {(xj + 360) - xi, xi, yj - yi, yi, 0, 1};
                    right.push_back(make_pair('s', sep));
                } else {
                    left.push_back(make_pair('s', sep));
                }
            }
        }
    }
    left.insert(left.end(), right.begin(), right.end());
    return left;
}

vector<pair<char, vector<float>>> find_separatres_coords(vector<vector<pair<int, char>>>& graph) {
    vector<pair<char, vector<int>>> neighbors;
    neighbors = find_neighbors(graph);
    vector<pair<int, vector<int>>> sourses;
    vector<pair<int, vector<int>>> saddles;
    vector<pair<int, vector<int>>> drains;
    // Count sourses, saddles and drains number
    for (int i = 0; i < neighbors.size(); i++) {
        if (neighbors[i].first == 'a') {
            sourses.push_back(make_pair(i, neighbors[i].second));
        } else if (neighbors[i].first == 's') {
            saddles.push_back(make_pair(i, neighbors[i].second));
        } else if (neighbors[i].first == 'o') {
            drains.push_back(make_pair(i, neighbors[i].second));
        }
    }
    int sourses_num = sourses.size();
    int saddles_num = saddles.size();
    int drains_num = drains.size();
    int side_len = 2 * sourses_num - 1;
    int center_len = 2 * drains_num - 1;
    vector<pair<char, int>> sourse_order;
    pair<int, vector<int>> curr_v = sourses[0];
    sourse_order.push_back(make_pair('a', curr_v.first));
    while (sourse_order.size() != side_len) {
        for (int i = 0; i < curr_v.second.size(); i++) {
            bool is_saddle = false;
            int saddle_index = -1;
            int saddle_value = -1;
            for (int j = 0; j < saddles.size(); j++) {
                if (saddles[j].first == curr_v.second[i]) {
                    is_saddle = true;
                    saddle_value = saddles[j].first;
                    saddle_index = j;
                    break;
                }
            }
            if (is_saddle) {
                bool already_was = false;
                for (int j = 0; j < sourse_order.size(); j++) {
                    if (saddle_value == sourse_order[j].second) {
                        already_was = true;
                    }
                }
                if (already_was) {
                    continue;
                }
                int sourse_index1 = -1;
                int sourse_value1 = -1;
                int sourse_index2 = -1;
                int sourse_value2 = -1;
                for (int t = 0; t < saddles[saddle_index].second.size(); t++) {
                    // there could be break_factor to speed up cycle
                    for (int j = 0; j < sourses.size(); j++) {
                        if (sourses[j].first == saddles[saddle_index].second[t]) {
                            if (sourse_index1 == -1) {
                                sourse_value1 = sourses[j].first;
                                sourse_index1 = j;
                            } else if (sourse_index2 == -1) {
                                sourse_value2 = sourses[j].first;
                                sourse_index2 = j;
                            } else if (sourse_index1 != -1 && sourse_index1 != -1) {
                                break;
                            }
                        }
                    }
                }
                if (sourse_index1 == sourse_index2) {
                    continue;
                }
                bool is_sourse_index1_considered = false;
                bool is_sourse_index2_considered = false;
                for (int j = 0; j < sourse_order.size(); j++) {
                    if (sourse_order[j].second == sourse_value1) {
                        is_sourse_index1_considered = true;
                    }
                    if (sourse_order[j].second == sourse_value2) {
                        is_sourse_index2_considered = true;
                    }
                }
                int next_source_value = -1;
                int next_source_index = -1;
                if (is_sourse_index1_considered && is_sourse_index2_considered) {
                    continue;
                } else if (is_sourse_index1_considered && !is_sourse_index2_considered) {
                    next_source_index = sourse_index2;
                    next_source_value = sourse_value2;

                } else if (!is_sourse_index1_considered && is_sourse_index2_considered) {
                    next_source_index = sourse_index1;
                    next_source_value = sourse_value1;
                }
                sourse_order.push_back(make_pair('s', saddle_value));
                sourse_order.push_back(make_pair('a', next_source_value));
                curr_v = sourses[next_source_index];
            }
        }
    }
    vector<pair<char, int>> drain_order;
    curr_v = drains[0];
    drain_order.push_back(make_pair('o', curr_v.first));
    while (drain_order.size() != center_len) {
        for (int i = 0; i < curr_v.second.size(); i++) {
            bool is_saddle = false;
            int saddle_index = -1;
            int saddle_value = -1;
            for (int j = 0; j < saddles.size(); j++) {
                if (saddles[j].first == curr_v.second[i]) {
                    is_saddle = true;
                    saddle_value = saddles[j].first;
                    saddle_index = j;
                    break;
                }
            }
            if (is_saddle) {
                bool already_was = false;
                for (int j = 0; j < sourse_order.size(); j++) {
                    if (saddle_value == sourse_order[j].second) {
                        already_was = true;
                    }
                }
                for (int j = 0; j < drain_order.size(); j++) {
                    if (saddle_value == drain_order[j].second) {
                        already_was = true;
                    }
                }
                if (already_was) {
                    continue;
                }
                int drain_index1 = -1;
                int drain_value1 = -1;
                int drain_index2 = -1;
                int drain_value2 = -1;
                for (int t = 0; t < saddles[saddle_index].second.size(); t++) {
                    // there could be break_factor to speed up cycle
                    for (int j = 0; j < drains.size(); j++) {
                        if (drains[j].first == saddles[saddle_index].second[t]) {
                            if (drain_index1 == -1) {
                                drain_value1 = drains[j].first;
                                drain_index1 = j;
                            } else if (drain_index2 == -1) {
                                drain_value2 = drains[j].first;
                                drain_index2 = j;
                            } else if (drain_index1 != -1 && drain_index1 != -1) {
                                break;
                            }
                        }
                    }
                }
                if (drain_index1 == drain_index2) {
                    continue;
                }
                bool is_drain_index1_considered = false;
                bool is_drain_index2_considered = false;
                for (int j = 0; j < drain_order.size(); j++) {
                    if (drain_order[j].second == drain_value1) {
                        is_drain_index1_considered = true;
                    }
                    if (drain_order[j].second == drain_value2) {
                        is_drain_index2_considered = true;
                    }
                }
                int next_drain_value = -1;
                int next_drain_index = -1;
                if (is_drain_index1_considered && is_drain_index2_considered) {
                    continue;
                } else if (is_drain_index1_considered && !is_drain_index2_considered) {
                    next_drain_index = drain_index2;
                    next_drain_value = drain_value2;

                } else if (!is_drain_index1_considered && is_drain_index2_considered) {
                    next_drain_index = drain_index1;
                    next_drain_value = drain_value1;
                }
                drain_order.push_back(make_pair('s', saddle_value));
                drain_order.push_back(make_pair('o', next_drain_value));
                curr_v = drains[next_drain_index];
                break;
            }
        }
    }
    vector<pair<char, vector<float>>> left;
    left = find_sep(side_len, center_len, sourse_order, drain_order, neighbors);
    bool bad_left_side = false;
    for (int i = 0; i < left.size(); i++) {
        for (int j = 0; j < left.size(); j++) {
            if (is_separatres_cross(left[i].second, left[j].second)) {
                bad_left_side = true;
                break;
            }
        }
        if (bad_left_side) {
            break;
        }
    }
    if (bad_left_side) {
        reverse(drain_order.begin(), drain_order.end());
        vector<pair<char, vector<float>>> left;
        left = find_sep(side_len, center_len, sourse_order, drain_order, neighbors);
    }
    return left;
}
