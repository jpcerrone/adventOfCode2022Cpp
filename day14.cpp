#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day14.txt";

const int ARRAY_SIZE = 1000; // Try changing for 600 later
std::array<std::array<char, ARRAY_SIZE>, ARRAY_SIZE> grid;

struct Coordinate{ // Extract later
    int x,y;
    void operator +=(Coordinate other){
        x += other.x;
        y += other.y;
    };
    bool operator ==(Coordinate other){
        return ((other.x == x ) && (other.y == y ));
    };
    Coordinate operator +(Coordinate other){
        Coordinate c = {x+other.x, y+other.y};
        return c;
    };
};

const Coordinate SAND_ORIGIN = {500,0};

int getUnitsOfSandAtRestAfterSimulation(std::array<std::array<char, ARRAY_SIZE>, ARRAY_SIZE> grid, int maxY = INT_MAX){
    int unitsOfSandAtRest = 0;
    bool simulateSand = true;
    while(simulateSand){
        Coordinate unitOfSandPos = {SAND_ORIGIN.x, SAND_ORIGIN.y};
        while(true){ // While not settling
            if (unitOfSandPos.y > maxY){
                simulateSand = false;
                break;
            }
            if (grid[unitOfSandPos.y+1][unitOfSandPos.x] == '.'){
                unitOfSandPos += {0,1};
            }
            else if (grid[unitOfSandPos.y+1][unitOfSandPos.x-1] == '.'){
                unitOfSandPos += {-1,1};
            }
            else if (grid[unitOfSandPos.y+1][unitOfSandPos.x+1] == '.'){
                unitOfSandPos += {1,1};
            }
            else{
                grid[unitOfSandPos.y][unitOfSandPos.x] = 'o';
                unitsOfSandAtRest++;
                if (unitOfSandPos.y == SAND_ORIGIN.y){
                    simulateSand = false;
                }
                break;
            }
        }   
    }
    return unitsOfSandAtRest;
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;


    for(int i=0;i < ARRAY_SIZE; i++){
        grid[i].fill('.');
    }

    int maxY = 0; // This will be the row after which we consider sand to not be going to settle anymore, we can get this after parsing the input

    // Parse input, add rocks to grid
    while(std::getline(ifs, line)){
        int firstX = std::stoi(line.substr(0, line.find(',')));
        int firstY = std::stoi(line.substr(line.find(',') + 1, line.find('-')));
        Coordinate firstPoint = {firstX, firstY};
        Coordinate previousPoint = firstPoint;
    
        while(line.find("->") != line.npos){
            line = line.substr(line.find("->") + 3);
            int newX = std::stoi(line.substr(0, line.find(',')));
            int newY;
            if (line.find("->") != line.npos){
                newY = std::stoi(line.substr(line.find(',') + 1, line.find("->")));
            }
            else{
                newY = std::stoi(line.substr(line.find(',') + 1));
            }
            Coordinate newPoint = {newX, newY};

            // Draw rocks
            int distanceRight = previousPoint.x - newPoint.x;
            int distanceDown = previousPoint.y - newPoint.y;
            for(int x=previousPoint.x; x != newPoint.x; x -= std::clamp(distanceRight, -1, 1)){
                grid[previousPoint.y][x] = '#';
                maxY = (previousPoint.y > maxY) ? previousPoint.y : maxY;
            }
            for(int y=previousPoint.y; y != newPoint.y; y -= std::clamp(distanceDown, -1, 1)){
                grid[y][previousPoint.x] = '#';
            }
            grid[newPoint.y][newPoint.x] = '#';
            maxY = (newPoint.y > maxY) ? newPoint.y : maxY;

            previousPoint = newPoint;

        }
    }

    std::array<std::array<char, ARRAY_SIZE>, ARRAY_SIZE> gridCopy = grid; // copy the initial state of the grid for Ex 2 and add the rock row
    for(int x=0; x < ARRAY_SIZE; x++){
        gridCopy[maxY + 2][x] = '#';
    }

    std::cout << "--Ex1 Output: " << getUnitsOfSandAtRestAfterSimulation(grid, maxY) << std::endl;
    std::cout << "--Ex2 Output: " << getUnitsOfSandAtRestAfterSimulation(gridCopy) << std::endl;

    return 0;
}