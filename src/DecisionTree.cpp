#include "DecisionTree.h"

#include <cmath>

using namespace std;

DecisionTree::DecisionTree(DataSet *trainingData) {

	this->err = -1;
	this->alph = -1;

	trainingData->resetIndices();
	this->trainingData = trainingData;
	trainingData->normalizeWeights();
	this->head = new DecisionTreeNode(*trainingData, 20);

	if (error() == 0)
		this->alph = 1;
	else
		this->alph = .5 * log((1.0 - error()) / error());

}

double DecisionTree::alpha() {

	return this->alph;

}
double DecisionTree::error() {

	if (this->err != -1)
		return this->err;

	double total = 0.0;
	double wrong = 0.0;
	for (unsigned int i = 0; i < trainingData->size(); i++) {
		total += (*trainingData)[i].weight;
		if (((*trainingData)[i].signal && classify(((*trainingData)[i])) == -1.0)
				|| (!(*trainingData)[i].signal && classify(((*trainingData)[i])) == 1.0))
			wrong += (*trainingData)[i].weight;
	}

	this->err = wrong / total;
	return this->err;

}
bool DecisionTree::check(DataSet::TrainingEvent &event){
	return (event.signal && (classify(event) == 1.0)) || (!event.signal && (classify(event) == -1.0));
}
double DecisionTree::classify(DataSet::Event &event){

	return head->getClassification(event);
}
double DecisionTree::classify(DataSet::TrainingEvent &event){

	return head->getClassification(event);
}


ostream& operator<<(ostream& os, const DecisionTree& dt){

	os<<dt.err<<" "<<dt.alph<<" "<<(*dt.head)<<" ";

	return os;

}
istream& operator>>(istream& is, DecisionTree& dt){

	is >> dt.err;
	is >> dt.alph;

	dt.head = new  DecisionTreeNode();
	is >> *dt.head;


}
