#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>

constexpr char INPUT_FILE[] = "day4.in";

int get_card_value(const std::string &s) {
    auto get_digit = [](char c) -> int {return (c>='0' && c<='9') ? c-'0' : -1;};
    auto match_whitespaces = [&s](size_t &i) {while(i < s.size() && isspace(s[i])) ++i;};
    auto match_number = [get_digit, &s](size_t &i) ->int {
        int result = 0;
        while(i < s.size()) {
            int digit = get_digit(s[i]);
            if (digit == -1) break; 
            result = result*10 + digit;
            ++i;
        }
        return result;
    };

    const int CARD_STRING_LENGTH = 4;
    size_t i = CARD_STRING_LENGTH;
    // the card index
    match_whitespaces(i);
    match_number(i);
    // match ':'
    ++i;

    bool reading_owned_numbers = false;
    std::unordered_set<int> winning_numbers;
    int score = 0;

    while(i < s.size()) {
        match_whitespaces(i); if (i >= s.size()) break;
        if(!reading_owned_numbers) {
            if (s[i] == '|') {
                reading_owned_numbers = true;
                ++i;
                continue;
            }
            int winning_number = match_number(i);
            winning_numbers.insert(winning_number);
        }
        else {
            int owned_number = match_number(i);
            if (winning_numbers.find(owned_number) != winning_numbers.end()) ++score;
        }
    }

    return score > 0 ? 1 << (score-1) : 0;
}

int main () {
    std::ifstream input_stream(INPUT_FILE);
    std::string s; 
    int card_values_sum = 0;
    while(std::getline(input_stream, s, '\n')) {
        card_values_sum += get_card_value(s);
    }
    std::cout << card_values_sum << std::endl;
    return 0;
}