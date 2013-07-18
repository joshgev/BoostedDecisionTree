#include "DataSet.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

DataSet::DataSet() {
	trainingData = 0;
	indices = 0;
	lastSortedVariable = -1;
	start = 0;
	end = -1;

}
void DataSet::initialize() {
	trainingData = new vector<TrainingEvent>;
	indices = new vector<unsigned int>;
	lastSortedVariable = -1;
	this->start = 0;
	this->end = -1;

}
DataSet::DataSet(vector<TrainingEvent> *trainingData) {
	this->trainingData = trainingData;
}
void DataSet::addTrainingEvent(TrainingEvent trainingEvent) {

	if (trainingData == 0)
		initialize();
	trainingEvent.weight = 1.0;

	trainingData->push_back(trainingEvent);
	indices->push_back(trainingData->size() - 1);

	this->end++;
}
void DataSet::swapVectorElements(unsigned int a, unsigned int b) {
	unsigned int temp = (*indices)[a];
	(*indices)[a] = (*indices)[b];
	(*indices)[b] = temp;
}
unsigned int DataSet::partition(unsigned int variable, unsigned int left,
		unsigned int right, unsigned int pivotIndex) {
	double pivotValue =
			(*trainingData)[(*indices)[pivotIndex]].variables[variable];
	swapVectorElements(right, pivotIndex);
	unsigned int storeIndex = left;
	for (unsigned int i = left; i < right; i++) {
		if ((*trainingData)[(*indices)[i]].variables[variable] < pivotValue) {
			swapVectorElements(i, storeIndex);
			storeIndex += 1;

		}
	}
	swapVectorElements(storeIndex, right);

	return storeIndex;
}
void DataSet::quicksortHelper(unsigned int variable, unsigned int left,
		unsigned int right) {
//	cout<<"left: "<<left<<" right: "<<right<<endl;
	if (right == 0 || right == 4294967295 || left == 4294967295)
		return;
	if (left >= right)
		return;
	unsigned int pivotIndex = (right + left) / 2;
	unsigned int pivotNewIndex = partition(variable, left, right, pivotIndex);
//	cout<<"pivotNewIndex: "<<pivotNewIndex<<endl;
	quicksortHelper(variable, left, pivotNewIndex - 1);
	quicksortHelper(variable, pivotNewIndex + 1, right);

	this->lastSortedVariable = variable;

}
void DataSet::sort(unsigned int variable) {

//	if (this->lastSortedVariable == variable)
//		return;
	this->lastSortedVariable = variable;

	quicksortHelper(variable, this->start, this->end );

	//debug code
//	cout<<"sorted data: ";
//	for(unsigned int i = start; i <= end; i++)
//		cout<<(*trainingData)[(*indices)[i]].variables[variable]<<" ";
//	cout<<endl;
}

void DataSet::normalizeWeights() {
	double total = 0.0;
	for (unsigned int i = 0; i < trainingData->size(); i++)
		total += (*trainingData)[i].weight;
	for (unsigned int i = 0; i < trainingData->size(); i++)
		(*trainingData)[i].weight /= total;
}
DataSet DataSet::subset(unsigned int start, unsigned int end) {
	DataSet ret(this->trainingData);
	ret.lastSortedVariable = this->lastSortedVariable;
	ret.indices = this->indices;
	ret.start = this->start + start;
	ret.end = this->start + end ;
//	cout<<"ret.start: "<<ret.start<<" ";
//	cout<<"ret.end: "<<ret.end<<endl;

	return ret;

}
bool DataSet::allSignalOrAllBackground() {
	unsigned int count = 0;
	for (unsigned int i = this->start; i <= this->end; ++i)
		count += (*trainingData)[(*indices)[i]].signal ? 1 : 0;

//	cout<<"count: "<<count<<endl;
	return (count == this->size()) || (count == 0);
}
double DataSet::weightedCount(unsigned int start) {
	double total = 0.0;
	for (unsigned int i = this->start + start; i <= this->end; i++)
		total += (*trainingData)[(*indices)[i]].weight;

	return total;
}
double DataSet::weightedCountS(unsigned int start) {
	double total = 0.0;
	for (unsigned int i = this->start + start; i <= this->end; i++)
		if ((*trainingData)[(*indices)[i]].signal)
			total += (*trainingData)[(*indices)[i]].weight;

	return total;
}

unsigned int DataSet::size() {
	return (this->end - this->start + 1);
}

DataSet::TrainingEvent& DataSet::operator[](const int index) {

	return (*trainingData)[(*indices)[index + this->start]];
}
//string DataSet::operator <<(){
//	string ret = ""
//	for(unsigned int i = 0; i < this->size(); i++)
//
//}
void DataSet::print() {
	for (unsigned int i = 0; i < this->size(); i++)
		cout << (*this)[i].variables[0] << " ";
	cout << endl;
	cout<<this->start<<" "<<this->end<<endl;
//	cout<<this->trainingData<<endl;
}
unsigned int DataSet::numVariables() {
	return (*trainingData)[0].variables.size();
}
void DataSet::resetIndices(){
	for(unsigned int i = 0; i < trainingData->size(); i++)
		(*indices)[i] = i;
}

void DataSet::randomize() {

	lastSortedVariable = -1;
	srand(time(NULL));
	cout << "Time: " << time(NULL) << endl;
	vector<TrainingEvent> temp = *trainingData;
	trainingData->clear();
	trainingData->resize(temp.size());
	unsigned int i = 0;
	while (temp.size() > 0) {
		vector<TrainingEvent>::iterator start = temp.begin();
		unsigned int index = rand() % temp.size();
		(*trainingData)[i] = (temp[index]);
		temp.erase(start + index);
		//		cout<<"temp.csize(): "<<temp.size()<<endl;
		//		cout<<"index: "<<index<<endl;
		i++;
	}
}


DataSet DataSet::copy(){
	DataSet ret;
	ret.trainingData = new vector<TrainingEvent> ();
	(*ret.trainingData) = *trainingData;
	ret.indices = new vector<unsigned int> ();
	(*ret.indices) = *indices;
	ret.start = start;
	ret.end = end;
	ret.lastSortedVariable = lastSortedVariable;
	return ret;
}
void DataSet::clear(){
	this->trainingData->clear();
	this->indices->clear();
	this->start = 0;
	this->end = -1;
}
