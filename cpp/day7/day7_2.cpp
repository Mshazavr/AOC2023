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

constexpr char INPUT_FILE[] = "day7.in";
 
constexpr int NUM_CARD_TYPES = 13;
struct Card {
    static const std::unordered_map<char, int> CARD_RANK;
    char value;
    bool operator<(const Card &right) const {
        return CARD_RANK.at(value) < CARD_RANK.at(right.value);
    }
    
    bool operator==(const Card &right) const {
        return value == right.value;
    }
    
};

const std::unordered_map<char, int> Card::CARD_RANK = {
    {'J', 0},
    {'2', 1},
    {'3', 2},
    {'4', 3},
    {'5', 4},
    {'6', 5},
    {'7', 6},
    {'8', 7},
    {'9', 8},
    {'T', 9},
    {'Q', 10},
    {'K', 11},
    {'A', 12},
};

struct Hand { 
    const static int NUM_CARDS_IN_HAND = 5;

    enum HandType {
        HighCard,
        OnePair,
        TwoPairs,
        ThreeOfAKind,
        FullHouse,
        FourOfAKind,
        FiveOfAKind,
    };

    std::array<Card, NUM_CARDS_IN_HAND> cards;
    HandType type;

    Hand(const std::string &card_string) {
        for(size_t i = 0; i < NUM_CARD_TYPES; ++i) {
            cards[i] = {card_string[i]};
        }
        type = compute_hand_type();
    }

    Hand(std::array<Card, NUM_CARDS_IN_HAND> cards): cards(cards) {
        type = compute_hand_type();
    }

    HandType compute_hand_type() {
        std::array<int, NUM_CARD_TYPES> card_counts;
        card_counts.fill(0);
        for (const auto& card: cards) {
            ++card_counts[Card::CARD_RANK.at(card.value)];
        }

        int num_pairs=0, num_triples=0, num_quadruples=0, num_fivetuples=0;
        int joker_count=0;
        for(int i = 0; i < NUM_CARD_TYPES; ++i) {
            if (i == 0) { // Jokers
                joker_count = card_counts[i];
                continue;
            }
            if (card_counts[i] == 2) ++num_pairs;
            if (card_counts[i] == 3) ++num_triples;
            if (card_counts[i] == 4) ++num_quadruples;
            if (card_counts[i] == 5) ++num_fivetuples;
        }

        if (num_fivetuples == 1) return HandType::FiveOfAKind;
        if (num_quadruples == 1) {
            if (joker_count == 1) return HandType::FiveOfAKind;
            return HandType::FourOfAKind;
        }
        if (num_triples == 1 && num_pairs == 1) return HandType::FullHouse;
        if (num_triples == 1) {
            if (joker_count == 2) return HandType::FiveOfAKind;
            if (joker_count == 1) return HandType::FourOfAKind;
            return HandType::ThreeOfAKind;
        }
        if (num_pairs == 2) {
            if (joker_count == 1) return HandType::FullHouse;
            return HandType::TwoPairs;
        }
        if (num_pairs == 1) {
            if (joker_count == 3) return HandType::FiveOfAKind;
            if (joker_count == 2) return HandType::FourOfAKind;
            if (joker_count == 1) return HandType::ThreeOfAKind;
            return HandType::OnePair;
        }

        if(joker_count >= 4) return HandType::FiveOfAKind;
        if(joker_count == 3) return HandType::FourOfAKind;
        if(joker_count == 2) return HandType::ThreeOfAKind;
        if(joker_count == 1) return HandType::OnePair;
        return HandType::HighCard;
    }

    bool operator<(const Hand &right) const {
        if (type == right.type) {
            for(int i = 0; i < NUM_CARDS_IN_HAND; ++i) {
                if (cards[i] == right.cards[i]) continue;
                return cards[i] < right.cards[i];
            }
        }
        return type < right.type;
    }

};

int main () {
    std::ifstream input_stream(INPUT_FILE);

    std::string card_string;
    int bid;
    std::vector<std::pair<Hand, int>> hands_with_bids;
    while(input_stream >> card_string) {
        input_stream >> bid;
        hands_with_bids.push_back({{card_string}, bid});
    }
    sort(hands_with_bids.begin(), hands_with_bids.end());

    int total_winnings = 0;
    for(size_t i = 0; i < hands_with_bids.size(); ++i) {
        const auto &[hand, bid] = hands_with_bids[i];

        for (int j=0;j<5;++j) std::cout << hand.cards[j].value;
        std::cout << " " << hand.type << std::endl;

        total_winnings += bid * (i+1);
    }

    std::cout << total_winnings << std::endl;
    return 0;
}