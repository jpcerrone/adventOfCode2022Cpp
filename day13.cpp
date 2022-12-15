#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define LOG(x) std::cout << x << std::endl;

const std::string inputFilePath = "day13.txt";

bool isCharNumber(char c){
    return ((c >= 48) && ( c <= 57));
}

struct ListOfLists{
        std::vector<int> numbers;
        std::vector<ListOfLists*> lists;
        ListOfLists* parent;
        void push(int number){
            numbers.push_back(number);
            lists.push_back(nullptr);
        };
        void push(ListOfLists* list){
            numbers.push_back(-1); // We use -1 to mark that this index contains a list, not a number
            lists.push_back(list);
        };
        int size(){
            return numbers.size();
        };
};

ListOfLists parseList(std::string line){
        ListOfLists topList;
        ListOfLists* currentList = &topList;
        for(int i=1; i < line.length();i++){
            if (line[i] == '['){
                ListOfLists* newList = new ListOfLists();
                currentList->push(newList);
                newList->parent = currentList;
                currentList = newList;
            }
            else if (line[i] == ']'){
                currentList = currentList->parent;
            }
            else if (isCharNumber(line[i])){
                const int numberEnding = std::min(line.substr(i).find(','), line.substr(i).find(']'));
                currentList->push(std::stoi(line.substr(i, numberEnding)));
                i += (numberEnding-1);
            }
        }
        return topList;
}

int compareLists(ListOfLists l1, ListOfLists l2){
    for(int i = 0; i < std::min(l1.size(), l2.size());i++){ // If l2 has more elements i dont really care as those wont be compared
        if(l1.numbers[i] != -1 && l2.numbers[i] != -1){ // Both numbers
            if(l1.numbers[i] > l2.numbers[i]){
                return -1;
            }
            if(l1.numbers[i] < l2.numbers[i]){
                return 1;
            }
        }
        else if ((l1.numbers[i] == -1) && (l2.numbers[i] == -1)){ // Both lists
            if (compareLists(*l1.lists[i], *l2.lists[i]) != 0){
                return compareLists(*l1.lists[i], *l2.lists[i]);
            }
        }
        else if (l1.numbers[i] == -1 && l2.numbers[i] != -1){ // List in l1 and number in l2
            ListOfLists* listWithOneNumber = new ListOfLists();
            listWithOneNumber->push(l2.numbers[i]);
            if(compareLists(*l1.lists[i], *listWithOneNumber)!=0){
                return compareLists(*l1.lists[i], *listWithOneNumber);
            }
        }
        else if (l1.numbers[i] != -1 && l2.numbers[i] == -1){ // List in l2 and number in l1
            ListOfLists* listWithOneNumber = new ListOfLists();
            listWithOneNumber->push(l1.numbers[i]);
            if(compareLists(*listWithOneNumber, *l2.lists[i]) != 0){
                return compareLists(*listWithOneNumber, *l2.lists[i]);
            }
        }
    }
    if (l2.size() > l1.size()){
        return 1;
    }
    else if(l2.size() < l1.size()){
        return -1;
    }
    return 0;
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    std::vector<ListOfLists> packets;

    while(std::getline(ifs, line)){
        ListOfLists firstList = parseList(line);
        std::getline(ifs, line);
        ListOfLists secondList = parseList(line);
        packets.push_back(firstList);
        packets.push_back(secondList);
        std::getline(ifs, line); // Skip empty line
    }

    // Ex 1
    int sumOfIndicesOfPairsInRightOrder = 0;
    for(int i=0; i < packets.size(); i+=2){
        if (compareLists(packets[i], packets[i+1]) >= 0){
            sumOfIndicesOfPairsInRightOrder += ((i/2)+1);
        }
    }

    // Ex 2
    ListOfLists divider1;
    divider1.push(2);
    ListOfLists divider2;
    divider2.push(6);

    packets.push_back(divider1);
    packets.push_back(divider2);

    // Bubble Sort using compareLists as the comparisson function
    bool hadToSwap = true;
    while(hadToSwap){
        hadToSwap = false;
        for(int i=0; i < packets.size() - 1; i++){
            if (compareLists(packets[i], packets[i+1]) == -1){
                // Swap
                ListOfLists tempPacket = packets[i+1];
                packets[i+1] = packets[i];
                packets[i] = tempPacket;
                hadToSwap = true;
            }
        }
    }

    // Find position of the dividers
    int positionDivider1;
    int positionDivider2;
    for(int i=0; i < packets.size(); i++){
        if (compareLists(packets[i], divider1) == 0){
            positionDivider1 = i+1;
        }
        if (compareLists(packets[i], divider2) == 0){
            positionDivider2 = i+1;
        }
    }

    std::cout << "--Ex1 Output: " << sumOfIndicesOfPairsInRightOrder << std::endl;
    std::cout << "--Ex2 Output: " << positionDivider1*positionDivider2 << std::endl;

    return 0;
}