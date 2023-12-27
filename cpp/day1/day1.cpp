#include <iostream>
#include <ranges>
#include <string>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <fstream>

constexpr char INPUT_FILE[] = "day1.in";

int main() {
    std::ifstream inputStream(INPUT_FILE);
    
    auto is_digit = [](char c) -> bool { return c >= '0' && c <= '9';};    

    auto extract_calibration_value = [is_digit](std::string line) -> uint16_t {
        char first = *std::ranges::find_if(line, is_digit);
        char last = *std::ranges::find_if(std::views::reverse(line), is_digit);
        return (first-'0')*10 + (last-'0');
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