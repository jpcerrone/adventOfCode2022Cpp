#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day8.txt";

std::vector<std::vector<int>> treeGrid; // Assuming square input
std::vector<std::vector<int>> visibleTreeGrid; // Assuming square input.

void fillMatrixWith0(std::vector<std::vector<int>> &matrix, const int rowSize){
    for(int y=0;y < rowSize; y++){
        std::vector<int> row;
        for(int x=0;x < rowSize; x++){
            row.push_back(0);
        }
        matrix.push_back(row);
    }
}

int checkTreeVisible(int x, int y, int &maxHeight){
    int visibleTrees = 0;
    if (treeGrid[y][x] > maxHeight)
    {
        maxHeight = treeGrid[y][x];
        if (visibleTreeGrid[y][x] == 0)
        {
            visibleTreeGrid[y][x] = 1;
            visibleTrees++;
        }
    }
    return visibleTrees;
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string row;

    // Construct input matrix based on row size, fill it with 0's
    std::getline(ifs, row);
    const size_t rowSize = row.length();
    fillMatrixWith0(treeGrid, rowSize);
    fillMatrixWith0(visibleTreeGrid, rowSize);

    ifs.seekg (0, ifs.beg); // Reset to begining
    for(int y = 0; std::getline(ifs, row); y++){
        for(int x=0;x < row.size();x++){
            treeGrid[y][x] = row[x] - 48; // Substract 48 to convert the ASCII value of the char to the actual number we want
        }
    }

    // Iterate from all 4 sides
    int visibleTrees = 0;
    for(int y = 0; y < rowSize; y++){
        //left-right
        int maxHeight = treeGrid[y][0];
        visibleTreeGrid[y][0] = 1;
        visibleTrees++;
        int x = 1;
        while(x < rowSize - 1){
            visibleTrees += checkTreeVisible(x,y,maxHeight);
            x++;
        }
        //right-left
        maxHeight = treeGrid[y][x];
        visibleTreeGrid[y][x] = 1;
        visibleTrees++;
        while(--x > 0){
            visibleTrees += checkTreeVisible(x,y,maxHeight);
            x--;
        }
    }
    for(int x = 1; x < rowSize - 1; x++){
        //up-down
        int maxHeight = treeGrid[0][x];
        if (visibleTreeGrid[0][x] == 0){
            visibleTreeGrid[0][x] = 1;
            visibleTrees++;
        }
        int y = 1;
        while(y < rowSize - 1){
            visibleTrees += checkTreeVisible(x,y,maxHeight);
            y++;
        }
        //down-up
        maxHeight = treeGrid[y][x];
        if (visibleTreeGrid[y][x] == 0){
            visibleTreeGrid[y][x] = 1;
            visibleTrees++;
        }
        while(y > 0){
            visibleTrees += checkTreeVisible(x,y,maxHeight);
            y--;
        }
    }

    // Ex2
    int maxScenicScore = 0;
    for(int y=0; y < rowSize;y++){
        for(int x=0; x < rowSize;x++){
            const int treeHeight = treeGrid[y][x];
            std::vector<int> viewingDistances;
            // check right
            int rightVD = 0;
            int i=x;
            while(++i < rowSize){
                rightVD += 1;
                if(treeGrid[y][i] >= treeHeight){
                    break;
                }
            }
            
            // check left
            int leftVD = 0;
            i = x;
            while(--i >= 0){
                leftVD += 1;
                if(treeGrid[y][i] >= treeHeight){
                    break;
                }
            }

            // check down
            int downVD = 0;
            int j=y;
            while(++j < rowSize){
                downVD += 1;
                if(treeGrid[j][x] >= treeHeight){
                    break;
                }
            }
            // check up
            int upVD = 0;
            j = y;
            while(--j >= 0){
                upVD += 1;
                if(treeGrid[j][x] >= treeHeight){
                    break;
                }
            }
            // after all checked
            viewingDistances = {rightVD, leftVD, downVD, upVD};
            int treeScenicScore = 1;
            for(int i=0; i < viewingDistances.size() ;i++){
                treeScenicScore = treeScenicScore*viewingDistances[i];
            }
            maxScenicScore = __max(maxScenicScore, treeScenicScore);
        }
    }

    std::cout << "--Ex1 Output: " << visibleTrees << std::endl;
    std::cout << "--Ex2 Output: " << maxScenicScore << std::endl;

    return 0;
}