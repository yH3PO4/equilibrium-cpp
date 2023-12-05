#include <iostream>

#include "assignment.hpp"
#include "io.hpp"

const double Network::alpha = 0.48;  // BPR関数のパラメータ
const double Network::beta = 2.52;   // BPR関数のパラメータ

const double Network::gamma = 16.0;  // 日換算係数
const double Network::c = 350.0;     // 1車線あたりの交通容量

bool test(const std::string &in_vertex_path, const std::string &in_edge_path,
          const std::string &od_path, const std::string &output_path) {
    Network network = io::read_network(in_vertex_path, in_edge_path);
    std::vector<OD> ods = io::read_od(od_path);
    assignment::set_nearest_vertex(network, ods);
    assignment::assignment(network, ods);
    // io::output_flow(output_path, network);

    std::unordered_map<size_t, std::tuple<size_t, size_t, double>> exact_solution =
        io::read_flow(output_path);
    for (const auto &[source_props, target_props, edge_props] :
         network.get_link_flow()) {
        int source = source_props.outerID;
        int target = target_props.outerID;
        double flow = edge_props.flow;
        const auto &[exact_source, exact_target, exact_flow] =
            exact_solution.at(edge_props.outerID);
        if (source != exact_source || target != exact_target ||
            std::abs(flow - exact_flow) / exact_flow > 1e-3) {
            std::cout << "failed at edge: " << edge_props.outerID
                      << ", source: " << source << ", target: " << target
                      << ", flow: " << flow << ", exact_flow:" << exact_flow
                      << std::endl;
            return false;
        }
    }
    return true;
}

int main() {
    if (test("Input_data/test/1/testNode.csv", "Input_data/test/1/testLink.csv",
             "Input_data/test/1/testOD.csv",
             "Output_data/test/1/testLinkFlow.csv")) {
        std::cout << "Test 1: Success" << std::endl;
    } else {
        std::cout << "Test 1: Failed" << std::endl;
    }

    if (test("Input_data/test/2/testNode.csv", "Input_data/test/2/testLink.csv",
             "Input_data/test/2/testOD.csv",
             "Output_data/test/2/testLinkFlow.csv")) {
        std::cout << "Test 2: Success" << std::endl;
    } else {
        std::cout << "Test 2: Failed" << std::endl;
    }
}
