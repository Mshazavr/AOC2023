#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdint>

constexpr char INPUT_FILE[] = "day21.in";
constexpr char OUTPUT_FILE[] = "day21.out";

constexpr int NUM_STEPS = 64;

int main() {
    std::fstream input_stream(INPUT_FILE);

    std::string line;
    std::vector<std::string> grid;
    std::vector<std::vector<bool>> grid_mask;
    while(getline(input_stream, line)) {
        while(isspace(line[line.size()-1])) line.pop_back();
        grid.push_back(line);
    }

    int n = grid.size();
    int m = grid[0].size();

    std::cout << n << ", " << m << std::endl;

    for(int i = 0; i < n; ++i) {
        std::vector<bool> row_mask(m, false);
        grid_mask.push_back(row_mask);
    }


    for(int k = 0; k < NUM_STEPS; ++k) {
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                grid_mask[i][j] = false;
            }
        }

        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                if (grid[i][j] == 'S') {
                    for(int dx = -1; dx <= 1; ++dx) {
                        for(int dy = -1; dy <= 1; ++dy) {
                            if (dx != 0 && dy != 0) continue;
                            if (dx == 0 && dy == 0) continue;
                            int nx = i+dx;
                            int ny = j+dy;
                            if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
                                grid_mask[nx][ny] = true;
                            }
                        }
                    }
                }
            }
        }

        
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                if (grid_mask[i][j] && grid[i][j] != '#') {
                    grid[i][j] = 'S';
                }
                else if (grid[i][j] == 'S') grid[i][j] = '.';
            }
        }
    }

    int answ = 0;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            if(grid[i][j] == 'S') ++answ;
        }
    }
    std::cout << answ << std::endl;
    return 0;
}