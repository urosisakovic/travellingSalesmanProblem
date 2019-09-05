#pragma once
#include "Data_holder.h"
#include <vector>
#include <random>
#include <time.h>
using namespace std;

/*
	Class that encapsulating all the important functions for solving TSP
	by brute force method and using 3-opt heuristics.

	For the further info about methods, consult doc string of each method.
*/
class TSP_solver
{
	struct Node
	{
		int data;
		Node* next;

		Node(int d) : data(d), next(nullptr) {}
	};

	Node* create_list(int n)
	{
		Node *head = new Node(0);
		Node *tail = head;
		for (int i = 1; i < n; i++)
		{
			Node *new_node = new Node(i);
			tail->next = new_node;
			tail = new_node;
		}

		tail->next = head;
		return head;
	}

	void print_list(Node* head, int n)
	{
		Node *p = head;
		for (int i = 0; i < n; i++)
		{
			cout << p->data << " ";
			p = p->next;
		}
		cout << endl;
	}

	void delete_list(Node* head, int n)
	{
		Node *p = head, *q = nullptr;
		for (int i = 0; i < n; i++)
		{
			q = p;
			p = p->next;
			delete q;
		}
	}

	void reverse_list(Node* n1, Node* n2)
	{
		Node *p = n1->next;
		Node* q = n1;
		Node *r = nullptr;

		while (q != n2)
		{
			r = p->next;
			p->next = q;
			q = p;
			p = r;
		}
	}

	const double MAX_DOUBLE = 2 * 1e9;
	void test_all_permutations(const vector< vector<double> >& graph, int permutation[], double& best_result, int best_permutation[], int curr_idx = 1);
	double test_permutation(const vector< vector<double> >& graph, int permutation[]);
	int random_number(int interval_start, int interval_end);
	void get_random_edges_indices(int e[3], int vertex_count);
	void get_random_edge_starts(Node *head, int edge_indices[3], Node *edges[3]);
	bool improve_edges(const vector< vector<double> >& graph, Node *edges[3]);
public:
	TSP_solver() { srand((unsigned int)time(NULL)); }

	void brute_force_solve(const vector< vector<double> >& graph, int start, double& best_result, int **best_permutation);
	void brute_force_solve(Data_holder& d);

	void triopt_heuristics_solve(const vector< vector<double> >& graph, int start, double& best_result, int **best_permutation);
	void triopt_heuristics_solve(Data_holder& d);
};