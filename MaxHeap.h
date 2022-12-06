#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
using namespace std;

class MaxHeap 
{ 
 private: 
  struct Vertex
  {
    double rating;
    string name;
    string hyperlink;
    string category;
    vector<string> keywords;
    // other data from adam
  };

  map<string,vector<Vertex>> HeapMap;
  // referenced geeksforgeeks
  void swap(double *x, double *y)
  {
    double temp = *x;
    *x = *y;
    *y = temp;
  }

  void heapify(vector<Vertex> list, int root)
  {
    int largest = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    // if the left node is larger than the root, make it the root
    if (left < list.size() && list.at(left).rating > list.at(root).rating)
    {
      largest = left;
    }

    // same for right
    if (right < list.size() && list.at(right).rating > list.at(root).rating)
    {
      largest = right;
    }

    // if the largest is not the root, swap them
    if (largest != root)
    {
      // swaps the values in the list
      swap(&list.at(largest).rating, &list.at(root).rating);
      // runs heapify again at the index of the child
      heapify(list, largest);
    }
  }

 public: 
  void BuildHeap(vector<Vertex>);
  void BuildMaps(vector<Vertex>);
}; 

void MaxHeap::BuildHeap(vector<Vertex> list)
{
  // begin at the first non-leaf node
  int root = (list.size() / 2) - 1;
  for (int i = root; i >= 0; i--)
  {
    heapify(list, i);
  }
}

void MaxHeap::BuildMaps(vector<Vertex> list)
{
  for (int i = 0; i < list.size(); i++)
  {
    // create a map for every category, and have it point to a vector of recipes 
    HeapMap[list.at(i).category].push_back(list.at(i));
  }

  map<string, vector<Vertex>>::iterator it = HeapMap.begin();
  while(it != HeapMap.end())
  {
    BuildHeap(it->second);
  }
}

