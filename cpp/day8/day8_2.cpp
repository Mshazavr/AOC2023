#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include <ranges>
#include <vector>
#include <cassert>
#include <array>
#include <algorithm>
#include <cstdint>

constexpr char INPUT_FILE[] = "day8.in";


int get_node_id(char a, char b, char c) {
    return (a-'A') + 26*(b-'A') + 26*26*(c-'A');
}

std::string get_node_name(int node_id) {
    std::string name = "___";
    name[0] = (node_id % 26) + 'A';
    node_id /= 26;
    name[1] = (node_id % 26) + 'A';
    node_id /= 26;
    name[2] = (node_id % 26) + 'A';
    return name;
}

bool node_ends_with(int node_id, char c) {
    node_id /= 26;
    node_id /= 26;
    return node_id == (c - 'A');
}

bool is_start_node(int node_id) {
    return node_ends_with(node_id, 'A');
} 

bool is_end_node(int node_id) {
    return node_ends_with(node_id, 'Z');
}

struct Node {
    int id; 
    Node* left;
    Node* right;
};

uint64_t lcm(uint64_t a, uint64_t b) {
    if (a < b) std::swap(a, b);
    uint64_t product = a * b;
    while(b != 0) {
        a %= b;
        std::swap(a, b);
    }
    return product / a;
}

struct ExtendedNode { 
    int id;
    size_t instruction_position;
    ExtendedNode *next;

    bool is_start() const { return is_start_node(id); }
    bool is_end() const { return is_end_node(id); }
};

int main () {
    std::ifstream input_stream(INPUT_FILE);

    std::string instructions; 
    input_stream >> instructions;

    std::unordered_map<int, Node> nodes;
    std::vector<int> start_nodes;
    std::string input_string;
    while(input_stream >> input_string) {
        auto add_node_to_store_if_doesnt_exist = [](int node_id, std::unordered_map<int, Node> &nodes){
            if (nodes.find(node_id) == nodes.end()) nodes[node_id] = { .id=node_id, .left=nullptr, .right=nullptr};
        };

        int node_id = get_node_id(input_string[0], input_string[1], input_string[2]);
        
        // read '='
        input_stream >> input_string;

        //read (left,
        input_stream >> input_string;
        int left_node_id = get_node_id(input_string[1], input_string[2], input_string[3]);
        
        //read right)
        input_stream >> input_string;
        int right_node_id = get_node_id(input_string[0], input_string[1], input_string[2]);

        add_node_to_store_if_doesnt_exist(node_id, nodes);
        add_node_to_store_if_doesnt_exist(left_node_id, nodes);
        add_node_to_store_if_doesnt_exist(right_node_id, nodes);

        nodes[node_id].left = &nodes[left_node_id];
        nodes[node_id].right = &nodes[right_node_id];

        if (is_start_node(node_id)) start_nodes.push_back(node_id);
    }

    
    //Build the extended graph 
    std::map<std::pair<int,int>, ExtendedNode> extended_nodes;
    for(const auto &[node_id, _]: nodes) {
        for(size_t pos = 0; pos < instructions.size(); ++pos) {
            extended_nodes[{node_id, pos}] = { .id=node_id, .instruction_position=pos, .next=nullptr };
        }
    }
    
    for(const auto &[node_id, node]: nodes) {
        for(size_t pos = 0; pos < instructions.size(); ++pos) {
            size_t next_pos = (pos + 1) % instructions.size();
            if (instructions[pos] == 'L') {
                extended_nodes[{node_id, pos}].next = &extended_nodes[{node.left->id, next_pos}];
            }
            else {
                extended_nodes[{node_id, pos}].next = &extended_nodes[{node.right->id, next_pos}];
            }
        }
    }

        
    std::map<std::pair<int, int>,  std::pair<ExtendedNode const*, int> > reachable_end_nodes;

    for(const auto &[node_id_and_pos, node]: extended_nodes) {
        ExtendedNode const *current_node = &node;
        if(node_id_and_pos.second != 0) continue;
        if (!current_node->is_start()) continue;
        int num_steps = 0;
        while(!current_node->is_end()) {
            ++num_steps;
            current_node = current_node->next; 
        }
        reachable_end_nodes[node_id_and_pos] = std::make_pair(current_node, num_steps);
    }


    uint64_t total_steps = 1;
    for(const auto &[node_id_and_pos, node]: reachable_end_nodes) {
        if(node_id_and_pos.second != 0) continue;
        std::cout << "Start: " << get_node_name(node_id_and_pos.first) << " --> " << "End: " << get_node_name(node.first->id);
        std::cout << "  Dist: " << node.second << std::endl;
        total_steps = lcm(total_steps, (uint64_t)node.second);
    }

    std::cout << total_steps << std::endl;
    

    return 0;
}

