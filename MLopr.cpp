#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include "MLremake.h"
using namespace std;

/*=======================DATA NODE CLASS======================*/
DNode::DNode(int& n, vector<string>& data) {
	factor = (double*)calloc(n + 1, sizeof(double));
	
	//YN setting
	if (data[0] == "-1") YN = false;
	else YN = true;

	//factor setting
    nfactor = n;
	for (int i = 1; i < (int)data.size(); i++) {
		stringstream token(data[i]);
		string ind, val;
		getline(token, ind, ':');
		//cout << ind << endl;
		getline(token, val);
		//cout << val << endl;
		factor[stoi(ind)] = stod(val);
	}
}
DNode::DNode() {
	factor = new double(0);
    nfactor = 0;
	YN = false;
}
void DNode::operator=(DNode& b) {
	factor = b.factor;
	YN = b.YN;
    nfactor = b.nfactor;
}
void DNode::Print() {
	cout << "Y/N:\t\t";
	if (YN == true) cout << "+1";
	else cout << "-1";
	cout << endl;
	for (int i = 1; i <= nfactor; i++) {
		cout << i << ":" << factor[i] << "\t";
	}
	cout << "\n\n";
}
void swap(DNode& a, DNode& b) {
	DNode tmp = a;
	a = b;
	b = tmp;
}
/*=======================DATA NODE CLASS======================*/
DTNode::DTNode() {
	thres = 0; findex = 0;
	lc = rc = nullptr;
	YN = false;
}
DTNode::DTNode(int& ind, double& thresh) {
	thres = thresh;
	findex = ind;
	lc = rc = nullptr;
	YN = false;
}
void DTNode::SetValue(int& ind, double& thresh) {
	findex = ind;
	thres = thresh;
	YN = false;
}
void DTNode::SetValue(double& ind, double& thresh) {
	findex = (int)ind;
	thres = thresh;
	YN = false;
}
void DTNode::SetRight(bool NY) {
	DTNode* DR = new DTNode;
	DR->YN = NY;
	rc = DR;
}

void DTNode::SetLeft(bool NY) {
	DTNode* DR = new DTNode;
	DR->YN = NY;
	lc = DR;
}
int DTNode::GetFindex() {
	return findex;
}
double DTNode::GetThres() {
	return thres;
}
DTNode* DTNode::GetRight() {
	return rc;
}
DTNode* DTNode::GetLeft() {
	return lc;
}
bool DTNode::GetYN() {
	return YN;
}

DTree::DTree() {
	root = nullptr;
}
void DTree::SetRoot(DTNode* DN) {
	root = DN;
}
DTNode* DTree::GetRoot() {
	return root;
}
void DTNode::SetRight(DTree* DT) {
	rc = DT->root;
}
void DTNode::SetLeft(DTree* DT) {
	lc = DT->root;
}
int DTNode::DepthCal() {
	if (this == NULL) { 
		//cout << 0 << endl; 
		return 0; 
	}
	int lcount = lc->DepthCal();
	int rcount = rc->DepthCal();
	if (lcount > rcount) {
		//cout << 1 + lcount << endl; 
		return 1 + lcount;
	}
	else { 
		//cout << 1 + rcount << endl; 
		return 1 + rcount; 
	}
}
int DTree::TreeDepth() {
	return root->DepthCal();
}

/*==========================MAIN FUNCTION=============================*/
vector<DNode> ReadData(istream& T){
    vector<vector<string>> tmp;
    string line;
	int MaxInd = 0;
	while (getline(T, line)) {
		tmp.push_back(split(line, ' '));
		stringstream tarp(tmp.back().back());
		string test;
		getline(tarp, test, ':');
		if (stoi(test) > MaxInd) MaxInd = stoi(test);
	}
	//printf("MaxIndex:\t\t%d\n", MaxInd);

	vector<DNode> HData;
	for (int i = 0; i < (int)tmp.size(); i++) {
		DNode a(MaxInd, tmp[i]);
		HData.push_back(a);
	}
	return HData;
}

