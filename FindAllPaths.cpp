#include "FindAllPaths.h"
#include "MapPanel.h"
#include <algorithm>
#include <set>

using namespace std;

void findAllPaths(int first,int last, const map<int, map<int, double>>& graph, vector<int>& currentPath, set<int>& visited, vector<vector<int>>& allPaths) {
    if (first == last) {
        allPaths.push_back(currentPath);
        return;
    }

    if (graph.count(first)) {
        for (auto& neighbor : graph.at(first)) {
            int nextNode = neighbor.first;

            if (visited.find(nextNode) == visited.end()) {
                visited.insert(nextNode);
                currentPath.push_back(nextNode);

                findAllPaths(
                    nextNode, last, graph,
                    currentPath, visited, allPaths
                );

                currentPath.pop_back();
                visited.erase(nextNode);
            }
        }
    }
}

vector<SimplePath> FindAllPaths(int startNodeIndex, int lastIndex, const vector<MapNode>& nodes, const map<int, map<int, double>>& graph) {
    vector<SimplePath> result;

    vector<vector<int>> allNodePaths;
    vector<int> currentPath = { startNodeIndex };
    set<int> visited = { startNodeIndex };

    findAllPaths(
        startNodeIndex, lastIndex, graph,
        currentPath, visited, allNodePaths
    );

    for (const auto& nodePath : allNodePaths) {
        SimplePath path;

        for (size_t i = 0; i < nodePath.size() - 1; ++i) {
            int from = nodePath[i];
            int to = nodePath[i + 1];
            path.edges.push_back({ from, to });
        }

        result.push_back(path);
    }

    return result;
}