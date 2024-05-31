#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

unordered_map<char, unordered_map<char, int>> distances = {
    {'A', {{'B', 10}, {'C', 15}, {'D', 20}}},
    {'B', {{'A', 10}, {'C', 35}, {'D', 25}}},
    {'C', {{'A', 15}, {'B', 35}, {'D', 30}}},
    {'D', {{'A', 20}, {'B', 25}, {'C', 30}}}
};

vector<char> cities = { 'A', 'B', 'C', 'D' };

int calculate_cost(const vector<char>& path) {
    int max_cost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        max_cost = max(max_cost, distances[path[i]][path[i + 1]]);
    }
    return max_cost;
}

pair<vector<char>, int> bfs(char start) {
    queue<pair<char, vector<char>>> q;
    q.push({ start, {start} });
    int best_cost = INT_MAX;
    vector<char> best_path;

    while (!q.empty()) {
        auto current_pair = q.front();
        char current = current_pair.first;
        vector<char> path = current_pair.second;
        q.pop();

        if (path.size() == cities.size()) {
            path.push_back(start);
            int cost = calculate_cost(path);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
            continue;
        }

        for (char next_city : cities) {
            if (find(path.begin(), path.end(), next_city) == path.end()) {
                vector<char> new_path = path;
                new_path.push_back(next_city);
                q.push({ next_city, new_path });
            }
        }
    }

    return { best_path, best_cost };
}

pair<vector<char>, int> ucs(char start) {
    priority_queue<pair<int, pair<char, vector<char>>>, vector<pair<int, pair<char, vector<char>>>>, greater<pair<int, pair<char, vector<char>>>>> pq;
    pq.push({ 0, {start, {start}} });
    int best_cost = INT_MAX;
    vector<char> best_path;

    while (!pq.empty()) {
        auto top_pair = pq.top();
        int cost = top_pair.first;
        auto state = top_pair.second;
        char current = state.first;
        vector<char> path = state.second;
        pq.pop();

        if (path.size() == cities.size()) {
            path.push_back(start);
            cost = calculate_cost(path);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
            continue;
        }

        for (char next_city : cities) {
            if (find(path.begin(), path.end(), next_city) == path.end()) {
                int new_cost = max(cost, distances[current][next_city]);
                vector<char> new_path = path;
                new_path.push_back(next_city);
                pq.push({ new_cost, {next_city, new_path} });
            }
        }
    }

    return { best_path, best_cost };
}

pair<vector<char>, int> a_star(char start) {
    auto heuristic = [](char current, const vector<char>& unvisited) {
        int max_distance = 0;
        for (char city : unvisited) {
            max_distance = max(max_distance, distances[current][city]);
        }
        return max_distance;
        };

    priority_queue<pair<int, pair<char, vector<char>>>, vector<pair<int, pair<char, vector<char>>>>, greater<pair<int, pair<char, vector<char>>>>> pq;
    pq.push({ 0, {start, {start}} });
    int best_cost = INT_MAX;
    vector<char> best_path;

    while (!pq.empty()) {
        auto top_pair = pq.top();
        int estimated_cost = top_pair.first;
        auto state = top_pair.second;
        char current = state.first;
        vector<char> path = state.second;
        pq.pop();

        if (path.size() == cities.size()) {
            path.push_back(start);
            int cost = calculate_cost(path);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
            continue;
        }

        vector<char> unvisited;
        for (char city : cities) {
            if (find(path.begin(), path.end(), city) == path.end()) {
                unvisited.push_back(city);
            }
        }

        for (char next_city : unvisited) {
            int new_cost = max(estimated_cost, distances[current][next_city]);
            int estimated_total_cost = new_cost + heuristic(next_city, unvisited);
            vector<char> new_path = path;
            new_path.push_back(next_city);
            pq.push({ estimated_total_cost, {next_city, new_path} });
        }
    }

    return { best_path, best_cost };
}

int main() {
    char start_city = 'A';
    auto bfs_result = bfs(start_city);
    vector<char> bfs_path = bfs_result.first;
    int bfs_cost = bfs_result.second;

    auto ucs_result = ucs(start_city);
    vector<char> ucs_path = ucs_result.first;
    int ucs_cost = ucs_result.second;

    auto a_star_result = a_star(start_city);
    vector<char> a_star_path = a_star_result.first;
    int a_star_cost = a_star_result.second;

    cout << "BFS Path: ";
    for (char city : bfs_path) cout << city << " ";
    cout << "Cost: " << bfs_cost << endl;

    cout << "UCS Path: ";
    for (char city : ucs_path) cout << city << " ";
    cout << "Cost: " << ucs_cost << endl;

    cout << "A* Path: ";
    for (char city : a_star_path) cout << city << " ";
    cout << "Cost: " << a_star_cost << endl;

    return 0;
}
