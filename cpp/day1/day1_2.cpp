#include <iostream>
#include <ranges>
#include <string>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <fstream>

constexpr char INPUT_FILE[] = "day1_2.in";

const std::string DIGIT_STRINGS[] = { 
    "one",  
    "two", 
    "three", 
    "four", 
    "five", 
    "six",
    "seven",
    "eight",
    "nine", 
};


constexpr uint8_t MAX_DIGIT_STRING_LENGTH = 5; 

int main() {
    // Read input lines from file
    std::ifstream inputStream(INPUT_FILE);

    auto is_digit = [](char c) -> bool { return c >= '0' && c <= '9';};   
    
    auto extract_calibration_value = [is_digit](const std::string &line) -> uint16_t {
        int first = -1;
        for (size_t i = 0; i < line.size(); ++i) {
            if (is_digit(line[i])) { first = line[i]-'0'; break; }
            for (int j = 0; j < 9; ++j) {
                if (line.substr(i, DIGIT_STRINGS[j].size()) == DIGIT_STRINGS[j]) {
                    first = j+1;
                    break;
                }
            }
            if (first != -1) break;
        }

        int last = -1;
        for (size_t i = line.size()-1; i >= 0; --i) {
            if (is_digit(line[i])) { last = line[i]-'0'; break; }
            for (int j = 0; j < 9; ++j) {
                if (line.substr(i, DIGIT_STRINGS[j].size()) == DIGIT_STRINGS[j]) {
                    last = j+1;
                    break;
                }
            }
            if (last != -1) break;
        }

        return first * 10 + last;
    };
        
    uint16_t calibration_sum = std::ranges::fold_left(
        std::ranges::istream_view<std::string>(inputStream) 
        | std::views::transform(extract_calibration_value),
        0, 
        [](uint16_t a, uint16_t b) -> uint16_t {return a+b;}
    );

    std::cout << calibration_sum << std::endl;
    return 0;
}