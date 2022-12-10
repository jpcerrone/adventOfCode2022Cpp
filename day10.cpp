#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day10.txt";

const std::vector<int> cyclesToCheck = {20, 60, 100, 140, 180, 220};

const int CRT_WIDTH = 40;
const int CRT_HEIGHT = 6;

std::vector<std::vector<char>> display;

void checkCycleAndAddSignalStrength(int cycles, int &currentCycleToCheck ,int &signalStrength, int regX){
    if (currentCycleToCheck < 6){
        if (cycles == cyclesToCheck.at(currentCycleToCheck)){
            signalStrength += regX * cycles;
            currentCycleToCheck++;
        }
    }
}

void checkXAndSetPixel(int cycles, int regX){
    int currentRow = (cycles-1) /  (CRT_WIDTH); // Cycles start at 1, but we want to index the display from 0->39
    int currentPixel = (cycles - 1)  % CRT_WIDTH;
    if (currentPixel >= regX-1 && currentPixel <= regX+1)
        display[currentRow][currentPixel] = '#';
}

int main(){
    int cycles = 1;
    int regX = 1; // Middle '#' in '###' (Ex 2)
    int signalStrength = 0;

    int currentCycleToCheck = 0;

    // Fill CRT with '.' (Ex 2)
    for(int y=0; y < CRT_HEIGHT;y++){
        std::vector<char> row = {};
        for(int x=0; x < CRT_WIDTH;x++){
            row.push_back('.');
        }
        display.push_back(row);
    }

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    while(std::getline(ifs, line)){
        if (line == "noop"){
            checkXAndSetPixel(cycles, regX); // Ex 2
            checkCycleAndAddSignalStrength(cycles, currentCycleToCheck, signalStrength, regX);
            cycles++; 
        }
        else{ //addx
            // first cycle
            checkXAndSetPixel(cycles, regX); // Ex 2
            checkCycleAndAddSignalStrength(cycles, currentCycleToCheck, signalStrength, regX);
            cycles++;

            // second cycle
            checkXAndSetPixel(cycles, regX); // Ex 2
            checkCycleAndAddSignalStrength(cycles, currentCycleToCheck, signalStrength, regX);
            regX += stoi(line.substr(5)); // perform addx
            cycles++;

        }


    }

    std::cout << "--Ex1 Output: " << signalStrength << std::endl;
    std::cout << "--Ex2 Output: " << std::endl;

    // Display CRT
    for(int y=0; y < CRT_HEIGHT; y++){
        for(int x=0; x < CRT_WIDTH;x++){
            std::cout << display[y][x];
        }
        std::cout<< std::endl;
    }

    return 0;
}