#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "utils/coordinate.h"

#define LOG(x) std::cout << x << std::endl;
#define Matrix std::vector<std::vector<bool>>

const uint64_t EX2_ROCKS = 1000000000000;

Matrix grid; // A stack of rows, starting from the bottom
const int GRID_WIDTH = 7;

const int NUMBER_OF_PIECES = 5;
std::vector<Matrix> pieces = {
    {{1,1,1,1}},
    {
        {0,1,0},
        {1,1,1},
        {0,1,0}
    },
    { // Inverted for easier calculations, bottom up
        {1,1,1},
        {0,0,1},
        {0,0,1}
    },
        {
        {1},
        {1},
        {1},
        {1}
    },
        {
        {1,1},
        {1,1},
    }   
};


struct State{
    int pieceIdx;
    int inputIdx;
    std::vector<int> heights;
    bool operator<(const State& other) const{
        return (pieceIdx < other.pieceIdx) || 
        (pieceIdx == other.pieceIdx && inputIdx < other.inputIdx) || 
        (pieceIdx == other.pieceIdx && inputIdx == other.inputIdx && heights < other.heights);
    }
};

struct StateValue{
    int height;
    int fallenPieces;
};

std::map<State, StateValue> states;

const std::string inputFilePath = "inputs/day17.txt";

bool canPieceMoveDown(Matrix* currentPiece, Coordinate position){
    const std::vector<bool> pieceLastRow = currentPiece->at(0);
    for(int x=0; x < pieceLastRow.size(); x++){
        int upperMostY = 0;
        for(int y=0; y < currentPiece->size(); y++){
            if (!currentPiece->at(y)[x]){
                upperMostY +=1;
            }else{
                break;
            }
        }
        if (currentPiece->at(upperMostY)[x] && grid[position.y + upperMostY -1][x + position.x]){
            return false;
        }
    }
    return true;
}

bool canPieceMoveLeft(Matrix* currentPiece, Coordinate position){
    if (position.x == 0){
        return false;
    }
    for (int y = 0; y < currentPiece->size(); y++){
        int leftMostX = 0;
        for(int x=0; x < currentPiece->at(y).size(); x++){
            if (!currentPiece->at(y)[x]){
                leftMostX +=1;
            }else{
                break;
            }
        }
        if (currentPiece->at(y)[leftMostX] && grid[position.y + y][position.x + leftMostX- 1]){
            return false;
        }
    }
    return true;
}

bool canPieceMoveRight(Matrix* currentPiece, Coordinate position){
    const std::vector<bool> pieceLastRow = currentPiece->at(currentPiece->size()-1);
    if (position.x + pieceLastRow.size()> GRID_WIDTH -1){
        return false;
    }
    for (int y = 0; y < currentPiece->size(); y++){
        int rightMostX = pieceLastRow.size() - 1;
        for(int x=currentPiece->at(y).size() - 1; x >= 0; x--){
            if (!currentPiece->at(y)[x]){
                rightMostX -=1;
            }
            else{
                break;
            }
        }
        if (currentPiece->at(y)[rightMostX] && grid[position.y + y][position.x +rightMostX + 1]){
            return false;
        }
    }
    return true;
}

void placePiece(Matrix* currentPiece, Coordinate position){
    for(int y=0; y < currentPiece->size(); y++){
        for(int x = 0; x < currentPiece->at(0).size(); x++){
            if (currentPiece->at(y)[x])
            grid[y+position.y][x+position.x] = currentPiece->at(y)[x];
        }
    }
}

// For debugging purposes only
void printGrid(Matrix* currentPiece, Coordinate position){
    for(int y = 30; y >= 0; y--){
        for (int x = 0; x < GRID_WIDTH ; x++){
            Coordinate current = {x,y};
            if (current.inRange({position.x, position.y}, {position.x + (int)currentPiece->at(0).size() -1, position.y + (int)currentPiece->size() - 1 })){
                std::cout << "@";
            }
            else
            std::cout << grid[y][x];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(){
    // Construct grid with 10K rows
    std::vector<bool> floor = {1,1,1,1,1,1,1};
    grid.push_back(floor);
    for(int i=0; i< 20000; i++){
        grid.push_back({0,0,0,0,0,0,0});
    }

    Coordinate currentPiecePos = {2, 4}; // Starting from the bottom left
    uint64_t rocksThatStopped = 0;
    uint64_t yForHighestRockInRoom = 0;
    uint64_t heightSurpluss;
    bool cycleFound = false;
    int heightForEx1;

    // Get input and iterator
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string input;
    std::getline(ifs, input);
    std::string::iterator it = input.begin(); // Hack to not perform i++ on the first iteration

    while(rocksThatStopped < EX2_ROCKS){ 
        if (rocksThatStopped == 2022){
            heightForEx1 = yForHighestRockInRoom;
        }
        if (it==input.end()){ // Repeat input if it has been fully consumed
            it = input.begin();
        }
        Matrix* currentPiece = &pieces[rocksThatStopped % NUMBER_OF_PIECES];

        // Move left/right and then down
        if ((*it == '<') && canPieceMoveLeft(currentPiece, currentPiecePos)){
            currentPiecePos.x -= 1;
        }
        else if ((*it == '>') && canPieceMoveRight(currentPiece, currentPiecePos)){
            currentPiecePos.x += 1;
        }
        if(canPieceMoveDown(currentPiece, currentPiecePos)){
            currentPiecePos.y -= 1;
        }else{
            placePiece(currentPiece, currentPiecePos);
            yForHighestRockInRoom = std::max(yForHighestRockInRoom, (uint64_t)(currentPiecePos.y + currentPiece->size() -1));
            rocksThatStopped += 1;
            currentPiecePos = {2, yForHighestRockInRoom + 4};

            if(!cycleFound){
                // Calculate State
                std::vector<int> heights;
                for(int x=0; x < GRID_WIDTH; x++){
                    int differenceWithHighest = 0;
                    for(int y=yForHighestRockInRoom; y >= 0; y--){
                        if (grid[y][x]){
                            heights.push_back(differenceWithHighest);
                            break;
                        }else{
                            differenceWithHighest++;
                        }
                    }
                }
                State s = {rocksThatStopped % NUMBER_OF_PIECES, std::distance(it, input.begin()), heights};
                StateValue sv = {yForHighestRockInRoom, rocksThatStopped};
                if (states.find(s) != states.end())
                {
                    int diffPieces = rocksThatStopped - states.at(s).fallenPieces;
                    int diffHeight = yForHighestRockInRoom - states.at(s).height;
                    uint64_t remainingRocks = EX2_ROCKS - rocksThatStopped;
                    uint64_t cyclesToProcess = remainingRocks / diffPieces;
                    rocksThatStopped += cyclesToProcess * diffPieces;
                    heightSurpluss = (cyclesToProcess)*diffHeight;
                    cycleFound = true;
                } else
                {
                    states.insert({s, sv});
                }
            }
        }
        it++;
    }

    std::cout << "--Ex1 Output: " << heightForEx1 << std::endl;
    std::cout << "--Ex2 Output: " << yForHighestRockInRoom + heightSurpluss << std::endl;

    return 0;
}