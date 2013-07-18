/*
 * DecisionTreeNode.h
 *
 *  Created on: May 27, 2012
 *      Author: jgevirtz
 */

#ifndef DECISIONTREENODE_H_
#define DECISIONTREENODE_H_

#include "DataSet.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>

using namespace std;

class DecisionTreeNode {

public:
	DecisionTreeNode(DataSet &trainingData, unsigned int maxBucketSize);
	DecisionTreeNode() {

	}
	double getClassification(DataSet::Event &event);
	double getClassification(DataSet::TrainingEvent &event);
	void chooseSplittingValue(unsigned int variable, unsigned int& bestIndex,
			double &bestGini);
	void splitData(unsigned int& variable, unsigned int& variableIndex,
			double& variableValue);

	friend ostream& operator<<(ostream& os, const DecisionTreeNode& dt);
	friend istream& operator>>(istream& is, DecisionTreeNode& dt);

private:

	DataSet *trainingData;
	DecisionTreeNode *left;
	DecisionTreeNode *right;
	bool leaf;
	double classification;
	unsigned int variable;
	double variableValue;

};



#endif /* DECISIONTREENODE_H_ */
