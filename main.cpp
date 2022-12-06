#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <iterator>
using namespace std;
//#include "HashTable.h"
struct Recipe
{
    double rating;
    string name;
    string hyperlink;
    string category;
    vector<string> keywords;
};

void readCurrentRecipe(std::ifstream& currentStream, std::map<std::string, std::vector<std::string>>& importantValues, std::vector<std::string> columns);
void stringToVector(std::string currentValue, std::vector<std::string>& expanded);

int main(){
    std::map<std::string, std::vector<std::string>> firstRecipeColumnVals;
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

    for (unsigned int i = 0; i < 0; i++){
        readCurrentRecipe(file, firstRecipeColumnVals, allColumnNames);
        std::cout << i << std::endl;
        firstRecipeColumnVals.clear();
        // currently this function simply outputs the values into a single map, hence it will only print the most recent value
    }
    //for (unsigned int i = 0; i < 9; i++){
    //    std::getline(file, line);
    //    // currently this function simply outputs the values into a single map, hence it will only print the most recent value
    //}
    //for (unsigned int i = 0; i < 90; i++){
    //    readCurrentRecipe(file, firstRecipeColumnVals, allColumnNames);
    //    firstRecipeColumnVals.clear();
    //    // currently this function simply outputs the values into a single map, hence it will only print the most recent value
    //}
    //for (unsigned int i = 0; i < 5; i++){
    //    std::getline(file, line);
    //    // currently this function simply outputs the values into a single map, hence it will only print the most recent value
    //}
    //for (unsigned int i = 0; i < 90; i++){
    //    readCurrentRecipe(file, firstRecipeColumnVals, allColumnNames);
    //    firstRecipeColumnVals.clear();
    //    // currently this function simply outputs the values into a single map, hence it will only print the most recent value
    //}
    
    readCurrentRecipe(file, firstRecipeColumnVals, allColumnNames);

    for(auto iterator = firstRecipeColumnVals.begin(); iterator != firstRecipeColumnVals.end(); iterator++){
        std::cout << iterator->first << " ";
        std::cout << iterator->second.size() << " ";
        for(unsigned int i = 0; i < iterator->second.size(); i++){
            std::cout << iterator->second.at(i) << " " << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}

void readCurrentRecipe(std::ifstream& currentStream, std::map<std::string, std::vector<std::string>>& importantValues, std::vector<std::string> columns){
    std::map<std::string, std::vector<std::string>> temporaryBuffer;
    std::string line;
    std::string fullLine = "";

    unsigned int numberFieldsRead = 0;
    std::string currentFragment = "";
    unsigned int index = 0;

    bool requiresContinuation = false;
    
    while(numberFieldsRead < columns.size()){
        requiresContinuation = false;
        std::getline(currentStream, line);
        index = 0;
        while(line.size() < 2){
            std::getline(currentStream, line);
        }
        //std::cout << line.size() << std::endl;
        // Case 1:
        while(index < line.size()){
            try{
                if(numberFieldsRead >= columns.size() == true){
                    throw std::exception();
                }
            }
            catch(std::exception& e){
                index = 0;
                line = "";
                importantValues.clear();
                bool validArgument = false;
                while (validArgument == false){
                    std::getline(currentStream, line);
                    try{
                        int currentValue = std::stoi(line.substr(0,2));
                        validArgument = true;
                    }
                    catch(std::exception& e){
                        continue;
                    }
                }
                numberFieldsRead = 0;
                continue;
            }
            currentFragment += line.at(index);
            index++;
            currentFragment += line.at(index);
            index++;
            if(currentFragment.substr(0, 1) != "\"" && currentFragment.substr(0, 2) != "\"c"){
                //std::cout << "Option 1 " << currentFragment.substr(0, 2) << std::endl;
                while(currentFragment.find(",") == std::string::npos){
                    if(index == line.size()){
                        requiresContinuation = true;
                        break;
                    }
                    currentFragment += line.at(index);
                    index++;
                }
                if(!requiresContinuation){
                    currentFragment = currentFragment.substr(0, currentFragment.size()-1);
                }
            }
            else if(currentFragment.substr(0, 1) == "\"" && currentFragment.substr(0, 2) != "\"c"){
                //std::cout << "Option 2 " << currentFragment.substr(0, 2) << " " << currentFragment << std::endl;
                if(numberFieldsRead == columns.size() - 1){
                    std::size_t found = currentFragment.find("\"\"\"");
                    while(currentFragment.find(".\"\"\"") != std::string::npos){
                        if(index == line.size()){
                            requiresContinuation = true;
                            break;
                        }
                        currentFragment += line.at(index);
                        index++;
                    }
                }
                else{
                    while(currentFragment.find("\",") == std::string::npos){
                        if(index == line.size()){
                            requiresContinuation = true;
                            break;
                        }
                        currentFragment += line.at(index);
                        index++;
                    }
                    if(!requiresContinuation){
                        currentFragment = currentFragment.substr(0, currentFragment.size()-1);
                    }
                }
            }
            else if(currentFragment.substr(0, 1) == "\"" && currentFragment.substr(0, 2) == "\"c"){
                //std::cout << "Option 3" << std::endl;
                if(numberFieldsRead == columns.size() - 1){
                    while(currentFragment.find("\")\"") == std::string::npos){
                        if(index == line.size()){
                            requiresContinuation = true;
                            break;
                        }
                        if(line.at(index) == '\n'){
                            index++;
                            continue;
                        }
                        currentFragment += line.at(index);
                        index++;
                    }
                }
                else{
                    while(currentFragment.find(")\",") == std::string::npos){
                        if(index == line.size()){
                            requiresContinuation = true;
                            break;
                        }
                        if(line.at(index) == '\n'){
                            index++;
                            continue;
                        }
                        currentFragment += line.at(index);
                        index++;
                    }
                }
            }
            else{
                //std::cout << currentFragment << std::endl;
            }
            if(!requiresContinuation){
                std::cout << currentFragment << std::endl;
                stringToVector(currentFragment, importantValues[columns.at(numberFieldsRead)]);
                currentFragment.clear();
                numberFieldsRead++;
            }
            
        }
        //importantValues = temporaryBuffer;
    }

}

void stringToVector(std::string currentValue, std::vector<std::string>& expanded){
    std::string substring = "";
    if (currentValue.find("\"") == std::string::npos) {
        expanded.push_back(currentValue);
    }
    else{
        if (currentValue.find("c(\"") != std::string::npos){
            std::string currentSubstring = "";
            unsigned int i = 0;
            unsigned int startPosition = 0;
            unsigned int endPosition = 0;

            std::string temporarySubstring = "";
            temporarySubstring = currentValue.substr(i, 2);
            while(i < currentValue.size()-1){
                while((temporarySubstring != "\"\"")){
                    temporarySubstring = currentValue.substr(i, 2);
                    if(i < currentValue.size()-1){
                        i++;
                    }
                    else{
                        break;
                    }
                }
                startPosition = i + 1;
                i += 2;
                temporarySubstring = currentValue.substr(i, 2);
                while(temporarySubstring != "\"\""){
                    temporarySubstring = currentValue.substr(i, 2);
                    if(i < currentValue.size()-1){
                        i++;
                    }
                    else{
                        break;
                    }
                }
                endPosition = i - 1;
                currentSubstring = currentValue.substr(startPosition, (endPosition - startPosition));
                i+=1;
                expanded.push_back(currentSubstring);
                //std::cout << currentSubstring << std::endl;
            }
        }
        else{
            std::string currentString = currentValue.substr(1, (currentValue.size() - 2));
            expanded.push_back(currentValue);
        }
        
    }
}



    /*
    0           1   2       3           4       5           6       7               8       9       10              11      12                          13
    RecipeId,Name,AuthorId,AuthorName,CookTime,PrepTime,TotalTime,DatePublished,Description,Images,RecipeCategory,Keywords,RecipeIngredientQuantities,Recipe
    14              15              16          17      18          19                  20              21              22                  23              24              25              26
IngredientParts,AggregatedRating,ReviewCount,Calories,FatContent,SaturatedFatContent,CholesterolContent,SodiumContent,CarbohydrateContent,FiberContent,SugarContent,ProteinContent,RecipeServings
27              28
RecipeYield,RecipeInstructions
    
    */
