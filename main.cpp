#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include "HashTable.h"


void readCurrentRecipe(std::ifstream& currentStream, std::map<std::string, std::string>& importantValues, std::vector<std::string> columns);
void generateTags(std::map<std::string, std::string>& importantValues, std::vector<std::string> columns, std::string currentString);

// testing out the committing process
int main(){
    std::map<std::string, std::string> firstRecipeColumnVals;
    std::set<std::string> firstRecipeTags;

    std::string line;
    std::ifstream file("archive/recipes.csv");
	std::getline(file, line);
    
    std::string currentColumnName;
    std::vector<std::string> allColumnNames;

    std::stringstream ss(line);
    while(!ss.eof()){
        getline(ss, currentColumnName, ',');
        allColumnNames.push_back(currentColumnName);
    }
    // Pushes back the name of every column to a vector, which is later used to generate the map of info within the data

    for (unsigned int i = 0; i < 25; i++){
        readCurrentRecipe(file, firstRecipeColumnVals, allColumnNames);
        // currently this function simply outputs the values into a single map, hence it will only print the most recent value
    }

    for(auto iterator = firstRecipeColumnVals.begin(); iterator != firstRecipeColumnVals.end(); iterator++){
        std::cout << iterator->first << " " << iterator->second << std::endl;
    }

    return 0;
}

void readCurrentRecipe(std::ifstream& currentStream, std::map<std::string, std::string>& importantValues, std::vector<std::string> columns){
    std::string line;
    std::string fullLine = "";

    std::getline(currentStream, line);
    fullLine += line;
    while(line.substr(line.size()-2, 2) != ")\""){
        std::getline(currentStream, line);
        fullLine += line;
    }
    // Notice that the end of any line must have the characters )\" as its last few characters; therefore, if these aren't the last value, read next line
    
    std::cout << fullLine << std::endl << std::endl;

    generateTags(importantValues, columns, fullLine);

}

void generateTags(std::map<std::string, std::string>& importantValues, std::vector<std::string> columns, std::string currentString){

    std::string currentFragment = "";
    unsigned int tagToAssign = 0;
    unsigned int currentPosition = 0;

    while(currentPosition < (currentString.size() - 2)){
        if(currentFragment.find("\"") != std::string::npos){
            if(currentFragment.size() < 4){
                currentFragment += currentString.at(currentPosition);
            }
            else{
                currentFragment += currentString.at(currentPosition);
                std::string temporaryString = currentFragment + currentString.at(currentPosition + 1);
                if(temporaryString.substr(temporaryString.size()-3, 3) != "\", " && temporaryString.substr(temporaryString.size()-3, 2) == "\","){
                    importantValues[columns.at(tagToAssign)] = currentFragment;
                    std::cout << currentFragment << std::endl << std::endl;
                    tagToAssign++;
                    currentFragment.clear();
                }
            }
        }
        else{
            if(currentString.at(currentPosition) == ','){
                importantValues[columns.at(tagToAssign)] = currentFragment;
                std::cout << currentFragment << std::endl << std::endl;
                tagToAssign++;
                currentFragment.clear();
            }
            else{
                currentFragment += currentString.at(currentPosition);
            }
        }
        currentPosition++;
    }
    if(currentPosition >= currentString.size()-2){
        currentFragment += currentFragment + currentString.at(currentPosition) + currentString.at(currentPosition + 1);
        importantValues[columns.at(tagToAssign)] = currentFragment;
        tagToAssign++;
        currentFragment.clear();
    }

    std::cout << "Completed" << std::endl;

// FOR THE TIME BEING, USE THIS 
    
}

    /*
    0           1   2       3           4       5           6       7               8       9       10              11      12                          13
    RecipeId,Name,AuthorId,AuthorName,CookTime,PrepTime,TotalTime,DatePublished,Description,Images,RecipeCategory,Keywords,RecipeIngredientQuantities,Recipe
    14              15              16          17      18          19                  20              21              22                  23              24              25              26
IngredientParts,AggregatedRating,ReviewCount,Calories,FatContent,SaturatedFatContent,CholesterolContent,SodiumContent,CarbohydrateContent,FiberContent,SugarContent,ProteinContent,RecipeServings
27              28
RecipeYield,RecipeInstructions
    
    */
