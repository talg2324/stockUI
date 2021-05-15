#include <iostream>
#include <vector>

#include "./control.cpp"
#include "./gui.cpp"

using namespace std;

int main()
{
    //vector<stock> interests = getInterests();
    //pyQuery(interests);
    initUI();
    while (true){
        loopUI();
    }
}