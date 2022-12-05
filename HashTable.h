#pragma once
#include <string>
#include <vector>


struct Node
{
    // for all of the individual recipes
};


class HashTable
{
    // stores all of the recipes togehter
    Node* Meals[530000];   // might wanna check if this is the right way to go
    int loadFactor;
    int currentSize;

public:
    HashTable();

    void InsertMeal();   // need to know what the insert format is
    int HashFunction(std::string s);
    std::vector<Node*> TagSearch(std::vector<std::string> tags, std::vector<std::string> blacklist);

};

int HashTable::HashFunction(std::string s)   // string s is a place holder
{
    // our hash function here
}

HashTable::HashTable()
{
    loadFactor = 0;
    currentSize = 530000;
}

void HashTable::InsertMeal()
{
    Node* meal;    // need to initialize these
    std::string hashString;   // put whatever we wanna have hashed into here

    int index = HashFunction(hashString);

    Meals[index] = meal;
    
}

// not sure how we are doing this yet
std::vector<Node*> HashTable::TagSearch(std::vector<std::string> tags, std::vector<std::string> blacklist)
{

}