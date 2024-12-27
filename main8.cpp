#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <utility>
#include <ctime>
#include <unordered_map>
#include <queue>



using namespace std;

string strip(const string& s, const string& chars = " \t\n\r\f\v")
{
    size_t start = 0;
    while (start < s.size() && chars.find(s[start]) != string::npos)
    {
        start++;
    }
    size_t end = s.size();
    while (end > start && chars.find(s[end - 1]) != string::npos)
    {
        end--;
    }
    return s.substr(start, end - start);
}

vector<string> split(const string& s, const string& delimiter = " ")
{
    vector<string> result;
    size_t start = 0;
    while (true)
    {
        size_t pos = s.find(delimiter, start);
        if (pos == string::npos)
        {
            result.push_back(s.substr(start));
            break;
        }
        result.push_back(s.substr(start, pos - start));
        start = pos + delimiter.size();
    }
    return result;
}

string replacechar(const string& s, char oldchar, char newchar)
{
    string result = s;
    for (auto &ch : result) {
        if (ch == oldchar) {
            ch = newchar;
        }
    }
    return result;
}

double euclidean_distance(const pair<double, double>& a, const pair<double, double>& b) {
    double dx = a.first - b.first;
    double dy = a.second - b.second;
    return sqrt(dx*dx + dy*dy);
}

struct Node {
    double lon, lat;
    vector<pair<Node*, double>> nodes;
};


struct Graph {
    vector<Node*> nodes;
    map<pair<double, double>, Node*> pair2node;

    Node* find_closest_node(double lat, double lon) {
        double min_dist = numeric_limits<double>::infinity();
        Node* min_node = nullptr;
        for (auto &node : nodes) {
            double dist = euclidean_distance({node->lat, node->lon}, {lat, lon});
            if (dist < min_dist) {
                min_dist = dist;
                min_node = node;
            }
        }
        return min_node;
    }

    Node* add_node(double lat, double lon) {
        Node* node = new Node();
        node->lat = lat;
        node->lon = lon;
        nodes.push_back(node);
        return node;
    }

    void read_graph(string filename) {
        ifstream fin(filename);
        if (!fin) {
            cerr << "cannot open file with graph\n";
        }
        string line;
        while (getline(fin, line)) {
            if (!line.empty() && line.back() == ';') {
                line.pop_back();
            }
            replace(line.begin(), line.end(), ':', ';');
            line = strip(line);
            line = replacechar(line, ';', ' ');
            vector<string> tokens = split(line);
            vector<string> tmp = split(tokens[0], ",");
            pair<double, double> from = {stod(tmp[1]), stod(tmp[0])};
            Node* parent_node;
            if (pair2node.count(from)) {
                parent_node = pair2node[from];
            } else {
                parent_node = add_node(stod(tmp[1]), stod(tmp[0]));
                pair2node[from] = parent_node;
            }
            for (int i = 1; i < tokens.size(); i++) {
                vector<string> tmp = split(tokens[i], ",");
                pair<double, double> to = {stod(tmp[1]), stod(tmp[0])};
                Node* child_node;
                if (pair2node.count(to)) {
                    child_node = pair2node[to];
                } else {
                    child_node = add_node(stod(tmp[1]), stod(tmp[0]));
                    pair2node[to] = child_node;
                }
                parent_node->nodes.emplace_back(child_node, stod(tmp[2]));
                child_node->nodes.emplace_back(parent_node, stod(tmp[2]));
            }
        }
    }
    void DFS_Recursive(map<Node*, bool>& visited, Node* start) {
        visited[start] = true;
        for (pair<Node*, double> neighbor : start->nodes) {
            if (!visited[neighbor.first]) {
                DFS_Recursive(visited, neighbor.first);
            }
        }
    }
 
    void DFS(Node* start) {
        map<Node*, bool> visited;
        for (Node* node : nodes) {
            visited[node] = false;
        }
        DFS_Recursive(visited, start);
    }

