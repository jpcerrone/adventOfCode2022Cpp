#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <map>
#include <vector>
#include "utils/coordinate.h"

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day9.txt";

const std::map<char, Coordinate> Directions = {
    {'L',{-1, 0}},
    {'R',{1, 0}},
    {'U',{0, -1}},
    {'D',{0, 1}}
};

void moveTail(Coordinate &tailsPos, Coordinate headPos, std::vector<Coordinate>* visitedPositions = nullptr, int* tailsVisitedPos = nullptr){
    if (abs(headPos.x - tailsPos.x) > 1)
    {
        if (headPos.x > tailsPos.x)
        {
            tailsPos.x++;
        }
        else
        {
            tailsPos.x--;
        }
        if (headPos.y != tailsPos.y)
        { // move diagonally
            if (headPos.y > tailsPos.y)
            {
                tailsPos.y++;
            }
            else
            {
                tailsPos.y--;
            }
        }
    }
    if (abs(headPos.y - tailsPos.y) > 1)
    {
        if (headPos.y > tailsPos.y)
        {
            tailsPos.y++;
        }
        else
        {
            tailsPos.y--;
        }
        if (headPos.x != tailsPos.x)
        { // move diagonally
            if (headPos.x > tailsPos.x)
            {
                tailsPos.x++;
            }
            else
            {
                tailsPos.x--;
            }
        }
    }
    if (visitedPositions != nullptr){
        bool alreadyVisited = false;
        for (int i = 0; i < visitedPositions->size(); i++)
        {
            if (visitedPositions->at(i) == tailsPos)
            {
                alreadyVisited = true;
                break;
            }
        }
        if (!alreadyVisited)
        {
            visitedPositions->push_back(tailsPos);
            (*tailsVisitedPos)++;
        }
    }
}

int main(){
    Coordinate headPos = {1,1};
    std::vector<Coordinate> tailsPositions = {
        {1,1}, // 1
        {1,1},
        {1,1},
        {1,1},
        {1,1},
        {1,1},
        {1,1},
        {1,1},
        {1,1} // 9
    };

    int tailsVisitedPos = 1;
    int ninthTailsVisitedPos = 1;

    std::vector<Coordinate> visitedPositions;
    std::vector<Coordinate> visitedPositionsNinthTail;
    visitedPositions.push_back({1, 1});
    visitedPositionsNinthTail.push_back({1, 1});

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    while(std::getline(ifs, line)){
        // Parse input
        char direction = line[0];
        int amount = stoi(line.substr(2));

        // Move head
        for(int i = 0; i < amount; i++){
            headPos += Directions.at(direction);
            for(int j=0; j < tailsPositions.size(); j++){
                // Move tails. For tails 1 and 9 calculate their visited positions (Ex1 and Ex2)
                if (j == 0){
                    moveTail(tailsPositions[j], headPos, &visitedPositions, &tailsVisitedPos);
                }
                else if (j == 8){
                    moveTail(tailsPositions[j], tailsPositions[j-1], &visitedPositionsNinthTail, &ninthTailsVisitedPos);
                }
                else{
                    moveTail(tailsPositions[j], tailsPositions[j-1]);
                }
            }
        }
        
    }

    std::cout << "--Ex1 Output: " << tailsVisitedPos << std::endl;
    std::cout << "--Ex2 Output: " << ninthTailsVisitedPos << std::endl;

    return 0;
}