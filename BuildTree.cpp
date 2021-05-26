#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "MLremake.h"
using namespace std;

int main(int argc, char* argv[])
{
    srand(time(0));
    ifstream train(argv[1]);
    double EPS = atof(argv[2]);
    vector<DNode> a = ReadData(train);
    train.close();
    int total = 0;

    DTree* Final = DecisionTree(a, 0, a.size()-1, EPS);

    //cout << Final->TreeDepth() << endl;
    PrintFunc(Final);
}

