#include "DecisionTreeNode.h"
#include <float.h>
#include <iostream>
using namespace std;


DecisionTreeNode::DecisionTreeNode(DataSet &trainingData, unsigned int maxBucketSize){
	this->trainingData = &trainingData;

	this->left = 0;
	this->right = 0;
	this-> leaf = false;
	this->classification = 0.0;
	this->variable = 9999999;
	this->variableValue = 0.0;
//	cout<<"NODE: "<<trainingData.size()<<endl;
	if(trainingData.size() <= maxBucketSize  || trainingData.allSignalOrAllBackground()){
		this->leaf = true;
		double signalFraction = trainingData.weightedCountS() / trainingData.weightedCount();
		this->classification = (signalFraction >= .5) ? 1.0 : -1.0;
		return;
	}

	unsigned int variableIndex;
	splitData(this->variable, variableIndex,this->variableValue);
//	cout<<"NODE: variableIndex: "<<variableIndex<<endl;
	DataSet left = trainingData.subset(0,variableIndex);
	DataSet right = trainingData.subset(variableIndex+1,trainingData.size()-1);
	//debug
	//cout<<"Splitting variable: "<<variableIndex<<" splitting value: "<<variableValue<<endl;

	this->left = new DecisionTreeNode(left,maxBucketSize);
	this->right = new DecisionTreeNode(right,maxBucketSize);

}
void DecisionTreeNode::chooseSplittingValue(unsigned int variable, unsigned int &bestIndex, double &bestGini){

	this->trainingData->sort(variable);
	double Ws_left = (*trainingData)[0].signal ? (*trainingData)[0].weight : 0.0;
	double Ws_right = trainingData->weightedCountS(1);
	double W_left = (*trainingData)[0].weight;
	double W_right = trainingData->weightedCount(1);

	double purity_right = Ws_right / W_right;
	double purity_left = Ws_left / W_left;
	double gini_right = W_right * purity_right * (1.0 - purity_right);
	double gini_left = W_left * purity_left * (1.0 - purity_left);
	double gini = gini_left + gini_right;

	bestIndex = 0;
	bestGini = gini;
	//debug code
//	cout<<"Choosing best splitting val: ";
//	for(unsigned int i = 0; i < trainingData->size(); i++)
//		cout<<(*trainingData)[i].variables[variable]<<" ";
//	cout<<endl;
	for(unsigned int i = 1; i < trainingData->size() - 1; i++){
		Ws_left += (*trainingData)[i].signal ? (*trainingData)[i].weight : 0.0;
		W_left += (*trainingData)[i].weight;
		Ws_right -= (*trainingData)[i].signal ? (*trainingData)[i].weight : 0.0;
		W_right -= (*trainingData)[i].weight;
		purity_right = Ws_right / W_right;
		purity_left = Ws_left / W_left;
		gini_right = W_right * purity_right * (1.0 - purity_right);
		gini_left = W_left * purity_left * (1.0 - purity_left);
		gini = gini_left + gini_right;

		if (bestGini >= gini) {
//			cout<<"gini: "<<gini<<endl;
			bestGini = gini;
			bestIndex = i;

		}

	}

}

void DecisionTreeNode::splitData(unsigned int& variable, unsigned int& variableIndex, double& variableValue){

	unsigned int nVars = trainingData->numVariables();
	double bestGini = DBL_MAX;
	variableIndex = -1;
	variable = -1;

	for(unsigned int i = 0; i < nVars; i++){
		double gini;
		unsigned int index;
		chooseSplittingValue(i,index,gini);
		if(gini < bestGini){
			bestGini = gini;
			variableIndex= index;
			variable = i;
			variableValue = (*trainingData)[variableIndex].variables[variable];
//			cout<<"variable: "<<variable<<endl;
//			cout<<"value: "<<variableValue<<endl;
		}
	}
	this->trainingData->sort(variable);
	variableValue = (*trainingData)[variableIndex].variables[variable];

}

double DecisionTreeNode::getClassification(DataSet::Event& event){

	if(this->leaf)
		return classification;
	return event.variables[this->variable] <= this->variableValue ? left->getClassification(event) : right->getClassification(event);

}
double DecisionTreeNode::getClassification(DataSet::TrainingEvent& event){

	if(this->leaf)
		return classification;
	return event.variables[this->variable] <= this->variableValue ? left->getClassification(event) : right->getClassification(event);

}


ostream& operator<<(ostream& os, const DecisionTreeNode& dt) {

	os << dt.variable <<" "<< dt.variableValue <<" "<< dt.leaf << " "<<dt.classification<<" ";
	if( !dt.leaf )
			os<< *dt.left <<" " <<*dt.right<<" ";

	return os;

}
istream& operator>>(istream& is, DecisionTreeNode& dt) {

	is >> dt.variable;
	is >> dt.variableValue;
	is >> dt.leaf;
	is >> dt.classification;


	if( !dt.leaf ){
		dt.left = new DecisionTreeNode();
		is >> *dt.left;

		dt.right = new DecisionTreeNode();
		is >> *dt.right;
	}

	return is;

}
