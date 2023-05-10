#pragma once

#include <vector>
#include <unordered_map>

struct Node
{
    int id;
    double lon;
    double lat;
    Node(int _nodeID, double _lat, double _lon);
};

enum class LinkType
{
    Road,
    Railway,
};

struct Link
{
    int id;
    int oNodeID;
    int dNodeID;
    int laneCount;
    int maxSpeed;
    LinkType type;
    Link(int _id, int _oNodeID, int _dNodeID, int _laneCount, int _maxSpeed);
};

struct Graph
{
    std::unordered_map<int, Node> nodes;
    // directed graph
    std::unordered_map<int, std::vector<Link>> adj_list;
    Graph(std::vector<Node> nodes, std::vector<Link> links);
};
