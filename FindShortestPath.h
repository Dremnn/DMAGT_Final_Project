#pragma once
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <wx/geometry.h>
#include <utility>

// Forward declaration của struct MapNode để tránh lỗi định nghĩa lại
struct MapNode;

double calculateDistance(const wxPoint& p1, const wxPoint& p2);

// Hàm tìm đường đi ngắn nhất bằng thuật toán Dijkstra
std::vector<std::pair<int, int>> findShortestPath(
    int startNodeIndex,
    int endNodeIndex,
    const std::vector<MapNode>& nodes,
    const std::map<int, std::map<int, double>>& graph);