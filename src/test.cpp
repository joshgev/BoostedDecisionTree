/*
 * test.cpp
 *
 *  Created on: May 27, 2012
 *      Author: jgevirtz
 */

#include "DataSet.h"
#include "BoostedDecisionTree.h"
#include "DecisionTree.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>

using namespace std;

void testDT() {
	DataSet ds;
	unsigned int N = 100000;
	for (unsigned int i = 0; i < N; i++) {
		DataSet::TrainingEvent trainingEvent;
		double x = rand() / double(RAND_MAX) * 2 * 3.14159;
		double y = (rand() / double(RAND_MAX) - .5) / .5;
		trainingEvent.variables.push_back(x);
		trainingEvent.variables.push_back(y);
//		cout<<"Adding..."<<x<<" "<<y<<endl;
		trainingEvent.signal = y > sin(x) ? true : false;
		ds.addTrainingEvent(trainingEvent);

	}
	cout << "Begin training..." << endl;
	DecisionTree dt(&ds);
	cout << "End training..." << endl;

	unsigned int count = 0;
	for (unsigned int i = 0; i < N; i++) {
		DataSet::Event testingEvent;
		double x = rand() / double(RAND_MAX) * 2 * 3.14159;
		double y = (rand() / double(RAND_MAX) - .5) / .5;
		testingEvent.variables.push_back(x);
		testingEvent.variables.push_back(y);
		//		cout<<"Adding..."<<x<<" "<<y<<endl;
		bool signal = y > sin(x) ? true : false;
		double result = dt.classify(testingEvent);
		count += ((result == 1 && signal) || (result == -1 && !signal)) ? 1 : 0;

	}
	cout << "% Correct: " << double(count) / N * 100 << endl;

}
void testBDT() {
	DataSet ds;
	unsigned int N = 10000;
	for (unsigned int i = 0; i < N; i++) {
		DataSet::TrainingEvent trainingEvent;
		double x = rand() / double(RAND_MAX) * 2 * 3.14159;
		double y = (rand() / double(RAND_MAX) - .5) / .5;
		trainingEvent.variables.push_back(x);
		trainingEvent.variables.push_back(y);
//		cout<<"Adding..."<<x<<" "<<y<<endl;
		trainingEvent.signal = y > sin(x) ? true : false;
		ds.addTrainingEvent(trainingEvent);

	}
	vector<DataSet::Event> testingData;
	for (unsigned int i = 0; i < N; i++) {
		DataSet::Event testingEvent;
		double x = rand() / double(RAND_MAX) * 2 * 3.14159;
		double y = (rand() / double(RAND_MAX) - .5) / .5;
		testingEvent.variables.push_back(x);
		testingEvent.variables.push_back(y);
		testingData.push_back(testingEvent);
		//		cout<<"Adding..."<<x<<" "<<y<<endl;

//		count += ((result > 0 && signal )||(result < 0 && !signal)) ? 1 : 0;

	}
	cout << "Begin training bdt 1..." << endl;
	ds.sort(0);
//	for (unsigned int i = 0; i < ds.size(); i++)
//		cout << "1: " << ds[i].variables[0] << endl;

	BoostedDecisionTree bdt1(&ds, 100);
	cout << "End training bdt 1..." << endl;
	//ds.randomize();
	ds.sort(0);
	cout << "Begin training bdt 2..." << endl;
//	for (unsigned int i = 0; i < ds.size(); i++)
//		cout << "2: " << ds[i].variables[0] << endl;

	BoostedDecisionTree bdt2(&ds, 100);
	cout << "End training bdt 2..." << endl;

	ofstream out("scores1");
	unsigned int count = 0;
	for (unsigned int i = 0; i < testingData.size(); i++) {
		double x = testingData[i].variables[0];
		double y = testingData[i].variables[1];
		bool signal = y > sin(x) ? true : false;
		double result = bdt1.classify(testingData[i]);

		count += ((result > 0 && signal) || (result < 0 && !signal)) ? 1 : 0;
		out << result << endl;
	}
	out.close();
	cout << "% Correct: " << double(count) / N * 100 << endl;
	out.open("scores2");

	count = 0;

	for (unsigned int i = 0; i < testingData.size(); i++) {
		double x = testingData[i].variables[0];
		double y = testingData[i].variables[1];
		bool signal = y > sin(x) ? true : false;
		double result = bdt2.classify(testingData[i]);

		count += ((result > 0 && signal) || (result < 0 && !signal)) ? 1 : 0;
		out << result << endl;
	}

	out.close();

	cout << "% Correct: " << double(count) / N * 100 << endl;
	cout << "Saving bdt1..." << endl;
	bdt1.save("simple_test1.bdt");
	bdt2.save("simple_test2.bdt");
	cout << "Loading tree into bdt3..." << endl;
	BoostedDecisionTree bdt3("simple_test1.bdt");
	cout << "Testing bdt3: " << endl;
	count = 0;
	out.open("scores3");
	for (unsigned int i = 0; i < testingData.size(); i++) {
		double x = testingData[i].variables[0];
		double y = testingData[i].variables[1];
		bool signal = y > sin(x) ? true : false;
		double result = bdt3.classify(testingData[i]);

		count += ((result > 0 && signal) || (result < 0 && !signal)) ? 1 : 0;

		out<<result<<endl;
	}
	out.close();
	cout << "% Correct: " << double(count) / N * 100 << endl;

}
void generateRandomIndices(unsigned int N, double trainingFraction,
		vector<unsigned int> &trainingIndices,
		vector<unsigned int> &testingIndices) {

	cout << "Generating random indices..." << endl;
	trainingIndices.clear();
	testingIndices.clear();

	for (unsigned int i = 0; i < N; i++)
		testingIndices.push_back(i);
	for (unsigned int i = 0; i < N * trainingFraction; i++) {
		unsigned int index = rand() % testingIndices.size();
		trainingIndices.push_back(testingIndices[index]);
		testingIndices.erase(testingIndices.begin() + index);

	}

//	for (unsigned int i = 0; i < trainingIndices.size(); i++)
//		cout <<"training: "<< trainingIndices[i] << endl;
//
//	for (unsigned int i = 0; i < testingIndices.size(); i++)
//		cout <<"testing: "<< testingIndices[i] << endl;

}
vector<DataSet::TrainingEvent> loadTrainingEvents(string filename) {
	ifstream in(filename.c_str());
	cout << "Reading data..." << endl;

	vector<DataSet::TrainingEvent> trainingEvents;
	while (in.good()) {

		DataSet::TrainingEvent trainingEvent;
		string s_line;
		getline(in, s_line);

		stringstream ss(s_line);
		double variable;
		while (!ss.eof()) {

			ss >> variable;
			trainingEvent.variables.push_back(variable);

		}
		trainingEvent.variables.pop_back();
		trainingEvent.signal = (variable == 1) ? true : false;
		if (trainingEvent.variables.size() != 0)
			trainingEvents.push_back(trainingEvent);

	}

	in.close();
	cout << "Done reading data." << endl;
	return trainingEvents;
}
vector<DataSet::TrainingEvent> loadTrainingEvents(string filename, bool signal) {
	ifstream in(filename.c_str());
	cout << "Reading data..." << endl;

	vector<DataSet::TrainingEvent> trainingEvents;
	while (in.good()) {

		DataSet::TrainingEvent trainingEvent;
		string s_line;
		getline(in, s_line);

		stringstream ss(s_line);
		double variable;
		while (!ss.eof()) {

			ss >> variable;
			trainingEvent.variables.push_back(variable);

		}

		trainingEvent.signal = signal;
		if (trainingEvent.variables.size() != 0)
			trainingEvents.push_back(trainingEvent);

	}

	in.close();
	cout << "Done reading data." << endl;
	return trainingEvents;
}
string itoa(int i) {
	char pc_val[255];
	sprintf(pc_val, "%d", i);

	return string(pc_val);

}
void writeTestingSet(vector<DataSet::TrainingEvent> trainingEvents,
		vector<unsigned int> indices, unsigned int id) {
	string filename = "testing/set";
	filename += itoa(id);
	filename += ".txt";

	ofstream out(filename.c_str());
	for (unsigned int i = 0; i < indices.size(); i++) {
		for (unsigned int j = 0; j < trainingEvents[i].variables.size(); j++) {
			out << trainingEvents[indices[i]].variables[j] << "\t";
		}
		out << trainingEvents[indices[i]].signal << endl;
	}
	out.close();
}
void TrainMPIR(unsigned int N,string filename_bdt) {

	vector<DataSet::TrainingEvent> trainingEvents = loadTrainingEvents(
			"train_GOOG.csv");
	cout << "Number of events read: " << trainingEvents.size() << endl;
	cout << "Number of variables: " << trainingEvents[10].variables.size()
			<< endl;
	vector<unsigned int> trainingIndices;
	vector<unsigned int> testingIndices;
	srand(time(NULL));

	unsigned int nEvents = trainingEvents.size();
	for (unsigned int i = 0; i < N; i++) {
		generateRandomIndices(nEvents, 1.0, trainingIndices, testingIndices);
		//writeTestingSet(trainingEvents, testingIndices, i+100);
		DataSet ds;
		for (unsigned int j = 0; j < trainingIndices.size(); j++)
			ds.addTrainingEvent(trainingEvents[trainingIndices[j]]);
		BoostedDecisionTree bdt(&ds, 50);

		bdt.save(filename_bdt);

	}

}
void EvaluateMPIR(unsigned int N, string filename_bdt, string filename_events) {

	for (unsigned int i = 0; i < N; i++) {

		vector<DataSet::TrainingEvent> trainingEvents = loadTrainingEvents(
				filename_events);
		string output_basename = filename_events.substr(
				0,
				filename_events.find(".") - 0 );
		cout<<"Processing evaluation set: "<<filename_events<<endl;
		cout << "Reading bdt: " << filename_bdt << endl;
		BoostedDecisionTree bdt(filename_bdt);

		ofstream ofstream_signal(
				((string) ((string) ("eval/signal_") + output_basename
						+ (string) (".txt"))).c_str());
		ofstream ofstream_background(
				((string) ((string) ("eval/background_") + output_basename
						+ (string) (".txt"))).c_str());
		for (unsigned int j = 0; j < trainingEvents.size(); j++) {
			DataSet::Event event;
			event.variables = trainingEvents[j].variables;
			if (trainingEvents[j].signal)
				ofstream_signal << bdt.classify(event) << endl;
			else
				ofstream_background << bdt.classify(event) << endl;

		}
		ofstream_background.close();
		ofstream_signal.close();
		cout<<"Done."<<endl;
	}

}
int main(int argc, char ** argv) {

	//testBDT();



	bool load = false;
	if (argc == 2)
		load = true;


	string signalFilename( "/home/jgevirtz/workspace/BDT2/Debug/clusters_signal_noisolation.txt" );
	string backgroundFilename( "/home/jgevirtz/workspace/BDT2/Debug/clusters_background_noisolation.txt" );

	double holdout = .2;

	vector<DataSet::TrainingEvent> sEvents = loadTrainingEvents( signalFilename, true );
	vector<DataSet::TrainingEvent> bEvents = loadTrainingEvents( backgroundFilename, false );
	vector<DataSet::TrainingEvent> testingEvents, trainingEvents;



	int n = sEvents.size() * (1.0 - holdout);
	DataSet trainingDS;

	for(int i = 0; i < n; ++i){



		trainingDS.addTrainingEvent( sEvents[i] );
		trainingDS.addTrainingEvent( bEvents[i] );
	}
	DataSet testingDS;
	for(int i = n+1; i < sEvents.size(); ++i){

		//testingEvents.push_back( sEvents[i] );
		//testingEvents.push_back( bEvents[i] );
		testingDS.addTrainingEvent( sEvents[i] );
		testingDS.addTrainingEvent( bEvents[i] );
	}

	BoostedDecisionTree bdt;

	if(!load)
		bdt = BoostedDecisionTree(&trainingDS, 50);
	else
		bdt = BoostedDecisionTree(string(argv[1]));



	cout<<"Size of testing set: "<<testingDS.size();

	ofstream out_signal("signal_scores");
	ofstream out_background("background_scores");
	for (unsigned int j = 0; j < testingDS.size(); j++) {
		DataSet::Event event;

		event.variables = testingDS[j].variables;


		cout<<"event.variables.size(): "<<event.variables.size()<<endl;
		cout<<"event.signal: "<<testingDS[j].signal<<endl;
		cout<<"prediction: "<<bdt.classify(event)<<endl;

		double score = bdt.classify( event );

		if (testingDS[j].signal && (score > 0) )
			cout << "11" << endl;
		else if(!trainingDS[j].signal && (score < 0) )
			cout << "00" << endl;
		else if(trainingDS[j].signal && (score < 0) )
			cout<<"10"<<endl;
		else if(!trainingDS[j].signal && (score > 0))
			cout<<"01"<<endl;
		if( testingDS[j].signal)
			out_signal << score <<endl;
		else
			out_background << score <<endl;

	}

	out_signal.close();
	out_background.close();



	if(!load)
		bdt.save("tree.bdt");


	return 0;
}
