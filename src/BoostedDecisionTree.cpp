#include "BoostedDecisionTree.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using namespace std;

BoostedDecisionTree::BoostedDecisionTree(DataSet *ds,
		unsigned int nTree) {
	this->nTree = nTree;


	DataSet dsCopy = ds->copy();
	cout<<"original pointer: "<<ds->trainingData<<endl;
	cout<<"new pointer: "<<dsCopy.trainingData<<endl;

//	dsCopy.clear();
//	cout<<"SIZE! "<<dsCopy.size()<<endl;
//	return;
	trees.push_back(new DecisionTree(&dsCopy));
	for (unsigned int i = 1; i < nTree; i++) {

		if(i - 1 >= trees.size())
			cout<<"Warning: "<<i - 1<<" "<<__LINE__<<endl;

		double currentError = trees.at(i - 1)->error();

		if (currentError < .5 && currentError > .00000001) {


			boostWeights(&dsCopy);
			trees.push_back(new DecisionTree(&dsCopy));
		}
		else{
			if(currentError > .5)
							cout<<"Error upper threshold exceeded..."<<endl;
						else if(currentError < .00000001)
							cout<<"Error lower threshold exceeded...: "<<currentError<<endl;
			break;
		}

	}
	cout<<"Final number of trees: "<<trees.size()<<endl;

}

 BoostedDecisionTree::BoostedDecisionTree(string filename){
	load(filename);
}
void BoostedDecisionTree::boostWeights(DataSet *ds) {

	DecisionTree *tree = trees[trees.size() - 1];
	double boostFactor = exp(tree->alpha());
	for (unsigned int i = 0; i < ds->size(); i++) {
		if (!tree->check((*ds)[i])){

			(*ds)[i].weight *= boostFactor;


		}

	}
}
double BoostedDecisionTree::classify(DataSet::Event &event){

	double score = 0.0;
	for(unsigned int i = 0; i < trees.size(); i++)
		score += trees[i]->classify(event) == 1.0 ? trees[i]->alpha() : -1.0 * trees[i]->alpha();


	return score;
}

unsigned int BoostedDecisionTree::max_depth(){
	unsigned int max = 0;
//	for(unsigned int i = 0; i < trees.size(); i++)
//		if (trees[i]->depth() > max)
//			max = trees[i]->depth();

	return max;
}
double  BoostedDecisionTree::average_depth(){
	double total = 0.0;
//	for(int i = 0; i < trees.size(); i++)
//		total += trees[i]->depth() / double(trees.size());

	return total;
}


void BoostedDecisionTree::save(string filename){

	ofstream out(filename.c_str());
	//boost::archive::binary_oarchive oa(out);
	//oa << (*this);
	out<<*this<<endl;
	out.close();
}

void BoostedDecisionTree::load(string filename){
	ifstream in(filename.c_str());
	//boost::archive::binary_iarchive ia(in);
	//ia >> (*this);
	in >> *this;
	in.close();
}
ostream& operator<<(ostream& os, const BoostedDecisionTree& bdt){

	os<<bdt.trees.size()<<" "<<bdt.nbins<<" ";
	for(int i = 0; i < bdt.trees.size(); ++i)
		os<< *bdt.trees[i]<<" ";

	return os;
}
istream& operator>>(istream& is,  BoostedDecisionTree& bdt){

	is >> bdt.nTree;
	is >> bdt.nbins;

	for( int i = 0; i < bdt.nTree; ++i){
		DecisionTree *dt = new DecisionTree();

		is >> *dt;
		bdt.trees.push_back(dt);
	}
}