void maxHeapify(vector<DNode>& tar, int head, int tail, int current, const int findex) {
	if ((current - head) * 2 + 1 + head > tail) return;
	else if ((current - head) * 2 + 2 + head > tail) {
		if (tar[current].factor[findex] < tar[(current - head) * 2 + 1 + head].factor[findex]) {
			swap(tar[current], tar[(current - head) * 2 + 1 + head]);
			maxHeapify(tar, head, tail, (current - head) * 2 + 1 + head, findex);
		}
		else return;
	}
	else {
		if (tar[current].factor[findex] > tar[(current - head) * 2 + 1 + head].factor[findex] \
		 && tar[current].factor[findex] > tar[(current - head) * 2 + 2 + head].factor[findex]) {
			return;
		}
		else if (tar[current].factor[findex] > tar[(current - head) * 2 + 1 + head].factor[findex] \
			   > tar[current].factor[findex] > tar[(current - head) * 2 + 2 + head].factor[findex]) {
			swap(tar[current], tar[(current - head) * 2 + 1 + head]);
			maxHeapify(tar, head, tail, (current - head) * 2 + 1 + head, findex);
		}
		else {
			swap(tar[current], tar[(current - head) * 2 + 2 + head]);
			maxHeapify(tar, head, tail, (current - head) * 2 + 2 + head, findex);
		}
	}
}

void heapSort(vector<DNode>& tar, int head, int tail, int findex) {
	// build heap
	for (int i = tail; i >= head; i--) {
		maxHeapify(tar, head, tail, i, findex);
	}
	// heap sort
	for (int i = tail; i > head; i--) {
		swap(tar[head], tar[i]);
		maxHeapify(tar, head, --tail, head, findex);
	}
}

DTree* DecisionTree(vector<DNode>& tar, int head, int tail, double EPS) {
	//PrintStar(20);
	double* ans = MinConfusionCal(tar, head, tail);
	//double* ans = MaxConfusionCal(tar, head, tail);
	//double* ans = RandomConfusionCal(tar, head, tail);
	// 0: min_findex     1: min_index
	// 2: threshold      3: confuse

	DTNode* DN = new DTNode; DN->SetValue(ans[0], ans[2]);
	DTree* DT = new DTree; DT->SetRoot(DN);

	//if reach EPS
	if (ans[3] <= EPS || ans[0] == 0) {
		//printf("================REACH EPS AT FINDEX #%d==================\n", (int)ans[0]);
		int c = 0, e = 0;
		for (int i = head; i <= ans[1]; i++) {
			if (tar[i].YN) c++;
		}
		for (int i = (int)ans[1] + 1; i <= tail; i++) {
			if (tar[i].YN) e++;
		}
		if (c > (ans[1] - head + 1) / 2) {
			//printf("=======LEFT YES=======\n");
			DN->SetLeft(true);
		}
		else {
			//printf("=======LEFT NO=======\n");
			DN->SetLeft(false);
		}
		if (e > (tail - ans[1]) / 2) {
			//printf("=======RIGHT YES=======\n");
			DN->SetRight(true);
		}
		else {
			//printf("=======RIGHT NO=======\n");
			DN->SetRight(false);
		}
		//PrintStar(20);
		return DT;
	}

	//left, right child determine (recursive)
	if (SameYN(tar, head, ans[1])) {
		//cout << "=========LEFT REACH LEAF";
		//if (tar[head].YN) cout << " YES===========\n";
		//else cout << " NO===========\n";
		DN->SetLeft(tar[head].YN);
	}
	else {
		DN->SetLeft(DecisionTree(tar, head, ans[1], EPS));
	}
	if (SameYN(tar, ans[1] + 1, tail)) {
		//cout << "=========RIGHT REACH LEAF";
		//if (tar[tail].YN) cout << " YES===========\n";
		//else cout << " NO===========\n";
		DN->SetRight(tar[tail].YN);
	}
	else {
		DN->SetRight(DecisionTree(tar, ans[1] + 1, tail, EPS));
	}

	return DT;


}

