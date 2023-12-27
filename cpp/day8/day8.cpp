#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
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

struct Node {
    int id; 
    Node* left;
    Node* right;
};

int main () {
    std::ifstream input_stream(INPUT_FILE);

    std::string instructions; 
    input_stream >> instructions;

    std::unordered_map<int, Node> nodes;
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
    }


    const int start_node_id = get_node_id('A', 'A', 'A');
    const int end_node_id = get_node_id('Z', 'Z', 'Z');
    int current_node_id = start_node_id;
    int instruction_index = 0;
    int num_steps = 0;
    while(current_node_id != end_node_id) { 
        ++num_steps;
        if (instructions[instruction_index] == 'L') {
            current_node_id = nodes[current_node_id].left->id;
        }
        else {
            current_node_id = nodes[current_node_id].right->id;
        }

        instruction_index = (instruction_index + 1) % instructions.size();
    }

    std::cout << num_steps << std::endl;

    return 0;
}

