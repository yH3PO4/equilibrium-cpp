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
        const unsigned int vertexID = std::stoi(row_vec.at(0));
        const double lat = std::stod(row_vec.at(1));
        const double lon = std::stod(row_vec.at(2));
        network.add_vertex({vertexID, lon, lat});
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
        const unsigned int edgeID = std::stoi(row_vec.at(0));
        const unsigned int oVertexID = std::stoi(row_vec.at(1));
        const unsigned int dVertexID = std::stoi(row_vec.at(2));
        const int laneCount = std::stoi(row_vec.at(3));
        const int maxSpeed = std::stoi(row_vec.at(4));
        const double length = network.calc_length(oVertexID, dVertexID);
        network.add_edge(oVertexID, dVertexID,
                         {edgeID, laneCount, maxSpeed, length});
    }
}

Network io::read_network(const std::string &in_vertex_path,
                         const std::string &in_edge_path) {
    Network network;
    try {
        read_vertex(in_vertex_path, network);
        read_edge(in_edge_path, network);
    } catch (const char *e) {
        throw e;
    }
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
        const double olat = std::stod(row_vec.at(0));
        const double olon = std::stod(row_vec.at(1));
        const double dlat = std::stod(row_vec.at(2));
        const double dlon = std::stod(row_vec.at(3));
        const double flow = std::stod(row_vec.at(4));
        res.emplace_back(olon, olat, dlon, dlat, flow);
    }
    return res;
}

void io::output_flow(const std::string &output_path, const Network &network) {
    std::ofstream ofs(output_path);
    ofs << "edge_ID,oNodeID,oLat,oLon,dNodeID,dLat,dLon,flow" << std::endl;
    for (const auto &[source, target, edge] : network.get_link_flow()) {
        ofs << std::setprecision(10) << edge.outerID << "," << source.outerID
            << "," << source.lonlat.get<1>() << "," << source.lonlat.get<0>()
            << "," << target.outerID << "," << target.lonlat.get<1>() << ","
            << target.lonlat.get<0>() << "," << edge.flow << std::endl;
    }
}

std::unordered_map<unsigned int, std::tuple<unsigned int, unsigned int, double>>
io::read_flow(const std::string &flow_path) {
    std::ifstream ifs(flow_path);
    if (!ifs) {
        throw "Exception : flow network file not found";
    }
    std::unordered_map<unsigned int,
                       std::tuple<unsigned int, unsigned int, double>>
        res;
    std::string row_str;
    getline(ifs, row_str);  // 1行とばす
    while (getline(ifs, row_str)) {
        const auto row_vec = split(row_str);
        assert(row_vec.size() == 8);
        const unsigned int edgeID = std::stoi(row_vec.at(0));
        const unsigned int oNodeID = std::stoi(row_vec.at(1));
        const unsigned int dNodeID = std::stoi(row_vec.at(4));
        const double flow = std::stod(row_vec.at(7));
        res.emplace(edgeID, std::make_tuple(oNodeID, dNodeID, flow));
    }
    return res;
}