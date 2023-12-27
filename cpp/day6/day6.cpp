#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdint>

constexpr char INPUT_FILE[] = "day6.in";

int main() {
    std::fstream input_stream(INPUT_FILE);

    std::string category;
    uint64_t time, distance;
    std::vector<uint64_t> times, distances;

    input_stream >> category;
    while(input_stream >> time) {
        times.push_back(time);
    }
    input_stream.clear();
    
    input_stream >> category;
    while(input_stream >> distance) {
        distances.push_back(distance);
    }
    input_stream.clear();

    uint64_t all_ways_to_win_all = 1;
    for(size_t i = 0; i < times.size(); ++i) {
        uint64_t ways_to_win = 0;
        for(uint64_t base_speed = 0; base_speed <= times[i]; ++base_speed) {
            if (base_speed*(times[i]-base_speed) > distances[i]) ++ways_to_win;
        }
        all_ways_to_win_all *= ways_to_win;
    }

    std::cout << all_ways_to_win_all << std::endl;
    return 0;
}