#include <iostream>
using namespace std;

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define SIZE 50

class stock
{
public:
    string name;
    float buyPrice;
    int holdings, startPtr, endPtr, maxSize;
    float *data;

    stock(string name_, float buyPrice_, int holdings_, int size=SIZE);
    void enqueue(float val);
    void showData();
    float getCurrentMaxval();
    float getCurrentMinval(float maxVal);
};


#endif