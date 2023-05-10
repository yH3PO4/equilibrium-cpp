#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include "io.hpp"

std::vector<std::string> io::split(const std::string &row, const char delim = ',')
{
    std::vector<std::string> res;
    std::istringstream iss(row);
    std::string s;
    while (getline(iss, s, delim))
    {
        res.emplace_back(s);
    }
    return res;
}

std::vector<Node> io::read_node(const std::string &in_node_path)
{
    std::ifstream ifs(in_node_path);
    if (!ifs)
    {
        throw "Exception : node network file not found";
    }
    std::string row_str;
    getline(ifs, row_str); // 1行とばす

    std::vector<Node> res;
    while (getline(ifs, row_str))
    {
        const auto row_vec = split(row_str);
        assert(row_vec.size() == 3);
        int nodeID = std::stoi(row_vec.at(0));
        double lat = std::stod(row_vec.at(1));
        double lon = std::stod(row_vec.at(2));
        Node node = {nodeID, lat, lon};
        res.emplace_back(node);
    }
    return res;
}
std::vector<Link> io::read_link(const std::string &in_link_path)
{
    std::ifstream ifs(in_link_path);
    if (!ifs)
    {
        throw "Exception : link network file not found";
    }
    std::string row_str;
    getline(ifs, row_str); // 1行とばす

    std::vector<Link> res;
    while (getline(ifs, row_str))
    {
        const auto row_vec = split(row_str);
        assert(row_vec.size() == 5);
        int linkID = std::stoi(row_vec.at(0));
        int oNodeID = std::stoi(row_vec.at(1));
        int dNodeID = std::stoi(row_vec.at(2));
        int laneCount = std::stoi(row_vec.at(3));
        int maxSpeed = std::stoi(row_vec.at(4));
        Link link = {linkID, oNodeID, dNodeID, laneCount, maxSpeed};
        res.emplace_back(link);
    }
    return res;
}

Graph io::read_network(const std::string &in_node_path, const std::string &in_link_path)
{
    return Graph{read_node(in_node_path), read_link(in_link_path)};
}
