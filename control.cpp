#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <windows.h>

using namespace std;

class stock
{
public:
    string name;
    float buyPrice;
    int holdings;
};

vector<stock> getInterests()
{
    ifstream file("./utilfiles/interests.csv");
    string lineStr;
    vector<stock> stocks;
    int pos1;
    int pos0;

    string buyPriceStr, holdingsStr;

    // Parse Headers
    getline(file, lineStr);

    while (getline(file, lineStr)){
        
        stock row;
        pos1 = lineStr.find(",");
        row.name = lineStr.substr(0, pos1);

        pos0 = pos1;
        pos1 = lineStr.find(",", pos0+1);
        buyPriceStr = lineStr.substr(pos0+1, pos1-pos0-1);
        if (buyPriceStr.size()){
            row.buyPrice = stof(buyPriceStr);
        } else {
            row.buyPrice = 0.0f;
        }

        holdingsStr = lineStr.substr(pos1+1);
        row.holdings = stoi(holdingsStr);

        stocks.push_back(row);
    }

    return stocks;
}

void pyQuery(vector<stock> stocks){

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    string stockName;

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
    pValue = PyObject_CallFunctionObjArgs(pFunc, pArgs, NULL);

    vector<double> output;

    if (PyList_Check(pValue)){
        
        PyObject* next;
        for (int i = 0; i < PyList_Size(pValue); ++i){

            next = PyList_GetItem(pValue, i);
            output.push_back(PyFloat_AsDouble(next));
            cout << "Stock Name:" << stocks[i].name << " Price:" << output[i] << endl;
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

void loopUI(){

    if (marketOpen()){
        
    } 

    Sleep(1000);
}