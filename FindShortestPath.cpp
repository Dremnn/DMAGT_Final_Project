#include "FindShortestPath.h"
#include "MapPanel.h" 

using namespace std;

double calculateDistance(const wxPoint& p1, const wxPoint& p2){
	return (sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2))) * 6.565; // 6.565 là tỉ lệ bản đồ thực tế
}

vector<pair<int, int>> findShortestPath(
    int first,
    int last,
    const vector<MapNode>& nodes,               
    const map<int, map<int, double>>& graph)   
{
    vector<pair<int, int>> shortestPath;
    map<int, double> distances;
    map<int, int> prevNode;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

    for (int i = 0; i < nodes.size(); i++){
        distances[i] = INFINITY;
        prevNode[i] = -1;
    }

    distances[first] = 0;
    pq.push({ 0, first });

    while (!pq.empty()){
        int currentID = pq.top().second;
        double currentDistance = pq.top().first;

        pq.pop();

        if (currentDistance > distances[currentID]){
            continue;
        }

        if (currentID == last){
            break;
        }

        if (graph.count(currentID)){
            for (const auto& a : graph.at(currentID)){
                int aIndex = a.first;
                double weight = a.second;

                if (distances[currentID] + weight < distances[aIndex]){
                    distances[aIndex] = distances[currentID] + weight;
                    prevNode[aIndex] = currentID;
                    pq.push({ distances[aIndex], aIndex });
                }
            }
        }
    }

    if (prevNode.find(last) != prevNode.end() && prevNode.at(last) != -1){
        int cur = last;
        while (cur != -1){
            int prev = prevNode.at(cur);
            if(prev != -1) shortestPath.push_back({ prev, cur });
            cur = prev;
        }
        reverse(shortestPath.begin(), shortestPath.end());
    }

    return shortestPath;
}