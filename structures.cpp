#include <iostream>
#include "./structures.hpp"

using namespace std;

stock::stock(string name_, float buyPrice_, int holdings_, int size_){
        name = name_;
        buyPrice = buyPrice_;
        holdings = holdings_;
        maxSize = size_;

        startPtr = 0;
        endPtr = -1;
        data = new float[maxSize];
}

void stock::enqueue(float val){
    endPtr = (endPtr + 1) % maxSize;
    data[endPtr] = val;

    if (endPtr == startPtr) startPtr++;
}

float stock::getCurrentMaxval(){
    float maxVal = 0;
    for (int i = 0; i < maxSize; ++i) {
        if (data[i] > maxVal) maxVal = data[i];
    }
    return maxVal;
}

float stock::getCurrentMinval(float maxVal){
    float minVal = maxVal;
    for (int i = 0; i < maxSize; ++i) {
        if (data[i] < minVal) minVal = data[i];
    }
    return minVal;
}