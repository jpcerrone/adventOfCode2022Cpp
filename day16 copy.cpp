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
    std::map<Node*, int> neighbors;
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
    unsigned __int8 remainingMin1;
    unsigned __int8 remainingMin2;
    unsigned __int16 currentSum;
    std::vector<bool> openValves;
    bool operator==(const StateE& other) const{
        return (node1Idx == other.node1Idx) && (node2Idx == other.node2Idx) && (currentSum == other.currentSum) 
        && (remainingMin1 == other.remainingMin1) 
        && (remainingMin2 == other.remainingMin2)
        && (openValves == other.openValves) ;
    }
    struct HashFunction
    {
        size_t operator()(const StateE& s) const
        {
            std::size_t h = 0;
            hashCombine(h, s.node1Idx, s.node2Idx, s.remainingMin1, s.remainingMin2, s.currentSum, s.openValves);
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
        std::map<Node*, int>::iterator it;
        for (it = currentNode->neighbors.begin(); it != currentNode->neighbors.end(); it++)
        {
            if(it->first != previousNode){ // Avoid loops
                int currentMax = findMaxFlow(it->first, remainingMin - it->second, currentSum, openValves, currentNode);
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

std::pair<int, int> findMaxFlowWithElephant(Node* currentNode1, Node* currentNode2, int remainingMin1, int remainingMin2, int currentSum1, int currentSum2, std::vector<bool> openValves1, std::vector<bool> openValves2, Node* previousNode1 = nullptr, Node* previousNode2 = nullptr){
/*     StateE currentState = {numbersForValves.at(currentNode1->name), numbersForValves.at(currentNode2->name), remainingMin1, remainingMin2, currentSum, openValves};
    StateE currentStateReversed = {numbersForValves.at(currentNode2->name), numbersForValves.at(currentNode1->name), remainingMin2, remainingMin1, currentSum, openValves};
 */
    State currentState1 = {numbersForValves.at(currentNode1->name), remainingMin1, currentSum1, openValves1};
    State currentState2 = {numbersForValves.at(currentNode2->name), remainingMin2, currentSum2, openValves2};

    bool state1Cached = false;
    bool state2Cached = false;
    if(savedStates.find(currentState1) != savedStates.end()){
        state1Cached = true;
    }
    if(savedStates.find(currentState2) != savedStates.end()){
        state2Cached = true;
    }
    if(state1Cached && state2Cached){
        return {savedStates.at(currentState1), savedStates.at(currentState2)};
    }

    std::vector<bool> combinedOpenValves = openValves1;
    for(int i=0; i < openValves2.size(); i++){
        combinedOpenValves[i] = openValves2[i] ? openValves2[i] : combinedOpenValves[i];
    }

    // Get max of 4 possible states. Open + move, move + open, move + move, open + open
    std::map<Node*, int> neighbors1Idx;
    std::map<Node*, int> neighbors2Idx;

    int openValve1Sum = 0;
    if (!state1Cached && (combinedOpenValves.at(numbersForValves.at(currentNode1->name)) == 0) && (currentNode1->flowRate > 0) && (remainingMin1 > 1)){
        openValve1Sum += currentNode1->flowRate * (remainingMin1 - 1);
        neighbors1Idx.insert({currentNode1, 1});
    }

    int openValve2Sum = 0;
    if (!state2Cached && (combinedOpenValves.at(numbersForValves.at(currentNode2->name)) == 0) && (currentNode2->flowRate > 0) && (remainingMin2 > 1) && (currentNode2 != currentNode1)){
        openValve2Sum += currentNode2->flowRate * (remainingMin2 - 1);
        neighbors2Idx.insert({currentNode2, 1});

    }

//maybe if theyre the same put some on one and smoe on the other
    std::map<Node*, int>::iterator it;
    if (!state1Cached /*&& openValve1Sum == 0*/){
        for (it = currentNode1->neighbors.begin(); it != currentNode1->neighbors.end(); it++)
            {
            if (remainingMin1 > (it->second + 1)){
                if(it->first != previousNode1){ // Avoid loops
                    neighbors1Idx.insert({it->first, it->second});
                }
            }
        }
    }
    if (!state2Cached /*&& openValve2Sum == 0*/){
        for (it = currentNode2->neighbors.begin(); it != currentNode2->neighbors.end(); it++)
        {
            if (remainingMin2 > (it->second + 1)){
                if(it->first != previousNode2){ // Avoid loops
                    neighbors2Idx.insert({it->first, it->second});
                }
            }
        }
    }

    std::pair<int, int> maxCombination = {currentSum1, currentSum2};
    std::map<Node*, int>::iterator it1;
    std::map<Node*, int>::iterator it2;
    if (state1Cached || neighbors1Idx.empty()){
        for (it2 = neighbors2Idx.begin(); it2 != neighbors2Idx.end(); it2++){
            if (currentNode1 != it2->first){
                int sum2 = currentSum2;
                std::vector<bool> newOpenValves = openValves2;
                if (openValve2Sum > 0 && (it2->first == currentNode2)){
                    newOpenValves[numbersForValves.at(currentNode2->name)] = 1;
                    sum2 += openValve2Sum;
                }
                std::pair<int,int> combination = findMaxFlowWithElephant(currentNode1, it2->first, remainingMin1, remainingMin2 - it2->second, currentSum1, sum2, openValves1, newOpenValves, previousNode1, currentNode2);
                if (combination.first > maxCombination.first){
                    maxCombination.first = combination.first;
                }
                if (combination.second > maxCombination.second){
                    maxCombination.second = combination.second;
                }
            }
        }
    }
    else if (state2Cached || neighbors2Idx.empty()){
        for (it1 = neighbors1Idx.begin(); it1 != neighbors1Idx.end(); it1++){
            if (currentNode2 != it1->first){
                int sum1 = currentSum1;
                std::vector<bool> newOpenValves = openValves1;
                if (openValve1Sum > 0 && (it1->first == currentNode1)){
                    newOpenValves[numbersForValves.at(currentNode1->name)] = 1;
                    sum1 += openValve1Sum;
                }
                std::pair<int,int> combination = findMaxFlowWithElephant(it1->first, currentNode2, remainingMin1 - it1->second, remainingMin2, sum1, currentSum2, newOpenValves, openValves2, currentNode1, previousNode2);
                if (combination.first > maxCombination.first){
                    maxCombination.first = combination.first;
                }
                if (combination.second > maxCombination.second){
                    maxCombination.second = combination.second;
                }
            }
        }
    }
    else{
        for (it1 = neighbors1Idx.begin(); it1 != neighbors1Idx.end(); it1++){
            for (it2 = neighbors2Idx.begin(); it2 != neighbors2Idx.end(); it2++){
                if (it1->first != it2->first){
                    int sum1 = currentSum1;
                    int sum2 = currentSum2;
                    std::vector<bool> newOpenValves1 = openValves1;
                    std::vector<bool> newOpenValves2 = openValves2;
                    if (openValve1Sum > 0 && (it1->first == currentNode1)){
                        newOpenValves1[numbersForValves.at(currentNode1->name)] = 1;
                        sum1 += openValve1Sum;
                    }
                    if (openValve2Sum > 0 && (it2->first == currentNode2)){
                        newOpenValves2[numbersForValves.at(currentNode2->name)] = 1;
                        sum2 += openValve2Sum;
                    }
                    std::pair<int,int> combination = findMaxFlowWithElephant(it1->first, it2->first, remainingMin1 - it1->second, remainingMin2 - it2->second, sum1, sum2, newOpenValves1, newOpenValves2, currentNode1, currentNode2);
                    if(combination.first + combination.second > maxCombination.first + maxCombination.second){
                        maxCombination.first = combination.first;
                        maxCombination.second = combination.second;
                    }
                }
            }
        }
    }
    if (!state1Cached){
        savedStates[currentState1] = maxCombination.first; // SHOULD I STORE THE SHARED OPEN VALVES?
    }
    if (!state2Cached){
        savedStates[currentState2] = maxCombination.second;
    }
    std::pair<int, int> result;
    result.first = state1Cached ? savedStates.at(currentState1) : maxCombination.first;
    result.second = state2Cached ? savedStates.at(currentState2) : maxCombination.second;

    return result;

}

Node* createNewNode(std::string name){
    Node* newNode = new Node();
    newNode->name = name;
    nodeMap[name] = newNode;
    return newNode;
}

  void filterZeroNodes(Node* rootNode, std::map<Node*, bool> &visitedNodes){
    visitedNodes.insert({rootNode, true});
    if (rootNode->name != "AA" && rootNode->flowRate == 0){
            std::map<Node*, int> zeroNodeNeighbors = rootNode->neighbors;
            std::map<Node*, int>::iterator it;
            for (it = zeroNodeNeighbors.begin(); it != zeroNodeNeighbors.end(); it++){
                // Update refereneces for negihbors
                std::map<Node*, int>::iterator it2;
                for (it2 = zeroNodeNeighbors.begin(); it2 != zeroNodeNeighbors.end(); it2++){
                    if(it->first->neighbors.find(it2->first) == it->first->neighbors.end()){
                        if (it2->first != it->first){
                            it->first->neighbors.insert({it2->first, it->first->neighbors.at(rootNode) + rootNode->neighbors.at(it2->first)});
                        }
                    }
                }
                it->first->neighbors.erase(rootNode); // Delete reference to 0 node
                /* numbersForValves.erase(rootNode->name); */
                nodeMap.erase(rootNode->name);
            }
    }
    std::map<Node *, int>::iterator it;
    for (it = rootNode->neighbors.begin(); it != rootNode->neighbors.end(); it++)
    {
            if (visitedNodes.find(it->first) == visitedNodes.end()){
                filterZeroNodes(it->first, visitedNodes);
                it = rootNode->neighbors.begin();
            }
    }
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
            currentNode = createNewNode(name);
        }
        if (name == "AA"){
            rootNode = currentNode;
        }

 
        // Flowrate
        int flowRate = std::stoi(line.substr(23, line.find(';') - 23)); // flowrate=xx
        currentNode->flowRate = flowRate;

        if (flowRate > 0 || (name == "AA")){
            numbersForValves.insert({name, valveNum++});
        }

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
                currentNode->neighbors.insert({findNode(nameDestiny), 1});
            } else{
                Node* newNode = createNewNode(nameDestiny);
                currentNode->neighbors.insert({newNode, 1});

            }
            line = line.substr(line.find(',')+2);
        }
        if (findNode(line) != nullptr){
                currentNode->neighbors.insert({findNode(line), 1});
            } else{
                Node* newNode = createNewNode(line);
                currentNode->neighbors.insert({newNode, 1});
        }
    }

    const int MAX_MINUTES = 30;
    const int MAX_MINUTES_EX_2 = 26;

    auto t1 = std::chrono::high_resolution_clock::now();
    std::map<Node*, bool> n;
    filterZeroNodes(rootNode, n);
    std::vector<bool> openValves;
    for(int i=0; i < numbersForValves.size(); i++){
        openValves.push_back(false);
    }
    //std::cout << "--Ex1 Output: " << findMaxFlow(rootNode, MAX_MINUTES, 0, openValves) << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout <<  "in " << ms_int.count() << "ms" << std::endl;
    auto t3 = std::chrono::high_resolution_clock::now();
    std::pair ex2Result = findMaxFlowWithElephant(rootNode, rootNode, MAX_MINUTES_EX_2, MAX_MINUTES_EX_2, 0, 0, openValves, openValves);
    std::cout << "--Ex2 Output: " << ex2Result.first + ex2Result.second << std::endl;
    auto t4 = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    std::cout <<  "in " << ms_int.count() << "ms" << std::endl;

    return 0;
}