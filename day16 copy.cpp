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

const int MAX_MINUTES = 30;
const int MAX_MINUTES_EX_2 = 26;

struct Node{
    std::string name;
    int flowRate;
    std::map<Node*, int> neighbors;
};

Node* rootNode; // --> AA


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

    std::vector<bool>::iterator itV;
    bool allVisited = true;
    for(itV = openValves.begin(); itV != openValves.end(); itV++){
        if (*itV == false){
            allVisited = false;
            break;
        }
    }
    if(allVisited){
        return currentSum;
    }

    int openValveSum = currentSum;
    if ((currentNode->flowRate != 0) && (openValves.at(numbersForValves.at(currentNode->name)) == 0) && remainingMin > 1){
        openValveSum += currentNode->flowRate * (remainingMin - 1);
        openValves[numbersForValves.at(currentNode->name)] = 1;
        openValveSum = findMaxFlow(currentNode, remainingMin - 1, openValveSum, openValves, currentNode);
        //return openValveSum;
    }
    int neighborMax = currentSum;
    if (remainingMin > 1){
        std::map<Node*, int>::iterator it;
        for (it = currentNode->neighbors.begin(); it != currentNode->neighbors.end(); it++)
        {
            if((it->first != previousNode) && ((remainingMin - it->second) > 1) && ((it->first->name == "AA")  || !openValves.at(numbersForValves.at(it->first->name)))){ // Avoid loops and opened valves
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

int findMaxFlowWithElephant(Node* currentNode1, Node* currentNode2, int remainingMin1, int remainingMin2, int currentSum, std::vector<bool> openValves, Node* previousNode1 = nullptr, Node* previousNode2 = nullptr){
    StateE currentState = {numbersForValves.at(currentNode1->name), numbersForValves.at(currentNode2->name), remainingMin1, remainingMin2, currentSum, openValves};
    StateE currentStateReversed = {numbersForValves.at(currentNode2->name), numbersForValves.at(currentNode1->name), remainingMin2, remainingMin1, currentSum, openValves};

    if (savedStatesE.find(currentState) != savedStatesE.end()){
        return savedStatesE.at(currentState);
    } else if (savedStatesE.find(currentStateReversed) != savedStatesE.end()){
        return savedStatesE.at(currentStateReversed);
    }

    std::vector<bool>::iterator itV;
    bool allVisited = true;
    for(itV = openValves.begin(); itV != openValves.end(); itV++){
        if (*itV == false){
            allVisited = false;
            break;
        }
    }
    if(allVisited){
        return currentSum;
    }

    // Get max of 4 possible states. Open + move, move + open, move + move, open + open
    std::map<Node*, int> neighbors1Idx;
    std::map<Node*, int> neighbors2Idx;

    int openValve1Sum = 0;
    if (openValves.at(numbersForValves.at(currentNode1->name)) == 0 && (currentNode1->flowRate > 0) && remainingMin1 > 1){
        openValve1Sum += currentNode1->flowRate * (remainingMin1 - 1);
        neighbors1Idx.insert({currentNode1, 1});
    }

    int openValve2Sum = 0;
    if (openValves.at(numbersForValves.at(currentNode2->name)) == 0 && (currentNode1->flowRate > 0) && remainingMin2 > 1 && (currentNode2 != currentNode1)){
        openValve2Sum += currentNode2->flowRate * (remainingMin2 - 1);
        neighbors2Idx.insert({currentNode2, 1});

    }

//maybe if theyre the same put some on one and smoe on the other
    std::map<Node*, int>::iterator it;
    if (openValve1Sum == 0){
        for (it = currentNode1->neighbors.begin(); it != currentNode1->neighbors.end(); it++)
            {
            if (remainingMin1 > (it->second + 1)){
                if(it->first != previousNode1){ // Avoid loops
                    neighbors1Idx.insert({it->first, it->second});
                }
            }
        }
    }
    if (openValve2Sum == 0){
        for (it = currentNode2->neighbors.begin(); it != currentNode2->neighbors.end(); it++)
        {
            if (remainingMin2 > (it->second + 1)){
                if(it->first != previousNode2){ // Avoid loops
                    neighbors2Idx.insert({it->first, it->second});
                }
            }
        }
    }

    int maxCombination = currentSum;
    std::map<Node*, int>::iterator it1;
    std::map<Node*, int>::iterator it2;
    for (it1 = neighbors1Idx.begin(); it1 != neighbors1Idx.end(); it1++){
        for (it2 = neighbors2Idx.begin(); it2 != neighbors2Idx.end(); it2++){
            if (it1->first != it2->first){
                int sum = currentSum;
                std::vector<bool> newOpenValves = openValves;
                if (openValve1Sum > 0 && (it1->first == currentNode1)){
                    newOpenValves[numbersForValves.at(currentNode1->name)] = 1;
                    sum += openValve1Sum;
                }
                if (openValve2Sum > 0 && (it2->first == currentNode2)){
                    newOpenValves[numbersForValves.at(currentNode2->name)] = 1;
                    sum += openValve2Sum;
                }
                int combination = findMaxFlowWithElephant(it1->first, it2->first, remainingMin1 - it1->second, remainingMin2 - it2->second, sum, newOpenValves, currentNode1, currentNode2);
                if (combination > maxCombination){
                    maxCombination = combination;
                }
            }
        }
    }
    savedStatesE[currentState] = maxCombination;
    return maxCombination;

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

int combs = 0;

int getMaxCombination(std::vector<bool> openValves, int valvesToConsider){
    if (valvesToConsider > 0){
        openValves[valvesToConsider-1] = true;
        int max1 = getMaxCombination(openValves, valvesToConsider-1);
        openValves[valvesToConsider-1] = false;
        int max2 = getMaxCombination(openValves, valvesToConsider-1);
        return std::max(max1, max2);
    }
    else{
        std::vector<bool> reverseOpenValves = openValves;
        int nOpenValves = 0;
        for(int i=0; i < openValves.size();i++){
            if (openValves[i]){ nOpenValves++;};
            reverseOpenValves[i] = !openValves[i];
        }
        LOG(combs++);
        if (nOpenValves >= 4)
            return findMaxFlow(rootNode, MAX_MINUTES_EX_2, 0, openValves) + findMaxFlow(rootNode, MAX_MINUTES_EX_2, 0, reverseOpenValves);
        else{
            return 0;
        }    
    }
}

int findMinPathBetweenTwoNodes(Node* a, Node* b, std::set<Node*> visitedNodes = {}){
    visitedNodes.insert(a);
    int minDistance = 10000;

    if (a == b){
        return 0;
    }else{
        if (a->neighbors.find(b) != a->neighbors.end()){
           return a->neighbors.at(b);
        }
        for(std::map<Node*, int>::iterator it = a->neighbors.begin(); it != a->neighbors.end();it++){
            if (visitedNodes.find(it->first) == visitedNodes.end()){
                minDistance = std::min(minDistance, findMinPathBetweenTwoNodes(it->first, b, visitedNodes) + it->second) ;
            }
        }
    }
        return minDistance;
}   

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    int valveNum = 0;

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

        if (flowRate > 0){
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
    numbersForValves.insert({"AA", valveNum++});


    auto t1 = std::chrono::high_resolution_clock::now();
    std::map<Node*, bool> n;
    filterZeroNodes(rootNode, n);
    std::vector<bool> openValves;
    for(int i=0; i < numbersForValves.size() -1; i++){
        openValves.push_back(false);
    }
/*      for(std::map<std::string, Node*>::iterator it = nodeMap.begin(); it != nodeMap.end();it++){
        LOG(it->first);
        for(std::map<std::string, Node*>::iterator it2 = std::next(it, 1); it2 != nodeMap.end();it2++){ 
            if (it->first != it2->first){
                if (it->second->neighbors.find(it2->second) == it->second->neighbors.end())
                {
                    const int minPath = findMinPathBetweenTwoNodes(it->second, it2->second);
                    it->second->neighbors.insert({it2->second, minPath});
                    it2->second->neighbors.insert({it->second, minPath});
                }
            }
        }
    } */
    LOG("GATO");
    std::cout << "--Ex1 Output: " << findMaxFlow(rootNode, MAX_MINUTES, 0, openValves) << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout <<  "in " << ms_int.count() << "ms" << std::endl;
    auto t3 = std::chrono::high_resolution_clock::now();

    int maxCombination = getMaxCombination(openValves, openValves.size() -1); // -1 since we dont have to check last bit as all of those combinartions will already have been tested

    std::cout << "--Ex2 Output: " << maxCombination << std::endl;

    auto t4 = std::chrono::high_resolution_clock::now();
    ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    std::cout <<  "in " << ms_int.count() << "ms" << std::endl;

    return 0;
}