#pragma once
#include <string>
#include <vector>
#include <map>


struct Node
{
    // for all of the individual recipes
    std::string name;
    std::string description;
    std::string category;
    std::vector<std::string> mealTags;
    std::vector<std::string> ingredients;
    std::vector<std::string> ingredientAmounts;
    std::vector<std::string> steps;
    Node* next;
    Node* prev;

// need to adjust this to some degree probably for ease of display
    Node(): name(), description(), mealTags(), ingredients(), ingredientAmounts(), steps(), next(nullptr), prev(nullptr){};
    Node(std::string name_, std::string description_, std::string link_, std::string category_, std::vector<std::string> mealTags_, 
    std::vector<std::string> ingredients_, std::vector<std::string> ingredientAmounts_, std::vector<std::string> steps_) :
     name(name_), description(description_), mealTags(mealTags_), ingredients(ingredients_), ingredientAmounts(ingredientAmounts_), steps(steps_), 
     next(nullptr), prev(nullptr){}

};

class innerHashTable
{
    Node* Meals;                                                            // where all of the meals are linked to from the tags
    std::map<std::string,int> tags;                         //1 stores the tags. an optimization so that we don't have to keep finding the same index if it has 
    double loadFactor;                                                      //2 already been through the hash function
    int currentSize;
    int maxSize;

public:
    bool exist;

    innerHashTable();
    ~innerHashTable();

    void InsertMeal(Node* meal);
    void insertLL(Node* meal, int index);
    void rehash();
    int checkCollision(int index, std::string hashString);

};

class HashTable
{
    
    innerHashTable* CategoryHashTable;                                // where all the meal categories are linked
    std::map<std::string,int> Cats;                                 // similar to the tags map but stores all of the categories


public:
    HashTable();

    void InsertMeal(std::string name_, std::string description_, std::string link_, std::string category_, std::vector<std::string> mealTags_, 
    std::vector<std::string> ingredients_, std::vector<std::string> ingredientAmounts_, std::vector<std::string> steps_);
    std::vector<Node*> TagSearch(std::string category, std::vector<std::string> whitelist, std::vector<std::string> blacklist);
    int checkCollision(std::string hashString, int index);

};

//===========================================Constructors================================//

HashTable::HashTable()
{
    CategoryHashTable[400] = {};
}

innerHashTable::innerHashTable()
{
    Meals = new Node[100]();
    loadFactor = 0.0;
    currentSize = 0;
    maxSize = 100;
    exist = false;
}

innerHashTable::~innerHashTable()
{
    delete[] Meals;
}

//============================================Insertions=============================================//

int HashFunction(std::string hashString, std::map<std::string,int>& tags, int maxSize)
{
    // our hash function here came from the class slides and uses the powers of 27 
    if (tags.find(hashString) == tags.end())
    {
        int index = -1;
        long preIndex = 0;
        for (int i = 0; i < hashString.length(); i++)
        {
            int ascii = int(hashString[i]);
            preIndex += ascii * pow(27,i);
        }

        index = maxSize % preIndex;

        // still need to iterate in case of collisions

        tags[hashString] = index;

        return index;
    }

    else
        return tags[hashString];
}

// check collisions checks for collistions and iterates the index if it finds a collision
int innerHashTable::checkCollision(int index, std::string hashString)
{
    if (Meals[index].name == "")
    {
        tags[hashString] = index;
        return index;
    }
    
    
    int i = 0;
    while(Meals[index].name != "")
    {
        index++;
        index = maxSize % index;
    }

    tags[hashString] = index;
    return index;
}

int HashTable::checkCollision(std::string hashString, int index)
{
    if (!CategoryHashTable[index].exist)
    {
        Cats[hashString] = index;
        return index;
    }

    while(CategoryHashTable[index].exist)
    {
        index++;
        index = 400 % index;
    }

    Cats[hashString] = index;
    return index;
}

void HashTable::InsertMeal(std::string name_, std::string description_, std::string link_, std::string category_, std::vector<std::string> mealTags_, 
    std::vector<std::string> ingredients_, std::vector<std::string> ingredientAmounts_, std::vector<std::string> steps_)
{
    Node* meal = &Node(name_, description_, link_, category_, mealTags_, ingredients_, ingredientAmounts_, steps_);
    std::string hashString = meal->category;   

    int index = HashFunction(hashString, Cats, 400);
    index = checkCollision(hashString, index);

    CategoryHashTable[index].InsertMeal(meal);
    
}

void innerHashTable::InsertMeal(Node* meal)
{
    std::vector<std::string> mealTags = meal->mealTags;
    if (mealTags.size() == 0)
    {
        int index = HashFunction("No Tags", tags, maxSize);
        index = checkCollision(index, "No Tags");
        insertLL(meal, index);
    }
    
    else
    {
        for (int i = 0; i < mealTags.size(); i++)
        {
            int index = HashFunction(mealTags[i], tags, maxSize);
            index = checkCollision(index, mealTags[i]);
            insertLL(meal, index);
        }
    }

    currentSize++;
    loadFactor = currentSize / maxSize;

    exist = true;

}

// inserts into the linked list in the Meals array
// needs to be changed of gotten rid of 
void innerHashTable::insertLL(Node* meal, int index)
{
    Node* currentNode = &Meals[index];
    while(currentNode->next != nullptr)
    {
        currentNode = currentNode->next;
    }

    currentNode->next = meal;
}

void innerHashTable::rehash()
{
    Node* oldMealsArr = new Node[maxSize];
    oldMealsArr = Meals;


}

//=======================================searches=====================================//

// working on this tommorrow
std::vector<Node*> HashTable::TagSearch(std::string category, std::vector<std::string> whitelist, std::vector<std::string> blacklist)
{

}


/*
    stuff I need to work on in the morning cause brain power is depleteing cuz its late
    1. the nodes can't be a linked list need to change that create a new class that stores a node pointer vector
    2. update all of the insert functions to go with the above change
    2. rehash function
    4. TagSearch function (should be simple)
*/