#include <iostream>
#include <string>
#include <fstream>
#include <unordered_set>
#include <vector>

constexpr char INPUT_FILE[] = "day4.in";

struct Card {
    int id;
    int score;
};

Card parse_card(const std::string &s) {
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
    int id = match_number(i);
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

    return { .id=id, .score=score };
}

int main () {
    std::ifstream input_stream(INPUT_FILE);
    std::string s; 

    struct Context {
        Card card;
        int stop_flags;
    };

    std::vector<Context> cards;

    while(std::getline(input_stream, s, '\n')) {
        Card current_card = parse_card(s);
        cards.push_back({current_card, 0});
    }

    
    int num_owned_cards = 0;
    int multiplier = 1;
    for (size_t i=0; i < cards.size(); ++i) {
        num_owned_cards += multiplier;
        if (cards[i].card.score > 0) {
            cards[i+cards[i].card.score].stop_flags += multiplier;
            multiplier += multiplier;
        }
        multiplier -= cards[i].stop_flags;
    }
    std::cout << num_owned_cards << std::endl;
    return 0;
}