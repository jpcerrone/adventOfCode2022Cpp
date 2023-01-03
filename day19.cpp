#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

template <typename T, typename... Rest>
inline void hashCombine(std::size_t &seed, T const &v, Rest &&... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (int[]){0, (hashCombine(seed, std::forward<Rest>(rest)), 0)...};
}

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "inputs/day19.txt";

struct RobotCost{
    int ore = 0;
    int clay = 0;
    int obsidian = 0;
};

struct Blueprint{
    RobotCost oreRobot;
    RobotCost clayRobot;
    RobotCost obsidianRobot;
    RobotCost geodeRobot;
};

Blueprint currentBluePrint;

struct Production{
    uint8_t ore = 0;
    uint8_t clay = 0;
    uint8_t obsidian = 0;
    uint8_t geode = 0;
    bool operator==(const Production other) const{
        return ore == other.ore && clay == other.clay && obsidian == other.obsidian && geode == other.geode;
    }
};

struct State{
    uint8_t remainingMin;
    uint8_t ore;
    uint8_t clay;
    uint8_t obsidian;
    uint8_t geode;
    Production pr;
    bool operator==(const State other) const{
        return remainingMin == other.remainingMin &&
        ore == other.ore &&
        clay == other.clay &&
        obsidian == other.obsidian &&
        geode == other.geode &&
        pr == other.pr;
    }
    struct HashFunction
    {
        size_t operator()(const State& s) const
        {
            std::size_t h = 0;
            hashCombine(h, s.remainingMin, s.ore, s.clay, s.obsidian, s.geode, s.pr.ore, s.pr.clay, s.pr.obsidian, s.pr.geode);
            return h;
        }
    };
};

std::unordered_map<State, uint8_t, State::HashFunction> states;

uint8_t findMaxCombination(uint8_t remainingMin, uint8_t ore, uint8_t clay, uint8_t obsidian, uint8_t geode, Production pr){     State s = {remainingMin, ore, clay, obsidian, geode, pr};
    if (states.find(s) != states.end()){
        return states.at(s);
    }
    ore += pr.ore;
    clay += pr.clay;
    obsidian += pr.obsidian;
    geode += pr.geode;

    uint8_t maxCombination = geode;
    // do nothing..
    if (remainingMin > 0){
        if ((ore - pr.ore) >= currentBluePrint.geodeRobot.ore && 
            (obsidian - pr.obsidian) >= currentBluePrint.geodeRobot.obsidian){
            Production newPr = pr;
            newPr.geode += 1;
            uint8_t newCombination = findMaxCombination(remainingMin - 1, ore - currentBluePrint.geodeRobot.ore, clay, obsidian - currentBluePrint.geodeRobot.obsidian, geode, newPr);
            maxCombination = std::max(maxCombination, newCombination);
        } else{
        if ((ore - pr.ore) >= currentBluePrint.obsidianRobot.ore && 
            (clay - pr.clay) >= currentBluePrint.obsidianRobot.clay &&
            pr.obsidian < currentBluePrint.geodeRobot.obsidian)
            {
            Production newPr = pr;
            newPr.obsidian += 1;
            uint8_t newCombination = findMaxCombination(remainingMin - 1, ore - currentBluePrint.obsidianRobot.ore, clay - currentBluePrint.obsidianRobot.clay, obsidian, geode, newPr);
            maxCombination = std::max(maxCombination, newCombination);
        }
        if ((ore - pr.ore) >= currentBluePrint.clayRobot.ore &&
            pr.clay < currentBluePrint.obsidianRobot.clay
        ){
            Production newPr = pr;
            newPr.clay += 1;
            uint8_t newCombination = findMaxCombination(remainingMin - 1, ore - currentBluePrint.clayRobot.ore, clay, obsidian, geode, newPr);
            maxCombination = std::max(maxCombination, newCombination);
        }
        if ((ore - pr.ore) >= currentBluePrint.oreRobot.ore &&
            pr.ore < std::max(std::max(currentBluePrint.oreRobot.ore, currentBluePrint.clayRobot.ore), std::max(
                currentBluePrint.obsidianRobot.ore, currentBluePrint.geodeRobot.ore))){
            Production newPr = pr;
            newPr.ore += 1;
            uint8_t newCombination = findMaxCombination(remainingMin - 1, ore - currentBluePrint.oreRobot.ore, clay, obsidian, geode, newPr);
            maxCombination = std::max(maxCombination, newCombination);
        }
        maxCombination = std::max(maxCombination, findMaxCombination(remainingMin - 1, ore, clay, obsidian, geode, pr));
        }
    }
    states.insert({s, maxCombination});
    return maxCombination;
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    int bluePrintId = 1;
    int qualityLevels = 0;
    int multFirst3BluePrints = 1;
    while(std::getline(ifs, line)){
        // parse input into and get max
        RobotCost oreRobot;
        {
            int oreCost = {std::stoi(line.substr(line.find("costs") + 6, line.find("ore.") - line.find("costs") - 6))};
            oreRobot = {oreCost};
        }
        line = line.substr(line.find("clay robot"));
        RobotCost clayRobot;
        {
            int oreCost = {std::stoi(line.substr(line.find("costs") + 6, line.find("ore.") - line.find("costs") - 6))};
            clayRobot = {oreCost};        
        }
        line = line.substr(line.find("obsidian robot"));
        RobotCost obisidanRobot;
        {
            int oreCost = {std::stoi(line.substr(line.find("costs") + 6, line.find("ore.") - line.find("costs") - 6))};
            int clayCost = {std::stoi(line.substr(line.find("and") + 4, line.find("clay") - line.find("and") - 3))};
            obisidanRobot = {oreCost, clayCost};
        }
        line = line.substr(line.find("geode robot"));
        RobotCost geodeRobot;
        {
            int oreCost = {std::stoi(line.substr(line.find("costs") + 6, line.find("ore.") - line.find("costs") - 6))};
            int obisidianCost = {std::stoi(line.substr(line.find("and") + 4, line.find("obisidian") - line.find("and") - 3))};
            geodeRobot = {oreCost, 0, obisidianCost};
        }
        currentBluePrint = {oreRobot, clayRobot, obisidanRobot, geodeRobot};
        int maxCombination = findMaxCombination(23 ,0,0,0,0,{1});
        if (bluePrintId <= 3)
            multFirst3BluePrints *= findMaxCombination(31 ,0,0,0,0,{1});
        states.clear();
        qualityLevels = qualityLevels + bluePrintId*maxCombination;
        bluePrintId++;
    }

    std::cout << "--Ex1 Output: " << qualityLevels << std::endl;
    std::cout << "--Ex2 Output: " << multFirst3BluePrints << std::endl;

    return 0;
}