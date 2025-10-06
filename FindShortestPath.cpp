#include "FindShortestPath.h"
#include "MapPanel.h" 

using namespace std;

// Hàm tính khoảng cách Euclidean giữa hai điểm
double caculateDistance(const wxPoint& p1, const wxPoint& p2) {
    double pixelDistance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    return pixelDistance * 6.565; // Trả về khoảng cách thực tế tính bằng mét
}

// Hàm tìm đường đi ngắn nhất bằng Dijkstra
vector<pair<int, int>> findShortestPath(
    int first,
    int last,
    const vector<MapNode>& nodes,               // mảng lưu các node
    const map<int, map<int, double>>& graph)    // đồ thị ma trận kề
{
    vector<pair<int, int>> shortestPath;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    map<int, double> distances;
    map<int, int> previousNodes;

    for (size_t i = 0; i < nodes.size(); ++i) {
        distances[i] = INFINITY;
        previousNodes[i] = -1;
    }

    distances[first] = 0;
    pq.push({ 0, first });

    while (!pq.empty()) {
        double currentDistance = pq.top().first;
        int currentNodeIndex = pq.top().second;
        pq.pop();

        if (currentDistance > distances[currentNodeIndex]) {
            continue;
        }

        if (currentNodeIndex == last) {
            break;
        }

        if (graph.count(currentNodeIndex)) {
            for (const auto& neighbor : graph.at(currentNodeIndex)) {
                int neighborIndex = neighbor.first;
                double weight = neighbor.second;

                if (distances[currentNodeIndex] + weight < distances[neighborIndex]) {
                    distances[neighborIndex] = distances[currentNodeIndex] + weight;
                    previousNodes[neighborIndex] = currentNodeIndex;
                    pq.push({ distances[neighborIndex], neighborIndex });
                }
            }
        }
    }

    if (previousNodes.count(last) && previousNodes.at(last) != -1) {
        int currentNodeIndex = last;
        while (currentNodeIndex != -1) {
            int previousNodeIndex = previousNodes.at(currentNodeIndex);
            if (previousNodeIndex != -1) {
                shortestPath.push_back({ previousNodeIndex, currentNodeIndex });
            }
            currentNodeIndex = previousNodeIndex;
        }
        reverse(shortestPath.begin(), shortestPath.end());
    }

    return shortestPath;
}