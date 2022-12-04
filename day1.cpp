#include <iostream>
#include <fstream>
#include <string>
#include <array>

std::string::size_type sz;   // Size of input ints. alias of size_t (unisgned int)
const std::string inputPath = "input1.txt";
int main(){

    // Ex1
    std::ifstream ifs(inputPath, std::ifstream::in);

    int currentCalorieCount = 0;
    int maxCalories = 0;

    std::string line;
    while(std::getline(ifs, line)){  // stops at eof
        if (line != ""){
            currentCalorieCount += std::stoi(line, &sz);
        }
        else{
            if (currentCalorieCount > maxCalories){
                maxCalories = currentCalorieCount;
            }
            currentCalorieCount = 0;
        }
    }
    ifs.close();
    std::cout << "--Ex1 Output " << maxCalories << std::endl;

    // Ex2
    std::array<int, 3> topThreeCaloriesCounts = {0, 0, 0};
    ifs.open(inputPath, std::ifstream::in);
    currentCalorieCount = 0;

    while(std::getline(ifs, line)){  // stops at eof
        if (line != ""){
            currentCalorieCount += std::stoi(line, &sz);
        }
        else{
            // getMinCaloriCountFromTop
            int min = INT_MAX;
            int minIdx;
            for (int j = 0; j < topThreeCaloriesCounts.size(); j++)
            {
                if (topThreeCaloriesCounts[j] < min)
                {
                    min = topThreeCaloriesCounts[j];
                    minIdx = j;
                }
            }

            if (currentCalorieCount > topThreeCaloriesCounts[minIdx])
            {
                topThreeCaloriesCounts[minIdx] = currentCalorieCount;
            }
            currentCalorieCount = 0;
        }
    }
    ifs.close();

    int fullCalorySum = 0;
    for(int i=0; i < topThreeCaloriesCounts.size();i++){
        fullCalorySum += topThreeCaloriesCounts[i];
    }

    std::cout << "--Ex2 Output " << fullCalorySum << std::endl;

    return 0;
}