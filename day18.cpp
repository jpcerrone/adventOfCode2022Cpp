#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#define LOG(x) std::cout << x << std::endl;
#define Matrix3D std::vector<std::vector<std::vector<int>>>

Matrix3D grid;

const std::string inputFilePath = "day18.txt";

int getSurfaceAreaOfBlock(bool &inBlock, int x, int y, int z){
    if (!inBlock)
    {
        if (grid[x][y][z])
        {
            inBlock = true;
            return 1;
        }
    }
    else
    {
        if (!grid[x][y][z])
        {
            inBlock = false;
            return 1;
        }
    }
}

struct Coordinate3d{
    int x, y, z;
    bool operator<(const Coordinate3d &other) const {
        return (x < other.x) ||
        (x == other.x && y < other.y ) ||
        ((x == other.x && y == other.y ) && (z < other.z));
    }
};

const int GRID_SIZE = 23; // Could be calculated from input

int getOuterSurfaceArea(int x, int y, int z, std::set<Coordinate3d>* visitedPoints = nullptr){
    if (visitedPoints == nullptr){
        visitedPoints = new std::set<Coordinate3d>();
    }
    visitedPoints->insert({x, y, z});
    int surfaceAreaOnPoint = 0;
    if ((z+1) < GRID_SIZE){
        if (grid[x][y][z+1]){
            surfaceAreaOnPoint++;
        }
        else if (visitedPoints->find({x, y , z+1}) == visitedPoints->end()){
            surfaceAreaOnPoint += getOuterSurfaceArea(x, y, z+1, visitedPoints);
        }
    }
    if ((z-1) >= 0){
        if (grid[x][y][z-1]){
            surfaceAreaOnPoint++;
        }
        else if (visitedPoints->find({x, y , z-1}) == visitedPoints->end()){
            surfaceAreaOnPoint += getOuterSurfaceArea(x, y, z-1, visitedPoints);
        }
    }
    if ((x+1) < GRID_SIZE){
        if (grid[x+1][y][z]){
            surfaceAreaOnPoint++;
        }
        else if (visitedPoints->find({x + 1, y , z}) == visitedPoints->end()){
            surfaceAreaOnPoint += getOuterSurfaceArea(x + 1, y, z, visitedPoints);
        }
    }
    if ((x-1) >= 0){
        if (grid[x - 1][y][z]){
            surfaceAreaOnPoint++;
        }
        else if (visitedPoints->find({x - 1, y , z}) == visitedPoints->end()){
            surfaceAreaOnPoint += getOuterSurfaceArea(x - 1, y, z, visitedPoints);
        }
    }
    if ((y+1) < GRID_SIZE){
        if (grid[x][y + 1][z]){
            surfaceAreaOnPoint++;
        }
        else if (visitedPoints->find({x, y + 1, z}) == visitedPoints->end()){
            surfaceAreaOnPoint += getOuterSurfaceArea(x, y + 1, z, visitedPoints);
        }
    }
    if ((y-1) >= 0){
        if (grid[x][y - 1][z]){
            surfaceAreaOnPoint++;
        }
        else if (visitedPoints->find({x, y - 1, z}) == visitedPoints->end()){
            surfaceAreaOnPoint += getOuterSurfaceArea(x, y - 1, z, visitedPoints);
        }
    }
    return surfaceAreaOnPoint;
}

int main(){

    // Construct grid
    {
        std::vector<int> zRow;
        for(int z = 0; z< GRID_SIZE; z++){
            zRow.push_back(0);
        }
        std::vector<std::vector<int>> yRow;
        for(int y = 0; y< GRID_SIZE; y++){
            yRow.push_back(zRow);
        }
        for(int x = 0; x < GRID_SIZE; x++){
            grid.push_back(yRow);
        }
    }

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    while(std::getline(ifs, line)){
        int x = std::stoi(line.substr(0, line.find(',')));
        line = line.substr(line.find(',') + 1);
        int y = std::stoi(line.substr(0, line.find(',')));
        line = line.substr(line.find(',') + 1);
        int z = std::stoi(line);
        grid[x+1][y+1][z+1] = 1; // Leave one row free on each dimension to be able to go around the object while calculating total surfae area in ex 2
    }

    int fullSurfaceArea = 0;

    // z = check forward and backwards
    for(int x=0; x < GRID_SIZE; x++){
        for(int y=0; y < GRID_SIZE; y++){
            bool inBlock = false;
            for(int z = 0; z <GRID_SIZE; z++){
                fullSurfaceArea += getSurfaceAreaOfBlock(inBlock, x, y, z);
            }
            if (grid[x][y][GRID_SIZE - 1]){ // remaining face
                fullSurfaceArea++;
            }
        }
    }
    // y = check forward and backwards
    for(int x=0; x < GRID_SIZE; x++){
        for(int z=0; z < GRID_SIZE; z++){
            bool inBlock = false;
            for(int y = 0; y <GRID_SIZE; y++){
                fullSurfaceArea += getSurfaceAreaOfBlock(inBlock, x, y, z);
            }
            if (grid[x][GRID_SIZE -1][z]){ // remaining face
                fullSurfaceArea++;
            }
        }
    }
    // x = check forward and backwards
    for(int y=0; y < GRID_SIZE; y++){
        for(int z=0; z < GRID_SIZE; z++){
            bool inBlock = false;
            for(int x = 0; x <GRID_SIZE; x++){
                fullSurfaceArea += getSurfaceAreaOfBlock(inBlock, x, y, z);
            }
            if (grid[GRID_SIZE - 1][y][z]){ // remaining face
                fullSurfaceArea++;
            }
        }
    }

    std::cout << "--Ex1 Output: " << fullSurfaceArea << std::endl;
    std::cout << "--Ex2 Output: " << getOuterSurfaceArea(0,0,0) << std::endl;

    return 0;
}