double* MinConfusionCal(vector<DNode>& tar, int head, int tail) {
	
	int minfindex = 0;
	int minindex = 0;
	double minthres = 0;
	double minconfuse = 1E5;

	for (int findex = 1; findex <= tar[0].nfactor; findex++) {
		//cout << "DEALING WITH: " << findex << endl;
		//if all elements have same value, no need to use;
		if (SameFactor(tar, head, tail, findex)) {
			//printf("=================ALL ELEMENTS SAME AT FINDEX #%d=================\n", findex);
			continue;
		}
		heapSort(tar, head, tail, findex);
		//calculation begin
		int mindex = 0;
		double mthres = 0;
		double mconfuse = 0;
		
		int scount = 0;					//same confuse count
		int vcount = 0;					//valid confuse count
		//int minindex = head;

		for (int i = head; i < tail; i++) {
			//neglect two nodes with identical factor value
			if (tar[i].factor[findex] == tar[i + 1].factor[findex]) {
				//printf("#%d, #%d: SAME FACTOR VALUE ((%f)) AT FINDEX %d\n", i, i + 1, tar[i].factor[findex], findex);
				continue;
			}
			double thres = (tar[i].factor[findex] + tar[i + 1].factor[findex]) / 2;
			int c = 0, d = 0, e = 0, f = 0;

			//calculate confusion
			for (int k = head; k <= i; k++) {
				if (tar[k].YN) c++;
				else d++;
			}
			for (int k = i + 1; k <= tail; k++) {
				if (tar[k].YN) e++;
				else f++;
			}
			double confuse = 1.0 / ((double)tail - (double)head + 1.0) * 2.0 * (double)min(c, d) + 1.0 / ((double)tail - (double)head + 1.0) * 2.0 * (double)min(e, f);

			//printf("Confuse: #%d, c: %d, d: %d, e: %d, f: %d, thres: %f, confuse: %f\n", i, c, d, e, f, thres, confuse);

			if (vcount == 0) {						//first confuse to be calculated
				//printf("FIRST IN #%d\n", findex);
				mconfuse = confuse;
				mthres = thres;
				mindex = i;
			}
			else {
				if (mconfuse > confuse) {
					//printf("reach smaller\n");
					mconfuse = confuse;
					mthres = thres;
					mindex = i;
				}
				else if (mconfuse == confuse) {
					scount++;
				}
			}
			vcount++;
		}
		//printf("findex: %f, mconfuse: %f, mthres: %f\n", (double)findex, mconfuse, mthres);
		if (scount == vcount - 1 && vcount > 1) {
			//"cout << no use\n"; 
			continue;
		}
		if (findex == 1 || minconfuse > mconfuse) {
			//printf("SAMLLER FINDEX AT #%d\n", findex);
			minfindex = findex;
			minindex = mindex;
			minconfuse = mconfuse;
			minthres = mthres;
		}
	}
	heapSort(tar, head, tail, minfindex);
	double* ans = new double[4];
	// 0: min_findex     1: min_index
	// 2: threshold      3: confuse
	//printf("minfindex: %f, minindex: %f, minthres: %f, minconfuse: %f\n", (double)minfindex, (double)minindex, minthres, minconfuse);
	ans[0] = minfindex; 
	ans[1] = minindex;
	ans[2] = minthres;
	ans[3] = minconfuse;

	return ans;
}
double* MaxConfusionCal(vector<DNode>& tar, int head, int tail) {

	int maxfindex = 0;
	int maxindex = 0;
	double maxthres = 0;
	double maxconfuse = 0;

	for (int findex = 1; findex <= tar[0].nfactor; findex++) {
		//cout << "DEALING WITH: " << findex << endl;
		//if all elements have same value, no need to use;
		if (SameFactor(tar, head, tail, findex)) {
			//printf("=================ALL ELEMENTS SAME AT FINDEX #%d=================\n", findex);
			continue;
		}
		heapSort(tar, head, tail, findex);

		//calculation begin
		int mindex = 0;
		double mthres = 0;
		double mconfuse = 0;

		int scount = 0;					//same confuse count
		int vcount = 0;					//valid confuse count

		for (int i = head; i < tail; i++) {
			//neglect two nodes with identical factor value
			if (tar[i].factor[findex] == tar[i + 1].factor[findex]) {
				//printf("#%d, #%d: SAME FACTOR VALUE ((%f)) AT FINDEX %d\n", i, i + 1, tar[i].factor[findex], findex);
				continue;
			}
			double thres = (tar[i].factor[findex] + tar[i + 1].factor[findex]) / 2;
			int c = 0, d = 0, e = 0, f = 0;

			//calculate confusion
			for (int k = head; k <= i; k++) {
				if (tar[k].YN) c++;
				else d++;
			}
			for (int k = i + 1; k <= tail; k++) {
				if (tar[k].YN) e++;
				else f++;
			}
			double confuse = 1.0 / ((double)tail - (double)head + 1.0) * 2.0 * (double)min(c, d) + 1.0 / ((double)tail - (double)head + 1.0) * 2.0 * (double)min(e, f);

			//printf("Confuse: #%d, c: %d, d: %d, e: %d, f: %d, thres: %f, confuse: %f\n", i, c, d, e, f, thres, confuse);

			if (vcount == 0) {						//first confuse to be calculated
				//printf("FIRST IN #%d\n", findex);
				mconfuse = confuse;
				mthres = thres;
				mindex = i;
			}
			else {
				if (mconfuse < confuse) {
					//printf("reach smaller\n");
					mconfuse = confuse;
					mthres = thres;
					mindex = i;
				}
				else if (mconfuse == confuse) {
					scount++;
				}
			}
			vcount++;
		}
		//printf("findex: %d, index: %d, mconfuse: %f, mthres: %f\n", findex, mindex, mconfuse, mthres);
		if (scount == vcount - 1 && vcount > 1) {
			//cout << "no use\n"; 
			continue;
		}
		if (findex == 1 || maxconfuse < mconfuse) {
			//printf("SAMLLER FINDEX AT #%d\n", findex);
			maxfindex = findex;
			maxindex = mindex;
			maxconfuse = mconfuse;
			maxthres = mthres;
		}
	}
	heapSort(tar, head, tail, maxfindex);
	double* ans = new double[4];
	// 0: min_findex     1: min_index
	// 2: threshold      3: confuse
	//printf("maxfindex: %f, maxindex: %f, maxthres: %f, maxconfuse: %f\n", (double)maxfindex, (double)maxindex, maxthres, maxconfuse);
	ans[0] = maxfindex;
	ans[1] = maxindex;
	ans[2] = maxthres;
	ans[3] = maxconfuse;

	return ans;
}
double* RandomConfusionCal(vector<DNode>& tar, int head, int tail) {
	//printf("head: %d, tail: %d\n", head, tail);
	int findex;
	while (true) {
		findex = rand() % (tar[0].nfactor) + 1;
		if (!SameFactor(tar, head, tail, findex)) break;
		
	}
	heapSort(tar, head, tail, findex);
	
	int index = (head + tail) / 2;
	if (SameFactor(tar, index, tail, findex)) {
		while (tar[index].factor[findex] == tar[index - 1].factor[findex]) index--;
	}
	else {
		while (tar[index].factor[findex] == tar[index + 1].factor[findex]) index++;
	}

	double thres = (tar[index].factor[findex] + tar[index + 1].factor[findex]) / 2;
	int c = 0, d = 0, e = 0, f = 0;

	//calculate confusion
	for (int k = head; k <= index; k++) {
		if (tar[k].YN) c++;
		else d++;
	}
	for (int k = index + 1; k <= tail; k++) {
		if (tar[k].YN) e++;
		else f++;
	}
	double confuse = 1.0 / ((double)tail - (double)head + 1.0) * 2.0 * (double)min(c, d) + 1.0 / ((double)tail - (double)head + 1.0) * 2.0 * (double)min(e, f);

	double* ans = new double[4];
	ans[0] = findex;
	ans[1] = index;
	ans[2] = thres;
	ans[3] = confuse;
	return ans;
}
void PrintFunc(DTree* TREE) {
	printf("int tree_predict(double* attr) {\n");
	PrintTree(TREE->GetRoot(), 1);
	printf("}\n");
}
void PrintTree(DTNode* itr, int n) {
	if (itr->GetLeft() == nullptr && itr->GetRight() == nullptr) {
		PrintSpace(n);
		if (itr->GetYN()) cout << "return +1;\n";
		else cout << "return -1;\n";
		return;
	}
	PrintSpace(n);
	printf("if (attr[%d] < %f) {\n", itr->GetFindex(), itr->GetThres());
	if (itr->GetLeft() != nullptr) {
		PrintTree(itr->GetLeft(), n + 1);
	}
	PrintSpace(n);
	printf("}\n");
	PrintSpace(n);
	printf("else {\n");
	if (itr->GetRight() != nullptr) {
		PrintTree(itr->GetRight(), n + 1);
	}
	PrintSpace(n);
	printf("}\n");
}
void HeapPrint(vector<DNode>& tar, const int ind) {
	for (int i = 0; i < tar.size(); i++) {
		cout << "NODE#:\t\t" << i << endl;
		cout << "key:\t\t" << tar[i].factor[ind] << endl;
		cout << endl;
	}
}
bool SameFactor(vector<DNode>& tar, int head, int tail, int findex) {
	double c = tar[head].factor[findex];
	int count = 1;
	for (int i = head + 1; i <= tail; i++) {
		if (c == tar[i].factor[findex]) count++;
	}
	if (count == tail - head + 1) return true;
	else return false;
}
bool SameYN(vector<DNode>& tar, int head, int tail) {
	int count = 1;
	for (int i = head + 1; i <= tail; i++) {
		if (tar[head].YN == tar[i].YN) count++;
	}
	if (count == tail - head + 1) return true;
	else return false;
}

vector<string> split(const string& tar, const char& delim) {
	vector<string> ans;
	if (tar == "") { return ans; }
	stringstream tarp(tar);
	string token;
	while (getline(tarp, token, delim)) {
		ans.push_back(token);
	}
	return ans;
}

void PrintStar(int num) {
	for (int i = 0; i < num; i++) cout << "*";
	cout << endl;
}

void PrintSpace(int num) {
	for (int i = 0; i < num * 4; i++) {
		printf(" ");
	}
}