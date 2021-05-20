/*===================MACHINE LEARNING=====================*/
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <stdlib.h>
#include <time.h>
using namespace std;

/*==================USEFUL TOOL============================*/
vector<string> split(const string&, const char&);
void PrintStar(int num);
void PrintSpace(int num);

/*=======================DATA NODE CLASS======================*/
class DNode {
public:
	DNode(int& n, vector<string>& data);
	DNode();
	void operator=(DNode& b);
	void Print();

    int nfactor;
	double* factor;
	bool YN;
};

/*=======================DATA NODE CLASS======================*/
class DTree;
class DTNode {
private:
	double thres;
	int findex;
	bool YN;
	DTNode* lc;
	DTNode* rc;
public:
	DTNode();
	DTNode(int& ind, double& thresh);
	void SetValue(int& ind, double& thresh);
	void SetValue(double& ind, double& thresh);
	void SetRight(bool NY);
	void SetRight(DTree*);
	void SetLeft(bool NY);
	void SetLeft(DTree*);
	int GetFindex();
	int DepthCal();
	double GetThres();
	bool GetYN();
	DTNode* GetRight();
	DTNode* GetLeft();
	friend class DTree;
};

class DTree {
private:
	DTNode* root;
public:
	DTree();
	void SetRoot(DTNode*);
	int TreeDepth();
	DTNode* GetRoot();
	friend class DTNode;
};

/*==========================MAIN FUNCTION=============================*/
vector<DNode> ReadData(istream&);
void HeapSort(vector<DNode>& tar, int head, int tail, const int findex);
void SortHeapify(vector<DNode>& tar, int head, int current, int lim, int findex);
double* ConfusionCal(vector<DNode>& tar, int head, int tail, int findex);
double* MinConfusionCal(vector<DNode>& tar, int head, int tail);
double* MaxConfusionCal(vector<DNode>& tar, int head, int tail);
double* RandomConfusionCal(vector<DNode>& tar, int head, int tail);
DTree* DecisionTree(vector<DNode>& tar, int head, int tail, double EPS);
void PrintFunc(DTree* TREE);
void PrintTree(DTNode*, int);
bool SameFactor(vector<DNode>& tar, int head, int tail, int findex);
bool SameYN(vector<DNode>& tar, int head, int tail);

