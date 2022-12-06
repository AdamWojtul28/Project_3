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

void readCurrentRecipe(ifstream& currentStream, map<string, string>& importantValues, vector<string> columns);
void generateTags(map<string, string>& importantValues, vector<string> columns, string currentString);
void vectorToString(string currentValue, vector<string>& expanded);
void vectorToString(std::string currentValue, std::vector<std::string>& expanded);

int main(){
    map<string, string> firstRecipeColumnVals;
    set<string> firstRecipeTags;
    vector<Recipe> recipeList;

    //============================================Read the data from the file and store into Recipe nodes=============================================//
    string line;
    // the recipes.csv file should be a part of the project folder at some point
    ifstream file("archive/recipes.csv");
	getline(file, line);
    
    string currentColumnName;
    vector<string> allColumnNames;

    stringstream ss(line);
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
        cout << iterator->first << " " << iterator->second << endl;
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

void readCurrentRecipe(ifstream& currentStream, map<string, string>& importantValues, vector<string> columns){
    string line;
    string fullLine = "";

    getline(currentStream, line);
    fullLine += line;
    while(line.substr(line.size()-2, 2) != ")\""){
        getline(currentStream, line);
        fullLine += line;
    }
    // Notice that the end of any line must have the characters )\" as its last few characters; therefore, if these aren't the last value, read next line
    
    cout << fullLine << endl << endl;

    generateTags(importantValues, columns, fullLine);

}

void generateTags(map<string, string>& importantValues, vector<string> columns, string currentString){

    string currentFragment = "";
    unsigned int tagToAssign = 0;
    unsigned int currentPosition = 0;

    while(currentPosition < (currentString.size() - 2)){
        if(currentFragment.find("\"") != string::npos){
            if(currentFragment.size() < 4){
                currentFragment += currentString.at(currentPosition);
            }
            else{
                currentFragment += currentString.at(currentPosition);
                string temporaryString = currentFragment + currentString.at(currentPosition + 1);
                if(temporaryString.substr(temporaryString.size()-3, 3) != "\", " && temporaryString.substr(temporaryString.size()-3, 2) == "\","){
                    importantValues[columns.at(tagToAssign)] = currentFragment;
                    cout << currentFragment << endl << endl;
                    tagToAssign++;
                    currentFragment.clear();
                }
            }
        }
        else{
            if(currentString.at(currentPosition) == ','){
                importantValues[columns.at(tagToAssign)] = currentFragment;
                cout << currentFragment << endl << endl;
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
        currentFragment += currentString.at(currentPosition) + currentString.at(currentPosition + 1);
        importantValues[columns.at(tagToAssign)] = currentFragment;
        tagToAssign++;
        currentFragment.clear();
    }

    std::cout << "Completed" << std::endl;
    
}

void vectorToString(std::string currentValue, std::vector<std::string>& expanded){
    std::vector<std::string> substrings;
    std::string substring = "";
    for (unsigned int i = 0; i < currentValue.size() - 1; i++){
        if(currentValue.at(i) == '\"' && currentValue.at(i + 1) == '\"'){
            i += 2;
            while(!(currentValue.at(i) == '\"' && currentValue.at(i + 1) == '\"')){
                if(currentValue.at(i) == '\n'){
                    continue;
                }
                substring += currentValue.at(i);
                i++;
            }
            i++;
            substrings.push_back(substring);
            substring.clear();
        }
        else if(currentValue.at(i) == '\n'){
            continue;
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
