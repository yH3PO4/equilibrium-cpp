#pragma once

#include <unordered_map>
#include <vector>

struct Node {
    int id;
    double lon, lat;
    Node(int _nodeID, double _lat, double _lon);
};

enum class LinkType {
    Road,
    Railway,
};

struct Link {
    int id, oNodeID, dNodeID, laneCount, maxSpeed;
    LinkType type;
    double flow, newflow, cost, freecost, capacity;
    Link(int _id, int _oNodeID, int _dNodeID, int _laneCount, int _maxSpeed);
};

class Graph {
   public:
    std::unordered_map<int, Node> nodes;
    // directed graph
    std::unordered_map<int, std::vector<Link>> adj_list;
    Graph(std::vector<Node> nodes, std::vector<Link> links);
    void assignment(double &alpha, double &beta);

   private:
};

double bpr(const double &flow, const double &freecost, const double &capacity,
           const double &alpha, const double &beta);
