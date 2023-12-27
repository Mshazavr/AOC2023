#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ranges>
#include <unordered_map>

constexpr char INPUT_FILE[] = "day3.in";

struct Annotation {
    int id;
    int number;
};


int main () {
    std::ifstream inputStream(INPUT_FILE);
    std::vector<std::string> grid;
    for (const auto &line: std::views::istream<std::string>(inputStream)) {
        grid.push_back(line);
    }

    int n = grid.size();
    int m = grid[0].size();

    typedef std::vector<std::vector<Annotation*>> AnnotationGrid;

    AnnotationGrid annotation(n, std::vector<Annotation*>(m, nullptr));

    auto is_within_bounds = [n, m](int i, int j) -> bool { return (i>=0 && j>=0 && i<n && j<m); };
    auto get_digit = [](char c) -> int { return (c >= '0' && c <= '9' ? c-'0' : -1); };
    auto is_gear = [](char c) -> bool { return c == '*'; };
    auto annotate_number = [get_digit, n, m, grid](int i, int& j, AnnotationGrid &annotation) {
        while(j < m && get_digit(grid[i][j]) == -1) ++j;

        int res = 0;
        int leftmost = j;
        
        while(j < m) {
            int digit = get_digit(grid[i][j]);
            if (digit == -1) break; 
            res = res*10 + digit;
            ++j;
        }

        auto current_annotation = new Annotation { .id = i*n+leftmost, .number = res };

        for (const auto &k : std::views::iota(leftmost, j)) annotation[i][k] = current_annotation;
    };
    auto unique_numbers_next_to_gear = [is_within_bounds](int i, int j, AnnotationGrid &annotation) -> std::vector<int> {
        std::unordered_map<int, bool> used_ids;
        std::vector<int> numbers;
        numbers.reserve(8);
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (
                    (dx == 0 && dy == 0) || 
                    !is_within_bounds(i+dx, j+dy) || 
                    !annotation[i+dx][j+dy] || 
                    used_ids[annotation[i+dx][j+dy]->id]
                ) continue;

                used_ids[annotation[i+dx][j+dy]->id] = true; 
                numbers.push_back(annotation[i+dx][j+dy]->number);
            }
        }
        return numbers;
    };


    for (const auto &i: std::views::iota(0, n)) {
        int j = 0;
        while(j < m) {
            annotate_number(i, j, annotation);
        }
    }

    int sum_of_gear_ratios = 0;
    for (const auto &i: std::views::iota(0, n)) {
        for (const auto &j: std::views::iota(0, m)) {
            if (is_gear(grid[i][j])) {
                auto numbers = unique_numbers_next_to_gear(i, j, annotation);
                if (numbers.size() == 2) sum_of_gear_ratios += numbers[0] * numbers[1];
            }
        }
    }

    std::cout << sum_of_gear_ratios << std::endl;
    return 0;
}