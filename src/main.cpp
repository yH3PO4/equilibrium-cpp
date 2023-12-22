#include <iostream>

#include "assignment.hpp"
#include "io.hpp"

const double Network::alpha = 0.48;  // BPR関数のパラメータ
const double Network::beta = 2.52;   // BPR関数のパラメータ

const double Network::gamma = 16.0;  // 日換算係数
const double Network::c = 350.0;     // 1車線あたりの交通容量

int main() {
    const std::string in_vertex_path = "Input_data/Tokyo/TokyoNode.csv";
    const std::string in_edge_path = "Input_data/Tokyo/TokyoLink.csv";
    const std::string od_path = "Input_data/Tokyo/TokyoOD.csv";
    const std::string output_path = "Output_data/Tokyo/TokyoLinkFlow.csv";

    Network network;
    std::vector<OD> ods;
    try {
        network = io::read_network(in_vertex_path, in_edge_path);
        ods = io::read_od(od_path);
    } catch (const char *e) {
        std::cout << e << std::endl;
        return -1;
    }

    std::cout << "Number of vertices: " << network.num_vertices() << std::endl;
    std::cout << "Number of edges: " << network.num_edges() << std::endl;
    std::cout << "Number of OD pairs: " << ods.size() << std::endl;
    assignment::set_nearest_vertex(network, ods);
    assignment::assignment(network, ods);
    io::output_flow(output_path, network);
}
