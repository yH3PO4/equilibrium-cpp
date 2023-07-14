#include "io.hpp"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

std::vector<std::string> io::split(const std::string &row,
                                   const char delim = ',') {
    std::vector<std::string> res;
    std::istringstream iss(row);
    std::string s;
    while (getline(iss, s, delim)) {
        res.emplace_back(s);
    }
    return res;
}

void io::read_vertex(const std::string &in_vertex_path, Network &network) {
    std::ifstream ifs(in_vertex_path);
    if (!ifs) {
        throw "Exception : vertex network file not found";
    }
    std::string row_str;
    getline(ifs, row_str);  // 1行とばす
    while (getline(ifs, row_str)) {
        const auto row_vec = split(row_str);
        assert(row_vec.size() == 3);
        size_t vertexID = std::stoi(row_vec.at(0));
        double lat = std::stod(row_vec.at(1));
        double lon = std::stod(row_vec.at(2));
        network.add_vertex(vertexID, {lat, lon});
    }
}
void io::read_edge(const std::string &in_edge_path, Network &network) {
    std::ifstream ifs(in_edge_path);
    if (!ifs) {
        throw "Exception : edge network file not found";
    }
    std::string row_str;
    getline(ifs, row_str);  // 1行とばす

    while (getline(ifs, row_str)) {
        const auto row_vec = split(row_str);
        assert(row_vec.size() == 5);
        size_t edgeID = std::stoi(row_vec.at(0));
        size_t oVertexID = std::stoi(row_vec.at(1));
        size_t dVertexID = std::stoi(row_vec.at(2));
        int laneCount = std::stoi(row_vec.at(3));
        int maxSpeed = std::stoi(row_vec.at(4));
        double length = network.calc_length(oVertexID, dVertexID);
        network.add_edge(edgeID, oVertexID, dVertexID, {laneCount, maxSpeed, length});
    }
}

Network io::read_network(const std::string &in_vertex_path,
                         const std::string &in_edge_path) {
    Network network;
    read_vertex(in_vertex_path, network);
    read_edge(in_edge_path, network);
    return network;
}

std::vector<OD> io::read_od(const std::string &od_path) {
    std::ifstream ifs(od_path);
    if (!ifs) {
        throw "Exception : od network file not found";
    }
    std::string row_str;
    getline(ifs, row_str);  // 1行とばす

    std::vector<OD> res;
    while (getline(ifs, row_str)) {
        const auto row_vec = split(row_str);
        assert(row_vec.size() == 5);
        double olat = std::stod(row_vec.at(0));
        double olon = std::stod(row_vec.at(1));
        double dlat = std::stod(row_vec.at(2));
        double dlon = std::stod(row_vec.at(3));
        double flow = std::stod(row_vec.at(4));
        OD od = {olat, olon, dlat, dlon, flow};
        res.emplace_back(od);
    }
    return res;
}
