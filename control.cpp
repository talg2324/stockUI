#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <windows.h>
#include "./gui.cpp"

using namespace std;

vector<stock> getInterests()
{
    ifstream file("./utilfiles/interests.csv");
    string lineStr;
    vector<stock> stocks;
    int pos1, pos0;

    string nameStr, buyPriceStr, holdingsStr;
    float buyPrice;
    int holdings;

    // Parse Headers
    getline(file, lineStr);

    while (getline(file, lineStr)){
        
        pos1 = lineStr.find(",");
        nameStr = lineStr.substr(0, pos1);

        pos0 = pos1;
        pos1 = lineStr.find(",", pos0+1);
        buyPriceStr = lineStr.substr(pos0+1, pos1-pos0-1);

        if (buyPriceStr.size()){
            buyPrice = stof(buyPriceStr);
        } else {
            buyPrice = 0.0f;
        }

        holdingsStr = lineStr.substr(pos1+1);
        holdings = stoi(holdingsStr);

        stock row = stock(nameStr, buyPrice, holdings);
        stocks.push_back(row);
    }

    return stocks;
}

void pyQuery(vector<stock>& stocks){

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    string stockName;
    double queueVal;

    Py_SetPath(L"C:\\Users\\TalG\\Miniconda3\\Lib");
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append(os.getcwd())");

    pName = PyUnicode_FromString("scrape_prices");
    pArgs = PyList_New(stocks.size());

    for (int i=0; i < stocks.size(); ++i){
        stockName = stocks[i].name;

        char* ptr = &stockName[0];
        pValue = PyUnicode_FromString(ptr);
        PyList_SetItem(pArgs, i, pValue);
    }

    pModule = PyImport_Import(pName);
    Py_DecRef(pName);

    pFunc = PyObject_GetAttrString(pModule, "query_price");
    //pFunc = PyObject_GetAttrString(pModule, "debug_query_price");
    pValue = PyObject_CallFunctionObjArgs(pFunc, pArgs, NULL);

    if (PyList_Check(pValue)){
        
        PyObject* next;
        for (int i = 0; i < PyList_Size(pValue); ++i){
            next = PyList_GetItem(pValue, i);
            queueVal = PyFloat_AsDouble(next);
            stocks[i].enqueue((float) queueVal);
        }
    }

    Py_DecRef(pValue);
    Py_DecRef(pFunc);
    Py_DecRef(pModule);
}

bool marketOpen(){

    time_t now = time(0);
    struct tm *tmp = gmtime(&now);

    // UTC Time vs EST Time
    int offset = 4;
    int hr = tmp->tm_hour - offset;
    bool isMarketOpen = false;

    if ((tmp->tm_wday < 5) && (hr > 9) && (hr < 18)){
        isMarketOpen = true;
    }

    //return isMarketOpen;
    return true;
}

void loop(vector<stock>& stocks){

    if (marketOpen()){
        pyQuery(stocks);
        loopUI();
    } 
}