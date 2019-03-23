#include "pch.h"
#include "DecisionTree.h"
#include <iostream>
using namespace std;
Tree::Tree() {
	/*���������б�����ÿ�����Եķ������Ը�������*/
	int stop = 0, num = 0;
	while (!stop) {
		vector<int> temp;
		cout << "Attribute" << "[" << num << "]" << ":";
		int aa;
		cin >> aa;
		for (int i = 0; i < aa; i++)
			temp.push_back(i);
		AttrData.push_back(temp);
		cout << "Stop?" << endl;
		cin >> stop;
		num++;
	}

	/*����ѵ�����ݣ�ֱ�Ӱ�˳����������������*/
	TrainData data;
	stop = 0;
	while (!stop) {
		vector<int> train;
		cout << "TrainData:";
		int aa = 0;
		for (unsigned int i = 0; i < AttrData.size(); i++) {
			cin >> aa;
			train.push_back(aa);
		}
		cout << "OutPut:";
		bool aaa;
		cin >> aaa;
		data.InSertData(train, aaa);
		cout << "Stop?" << endl;
		cin >> stop;
	}

	vector<int> temp2;
	Root = CreateTree(data, temp2);
	cout << "Training........." << endl;
}


Node* Tree::CreateTree(const TrainData& data, vector<int> usedAttr) {

	Node* root = new Node(0, 0);//�������ڵ�

	/*��������һ�����򴴽�һ���ڵ㣬ֵΪ���������ΪҶ�ڵ�*/
	int stop = 1;
	for (unsigned int i = 1; i < data.OutPut.size(); i++) {
		if (data.OutPut[i] != data.OutPut[i - 1])
			stop = 0;
	}
	if (stop)
		return new Node(data.OutPut[0], 1);

	/*����������Զ��ù�����ô���صĽڵ��ֵΪ���ձ���������ΪҶ�ڵ�*/
	if (usedAttr.size() == AttrData.size())
		return new Node(MostNormalOutPut(data), 1);

	/*ѡ����Ϣ������ߵ�������Ϊ�ڵ�*/
	int A = Best(data, usedAttr);
	usedAttr.push_back(A);//������ʹ������
	root->Attribute = A;

	/*�ݹ����ÿһ�������������½�һ����*/
	for (unsigned int i = 0; i < AttrData[A].size(); i++) {
		TrainData tempExample;
		for (unsigned int j = 0; j < data.OutPut.size(); j++) {
			if (i == data.Input[j][A]) {
				tempExample.InSertData(data.Input[j], data.OutPut[j]);
			}
		}
		if (tempExample.OutPut.empty()) {
			root->Num.push_back(new Node(MostNormalOutPut(data), 1));
		}
		else {
			root->Num.push_back(CreateTree(tempExample, usedAttr));
		}
	}

	return root;
}

int Tree::MostNormalOutPut(const TrainData& data) {
	vector<int> out;//��¼���������
	vector<int> count;//��¼���������
	for (unsigned int i = 0; i < data.OutPut.size(); i++) {
		bool ex = 0;
		int index = 0;
		for (unsigned int j = 0; j < out.size(); j++) {
			if (out[j] == data.OutPut[i]) {
				ex = 1;
				index = j;
			}
		}
		if (ex) {
			count[index]++;
		}
		else {
			out.push_back(data.OutPut[i]);
			count.push_back(1);
		}
	}
	/*����������������*/
	int maxi = 0;
	int max = 0;
	for (unsigned int i = 0; i < count.size(); i++) {
		if (count[i] > max) {
			maxi = i;
			max = count[i];
		}
	}
	return out[maxi];
}

double Tree::Entropy(const TrainData& data) {
	/*����������������*/
	vector<double> out;
	vector<double> count;
	for (unsigned int i = 0; i < data.OutPut.size(); i++) {
		bool ex = 0;
		int index = 0;
		for (unsigned int j = 0; j < out.size(); j++) {
			if (out[j] == data.OutPut[i]) {
				ex = 1;
				index = j;
			}
		}
		if (ex) {
			count[index]++;
		}
		else {
			out.push_back(data.OutPut[i]);
			count.push_back(1);
		}
	}
	/*������Ϣ��*/
	double total = 0;
	for (unsigned int i = 0; i < count.size(); i++)
		total += count[i];
	double sum = 0;
	for (unsigned int i = 0; i < count.size(); i++) {
		double a = 0;
		if ((count[i] / total) != 0)
			a = log2((count[i] / total));
		sum -= (count[i] / total)*a;
	}
	return sum;
}

int Tree::Best(const TrainData& data, const vector<int>& usedAttr) {
	vector<double>  Gain;//��¼ÿһ�����Ե���Ϣ����

	bool used;
	/*��ʹ�ù������Ե���Ϣ��������Ϊ0*/
	for (unsigned int i = 0; i < AttrData.size(); i++) {
		used = 0;
		for (unsigned int k = 0; k < usedAttr.size(); k++)
			if (i == usedAttr[k]) {
				Gain.push_back(0.0);
				used = 1;
			}
		if (used)
			continue;
		/*������Ϣ����*/
		else {
			double es = Entropy(data);
			for (unsigned int j = 0; j < AttrData[i].size(); j++) {
				TrainData tempData;
				for (unsigned int k = 0; k < data.Input.size(); k++) {
					if (j == data.Input[k][i]) {
						tempData.InSertData(data.Input[k], data.OutPut[k]);
					}
				}
				if (!tempData.Input.empty()) {
					es -= (double(tempData.Input.size()) / double(data.Input.size()))*Entropy(tempData);
				}
			}
			Gain.push_back(es);
		}
	}

	/*������Ϣ������ߵ�����*/
	int maxi = 0;
	double max = 0;
	for (unsigned int i = 0; i < Gain.size(); i++) {
		if (Gain[i] > max) {
			maxi = i;
			max = Gain[i];
		}
	}
	return maxi;
}

void Tree::GetOutPut() {
	vector<int> data;
	cout << "TestData:";
	int aa = 0;
	for (int i = 0; i < AttrData.size(); i++) {
		cin >> aa;
		data.push_back(aa);
	}
	if (Root->IsLeaf) {
		cout << "OutPut:" << Root->Attribute << endl;
		return;
	}
	Node* current = Root->Num[data[Root->Attribute]];
	while (!current->IsLeaf)
		current = current->Num[data[current->Attribute]];
	cout << "OutPut:" << current->Attribute << endl;
}
