#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

// hisHand: A rock B paper C scissors
// myHand: X rock Y paper Z scissors
//          1 pt   2 pt    3pt
// Win 6pt Draw 3pt Lose 0pt

// Example input:
/*
    A Y
    B X
    C Z 
*/

const std::string inputFilePath = "inputs/day2.txt";

const int WIN_POINTS = 6;
const int DRAW_POINTS = 3;
const int LOSE_POINTS = 0;

std::map<char,int> myHandScores =  {{'X',1}, {'Y',2}, {'Z',3}};
std::map<char,char> hisValuesTranslation =  {{'A','X'}, {'B','Y'}, {'C','Z'}};

int calculateEx1(){
    int totalScore = 0;

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string match;
    while(std::getline(ifs, match)){
        const char hisHand = match.at(0);
        const char myHand = match.at(2);

        // Calculate my hand's score
        const int value = myHandScores[myHand];
        totalScore += value;

        // Calculate outcome and it's score
        const char hisHandTranslated = hisValuesTranslation[hisHand];
        if(hisHandTranslated == myHand){
            totalScore += DRAW_POINTS;
        }
        else switch (hisHandTranslated)
        {
        case 'X':
            if (myHand == 'Y'){
                totalScore += WIN_POINTS;
            }
            break;
        case 'Y':
            if (myHand == 'Z'){
                totalScore += WIN_POINTS;
            }
            break;
        case 'Z':
            if (myHand == 'X'){
                totalScore += WIN_POINTS;
            }
            break;
        };
    }
    ifs.close();
    return totalScore;
}

int calculateEx2(){
    // X need to lose, Y need to draw, Z need to win, 

    std::map<char,int> outcomeScores =  {{'X', LOSE_POINTS}, {'Y', DRAW_POINTS}, {'Z', WIN_POINTS}};

    int totalScore = 0;

    std::ifstream ifs(inputFilePath, std::ifstream::in);
    std::string match;
    while(std::getline(ifs, match)){
        const char hisHand = match.at(0);
        const char outcome = match.at(2);

        // Get score from outcome
        totalScore += outcomeScores.at(outcome);

        // Get score from my hand
        const char hisHandTranslated = hisValuesTranslation.at(hisHand);
        switch (outcome)
        {
            case 'Y': // draw
                totalScore += myHandScores.at(hisHandTranslated);
                break;

            case 'Z': // win
                switch (hisHandTranslated)
                {
                case 'Z':
                    totalScore += myHandScores.at('X');
                    break;

                default: // X, Y
                    totalScore += myHandScores.at((char)(hisHandTranslated + 1));
                }
                break;

            case 'X': // lose
                switch (hisHandTranslated)
                {
                case 'X':
                    totalScore += myHandScores.at('Z');
                    break;
                default: // Y, Z
                    totalScore += myHandScores.at((char)(hisHandTranslated - 1));
                }
                break;
            }
    }
    ifs.close();
    return totalScore;
}

int main(){

    std::cout << "--Ex1 Output: " << calculateEx1() << std::endl;
    std::cout << "--Ex2 Output: " << calculateEx2() << std::endl;

    return 0;
}