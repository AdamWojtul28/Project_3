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

void readCurrentRecipe(ifstream& currentStream, map<string, string>& importantValues, vector<string> columns);
void generateTags(map<string, string>& importantValues, vector<string> columns, string currentString);
void vectorToString(string currentValue, vector<string>& expanded);

int main(){
    map<string, string> firstRecipeColumnVals;
    set<string> firstRecipeTags;

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

    cout << "Completed" << endl;
    
}

void vectorToString(string currentValue, vector<string>& expanded){
    vector<string> substrings;
    string substring = "";
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
