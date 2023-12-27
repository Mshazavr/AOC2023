#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ranges>

constexpr char INPUT_FILE[] = "day3.in";

int main () {
    std::ifstream inputStream(INPUT_FILE);
    std::vector<std::string> grid;
    for (const auto &line: std::views::istream<std::string>(inputStream)) {
        grid.push_back(line);
    }

    int n = grid.size();
    int m = grid[0].size();

    auto is_within_bounds = [n, m](int i, int j) -> bool { return (i>=0 && j>=0 && i<n && j<m); };
    auto get_digit = [](char c) -> int { return (c >= '0' && c <= '9' ? c-'0' : -1); };
    auto is_symbol = [get_digit](char c) -> bool { return c != '.' && get_digit(c) == -1; };
    auto is_next_to_symbol = [is_symbol, is_within_bounds, grid](int i, int j) -> bool {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if ((dx == 0 && dy == 0) || !is_within_bounds(i+dx, j+dy)) continue;
                if (is_symbol(grid[i+dx][j+dy])) return true;
            }
        }
        return false;
    };
    auto match_number_next_to_symbol = [is_next_to_symbol, get_digit, n, m, grid](int i, int& j) -> int {
        bool found_symbol_neighbour = false;
        while(j < m && get_digit(grid[i][j]) == -1) ++j;

        int res = 0;
        
        while(j < m) {
            int digit = get_digit(grid[i][j]);
            if (digit == -1) break; 
            res = res*10 + digit;
            found_symbol_neighbour |= is_next_to_symbol(i, j);
            ++j;
        }

        if (!found_symbol_neighbour) return 0;
        return res;
    };

    int sum_valid_numbers = 0;
    for (int i = 0; i < n; ++i) {
        int j = 0;
        while(j < m) {
            sum_valid_numbers += match_number_next_to_symbol(i, j);
        }
    }

    std::cout << sum_valid_numbers << std::endl;

    return 0;
}