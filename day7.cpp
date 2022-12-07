#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define LOG(x) std::cout << x << std::endl;

const int SIZE_LIMIT = 100000;
const int DISK_SPACE = 70000000;
const int UPDATE_SIZE = 30000000;

const std::string inputFilePath = "day7.txt";

struct File{
    std::string name;
    int size;
};

struct Directory{
    std::string name;
    int size;
    std::vector<Directory*> subDirs;
    std::vector<File> files;
    Directory* parent;
    ~Directory(){
        parent = nullptr;
        for(int i=0; i < subDirs.size();i++){
            delete subDirs[i];
        }
    }
};

Directory* findDirToDelete(Directory* currentDir, const int spaceNeeded){
    // Set current dir as best fit if it has enough size
    Directory* bestFit = nullptr;
    if (currentDir->size >= spaceNeeded){
        bestFit = currentDir;
    }

    // Compare best fit with the dir children's
    for(int i=0; i < (currentDir->subDirs).size();i++){
        Directory* currentChildBestFit = findDirToDelete(currentDir->subDirs[i], spaceNeeded);
        if (currentChildBestFit && (currentChildBestFit->size < bestFit->size)){
            bestFit = currentChildBestFit;
        }
    }

    return bestFit;
}

int DFSandSetSizes(Directory* currentDir){
    for(int i=0; i < (currentDir->subDirs).size();i++){
        currentDir->size += DFSandSetSizes(currentDir->subDirs[i]);
    }
    for(int i=0; i < (currentDir->files).size();i++){
        currentDir->size += currentDir->files[i].size;
    }
    return currentDir->size;
}

int getSumOfAllDirsWithinSizeLimit(Directory* currentDir){
    int sum = 0;
    for (int i = 0; i < (currentDir->subDirs).size(); i++){
        sum += getSumOfAllDirsWithinSizeLimit(currentDir->subDirs[i]);
    }
    if (currentDir->size <= SIZE_LIMIT){
        return sum + currentDir->size;
    }
    else{
        return sum;
    }
}

int main(){
    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string line;

    // Skip first line "cd /"
    std::getline(ifs, line); 
    Directory rootDir = {"/", 0, {}, {}};
    Directory* currentDir = &rootDir;

    while(std::getline(ifs, line)){
        if(line[0] == '$'){ // User command
            std::string command = line.substr(2, 2);
            if (command == "cd")
            {
                std::string whereTo = line.substr(5);
                if (whereTo == ".."){
                    currentDir = currentDir->parent;
                }
                else{
                    for(int i=0; i < currentDir->subDirs.size();i++){
                        if (currentDir->subDirs[i]->name == whereTo){
                            currentDir = currentDir->subDirs[i];
                            break;
                        }
                    }
                }
            }
        }
        else { // We're listing files after the 'ls' command has been used
            const std::string dirOrSize = line.substr(0, line.find(" "));
            const std::string name = line.substr(line.find(" ") + 1);
            int size = 0;
            if(dirOrSize == "dir"){
                Directory* newDir = new Directory{name, size, {}, {} ,currentDir};
                currentDir->subDirs.push_back(newDir);
            } else{ // file
                size = std::stoi(dirOrSize);
                File newFile = {name, size};
                currentDir->files.push_back(newFile);
            }
        }
    }

    
    DFSandSetSizes(&rootDir); // Set sizes of every dir

    // Ex1
    const int ex1Output = getSumOfAllDirsWithinSizeLimit(&rootDir);
    
    // Ex2
    const int spaceNeeded = rootDir.size - (DISK_SPACE - UPDATE_SIZE);
    const Directory* bestDirToDelete = findDirToDelete(&rootDir, spaceNeeded);

    std::cout << "--Ex1 Output: " << ex1Output << std::endl;
    std::cout << "--Ex2 Output: " << bestDirToDelete->size << std::endl;

    return 0;
}

