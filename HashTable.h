#pragma once
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <iostream>

struct Node
{
    // for all of the individual recipes
    std::string name;
    std::string description;
    std::string category;
    double rating;
    std::vector<std::string> mealTags;
    std::vector<std::string> ingredients;
    std::vector<std::string> ingredientAmounts;
    std::vector<std::string> steps;

    Node();
    Node(std::string name_, std::string description_, std::string category_, std::string rating_, std::vector<std::string> mealTags_, 
    std::vector<std::string> ingredients_, std::vector<std::string> ingredientAmounts_, std::vector<std::string> steps_) :
     name(name_), description(description_),rating(stod(rating_)), mealTags(mealTags_), ingredients(ingredients_), ingredientAmounts(ingredientAmounts_), steps(steps_){}

};

struct NodeVect
{
    std::vector<Node*> nodes;

    NodeVect(): nodes(){};
};

class innerHashTable
{
    NodeVect* Meals;                                                            // where all of the meals are linked to from the tags
    std::map<std::string,int> tags;                         //1 stores the tags. an optimization so that we don't have to keep finding the same index if it has 
    double loadFactor;                                                      //2 already been through the hash function
    int currentSize;
    int maxSize;

public:
    bool exist;                                                             // so we can see what hashtables in CategoryHashtable are occupied

    innerHashTable();
    ~innerHashTable();

    void InsertMeal(Node* meal);
    int HashFunction(std::string hashString);
    void rehash();

    std::set<Node*> TagSearch(std::vector<std::string> taglist, bool and);

    void Print();

    void retrieveTags(std::set<std::string>& keys);

};

class HashTable
{
    
    innerHashTable* CategoryHashTable;                                // where all the meal categories are linked
    std::map<std::string,int> Cats;                                 // similar to the tags map but stores all of the categories


public:
    HashTable();

    void InsertMeal(std::string name_, std::string description_, std::string category_, std::string rating_, std::vector<std::string> mealTags_, 
        std::vector<std::string> ingredients_, std::vector<std::string> ingredientAmounts_, std::vector<std::string> steps_);
    int HashFunction(std::string hashString);

    std::set<Node*> TagSearch(std::string category, std::vector<std::string> whitelist, std::vector<std::string> blacklist, bool and);

    void Print();

    std::vector<std::string> retrieveAllTags();
    std::vector<std::string> retrieveCatTags(std::string cat);

};

//===========================================Constructors================================//

// there are less than 400 categories in our data set so I initialized the array to accomidate all of them to prevent rehashing
HashTable::HashTable()
{
    CategoryHashTable[400] = {};
}

// it depends on the category what sort of tags will be in there so rehashing isn't preventable and thus this takes on an initial value of 100 spaces and doubles every rehash
innerHashTable::innerHashTable()
{
    Meals = new NodeVect[100]();
    loadFactor = 0.0;
    currentSize = 0;
    maxSize = 100;
    exist = false;
}

// destructor
innerHashTable::~innerHashTable()
{
    delete[] Meals;
}

//============================================Hash Functions=============================================//

// will take in the hashable string and turn it into a usable index by taking the powers of 27 multiplied by the ascii values of the string then it takes the modulus of the size of the array
// to give us an index. if there is a collision it will detect that and iterate the index by 1 until an open spot is found. the index is then loaded into the map keeping log of all of the 
// hash string -> index pairs
int HashTable::HashFunction(std::string hashString)
{
    if (Cats.find(hashString) == Cats.end())
    {
        long preIndex = 0;
        for (int i = 0; i < hashString.length(); i++)
        {
            int ascii = int(hashString[i]);
            preIndex += ascii * pow(27,i);
        }

        int index = 400 % preIndex;

        if (CategoryHashTable[index].exist)
        {
            while(CategoryHashTable[index].exist)
            {
                index++;
                index = 400 % index;
            }
        }

        Cats[hashString] = index;

        return index;
    }

    else
        return Cats[hashString];
}

// similar to the above but also iterates the size and load factor
int innerHashTable::HashFunction(std::string hashString)
{
    if (tags.find(hashString) == tags.end())
    {
        long preIndex = 0;
        for (int i = 0; i < hashString.length(); i++)
        {
            int ascii = int(hashString[i]);
            preIndex += ascii * pow(27,i);
        }

        int index = maxSize % preIndex;

        if (!Meals[index].nodes.empty())
        {
            while(!Meals[index].nodes.empty())
            {
                index++;
                index = maxSize % index;
            }
        }

        tags[hashString] = index;
        currentSize++;
        loadFactor = (1.0 * currentSize) / maxSize;

        return index;
    }

    else
        return tags[hashString];
}

//==============================================================Insertions=================================================//

// creates meal node then
// finds and inserts into the right category first then sends it to the next insert function
void HashTable::InsertMeal(std::string name_, std::string description_, std::string category_, std::string rating_, std::vector<std::string> mealTags_, 
    std::vector<std::string> ingredients_, std::vector<std::string> ingredientAmounts_, std::vector<std::string> steps_)
{
    Node* meal = &Node(name_, description_, category_, rating_, mealTags_, ingredients_, ingredientAmounts_, steps_);
    std::string hashString = meal->category;   

    int index = HashFunction(hashString);

    CategoryHashTable[index].InsertMeal(meal);
    
}

