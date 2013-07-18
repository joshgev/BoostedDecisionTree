#ifndef DATASET_H_
#define DATASET_H_

#include <vector>
#include <string>

using namespace std;

class DataSet{
public:
	struct TrainingEvent{
		bool signal;
		double weight;
		vector<double> variables;
	};

	struct Event{
		vector<double> variables;
	};

	DataSet();
	~DataSet(){
//		delete [] trainingData;
//		delete [] indices;
	}
	void addTrainingEvent(TrainingEvent trainingEvent);
	void sort(unsigned int variable);
	void normalizeWeights();
	DataSet subset(unsigned int start, unsigned int end);

	bool allSignalOrAllBackground();

	double weightedCount(unsigned int start = 0);
	double weightedCountS(unsigned int start = 0);

	unsigned int size();

	unsigned int numVariables();

	TrainingEvent &operator[](const int index);
//	string operator<<();
	void print();
	void resetIndices();
	//testing!
	void randomize();
	DataSet copy();
	void clear();
	vector<TrainingEvent> *trainingData;

private:


	DataSet(vector<TrainingEvent> *trainingData);
	void swapVectorElements(unsigned int a, unsigned int b);
	unsigned int partition(unsigned int variable,unsigned int left, unsigned int right, unsigned int pivotIndex);
	void quicksortHelper(unsigned int variable, unsigned int left, unsigned int right);
	void initialize();


	vector<unsigned int> *indices;
	unsigned int start, end;
	unsigned int lastSortedVariable;
};





#endif
