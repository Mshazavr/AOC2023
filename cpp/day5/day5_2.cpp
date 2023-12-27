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

std::vector<std::pair<uint64_t, uint64_t>> get_sorted_destination_ranges(
    std::vector<std::pair<uint64_t, uint64_t>> sorted_source_ranges,
    std::vector<RangedMapElement> sorted_ranged_maps
) {

    std::vector<std::pair<uint64_t, uint64_t>> destination_ranges;

    auto map_it = sorted_ranged_maps.begin();
    bool z;
    for (auto [start, end] : sorted_source_ranges) {
        while(start <= end) {
            // source           . .[. . .]. .
            // map source  [. . .]. . . . . . . . .
            while (map_it < sorted_ranged_maps.end() && map_it->source_start + map_it->range <= start) {
                ++map_it;
            }

            // no maps left OR
            // source           . .[. . .]. .
            // map source   . . . . . . . .[. . .].
            if (map_it == sorted_ranged_maps.end() || map_it->source_start > start) {
                destination_ranges.push_back({start, end});
                break;
            }

            // source              . .[. . . .]. .
            // map source v1 . .[. . . . .]. . . . . . .
            // map source v2 . .[. . . . . . . . .]. . .
            if (map_it->source_start <= start) {
                destination_ranges.push_back(
                    {
                        map_it->destination_start + start - map_it->source_start,
                        map_it->destination_start + start - map_it->source_start + std::min(map_it->range - 1, end-start),
                    }
                );
                start = map_it->source_start + map_it->range;
                continue;
            }

            // source            . .[. . . .]. .
            // map source v1 . . . . .[. .]. . . . . .
            // map source v2 . . . . .[. . . .]. . . .
            if (
                map_it->source_start > start && 
                map_it->source_start <= end
            ) {
                destination_ranges.push_back({start, map_it->source_start - 1,});
                start = map_it->source_start;
                continue;
            }

        }
    }

    sort(destination_ranges.begin(), destination_ranges.end());

    return destination_ranges;
}

int main () {
    std::fstream input_stream(INPUT_FILE);
    std::string category;
    
    input_stream >> category;
    assert(category == "seeds:");

    // read seads
    uint64_t seed_start, seed_range;
    std::vector<std::pair<uint64_t, uint64_t>> seed_ranges;
    while(input_stream >> seed_start) {
        input_stream >> seed_range;
        seed_ranges.push_back({seed_start, seed_start+seed_range-1});
    }
    input_stream.clear();
    sort(seed_ranges.begin(), seed_ranges.end());

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

    std::vector<std::pair<uint64_t, uint64_t>> destination_ranges = seed_ranges;
    for(const auto &destination_type: ordered_destination_types) {
        destination_ranges = get_sorted_destination_ranges(destination_ranges, ranged_maps[destination_type]);
    }

    uint64_t min_location_id = -1;
    for(const auto &[start, end]: destination_ranges) {
        min_location_id = min_location_id == -1 ? start : std::min(min_location_id, start);
    }

    std::cout << min_location_id << std::endl;

    return 0;
}