// inserts node pointers into all of the associated tags
// checks loadFactor and if loadfactor is too high rehashes the function
void innerHashTable::InsertMeal(Node* meal)
{
    std::vector<std::string> mealTags = meal->mealTags;
    if (mealTags.size() == 0)
    {
        int index = HashFunction("No Tags");
        Meals[index].nodes.push_back(meal);
    }
    
    else
    {
        for (int i = 0; i < mealTags.size(); i++)
        {
            int index = HashFunction(mealTags[i]);
            Meals[index].nodes.push_back(meal);
        }
    }

    exist = true;

    if (loadFactor >= 0.8)
    {
        rehash();
    }

}

// doubles table size and redoes all of the hashing for the hash strings stored in tags
// then stores the data in their new homes
void innerHashTable::rehash()
{
    NodeVect* oldMealsArr = new NodeVect[maxSize];
    for (int i = 0; i < maxSize; i++)
    {
        oldMealsArr[i] = Meals[i];
    }
    delete[] Meals;
    std::map<std::string,int> oldTags = tags;
    tags.clear();
    
    maxSize *= 2;
    currentSize = 0;
    Meals = new NodeVect[maxSize]();

    for (auto itr = oldTags.begin(); itr != oldTags.end(); itr++)
    {
        int index = HashFunction(itr->first);
        Meals[index] = oldMealsArr[itr->second];
    }

    delete[] oldMealsArr;

}

//=======================================searches=====================================//

// finds the right category then sends the function off to fill in the next tag search useing the whitelist
// once filled from the white list we find all of the recipes that cotain tags in the blacklist then pull those recipes out of our set
std::set<Node*> HashTable::TagSearch(std::string category, std::vector<std::string> whitelist, std::vector<std::string> blacklist, bool and)
{
    std::set<Node*> returnSet;
    std::set<Node*> blacklistSet;

    // if whitelist is empty we put all of the tags in there and just grab everything
    if (whitelist.empty())
    {
        for (auto iter = Cats.begin(); iter != Cats.end(); iter++)
            whitelist.push_back(iter->first);
    }
    
    returnSet = CategoryHashTable[Cats[category]].TagSearch(whitelist, and);

    // if blacklist is empty we ingnore it
    if (!blacklist.empty())
    {
        blacklistSet = CategoryHashTable[Cats[category]].TagSearch(blacklist, false);
        std::set<Node*> intersect;
        set_intersection(blacklistSet.begin(), blacklistSet.end(), returnSet.begin(), returnSet.end(), intersect.begin());

        for (auto itr = intersect.begin(); itr != intersect.end(); itr++)
        {
            if (returnSet.count(*itr))
                returnSet.erase(*itr);
        }
    }

    return returnSet;
}

// searches the category for recipies containing one or all of the tags
// bool and is for determining if we are going to return any recipe with any of the tags (false) or recipes with only all of the tags (true)
std::set<Node*> innerHashTable::TagSearch(std::vector<std::string> tagList, bool and)
{
    std::set<Node*> output;
    
    if (and)
    {
        std::set<Node*> compare1;
        std::set<Node*> compare2;
        for (int j = 0; j < Meals[tags[tagList[0]]].nodes.size(); j++)
            compare1.insert(Meals[tags[tagList[0]]].nodes[j]);
        
        for (int i = 1; i < tagList.size(); i++)
        {
            for (int j = 0; j < Meals[tags[tagList[i]]].nodes.size(); j++)
                compare2.insert(Meals[tags[tagList[i]]].nodes[j]);

            
            set_intersection(compare1.begin(), compare1.end(), compare2.begin(), compare2.end(), output.begin());
            compare1 = output;
        }
    }

    else
    {
        for (int i = 1; i < tagList.size(); i++)
        {
            for (int j = 0; j < Meals[tags[tagList[i]]].nodes.size(); j++)
                output.insert(Meals[tags[tagList[i]]].nodes[j]);
        }
    }

    return output;
}

//=========================================================Prints==========================================================//

void HashTable::Print()
{
    for (auto itr = Cats.begin(); itr != Cats.end(); itr++)
    {
        std::cout << itr->first << std::endl;
        CategoryHashTable[itr->second].Print();
    }
}

void innerHashTable::Print()
{
    for(auto itr = tags.begin(); itr != tags.end(); itr++)
    {
        std::cout << itr->first << std::endl;
        for (int i = 0; i < Meals[itr->second].nodes.size(); i++)
        {
            std::cout << Meals[itr->second].nodes[i]->name << ", " << std::endl;
        }

        std::cout << std::endl << std::endl;
    }
}

//=====================================================Givers===============================================================//

// retrieves the tags of the category that calls it
void innerHashTable::retrieveTags(std::set<std::string>& keys)
{
    for (auto iter = tags.begin(); iter != tags.end(); iter++)
    {
        keys.insert(iter->first);
    }
}

// retrieves tags of all categories
std::vector<std::string> HashTable::retrieveAllTags()
{
    std::set<std::string> keys;
    for (auto iter = Cats.begin(); iter != Cats.end(); iter++)
    {
        CategoryHashTable[iter->second].retrieveTags(keys);
    }

    std::vector<std::string> allTags;
    for (auto iter = keys.begin(); iter != keys.end(); iter++)
    {
        allTags.push_back(*iter);
    }

    return allTags;


}

// returns all tags in a given category
std::vector<std::string> HashTable::retrieveCatTags(std::string cat)
{
    std::set<std::string> keys;
    CategoryHashTable[Cats[cat]].retrieveTags(keys);

    std::vector<std::string> catTags;
    for (auto iter = keys.begin(); iter != keys.end(); iter++)
    {
        catTags.push_back(*iter);
    }

    return catTags;

}