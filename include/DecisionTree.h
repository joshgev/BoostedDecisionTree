/*
 * DecisionTree.h
 *
 *  Created on: May 27, 2012
 *      Author: jgevirtz
 */

#ifndef DECISIONTREE_H_
#define DECISIONTREE_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "DataSet.h"
#include "DecisionTreeNode.h"
#include <vector>
#include <float.h>

using namespace std;

class DecisionTree {
public:
	DecisionTree(DataSet *trainingData);
	DecisionTree(){

	}

	double error();
	double classify(DataSet::Event &event);
	double classify(DataSet::TrainingEvent &event);
	bool check(DataSet::TrainingEvent &event);
	double alpha();


	friend ostream& operator<<(ostream& os, const DecisionTree& dt);
	friend istream& operator>>(istream& is, DecisionTree& dt);
private:



	DataSet *trainingData;

	double err;
	double alph;
	DecisionTreeNode *head;

};

#endif /* DECISIONTREE_H_ */
