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

void bfs(int source_x, int source_y, std::array<std::array<int, 3*N>, 3*N> *dist, const std::vector<std::string>&grid) {
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
                    nx < 0 || nx >= 3*N || ny < 0 || ny >= 3*N || 
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
        line = line + line + line;
        grid.push_back(line);
    }
    for(int i = 0; i < 2*N; ++i) {
        grid.push_back(grid[i]);
    }

    assert(grid.size() == 3*N);
    assert(grid[0].size() == 3*N);

    std::array<std::array<int, 3*N>, 3*N> *dist = 
    new std::array<std::array<int, 3*N>, 3*N>();
    
    for(int i = 0; i < 3*N; ++i) {
        for(int j = 0; j < 3*N; ++j) {
            (*dist)[i][j] = -1;
        }
    }
    (*dist)[Sx+N][Sy+N] = 0;
    bfs(Sx+N, Sy+N, dist, grid);

    int res = 0;
    for(int i = 0; i < 3*N; ++i) { 
        for(int j = 0; j < 3*N; ++j) {
            if (grid[i][j] == '#' || (*dist)[i][j] == -1) output_stream << "#";
            else if ((*dist)[i][j] > 65+131) output_stream << '.';
            else {
                output_stream << "O";
                if((i + j) % 2 == 1) ++res;
            }
            
           //output_stream << " " << grid[i][j] << " ";
        }
        output_stream << std::endl;
    }

    std::cout << res << std::endl;
    
    
    return 0;
}