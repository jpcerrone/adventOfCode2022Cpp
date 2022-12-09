/*
    .....    .....    .....
    .....    ..H..    ..H..
    ..H.. -> ..... -> ..T..
    .T...    .T...    .....
    .....    .....    .....

    .....    .....    .....
    .....    .....    .....
    ..H.. -> ...H. -> ..TH.
    .T...    .T...    .....
    .....    .....    .....

*/

#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <map>
#include <vector>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day9.txt";

struct Coordinate{ // extract later
    int x,y;
    void operator +=(Coordinate other){
        x += other.x;
        y += other.y;
    };
    bool operator ==(Coordinate other){
        return ((other.x == x ) && (other.y == y ));
    };
};

void moveTail(Coordinate &tailPos, Coordinate headPos){
    

}

int main(){

    const std::map<char, Coordinate> Directions = {
        {'L',{-1, 0}},
        {'R',{1, 0}},
        {'U',{0, -1}},
        {'D',{0, 1}}
    };

    std::vector<Coordinate> visitedPositions;
    std::vector<Coordinate> visitedPositionsNewTail;
    int tailsVisitedPos = 1;
    Coordinate gridDimensions = {1,1};
    Coordinate headPos = {1,1};
    Coordinate tailsPos = {1,1};

    visitedPositions.push_back({1, 1});
    visitedPositionsNewTail.push_back({1, 1});

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    // assuming no negative movement for now
    while(std::getline(ifs, line)){
        // Parse input
        char direction = line[0];
        int amount = stoi(line.substr(2));

        // Move head
        for(int i = 0; i < amount; i++){
            headPos += Directions.at(direction);
            // Move tails
            if (abs(headPos.x - tailsPos.x) > 1){
                if(headPos.x > tailsPos.x){
                        tailsPos.x++;
                    }
                else{
                    tailsPos.x--;
                }
                if(headPos.y != tailsPos.y){ // move diagonally
                    if(headPos.y > tailsPos.y){
                        tailsPos.y++;
                    }
                     else{
                        tailsPos.y--;
                    }
                }
            }
            if (abs(headPos.y - tailsPos.y) > 1){
                if(headPos.y > tailsPos.y){
                        tailsPos.y++;
                    }
                else{
                    tailsPos.y--;
                }
                if(headPos.x != tailsPos.x){ // move diagonally
                    if(headPos.x > tailsPos.x){
                        tailsPos.x++;
                    }
                     else{
                        tailsPos.x--;
                    }
                }
            }
            bool alreadyVisited = false;
            for(int i=0; i < visitedPositions.size();i++){
                if (visitedPositions[i] == tailsPos){
                    alreadyVisited = true;
                    break;
                }
            }
            if (!alreadyVisited){
                visitedPositions.push_back(tailsPos);
                tailsVisitedPos++;
            }
        }
        
    }

    std::cout << "--Ex1 Output: " << tailsVisitedPos << std::endl;
    std::cout << "--Ex2 Output: " << "" << std::endl;

    return 0;
}