#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "utils/coordinate.h"

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "inputs/day15.txt";

struct Sensor{
    Coordinate position;
    int rangeClosestBeacon;
};
std::vector<Sensor> sensors;

bool vectorContainsBeacon(std::vector<Coordinate> beacons, Coordinate beacon){
    bool found = false;
     for(int i=0; i < beacons.size();i++){
        if (beacons[i] == beacon){
            found = true;
        }
    }
    return found;
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    std::vector<Coordinate> beacons;
    // Parseinput, get sensor position + closest beacon range
    int minX = INT_MAX;
    int maxX = INT_MIN;

    while(std::getline(ifs, line)){
        Sensor s;
        s.position.x = std::stoi(line.substr(line.find("x=") + 2, line.find(",")));
        s.position.y = std::stoi(line.substr(line.find("y=") + 2, line.find(":")));

        Coordinate beaconPos;
        line = line.substr(line.find("beacon"));
        beaconPos.x = std::stoi(line.substr(line.find("x=") + 2, line.find(",")));
        beaconPos.y = std::stoi(line.substr(line.find("y=") + 2));

        s.rangeClosestBeacon = s.position.distance(beaconPos);
        sensors.push_back(s);

        if (!vectorContainsBeacon(beacons, beaconPos)){
            beacons.push_back(beaconPos);
        }

        // Set row checking boundaries for Ex 1
        minX = std::min(s.position.x - s.rangeClosestBeacon,beaconPos.x) < minX ? std::min(s.position.x + s.rangeClosestBeacon,beaconPos.x) : minX;
        maxX = std::max(s.position.x + s.rangeClosestBeacon,beaconPos.x) > maxX ? std::max(s.position.x + s.rangeClosestBeacon,beaconPos.x) : maxX;
    }

    // Ex 1 - for each x in row, check if within range of any of the sesnsors and if not, count
    const int ROW_TO_CHECK = 2000000;
    int positionsThatCannotContainBeacon = 0;
    for(int x = minX; x <= maxX; x++){
        if (!vectorContainsBeacon(beacons, {x,ROW_TO_CHECK})){
            Coordinate current = {x, ROW_TO_CHECK};
            for(int i=0; i < sensors.size(); i++){
                if (current.distance(sensors[i].position) <= sensors[i].rangeClosestBeacon){
                    positionsThatCannotContainBeacon++;
                    break;
                }
            }
        }
    }

    // Ex 2 - Check the borders of every sensor's range (Only place a beacon could be)
    uint64_t tuningFrequency = 0;
    const uint64_t FREQ_RANGE= 4000000;
    bool found = false;
    for(int i=0; i < sensors.size(); i++){
        Sensor s = sensors[i];
        int yOffset = 0;
        for(int x=s.position.x - s.rangeClosestBeacon - 1; x <= s.position.x + s.rangeClosestBeacon + 1; x++){
            Coordinate pointToCheck1 = {x, s.position.y + yOffset};
            Coordinate pointToCheck2 = {x, s.position.y - yOffset};
            if (pointToCheck1.inRange({0,0},{FREQ_RANGE, FREQ_RANGE})){
                bool canContainIt = true;
                for(int j=0; j < sensors.size(); j++){
                    if (pointToCheck1.distance(sensors[j].position) <= sensors[j].rangeClosestBeacon){
                        canContainIt = false;
                        break;
                    }
                }
                if (canContainIt){
                    tuningFrequency = (uint64_t)4000000*pointToCheck1.x + pointToCheck1.y;
                    found = true;
                    break;
                }
            }
            if (pointToCheck2.inRange({0,0},{FREQ_RANGE, FREQ_RANGE}) && (pointToCheck2 != pointToCheck1)){ 
                bool canContainIt = true;
                for(int j=0; j < sensors.size(); j++){
                    if (pointToCheck2.distance(sensors[j].position) <= sensors[j].rangeClosestBeacon){
                        canContainIt = false;
                        break;
                    }
                }
                if (canContainIt){
                    tuningFrequency = 4000000*pointToCheck2.x + pointToCheck2.y;
                    found = true;
                    break;
                }
            }
            if (x < s.position.x){ // Go up until reaching the sensor origin and then go down, to cover the whole perimteter
                yOffset++;
            }
            else{
                yOffset--;
            }
        }
    }
    std::cout << "--Ex1 Output: " << positionsThatCannotContainBeacon << std::endl;
    std::cout << "--Ex2 Output: " << tuningFrequency << std::endl;

    return 0;
}