/*
 * BoostedDecisionTree.h
 *
 *  Created on: Apr 17, 2012
 *      Author: jgevirtz
 */

#ifndef BOOSTEDDECISIONTREE_H_
#define BOOSTEDDECISIONTREE_H_

#include "DecisionTree.h"
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <string>

#include "DataSet.h"

class BoostedDecisionTree {
public:
	BoostedDecisionTree(DataSet *ds,
			unsigned int nTree);
	BoostedDecisionTree() {

	}
	BoostedDecisionTree(string filename);

	double classify(DataSet::Event &event);
	unsigned int max_depth();
	double average_depth();
	void save(string filename);

	friend ostream& operator<<(ostream& os, const BoostedDecisionTree& dt);
	friend istream& operator>>(istream& is, BoostedDecisionTree& dt);


private:

	std::vector<DecisionTree*> trees;


	void load(string filename);
	unsigned int nbins;
	unsigned int nTree;

	void boostWeights(DataSet *ds);
	//void computeProbabilities(vector<TrainingEvent> testingData,
//			unsigned int nbins);

};

#endif
