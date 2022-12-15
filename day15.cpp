#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day15.txt";



struct Coordinate{ // Extract later
    int x,y;
    void operator +=(Coordinate other){
        x += other.x;
        y += other.y;
    };
    void operator -=(Coordinate other){
        x -= other.x;
        y -= other.y;
    };
    bool operator ==(Coordinate other){
        return ((other.x == x ) && (other.y == y ));
    };
    Coordinate operator +(Coordinate other){
        Coordinate c = {x+other.x, y+other.y};
        return c;
    };
    Coordinate operator -(Coordinate other){
        Coordinate c = {x-other.x, y-other.y};
        return c;
    };
    int distance(Coordinate other){
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
};

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

Sensor* isThereASensorWithThisCoodinates(Coordinate coordinates){
     for(int i=0; i < sensors.size();i++){
        if (sensors[i].position == coordinates){
            return &sensors[i];
        }
    }
    return nullptr;
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
        int x = std::stoi(line.substr(line.find("x=") + 2, line.find(",")));
        int y = std::stoi(line.substr(line.find("y=") + 2, line.find(":")));
        s.position = {x,y};

        Coordinate beaconPos;
        line = line.substr(line.find("beacon"));
        beaconPos.x = std::stoi(line.substr(line.find("x=") + 2, line.find(",")));
        beaconPos.y = std::stoi(line.substr(line.find("y=") + 2));


        if (!vectorContainsBeacon(beacons, beaconPos)){
            beacons.push_back(beaconPos);
        }

        s.rangeClosestBeacon = s.position.distance(beaconPos);
        sensors.push_back(s);

        // Set row checking boundaries
        minX = std::min(x - s.rangeClosestBeacon,beaconPos.x) < minX ? std::min(x + s.rangeClosestBeacon,beaconPos.x) : minX;
        maxX = std::max(x + s.rangeClosestBeacon,beaconPos.x) > maxX ? std::max(x + s.rangeClosestBeacon,beaconPos.x) : maxX;
    }

    // for each x in row, check if within range of any of the sesnsors and if not, count
    const int rowToCheck = 10;
    int positionsThatCannotContainBeacon = 0;
    for(int x = minX; x <= maxX; x++){
        if (!vectorContainsBeacon(beacons, {x,rowToCheck})){
            Coordinate current = {x, rowToCheck};
            for(int i=0; i < sensors.size(); i++){
                if (current.distance(sensors[i].position) <= sensors[i].rangeClosestBeacon){
                    positionsThatCannotContainBeacon++;
                    break;
                }
            }
        }
    }
    // Ex 2
    long tuningFrequency = 0;
    const int FREQ_RANGE= 4000000;
    int reduction = 0;
    for(int y = 0; y <= FREQ_RANGE; y++){
        bool found = false;
        for(int x = 0; x <= FREQ_RANGE; x++){
            Coordinate current = {x, y};
            const Sensor* sensorInThisCoordinate = isThereASensorWithThisCoodinates(current);
            if (sensorInThisCoordinate != nullptr){
                x+= sensorInThisCoordinate->rangeClosestBeacon;
                //reduction+=sensorInThisCoordinate->rangeClosestBeacon;
            }
            else{
                bool canContainIt = true;
                for(int i=0; i < sensors.size(); i++){
                    if (current.distance(sensors[i].position) <= sensors[i].rangeClosestBeacon){
                        canContainIt = false;
                        int xDistanceToSensor = (sensors[i].position - current).x;
                        if (xDistanceToSensor>0){
                            x+=xDistanceToSensor*2;
                            //reduction+=xDistanceToSensor*2;
                        }
                        break;
                    }
                }
                if(canContainIt){
                    tuningFrequency = 4000000*x + y;
                    found = true;
                    break;
                }
            }
        }
        if (found){
            break;
        }
    }
    std::cout << "--Ex1 Output: " << positionsThatCannotContainBeacon << std::endl;
    std::cout << "--Ex2 Output: " << tuningFrequency << std::endl;
    std::cout << "--Ex2 Reduction: " << reduction << std::endl;


    return 0;
}