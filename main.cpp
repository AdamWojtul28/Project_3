#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include "HashTable.h"
//#include "MaxHeap.h"
using namespace std;
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
    map<string, string> firstRecipeColumnVals;
    set<string> firstRecipeTags;
    vector<Recipe> recipeList;

    //============================================Read the data from the file and store into Recipe nodes=============================================//
    string line;
    // the recipes.csv file should be a part of the project folder at some point
    ifstream file("archive/recipes.csv");
	getline(file, line);
    
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
    // should have a full vector recipeList at this point
    //============================================User Prompts=============================================//
    MaxHeap RecipeHeap;
    RecipeHeap.BuildMaps(recipeList);
    vector<string> availableCategories = RecipeHeap.GetCategories();
    cout << "Welcome to the Recipe Generator!\n";
    string userInput = "";
    string userCategory = "-1";
    vector<string> keywordList;
    while (stoi(userInput) != 0)
    {
        cout << "Menu:\n";
        cout << "0. Exit program\n";
        cout << "1. Enter a new food category to search in\n";
        cout << "2. Add or remove keywords from whitelist\n";
        cout << "3. Add or remove keywords from blacklist\n";
        cout << "4. Run search for x amount of recipes\n\n";
        cin >> userInput;
        switch (stoi(userInput))
        {
            case 0:
            {
                return 0;
            }

            case 1:
            {
                while (!userCategory.empty())
                {
                    cout << "Enter category (first letter uppercase for all words)\n";
                    cin >> userCategory;
                    // check that it is a category that is in the file
                    bool isCat = false;
                    for (int i = 0; i < availableCategories.size(); i++)
                    {
                        if (userCategory == availableCategories.at(i))
                        {
                            isCat = true;
                            break;
                        }
                    }
                    if (isCat)
                    {
                        cout << "Success!\n";
                    }

                    else
                    {
                        cout << "Either this category does not exist or check your spelling and capitalization.\n";
                        userCategory.clear();
                    }
                }
                break;
            }

            case 2:
            {
                string userInput2;
                cout << "Enter 0 to add a keyword to whitelist or 1 to remove a keyword.\n";
                cin >> userInput2;
                if (stoi(userInput2) == 0)
                {
                    if (keywordList.empty())
                    {
                        cout << "There are no keywords to remove!\n";
                    }

                    else
                    {
                        string userKeyword = "-1";
                        while (!userKeyword.empty())
                        {
                            bool inList = false;
                            int pos;
                            cout << "Enter keyword to add\n";
                            cin >> userKeyword;
                            for (int i = 0; i < keywordList.size(); i++)
                            {
                                if (userKeyword == keywordList.at(i))
                                {
                                    pos = i;
                                    inList = true;
                                    break;
                                }
                            }
                            if (inList)
                            {
                                keywordList.erase(keywordList.begin() + pos);
                                cout << "Success!\n";
                            }
                            else
                            {
                                cout << "Either this category does not exist or check your spelling and capitalization.\n";
                                userKeyword.clear();
                            }
                        }
                    }
                }

                if (stoi(userInput2) == 1)
                {
                    string userKeyword = "-1";
                    while (!userKeyword.empty())
                    {
                        cout << "Enter keyword to add\n";
                        cin >> userKeyword;
                        // check if the keyword exists using hashtable function here
                    }
                }
                break;
            }

            case 3:
            {
                // similar to case 2, just using blacklist variables
                break;
            }

            case 4:
            {
                // add in datastructure functions
                break;
            }

            default:
            {
                cout << "Invalid entry\n";
            }
        }
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
