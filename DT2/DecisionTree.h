#pragma once
#include<vector>
#include<cmath>
using namespace std;

//训练用数据
class TrainData{
public:
    vector<vector<int>> Input;//一行一条数据
    vector<int> OutPut;//允许离散的输出值
    void InSertData(const vector<int>& data,int out){//输入一行数据和目标输出
        Input.push_back(data);
        OutPut.push_back(out);
    }
};

class Node{
public:
    int Attribute;//属性序号
    bool IsLeaf;//是否是叶节点
    vector<Node*> Num;//子女节点
    Node(int ID,bool a):Attribute(ID),IsLeaf(a){}

};

//决策树
class Tree{
private:
    Node* Root;//根节点

    vector<vector<int> > AttrData;//属性列表

    Node* CreateTree(const TrainData& data,vector<int> usedAttr);//ID3算法生成树
    int MostNormalOutPut(const TrainData& data);//将最普遍的输出作为节点值
    int Best(const TrainData& data,const vector<int>& usedAttr);//计算信息增益最高的属性
    double Entropy(const TrainData& data);//计算信息熵
public:
    Tree();
    void GetOutPut();//输入一个案例，获得输出
};