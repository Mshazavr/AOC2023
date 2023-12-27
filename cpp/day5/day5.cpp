#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdint>

constexpr char INPUT_FILE[] = "day5.in";

enum DestinationEntityType {
    SOIL,
    FERTILIZER,
    WATER,
    LIGHT,
    TEMPERATURE,
    HUMIDITY,
    LOCATION,
};

const std::unordered_map<std::string, DestinationEntityType> STRING_TO_ENTITY_TYPE = {
    {"seed-to-soil map:", DestinationEntityType::SOIL},
    {"soil-to-fertilizer map:", DestinationEntityType::FERTILIZER},
    {"fertilizer-to-water map:", DestinationEntityType::WATER},
    {"water-to-light map:", DestinationEntityType::LIGHT},
    {"light-to-temperature map:", DestinationEntityType::TEMPERATURE},
    {"temperature-to-humidity map:", DestinationEntityType::HUMIDITY},
    {"humidity-to-location map:", DestinationEntityType::LOCATION}
};

struct RangedMapElement {
    uint64_t destination_start;
    uint64_t source_start;
    uint64_t range;

    bool operator<(const RangedMapElement &right) {
        return source_start < right.source_start;
    }
};

uint64_t find_destination_id(uint64_t source_id, std::vector<RangedMapElement> sorted_ranged_maps) {
    // this could be a bs
    auto target_range = sorted_ranged_maps.begin();
    while(target_range < sorted_ranged_maps.end()) {
        if (source_id >= target_range->source_start+target_range->range) ++target_range;
        else break;
    }

    if (target_range == sorted_ranged_maps.end()) return source_id;
    if (
        source_id >= target_range->source_start + target_range->range ||
        source_id < target_range->source_start
    ) return source_id;
    return target_range->destination_start + (source_id - target_range->source_start);
}

int main () {
    std::fstream input_stream(INPUT_FILE);
    std::string category;
    
    input_stream >> category;
    assert(category == "seeds:");

    // read seads
    uint64_t seed_id;
    std::vector<uint64_t> seeds;
    while(input_stream >> seed_id) {
        seeds.push_back(seed_id);
    }
    input_stream.clear();

    std::unordered_map<DestinationEntityType, std::vector<RangedMapElement>> ranged_maps;

    // read maps
    for (size_t i = 0; i < STRING_TO_ENTITY_TYPE.size(); ++i) {
        std::getline(input_stream, category);
        // remove trailing whitespace
        while(isspace(category[category.size()-1])) category.pop_back();

        uint64_t destination_start, source_start, range;
        std::vector<RangedMapElement> current_ranged_maps;

        while(input_stream >> destination_start) {
            input_stream >> source_start >> range;
            current_ranged_maps.push_back({destination_start, source_start, range});
        }
        input_stream.clear();
        
        sort(current_ranged_maps.begin(), current_ranged_maps.end());
        ranged_maps[STRING_TO_ENTITY_TYPE.at(category)] = current_ranged_maps;
    }

    DestinationEntityType ordered_destination_types[] = {
        DestinationEntityType::SOIL,
        DestinationEntityType::FERTILIZER,
        DestinationEntityType::WATER,
        DestinationEntityType::LIGHT,
        DestinationEntityType::TEMPERATURE,
        DestinationEntityType::HUMIDITY,
        DestinationEntityType::LOCATION,
    };

    uint64_t min_location_id = -1;
    for(const auto &seed: seeds) {
        uint64_t destination_id  = seed;
        for(const auto &destination_type: ordered_destination_types) {
            destination_id = find_destination_id(destination_id, ranged_maps[destination_type]);
        }
        min_location_id = min_location_id == -1 ? destination_id : std::min(min_location_id, destination_id);
    }

    std::cout << min_location_id << std::endl;

    return 0;
}