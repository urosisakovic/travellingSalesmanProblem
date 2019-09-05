#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/*
	Class that stores all the necessery data describing TSP. Also, it encapsulates a
	method for loading and printing the data in the interactive form.

	For the further info about methods, consult doc string of each method.
*/
class Data_holder
{
	vector< vector<double> > graph;
	int start;

	double best_result;
	int *best_permutation;
public:
	Data_holder() : best_permutation(nullptr) {}
	Data_holder(const Data_holder&) = delete;
	Data_holder& operator=(const Data_holder&) = delete;
	~Data_holder() { delete[] best_permutation; }

	void manual_load();
	void file_load();
	void print_result();

	friend class TSP_solver;
};

