#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

struct Recipe
  {
    double rating;
    string name;
    string hyperlink;
    string category;
    vector<string> keywords;
    // other data from adam
  };

class MaxHeap 
{ 
 private: 
  map<string,vector<Recipe>> HeapMap;
  // referenced geeksforgeeks
  void heapify(string category, int root)
  {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    // if the left node is larger than the root, make it the root
    if (left < this->HeapMap[category].size() && this->HeapMap[category].at(left).rating > this->HeapMap[category].at(largest).rating)
    {
      largest = left;
    }

    // same for right
    if (right < this->HeapMap[category].size() && this->HeapMap[category].at(right).rating > this->HeapMap[category].at(largest).rating)
    {
      largest = right;
    }

    // if the largest is not the root, swap them
    if (largest != root)
    {
      // swaps the values in the list
      Recipe temp = this->HeapMap[category].at(largest);
      this->HeapMap[category].at(largest) = this->HeapMap[category].at(root);
      this->HeapMap[category].at(root) = temp;

      // runs heapify again at the index of the child
      heapify(category, largest);
    }
  }

 public: 
  void BuildMaps(vector<Recipe>);
  set<Recipe> GetRecipes(bool mustMatchWL, int numRecipes, int minRating, vector<string> blacklist, vector<string> whitelist, string category);
  vector<string> GetCategories();
}; 

void MaxHeap::BuildMaps(vector<Recipe> list)
{
  for (int i = 0; i < list.size(); i++)
  {
    // create a map for every category, and have it point to a vector of recipes 
    HeapMap[list.at(i).category].push_back(list.at(i));
  }

  // for every category map
  for(const auto &pair : HeapMap)
  {
    // Heapify its vector list
    int root = (pair.second.size() / 2) - 1;
    for (int i = root; i >= 0; i--)
    {
      heapify(pair.first, i);
    }
  }
}

set<Recipe> MaxHeap::GetRecipes(bool mustMatchWL, int numRecipes, int minRating, vector<string> blacklist, vector<string> whitelist, string category)
{
  set<Recipe> result;
  // go through the entire list 
  for (int i = 0; i < this->HeapMap[category].size(); i++)
  {
    Recipe currLoc = this->HeapMap[category].at(i);
    bool validRecipe = true;
    if (currLoc.rating < minRating)
    {
      validRecipe = false;
    }
    if (validRecipe)
    {
      int numOfKeys = 0;
      // go through keyword vector of recipe
      for(int j = 0; j < currLoc.keywords.size(); j++)
      {
        if (!validRecipe)
        {
          break;
        }

        for (int k = 0; k < blacklist.size(); k++)
        {
          // if recipe contains blacklisted keyword, it is invalid
          if (currLoc.keywords.at(j) == blacklist.at(k))
          {
            validRecipe = false;
          }
        }

        for (int k = 0; k < whitelist.size(); k++)
        {
          // if recipe has a keyword from whitelist, add to counter
          if (currLoc.keywords.at(j) == whitelist.at(k))
          {
            numOfKeys++;
          }
        }
      }
      // if recipe must contain all keywords in whitelist, mark as invalid if it does not
      if (mustMatchWL && numOfKeys != whitelist.size())
      {
        validRecipe = false;
      }
      // if it does not have to have all the tags, check it has at least one
      else if (numOfKeys == 0)
      {
        validRecipe = false;
      }
      // if it is valid, add it to result set
      if (validRecipe)
      {
        result.insert(currLoc);
      }
    }
    // if the result is full, return
    if (result.size() == numRecipes)
    {
      return result;
    }
  }
  cout << "could not find n recipes!" << endl;
  return result;
}

vector<string> MaxHeap::GetCategories()
{
  vector<string> result;
  for(const auto &pair : this->HeapMap)
  {
    result.push_back(pair.first);
  }
  return result;
}