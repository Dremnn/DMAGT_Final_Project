#include "FindShortestPath.h"
#include "MapPanel.h" // Để có định nghĩa đầy đủ của MapNode

// Hàm tính khoảng cách Euclidean giữa hai điểm
double Distance(const wxPoint& p1, const wxPoint& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// Thuật toán Dijkstra tìm đường đi ngắn nhất
std::vector<std::pair<int, int>> Dijkstra(
    int startNodeIndex,
    int endNodeIndex,
    const std::vector<MapNode>& nodes,
    const std::map<int, std::map<int, double>>& graph)
{
    std::vector<std::pair<int, int>> shortestPath;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;
    std::map<int, double> distances;
    std::map<int, int> previousNodes;

    for (size_t i = 0; i < nodes.size(); ++i) {
        distances[i] = std::numeric_limits<double>::infinity();
        previousNodes[i] = -1;
    }

    distances[startNodeIndex] = 0;
    pq.push({ 0, startNodeIndex });

    while (!pq.empty()) {
        double currentDistance = pq.top().first;
        int currentNodeIndex = pq.top().second;
        pq.pop();

        if (currentDistance > distances[currentNodeIndex]) {
            continue;
        }

        if (currentNodeIndex == endNodeIndex) {
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

    if (previousNodes.count(endNodeIndex) && previousNodes.at(endNodeIndex) != -1) {
        int currentNodeIndex = endNodeIndex;
        while (currentNodeIndex != -1) {
            int previousNodeIndex = previousNodes.at(currentNodeIndex);
            if (previousNodeIndex != -1) {
                shortestPath.push_back({ previousNodeIndex, currentNodeIndex });
            }
            currentNodeIndex = previousNodeIndex;
        }
        std::reverse(shortestPath.begin(), shortestPath.end());
    }

    return shortestPath;
}