#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <ranges>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdint>
#include <array>
#include <queue>

constexpr char INPUT_FILE[] = "day21.in";
constexpr char OUTPUT_FILE[] = "day21.out";
constexpr int N = 131;
constexpr int Sx = 65, Sy = 65;

constexpr uint64_t NUM_STEPS = 26501365;

void bfs(int source_x, int source_y, std::array<std::array<int, N>, N> *dist, const std::vector<std::string>&grid) {
    std::queue<std::pair<int, int>> destinations; 
    destinations.push({source_x, source_y});
    while(!destinations.empty()) {
        const auto&[dest_x, dest_y] = destinations.front();
        destinations.pop();
        for(int dx = -1; dx <= 1; ++dx) {
            for(int dy = -1; dy <= 1; ++dy) {
                if (!((dx == 0) ^ (dy == 0))) continue;
                int nx = dest_x + dx;
                int ny = dest_y + dy;
                if(
                    nx < 0 || nx >= N || ny < 0 || ny >= N || 
                    grid[nx][ny] == '#' || 
                    (*dist)[nx][ny] != -1
                ) continue;
                (*dist)[nx][ny] = (*dist)[dest_x][dest_y] + 1;
                destinations.push({nx, ny});
            }
        }
    }
}

int main() {
    std::fstream input_stream(INPUT_FILE);
    std::ofstream output_stream(OUTPUT_FILE);

    std::string line;
    std::vector<std::string> grid;
    while(getline(input_stream, line)) {
        while(isspace(line[line.size()-1])) line.pop_back();
        grid.push_back(line);
    }

    std::array<std::array<int, N>, N> *dist = 
    new std::array<std::array<int, N>, N>();
    
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            (*dist)[i][j] = -1;
        }
    }
    (*dist)[Sx][Sy] = 0;
    bfs(Sx, Sy, dist, grid);

    constexpr int STRIDE_1 = 65;
    constexpr int STRIDE_2 = 131;

    uint64_t inner_diamond_evens_count = 0;
    uint64_t outer_diamond_evens_count = 0;
    uint64_t inner_diamond_odds_count = 0;
    uint64_t outer_diamond_odds_count = 0;
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if (grid[i][j] == '#' || (*dist)[i][j] == -1) continue;
            if ((*dist)[i][j] <= 65) {
                if ((i+j) % 2 == 0) inner_diamond_evens_count += 1ull;
                else inner_diamond_odds_count += 1ull;
            }
            else {
                if ((i+j) % 2 == 0) outer_diamond_evens_count += 1ull;
                else outer_diamond_odds_count += 1ull;
            }
        }
    }

    uint64_t total_plots = 0;

    // Initial Inner diamond
    total_plots += inner_diamond_odds_count;

    // Subsequent strides of length N
    for(uint64_t i = 1ull; i <= (NUM_STEPS - (uint64_t)STRIDE_1) / (uint64_t)STRIDE_2; i+=1ull) {
        total_plots += (2ull*i)*(outer_diamond_evens_count + outer_diamond_odds_count);
        if (i % 2ull == 1ull) {
            total_plots += (4ull*i) * inner_diamond_evens_count;
        }
        else {
            total_plots += (4ull*i) * inner_diamond_odds_count;
        }
    }

    /* 
    // Faster, less explanatory algorithm using arithmetic series
    constexpr uint64_t NUM_STRIDES = 202300;
    total_plots = inner_diamond_odds_count;
    total_plots += (NUM_STRIDES * inner_diamond_evens_count * NUM_STRIDES * NUM_STRIDES) / 2ull;
    total_plots += (NUM_STRIDES * 2ull * inner_diamond_odds_count * (NUM_STRIDES / 2ull) * (1ull + NUM_STRIDES / 2ull));
    total_plots += NUM_STRIDES * (outer_diamond_evens_count + outer_diamond_odds_count) * (NUM_STRIDES) * (NUM_STRIDES + 1ull);
    */
   
    std::cout << total_plots << std::endl;
    
    for(int i = 0; i < N; ++i) { 
        for(int j = 0; j < N; ++j) {
            if (grid[i][j] == '#' || (*dist)[i][j] == -1) output_stream << "#";
            else if ((*dist)[i][j] > STRIDE_1) output_stream << '.';
            else output_stream << "O";
        }
        output_stream << std::endl;
    }
    
    
    return 0;
}