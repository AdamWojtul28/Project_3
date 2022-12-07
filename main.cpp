#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <queue>
#include <cmath>
//#include "HashTable.h"
#include "MaxHeap.h"
using namespace std;

void readCurrentRecipe(std::ifstream& currentStream, std::map<std::string, std::vector<std::string>>& importantValues, std::vector<std::string> columns);
void stringToVector(std::string currentValue, std::vector<std::string>& expanded);
Recipe generateRecipe(Recipe& currentRecipe, map<string, vector<string>> firstRecipeColumnVals);
void assignCurrentValue(std::string valueTitle, map<string, vector<string>> firstRecipeColumnVals, std::string& recipeValue);

int main(){
    map<string, vector<string>> firstRecipeColumnVals;
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

    while(!file.eof()){
        readCurrentRecipe(file, firstRecipeColumnVals, allColumnNames);
        Recipe newRecipe;
        generateRecipe(newRecipe, firstRecipeColumnVals);
        // Insert this recipe into your data structures
        recipeList.push_back(newRecipe);
        firstRecipeColumnVals.clear();
    }
    // should have a full vector recipeList at this point
    //============================================User Prompts=============================================//
   // create heap
   MaxHeap RecipeHeap;
   RecipeHeap.BuildMaps(recipeList);
   
   // create Hash Table
   //HashTable RecipeHash;
   
    // other variables
   vector<string> availableCategories = RecipeHeap.GetCategories();
   string userInput = "";
   string userCategory = "";
   vector<string> keywordWhiteList;
   vector<string> keywordBlackList;
  // vector<string> allKeywords = RecipeHash.retrieveAllTags();

   cout << "Welcome to the Recipe Generator!\n";
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
               while (userCategory.empty())
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
               cout << "Enter 0 to remove a keyword to whitelist or 1 to add a keyword.\n";
               cin >> userInput2;
               if (stoi(userInput2) == 0)
               {
                   if (keywordWhiteList.empty())
                   {
                       cout << "There are no keywords to remove!\n";
                   }

                   else
                   {
                       string userKeyword = "";
                       while (userKeyword.empty())
                       {
                           bool inList = false;
                           int pos;
                           cout << "Enter keyword to remove\n";
                           cin >> userKeyword;
                           for (int i = 0; i < keywordWhiteList.size(); i++)
                           {
                               if (userKeyword == keywordWhiteList.at(i))
                               {
                                   pos = i;
                                   inList = true;
                                   break;
                               }
                           }
                           if (inList)
                           {
                               keywordWhiteList.erase(keywordWhiteList.begin() + pos);
                               cout << "Success!\n";
                           }
                           else
                           {
                               cout << "Either this keyword does not exist or check your spelling and capitalization.\n";
                               userKeyword.clear();
                           }
                       }
                   }
               }

               if (stoi(userInput2) == 1)
               {
                   string userKeyword = "";
                   while (userKeyword.empty())
                   {
                       cout << "Enter keyword to add\n";
                       cin >> userKeyword;
                       bool inList = false;
                       bool alreadyAdded = false;
                       for (int i = 0; i < keywordWhiteList.size(); i++)
                       {
                            if (userKeyword == keywordWhiteList.at(i))
                            {
                                alreadyAdded = true;
                                break;
                            }
                       }
                       if (alreadyAdded)
                       {
                            cout << "You already have this tag!\n";
                            userKeyword.clear();
                       }

                       if (!alreadyAdded)
                       {
                        // for (int i = 0; i < allKeywords.size(); i++)
                        // {
                        //     if (userKeyword == allKeywords.at(i))
                        //     {
                        //         inList = true;
                        //         break;
                        //     }
                        // }

                        if (inList)
                            {
                                cout << "Success!\n";
                                keywordWhiteList.push_back(userKeyword);
                            }

                            else
                            {
                                cout << "Either this keyword does not exist or check your spelling and capitalization.\n";
                                userKeyword.clear();
                            }
                       }
                   }
               }
               break;
           }

           case 3:
           {
               string userInput2;
               cout << "Enter 0 to remove a keyword to blacklist or 1 to add a keyword.\n";
               cin >> userInput2;
               if (stoi(userInput2) == 0)
               {
                   if (keywordBlackList.empty())
                   {
                       cout << "There are no keywords to remove!\n";
                   }

                   else
                   {
                       string userKeyword = "";
                       while (userKeyword.empty())
                       {
                           bool inList = false;
                           int pos;
                           cout << "Enter keyword to remove\n";
                           cin >> userKeyword;
                           for (int i = 0; i < keywordBlackList.size(); i++)
                           {
                               if (userKeyword == keywordBlackList.at(i))
                               {
                                   pos = i;
                                   inList = true;
                                   break;
                               }
                           }
                           if (inList)
                           {
                               keywordBlackList.erase(keywordBlackList.begin() + pos);
                               cout << "Success!\n";
                           }
                           else
                           {
                               cout << "Either this keyword does not exist or check your spelling and capitalization.\n";
                               userKeyword.clear();
                           }
                       }
                   }
               }

               if (stoi(userInput2) == 1)
               {
                   string userKeyword = "";
                   while (userKeyword.empty())
                   {
                       cout << "Enter keyword to add\n";
                       cin >> userKeyword;
                       bool inList = false;
                       bool alreadyAdded = false;
                       for (int i = 0; i < keywordBlackList.size(); i++)
                       {
                            if (userKeyword == keywordBlackList.at(i))
                            {
                                alreadyAdded = true;
                                break;
                            }
                       }
                       if (alreadyAdded)
                       {
                            cout << "You already have this tag!\n";
                            userKeyword.clear();
                       }

                       if (!alreadyAdded)
                       {
                        // for (int i = 0; i < allKeywords.size(); i++)
                        // {
                        //     if (userKeyword == allKeywords.at(i))
                        //     {
                        //         inList = true;
                        //         break;
                        //     }
                        // }

                        if (inList)
                            {
                                cout << "Success!\n";
                                keywordBlackList.push_back(userKeyword);
                            }

                            else
                            {
                                cout << "Either this keyword does not exist or check your spelling and capitalization.\n";
                                userKeyword.clear();
                            }
                       }
                   }
               }
               break;
           }

           case 4:
           {
                string numRecipes;
                string userMethod;
                string mustMatchWL;
                string minRating;
                vector<Recipe> result;
                bool WL;
               cout << "How many recipes would you like returned?\n";
               cin >> numRecipes;

               cout << "What is the minimum rating?\n";
               cin >> minRating;
               cout << "Would you like the recipes to have all whitelisted tags? Enter yes or no.\n";
               cin >> mustMatchWL;
               if (mustMatchWL == "yes")
               {
                WL = true;
               }
               else
               {
                WL = false;
               }
               cout << "Enter 0 to run search using Heap, 1 for Hash\n";
               cin >> userMethod;
               if (stoi(userMethod) == 0)
               {
                    result = RecipeHeap.GetRecipes(WL, stoi(numRecipes), stoi(minRating), keywordBlackList, keywordWhiteList, userCategory);
               }

               for (int i = 0; i < result.size(); i++)
               {
                cout << result.at(i).name << endl;
               }
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
            std::string temporaryHolder = currentValue.substr(3, currentValue.size()-4);
            std::cout << temporaryHolder << std::endl;
            int sizeValue = temporaryHolder.size();
        
            std::string currentSubstring = "";
            unsigned int i = 0;
            unsigned int startPosition = 0;
            unsigned int endPosition = 0;
        
            bool newItem = false;
            for(unsigned int i = 0; i < sizeValue; i++){
                if(temporaryHolder.at(i) == '\"' && temporaryHolder.at(i) == '\"' && newItem == false){
                    newItem = true;
                    startPosition = i;
                }
                if(temporaryHolder.at(i) == '\"' && temporaryHolder.at(i+1) == ',' && newItem == true){
                    endPosition = i;
                }
                if(endPosition > 0){
                    currentSubstring = temporaryHolder.substr(startPosition + 2, (endPosition - startPosition - 3));
                    expanded.push_back(currentSubstring);
                    startPosition = 0; 
                    endPosition = 0;
                    newItem = false;
                }
                if(i == sizeValue - 2){
                    endPosition = i;
                    if(temporaryHolder.at(i) == '\"'){
                        currentSubstring = temporaryHolder.substr(startPosition + 2, (endPosition - startPosition - 3));
                    }
                    else{
                        currentSubstring = temporaryHolder.substr(startPosition + 2, (endPosition - startPosition - 4));
                    }
                    expanded.push_back(currentSubstring);
                    break;
                }
            }
        }
        else{
            std::string currentString = currentValue.substr(1, (currentValue.size() - 2));
            expanded.push_back(currentValue);
        }
    }
}

Recipe generateRecipe(Recipe& currentRecipe, map<string, vector<string>> firstRecipeColumnVals){
    std::string ratingString = "";

    assignCurrentValue("Name", firstRecipeColumnVals, currentRecipe.name);
    assignCurrentValue("RecipeCategory", firstRecipeColumnVals, currentRecipe.category);
    assignCurrentValue("AggregatedRating", firstRecipeColumnVals, currentRecipe.category);
    currentRecipe.rating = std::stod(ratingString);
    auto iterator = firstRecipeColumnVals.find("Keys");
    for(unsigned i = 0; i < iterator->second.size(); i++){
        currentRecipe.keywords.push_back(iterator->second.at(i));
    }

}

void assignCurrentValue(std::string valueTitle, map<string, vector<string>> firstRecipeColumnVals, std::string& recipeValue){
    auto iterator = firstRecipeColumnVals.find(valueTitle);
    for(unsigned i = 0; i < iterator->second.size(); i++){
        recipeValue = iterator->second.at(i);
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