#pragma once
#include <vector>
#include <map>
#include <utility>

struct MapNode;

// Struct đơn giản để lưu trữ một đường đi
struct SimplePath {
    std::vector<std::pair<int, int>> edges;  // Các cạnh trong đường đi
};

// Hàm tìm tất cả đường đi từ startIndex đến endIndex
std::vector<SimplePath> FindAllPaths(
    int startNodeIndex,
    int endNodeIndex,
    const std::vector<MapNode>& nodes,
    const std::map<int, std::map<int, double>>& graph
);