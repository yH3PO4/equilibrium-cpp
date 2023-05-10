#include "network.hpp"

Node::Node(int _nodeID, double _lat, double _lon){
    this->id = _nodeID;
    this->lat = _lat;
    this->lon = _lon;
}

Link::Link(int _id, int _oNodeID, int _dNodeID, int _laneCount, int _maxSpeed){
    this->id = _id;
    this->oNodeID = _oNodeID;
    this->dNodeID = _dNodeID;
    this->laneCount = _laneCount;
    this->maxSpeed = _maxSpeed;
    this->type = LinkType::Road; // TODO; Railwayも追加
}

Graph::Graph(std::vector<Node> nodes, std::vector<Link> links)
{
    for (const auto &node : nodes)
    {
        this->nodes.emplace(node.id, node);
    }
    for (const auto &link : links)
    {
        adj_list[link.oNodeID].emplace_back(link);
    }
}
