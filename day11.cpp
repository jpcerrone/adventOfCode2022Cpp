#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "inputs/day11.txt";

enum Operation: int{
    mult,
    sum
};

struct Monkey{
    std::deque<uint64_t> items;

    Operation op;
    std::string opValue;

    int testValue;
    int monkeyTestTrue;
    int monkeyTestFalse;

    int inspectedItems = 0;
};

uint64_t getModuloOfMonkeyPrimes(uint64_t input){
    return input % (13 * 19 * 5 * 2 * 17 * 11 * 7 * 3); // * of all the monkey's divisivility factors
}

uint64_t divideBy3(uint64_t input){
    return input / 3;
}

uint64_t getMonkeyBusiness(std::vector<Monkey> monkeys){
    uint64_t topMonkeyItems1 = std::max(monkeys[0].inspectedItems, monkeys[1].inspectedItems);
    uint64_t topMonkeyItems2 = std::min(monkeys[0].inspectedItems, monkeys[1].inspectedItems);
    for(int i =2; i < monkeys.size(); i++){
        if (monkeys[i].inspectedItems > topMonkeyItems1){
            topMonkeyItems2 = topMonkeyItems1;
            topMonkeyItems1 = monkeys[i].inspectedItems;
        }
        else if (monkeys[i].inspectedItems > topMonkeyItems2){
            topMonkeyItems2 = monkeys[i].inspectedItems;
        }
    }
    return topMonkeyItems1*topMonkeyItems2;
}

void simulateRound(std::vector<Monkey> *monkeys, Monkey *m, uint64_t(*compressInput)(uint64_t))
{
    while (!(m->items.empty()))
    {
        // Inspect
        uint64_t inspectingItem = m->items.front();
        m->items.pop_front();
        m->inspectedItems++;

        // Worry level operation
        uint64_t value;
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

        // Compress
        inspectingItem = compressInput(inspectingItem);

        // Test divisivility
        if ((inspectingItem % m->testValue) == 0)
        {
            monkeys->at(m->monkeyTestTrue).items.push_back(inspectingItem);
        }
        else
        {
            monkeys->at(m->monkeyTestFalse).items.push_back(inspectingItem);
        }
    }
}

int main()
{
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;
    std::vector<Monkey> monkeys;

    while (std::getline(ifs, line))
    {
        // Parse File
        // Monkey #:
        Monkey m;
        std::getline(ifs, line);

        // Starting items: X, Y
        std::string items = line.substr(18);
        while (items.find(',') != items.npos)
        {
            int item = stoi(items.substr(0, items.find(',')));
            items = items.substr(items.find(',') + 1);
            m.items.push_back(item);
        }
        m.items.push_back(stoi(items)); // Add last item after ',''s dissapeared
        std::getline(ifs, line);

        // Operation: new = old * sth
        if (line.find('*') != line.npos)
        {
            m.op = Operation::mult;
            m.opValue = line.substr(line.find('*') + 2);
        }
        else
        { //+
            m.op = Operation::sum;
            m.opValue = line.substr(line.find('+') + 2);
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

     // Simulate Ex 1
    std::vector<Monkey> monkeysSim1 = monkeys;
    int ROUNDS = 20;
    for(int round = 0; round < ROUNDS; round++){
        for(int i=0; i < monkeysSim1.size(); i++){
            Monkey *m = &monkeysSim1[i];
            simulateRound(&monkeysSim1, m, &divideBy3);
        }
    } 

    // Simulate Ex 2
    std::vector<Monkey> monkeysSim2 = monkeys;
    ROUNDS = 10000;
    for(int round = 0; round < ROUNDS; round++){
        for(int i=0; i < monkeysSim2.size(); i++){
            Monkey *m = &monkeysSim2[i];
            simulateRound(&monkeysSim2, m, &getModuloOfMonkeyPrimes);
        }
    }

    std::cout << "--Ex1 Output: " << getMonkeyBusiness(monkeysSim1) << std::endl;
    std::cout << "--Ex2 Output: " << getMonkeyBusiness(monkeysSim2) << std::endl;

    return 0;
}