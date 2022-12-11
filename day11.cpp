#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day11.txt";

enum Operation: int{
    mult,
    sum
};

struct Monkey{
    std::deque<int> items;

    Operation op;
    std::string opValue;

    int testValue;
    int monkeyTestTrue;
    int monkeyTestFalse;

    int inspectedItems = 0;
};

std::vector<Monkey> monkeys;

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    while(std::getline(ifs, line)){
        // Parse File
        // Monkey #:
        Monkey m;
        std::getline(ifs, line);

        // Starting items: X, Y
        std::string items = line.substr(18);
        while(items.find(',') != items.npos){
            int item = stoi(items.substr(0, items.find(',')));
            items = items.substr(items.find(',') + 1);
            m.items.push_back(item);
        }
        m.items.push_back(stoi(items)); // Add last item after ',''s dissapeared
        std::getline(ifs, line);

        // Operation: new = old * sth
        if (line.find('*') != line.npos){
            m.op = Operation::mult;
            m.opValue = line.substr(line.find('*')+2);
        }
        else{ //+
            m.op = Operation::sum;
            m.opValue = line.substr(line.find('+')+2);
        }
        std::getline(ifs, line);

        // Test: divisible by *
        m.testValue = stoi(line.substr(line.find("by") + 2));
        std::getline(ifs, line);

        // If true: throw to monkey 2
        m.monkeyTestTrue = stoi(line.substr(line.find("monkey") + 7));
        std::getline(ifs, line);

        // If false: throw to monkey 3
        m.monkeyTestFalse = stoi(line.substr(line.find("monkey") + 7));
        monkeys.push_back(m);
        std::getline(ifs, line);
        // Empty line

    }

    // Simulate 20 rounds
    const int ROUNDS = 20;
    for(int round = 0; round < ROUNDS; round++){
        for(int i=0; i < monkeys.size(); i++){
            Monkey* m = &monkeys[i];
            while(!(m->items.empty())){
                // Inspect
                int inspectingItem = m->items.front();
                m->items.pop_front();
                m->inspectedItems++;

                // Worry level operation
                int value;
                if (m->opValue == "old"){
                    value = inspectingItem;
                }
                else{
                    value = stoi(m->opValue);
                }
                if (m->op == Operation::mult){
                    inspectingItem = inspectingItem * value;
                }
                else{ // sum
                    inspectingItem = inspectingItem + value;
                }

                // Divided by being bored
                inspectingItem = inspectingItem / 3;

                // Test divisivility
                if ((inspectingItem % m->testValue) == 0 ){
                    monkeys[m->monkeyTestTrue].items.push_back(inspectingItem);
                }
                else{
                    monkeys[m->monkeyTestFalse].items.push_back(inspectingItem);
                }
            }
        }
    }

    // Count monkey bussiness
    int topMonkeyItems1 = std::max(monkeys[0].inspectedItems, monkeys[1].inspectedItems);
    int topMonkeyItems2 = std::min(monkeys[0].inspectedItems, monkeys[1].inspectedItems);
    for(int i =2; i < monkeys.size(); i++){
        if (monkeys[i].inspectedItems > topMonkeyItems1){
            topMonkeyItems2 = topMonkeyItems1;
            topMonkeyItems1 = monkeys[i].inspectedItems;
        }
        else if (monkeys[i].inspectedItems > topMonkeyItems2){
            topMonkeyItems2 = monkeys[i].inspectedItems;
        }
    }
    std::cout << "--Ex1 Output: " << topMonkeyItems1 * topMonkeyItems2 << std::endl;
    std::cout << "--Ex2 Output: " << "" << std::endl;

    return 0;
}