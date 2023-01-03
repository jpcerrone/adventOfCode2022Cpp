#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

#include "utils/coordinate.h"

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "inputs/day12.txt";

std::vector<std::vector<char>> heightMap;
std::vector<std::vector<bool>> visitedMap;
std::vector<std::vector<int>> shortestDistanceToE;

int findShortestPath(Coordinate end , Coordinate start){ // Variation of Dikjstra's algorithm
    std::queue<Coordinate> positionsToProcess;

    positionsToProcess.push(end);
    shortestDistanceToE[end.y][end.x] = 0;
    while (!positionsToProcess.empty()){
        Coordinate position = positionsToProcess.front();
        positionsToProcess.pop();
        const char height = heightMap[position.y][position.x];
        const int currentDistanceToS = shortestDistanceToE[position.y][position.x];
        if ((position.x < heightMap[0].size() - 1) && (heightMap[position.y][position.x + 1] + 1 >= height) && (!visitedMap[position.y][position.x + 1])){
            if (shortestDistanceToE[position.y][position.x+1] > currentDistanceToS + 1){
                positionsToProcess.push({position.x+1, position.y});
                shortestDistanceToE[position.y][position.x+1] = currentDistanceToS + 1;
            }
        }
        if ((position.x > 0) && (heightMap[position.y][position.x - 1] + 1 >= height) && (!visitedMap[position.y][position.x - 1])){
            if (shortestDistanceToE[position.y][position.x-1] > currentDistanceToS + 1){
                positionsToProcess.push({position.x -1, position.y});
                shortestDistanceToE[position.y][position.x-1] = currentDistanceToS + 1;
            }
        }
        if ((position.y < heightMap.size() - 1) && (heightMap[position.y + 1][position.x] + 1 >= height) && (!visitedMap[position.y+1][position.x])){
            if (shortestDistanceToE[position.y+1][position.x] > currentDistanceToS + 1){   
                positionsToProcess.push({position.x, position.y + 1});
                shortestDistanceToE[position.y + 1][position.x] = currentDistanceToS + 1;
            }
        }
        if ((position.y > 0) && (heightMap[position.y - 1][position.x] + 1 >= height) && (!visitedMap[position.y-1][position.x])){
            if (shortestDistanceToE[position.y-1][position.x] > currentDistanceToS + 1){
                positionsToProcess.push({position.x, position.y - 1});
                shortestDistanceToE[position.y - 1][position.x] = currentDistanceToS + 1;
            }
        }
        visitedMap[position.y][position.x] = true;
    }
    return shortestDistanceToE[start.y][start.x];
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    Coordinate start;
    Coordinate end;
    // Construct height map
    int y=0;
    while(std::getline(ifs, line)){
        std::vector<char> row;
        for(int x=0; x < line.size();x++){
            if (line[x] == 'S'){
                row.push_back('a');
                start = {x, y};
            }
            else if (line[x] == 'E'){
                row.push_back('z');
                end = {x, y};
            }
            else{
                row.push_back(line[x]);
            }
        }
        heightMap.push_back(row);
        y++;
    }

    // Construct visitedMap and shortestDistanceToE
    for(int y=0; y < heightMap.size(); y++){
        visitedMap.push_back({});
        shortestDistanceToE.push_back({});
        for(int x=0; x < heightMap[0].size(); x++){
            visitedMap[y].push_back(false);
            shortestDistanceToE[y].push_back(INT_MAX);
        }      
    }

    // Dikjstra to find best path (Ex 1)
    std::cout << "--Ex1 Output: " << findShortestPath(end, start) << std::endl; // We perform Dikjstra from E to S

    // For each 'a' we get its distance to E from the shortestDistanceToE array (Ex 2)
    int shortestPathFromElevationA = INT_MAX;
    for(int y=0; y < heightMap.size() ;y++){
        for(int x=0; x < heightMap[0].size() ;x++){
            if ((heightMap[y][x] == 'a')){
                shortestPathFromElevationA = std::min(shortestDistanceToE[y][x], shortestPathFromElevationA);
            }
        }
    } 
    std::cout << "--Ex2 Output: " << shortestPathFromElevationA << std::endl;

    return 0;
}