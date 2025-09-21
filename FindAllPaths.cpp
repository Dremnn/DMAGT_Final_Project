#include "FindAllPaths.h"
#include "MapPanel.h"
#include <algorithm>
#include <set>

// Hàm đệ quy để tìm tất cả đường đi
void FindAllPathsRecursive(
    int currentNode,
    int endNode,
    const std::map<int, std::map<int, double>>& graph,
    std::vector<int>& currentPath,
    std::set<int>& visited,
    std::vector<std::vector<int>>& allPaths
) {
    // Nếu đã đến đích
    if (currentNode == endNode) {
        allPaths.push_back(currentPath);
        return;
    }

    // Duyệt các node kề
    if (graph.count(currentNode)) {
        for (const auto& neighbor : graph.at(currentNode)) {
            int nextNode = neighbor.first;

            // Nếu chưa thăm node này
            if (visited.find(nextNode) == visited.end()) {
                visited.insert(nextNode);
                currentPath.push_back(nextNode);

                FindAllPathsRecursive(
                    nextNode, endNode, graph,
                    currentPath, visited, allPaths
                );

                // Backtrack
                currentPath.pop_back();
                visited.erase(nextNode);
            }
        }
    }
}

std::vector<SimplePath> FindAllPaths(
    int startNodeIndex,
    int endNodeIndex,
    const std::vector<MapNode>& nodes,
    const std::map<int, std::map<int, double>>& graph
) {
    std::vector<SimplePath> result;

    // Tìm tất cả đường đi
    std::vector<std::vector<int>> allNodePaths;
    std::vector<int> currentPath = { startNodeIndex };
    std::set<int> visited = { startNodeIndex };

    FindAllPathsRecursive(
        startNodeIndex, endNodeIndex, graph,
        currentPath, visited, allNodePaths
    );

    // Chuyển đổi từ node paths sang edge paths
    for (const auto& nodePath : allNodePaths) {
        SimplePath path;

        // Tạo edges từ node path
        for (size_t i = 0; i < nodePath.size() - 1; ++i) {
            int from = nodePath[i];
            int to = nodePath[i + 1];
            path.edges.push_back({ from, to });
        }

        result.push_back(path);
    }

    return result;
}