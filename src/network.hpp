#pragma once

#include <unordered_map>
#include <vector>

struct Node {
    int id;
    double lon, lat;
    Node(int _nodeID, double _lat, double _lon);
};

struct Link {
    int id, oNodeID, dNodeID, laneCount, maxSpeed;
    double flow, newflow, cost, freecost, capacity;
    Link(int _id, int _oNodeID, int _dNodeID, int _laneCount, int _maxSpeed);
};

class Graph {
   public:
    Graph(std::vector<Node> nodes, std::vector<Link> links);
    std::vector<Link> shortest_path(const int &oNodeID, const int &dNodeID);
    void initialize_flow();

   private:
    const double alpha = 0.48;  // BPR関数のパラメータ
    const double beta = 2.52;   // BPR関数のパラメータ
    const double gamma = 16.0;  // 日換算係数
    const double c = 350.0;     // 1車線あたりの交通容量
    std::unordered_map<int, Node> nodes;
    // directed graph
    std::unordered_map<int, std::vector<Link>> adj_list;
    double bpr(const double &flow, const double &freecost,
               const double &capacity);
};