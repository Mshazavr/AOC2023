#include <iostream>
#include <ranges>
#include <string>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <fstream>
#include <stdexcept>

constexpr char INPUT_FILE[] = "day2.in";

constexpr int MAX_RED = 12;
constexpr int MAX_GREEN = 13;
constexpr int MAX_BLUE = 14;

enum Color {
    RED,
    GREEN,
    BLUE
};

struct Game { 
    int id;
    int red; 
    int green; 
    int blue;
};

//Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game parse_line(const std::string &line) {

    auto get_digit = [](char c) -> int {return (c >= '0' && c <= '9') ? c-'0' : -1;};
    auto match_whitespaces = [&line](size_t &i) { while(i < line.size() && isspace(line[i])) ++i;};
    auto match_delim = [match_whitespaces, &line](size_t &i) {
        match_whitespaces(i);
        if (line[i] == ',' || line[i] == ';' || line[i] == ':') ++i;
        else throw std::runtime_error("Couldn't match delim");
    };
    auto match_num = [match_whitespaces, get_digit, &line](size_t &i) -> int {
        int res = 0;
        match_whitespaces(i);
        while(i < line.size()) {
            int digit = get_digit(line[i]);
            if (digit != -1) res = res * 10 + digit;
            else break;
            ++i;
        }
        if (res == 0) throw std::runtime_error("Couldn't match a number");
        return res;
    };
    auto match_color = [match_whitespaces, &line](size_t &i) -> Color {
        match_whitespaces(i);
        if (line[i] == 'r') { i += 3; return Color::RED; }
        if (line[i] == 'g') { i += 5; return Color::GREEN; }
        if (line[i] == 'b') { i += 4; return Color::BLUE; }
        throw std::runtime_error("Couldn't match color");
    };

    int max_per_color[] = {0, 0, 0};
    size_t GAME_STRING_END = 3;
    size_t i = GAME_STRING_END + 1;

    int game_id = match_num(i);
    match_whitespaces(i);

    while(i < line.size()) {
        match_delim(i);
        int num = match_num(i);
        Color color = match_color(i);
        max_per_color[color] = std::max(max_per_color[color], num);
        match_whitespaces(i);
    }

    return { 
        .id=game_id, 
        .red=max_per_color[Color::RED],
        .green=max_per_color[Color::GREEN],
        .blue=max_per_color[Color::BLUE]
    };

}

int game_id_if_valid(const std::string &line) {
    Game g = parse_line(line);
    return (
        g.red <= MAX_RED && 
        g.blue <= MAX_BLUE && 
        g.green <= MAX_GREEN
    ) ? g.id : 0;
}

int game_power(const std::string &line) {
    Game g = parse_line(line);
    return g.red * g.blue * g.green;
}

int main () {
    std::ifstream inputStream(INPUT_FILE);

    std::string line;
    int game_power_sum = 0;
    while(std::getline(inputStream, line, '\n')) {
        //game_id_sum += game_id_if_valid(line);
        game_power_sum += game_power(line);
    }
    std::cout << game_power_sum << std::endl;
}