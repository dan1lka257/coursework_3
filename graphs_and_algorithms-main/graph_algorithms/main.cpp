#include "graph_functions.h"
#include <fstream>

int main() {
    ifstream fin;
    ofstream fout;
    fin.open("../tocpp.txt");

    // ENTERED GRAPH MUST BE INVOLVED IS SCTRICT REQUIREMENTS
    // DYNAMICAL SYSTEM INFO TESTING
    vector<vector<vector<pair<int, char>>>> graphs = graph_generator(0, 8);
    vector<pair<char, vector<float>>> separatres_coords = find_separatres_coords(graphs[0]);
    /*for (int i = 0; i < separatres_coords.size(); i++) {
        cout << separatres_coords[i].first << " ";
        for (int j = 0; j < separatres_coords[i].second.size(); j++) {
            cout << separatres_coords[i].second[j] << " ";
        }
        cout << "\n";
    }*/

    /*std::cout << "graph\n";
    int n, m;
    cin >> n >> m;
    vector<vector<pair<int, char>>> graph(n, vector<pair<int, char>>());
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a].push_back(make_pair(b, c));
        graph[b].push_back(make_pair(a, c));
    }*/

    /* DEBUG GRAPH COUT
    for (int i = 0; i < n; i++) {
        cout << i << ": ";
        for (int j = 0; j < graph[i].size(); j++) {
            cout << "in" << graph[i][j].first << "for" << graph[i][j].second << " ";
        }
        cout << endl;
    }
    */

    // GRAPH GENERATOR TESTING
    /*
    vector<vector<vector<pair<int, char>>>> graphs = graph_generator(-2, 8);
    for (int i = 0; i < graphs.size(); i++) {
        cout << "----------------------\n";
        for (int j = 0; j < graphs[i].size(); j++) {
            cout << j << "\t";
            for (int k = 0; k < graphs[i][j].size(); k++) {
                cout << graphs[i][j][k].first << " " << graphs[i][j][k].second << "\t";
            }
            cout << "\n";
        }
        print_dynamical_system_info(graphs[i]);
    }*/

    // FIND NEIGHBORS TESTING
    /*vector<vector<vector<pair<int, char>>>> graphs = graph_generator(2, 8);
    print_dynamical_system_info(graphs[1]);
    vector<pair<char, vector<int>>> neighbors = find_neighbors(graphs[1]);
    for (int i = 0; i < neighbors.size(); i++) {
        cout << i << " " << neighbors[i].first << " ";
        for (int j = 0; j < neighbors[i].second.size(); j++) {
            cout << neighbors[i].second[j] << " ";
        }
        cout << "\n";
    }*/

    fout.open("../separatres.txt", ios::trunc);
    for (int i = 0; i < separatres_coords.size(); i++) {
        fout << separatres_coords[i].first << " ";
        for (int j = 0; j < separatres_coords[i].second.size(); j++) {
            fout << separatres_coords[i].second[j] << " ";
        }
        fout << "\n";
    }
    fout.close();
    fin.close();
    return 0;
}