    void BFS(Node* start) {
        map<Node*, bool> visited;
        for (Node* node : nodes) {
            visited[node] = false;
        }
        queue<Node*> q;
        q.push(start);
        visited[start] = true;
        while (!q.empty()) {
            Node* v = q.front();
            q.pop();
            for (pair<Node*, double> u : v->nodes) {
                if (!visited[u.first]) {
                    visited[u.first] = true;
                    q.push(u.first);
                }
            }
        }
    }


    double Dijkstra(Node* start, Node* goal) {
        map<Node*, double> dist;
        for (Node* node : nodes) {
            dist[node] = numeric_limits<double>::infinity();
        }
        dist[start] = 0;
        priority_queue<pair<double, Node*>, vector<pair<double, Node*>>, greater<>> pq;
        pq.emplace(0, start);
        while (!pq.empty()) {
            pair<double, Node*> u = pq.top();
            pq.pop();
            if (dist[u.second] < u.first) {
                continue;
            }
            for (pair<Node*, double> v : u.second->nodes) {
                double new_dist = dist[u.second] + v.second;
                if (new_dist < dist[v.first]) {
                    dist[v.first] = new_dist;
                    pq.emplace(new_dist, v.first);
                }
            }
        }
        return dist[goal];
    } 

    double Astar(Node* start, Node* goal) {
        map<Node*, double> dist;
        map<Node*, double> f;
        for (Node* node : nodes) {
            dist[node] = numeric_limits<double>::infinity();
            f[node] = numeric_limits<double>::infinity();
        }
        dist[start] = 0;
        f[start] = euclidean_distance({start->lat, start->lon}, {goal->lat, goal->lon});
        priority_queue<pair<double, Node*>, vector<pair<double, Node*>>, greater<>> pq;
        pq.emplace(f[start], start);
        while (!pq.empty()) {
            pair<double, Node*> u = pq.top();
            pq.pop();
            if (u.second == goal) {
                return u.first;
            }
            for (pair<Node*, double> v : u.second->nodes) {
                double new_dist = dist[u.second] + v.second;
                if (new_dist < dist[v.first]) {
                    dist[v.first] = new_dist;
                    f[v.first] = euclidean_distance({v.first->lat, v.first->lon}, {goal->lat, goal->lon}) + new_dist;
                    pq.emplace(f[v.first], v.first);
                }
            }
        }
        return numeric_limits<double>::infinity();
    }
};



int main() {
    cout << setprecision(10);
    Graph graph;
    graph.read_graph("spb_graph.txt");
    cout << graph.nodes.size() << "\n";

    Node* start = graph.find_closest_node(59.885142, 30.368563);
    Node* end = graph.find_closest_node(59.944082, 30.295603);

    cout << "Start: " << start->lat << " " << start->lon << "\n";
    cout << "End: " << end->lat << " " << end->lon << "\n";

    time_t time1 = clock();
    graph.DFS(start);
    time_t time2 = clock();
    cout << "Time: " << (long double)(time2 - time1) / (long double)(CLOCKS_PER_SEC) << "\n";

    time_t time3 = clock();
    graph.BFS(start);
    time_t time4 = clock();
    cout << "Time: " << (long double)(time4 - time3) / (long double)(CLOCKS_PER_SEC) << "\n";
    
    time_t time5 = clock();
    cout << "Dijkstra: " << graph.Dijkstra(start, end) << "\n";
    time_t time6 = clock();
    cout << "Time: " << (long double)(time6 - time5) / (long double)(CLOCKS_PER_SEC) << "\n";

    time_t time7 = clock();
    cout << "Astar: " << graph.Astar(start, end) << "\n";
    time_t time8 = clock();
    cout << "Time: " << (long double)(time8 - time7) / (long double)(CLOCKS_PER_SEC) << "\n";
    return 0;

}
