#include <iostream>
#include <vector>

#include "./control.cpp"

using namespace std;

int main()
{
    bool runProgram = true;
    vector<stock> interests = getInterests();
    pyQuery(interests);
    initUI(interests, runProgram);
    loopUI();
    
    while (runProgram){
        loop(interests);
    }
}