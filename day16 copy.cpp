#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>
#include <chrono>

template <typename T, typename... Rest>
inline void hashCombine(std::size_t &seed, T const &v, Rest &&... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (int[]){0, (hashCombine(seed, std::forward<Rest>(rest)), 0)...};
}

#define LOG(x) std::cout << x << std::endl;

// Replacing valve names (AA, BB, CC) with numbers (0, 1, 2) for hashing
std::map<std::string, unsigned __int8> numbersForValves;

struct Node{
    std::string name;
    int flowRate;
    std::vector<Node*> neighbors;
};

struct State{
    unsigned __int8 nodeIdx;
    unsigned __int8 remainingMin;
    unsigned __int16 currentSum;
    std::vector<bool> openValves;
    bool operator==(const State& other) const{
        return (nodeIdx == other.nodeIdx) && (currentSum == other.currentSum) && (remainingMin == other.remainingMin) && (openValves == other.openValves) ;
    }
    struct HashFunction
    {
        size_t operator()(const State& s) const
        {
            std::size_t h = 0;
            hashCombine(h, s.nodeIdx, s.remainingMin, s.currentSum, s.openValves);
            return h;
        }
  };
};

struct StateE{ // State with elephants
    unsigned __int8 node1Idx;
    unsigned __int8 node2Idx;
    unsigned __int8 remainingMin;
    unsigned __int16 currentSum;
    std::vector<bool> openValves;
    bool operator==(const StateE& other) const{
        return (node1Idx == other.node1Idx) && (node2Idx == other.node2Idx) && (currentSum == other.currentSum) && (remainingMin == other.remainingMin) && (openValves == other.openValves) ;
    }
    struct HashFunction
    {
        size_t operator()(const StateE& s) const
        {
            std::size_t h = 0;
            hashCombine(h, s.node1Idx, s.node2Idx, s.remainingMin, s.currentSum, s.openValves);
            return h;
        }
  };
};

std::unordered_map<State, int, State::HashFunction> savedStates;
std::unordered_map<StateE, int, StateE::HashFunction> savedStatesE;

const std::string inputFilePath = "day16.txt";
std::map<std::string, Node*> nodeMap;

Node* findNode(std::string name, std::set<std::string> visitedNodes = {}){
    if (nodeMap.find(name) != nodeMap.end()){
        return nodeMap.find(name)->second;
    }
    else{
        return nullptr;
    }
}

int findMaxFlow(Node* currentNode, int remainingMin, int currentSum, std::vector<bool> openValves, Node* previousNode = nullptr){
    State currentState = {numbersForValves.at(currentNode->name), remainingMin, currentSum, openValves};
    if (savedStates.find(currentState) != savedStates.end()){
        return savedStates.at(currentState);
    }
    int openValveSum = currentSum;
    if (openValves.at(numbersForValves.at(currentNode->name)) == 0 && (currentNode->flowRate != 0) && remainingMin > 2){
        openValveSum += currentNode->flowRate * (remainingMin - 1);
        openValves[numbersForValves.at(currentNode->name)] = 1;
        openValveSum = findMaxFlow(currentNode, remainingMin - 1, openValveSum, openValves, currentNode);
    }
    int neighborMax = currentSum;
    if (remainingMin > 2){
        for (int i = 0; i < currentNode->neighbors.size(); i++)
        {
            if(currentNode->neighbors.at(i) != previousNode){ // Avoid loops
                int currentMax = findMaxFlow(currentNode->neighbors.at(i), remainingMin - 1, currentSum, openValves, currentNode);
                if (currentMax > neighborMax)
                {
                    neighborMax = currentMax;
                }
            }
        }
    }
    if (neighborMax >= openValveSum){
        savedStates[currentState] = neighborMax;
        return neighborMax;
    }
    else{
        savedStates[currentState] = openValveSum;
        return openValveSum;
    }
}

int findMaxFlowWithElephant(Node* currentNode1, Node* currentNode2, int remainingMin, int currentSum, std::vector<bool> openValves, Node* previousNode1 = nullptr, Node* previousNode2 = nullptr){
    StateE currentState = {numbersForValves.at(currentNode1->name), numbersForValves.at(currentNode2->name), remainingMin, currentSum, openValves};
    StateE currentStateReversed = {numbersForValves.at(currentNode2->name), numbersForValves.at(currentNode1->name), remainingMin, currentSum, openValves};

    if (savedStatesE.find(currentState) != savedStatesE.end()){
        return savedStatesE.at(currentState);
    } else if (savedStatesE.find(currentStateReversed) != savedStatesE.end()){
        return savedStatesE.at(currentStateReversed);
    }

    // Get max of 4 possible states. Open + move, move + open, move + move, open + open
    std::vector<Node*> neighbors1Idx;
    std::vector<Node*> neighbors2Idx;

    int openValve1Sum = 0;
    if (openValves.at(numbersForValves.at(currentNode1->name)) == 0 && (currentNode1->flowRate != 0) && remainingMin > 1){
        openValve1Sum += currentNode1->flowRate * (remainingMin - 1);
        neighbors1Idx.push_back(currentNode1);
    }

    int openValve2Sum = 0;
    if (openValves.at(numbersForValves.at(currentNode2->name)) == 0 && (currentNode2->flowRate != 0) && remainingMin > 1 && (currentNode2 != currentNode1)){
        openValve2Sum += currentNode2->flowRate * (remainingMin - 1);
        neighbors2Idx.push_back(currentNode2);

    }

//maybe if theyre the same put some on one and smoe on the other
    if (remainingMin > 2){
        for (int i = 0; i < currentNode1->neighbors.size(); i++)
            {
            if(currentNode1->neighbors.at(i) != previousNode1){ // Avoid loops
                neighbors1Idx.push_back(currentNode1->neighbors.at(i));
            }
        }
    

        for (int i = 0; i < currentNode2->neighbors.size(); i++)
            {
            if(currentNode2->neighbors.at(i) != previousNode2){ // Avoid loops
                neighbors2Idx.push_back(currentNode2->neighbors.at(i));
            }
        }
    }

    int maxCombination = currentSum;
    for (int i = 0; i < neighbors1Idx.size(); i++){
        for (int j = 0; j < neighbors2Idx.size(); j++){
            if (neighbors1Idx[i] != neighbors2Idx[j]){
                int sum = currentSum;
                std::vector<bool> newOpenValves = openValves;
                if (openValve1Sum > 0 && (neighbors1Idx[i] == currentNode1)){
                    newOpenValves[numbersForValves.at(currentNode1->name)] = 1;
                    sum += openValve1Sum;
                }
                if (openValve2Sum > 0 && (neighbors2Idx[j] == currentNode2)){
                    newOpenValves[numbersForValves.at(currentNode2->name)] = 1;
                    sum += openValve2Sum;
                }
                int combination = findMaxFlowWithElephant(neighbors1Idx[i], neighbors2Idx[j], remainingMin - 1, sum, newOpenValves, currentNode1, currentNode2);
                if (combination > maxCombination){
                    maxCombination = combination;
                }
            }
        }
    }
    savedStatesE[currentState] = maxCombination;
    return maxCombination;

}

Node* createNewNode(std::string name, int valveNum){
    Node* newNode = new Node();
    newNode->name = name;
    numbersForValves.insert({name, valveNum});
    nodeMap[name] = newNode;
    return newNode;
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    int valveNum = 0;

    Node* rootNode; // --> AA
    Node* currentNode;

    while(std::getline(ifs, line)){
        // Name
        std::string name = line.substr(6, 2); // Valve: 'XX'

        currentNode = findNode(name);
        if (currentNode == nullptr){
            currentNode = createNewNode(name, valveNum++);
        }
        if (name == "AA"){
            rootNode = currentNode;
        }

 
        // Flowrate
        int flowRate = std::stoi(line.substr(23, line.find(';') - 23)); // flowrate=xx
        currentNode->flowRate = flowRate;

        // List of neighbors
        if (line.find("valves") == line.npos){
            line = line.substr(line.find("valve") + 6);
        }
        else{
            line = line.substr(line.find("valves") + 7); 
        }
        std::string nameDestiny;
        while (line.find(',') != line.npos){
            nameDestiny = line.substr(0, line.find(','));
            if (findNode(nameDestiny) != nullptr){
                currentNode->neighbors.push_back(findNode(nameDestiny));
            } else{
                Node* newNode = createNewNode(nameDestiny, valveNum++);
                currentNode->neighbors.push_back(newNode);

            }
            line = line.substr(line.find(',')+2);
        }
        if (findNode(line) != nullptr){
                currentNode->neighbors.push_back(findNode(line));
            } else{
                Node* newNode = createNewNode(line, valveNum++);
                currentNode->neighbors.push_back(newNode);
        }
    }

    const int MAX_MINUTES = 30;
    const int MAX_MINUTES_EX_2 = 26;

    std::vector<bool> openValves;
    for(int i=0; i < numbersForValves.size(); i++){
        openValves.push_back(false);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    //std::cout << "--Ex1 Output: " << findMaxFlow(rootNode, MAX_MINUTES, 0, openValves) << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout <<  "in " << ms_int.count() << "ms" << std::endl;
    auto t3 = std::chrono::high_resolution_clock::now();
    std::cout << "--Ex2 Output: " << findMaxFlowWithElephant(rootNode, rootNode, MAX_MINUTES_EX_2, 0, openValves) << std::endl;
    auto t4 = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    std::cout <<  "in " << ms_int.count() << "ms" << std::endl;

    return 0;
}