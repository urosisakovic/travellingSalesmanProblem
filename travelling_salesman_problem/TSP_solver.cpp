#include "TSP_solver.h"

double TSP_solver::test_permutation(const vector< vector<double> >& graph, int permutation[])
{
	/*
	Method which computes lenght of trajectory if vertices are visited according to permutation array.
		
	Args:
		graph: Weight matrix.
		permutation: Array of lenght <number of vertices>. Permutation of 0, 1, 2, ... , <number of vertices> - 1.

	Returns:
		Sum of weights of edges between visited vertices.
	*/

	int vertex_count = graph.size();
	double distance = 0;

	// Go from first to last vertex.
	for (int i = 0; i < vertex_count - 1; i++)
		distance += graph[permutation[i]][permutation[i + 1]];

	// Return to the first vertex from the last.
	distance += graph[permutation[vertex_count - 1]][permutation[0]];

	return distance;
}


void TSP_solver::test_all_permutations(const vector< vector<double> >& graph, int permutation[], double& best_result, int best_permutation[], int curr_idx)
{
	/*
	Method which makes all the possible permutations using backtracking, 
	checks the length of every one of them and stores the best one.

	Args:
		graph: Weight matrix.
		permutation: Permutation of 0, 1, 2, ... , <number of vertices> - 1.
		best_result: Sum of weights of edges between visited vertices on the best path.
		best_permutation: Stores the permutation of best trajectory so far.
		curr_idx: Parameter of backtracking permutation algorithm.
	*/
	int vertex_count = graph.size();

	// Backtracking algorithm formed one permutation. Check it.
	if (curr_idx == vertex_count - 1)
	{
		double curr_result = test_permutation(graph, permutation);
		if (curr_result < best_result)
		{
			best_result = curr_result;
			for (int i = 0; i < vertex_count; i++)
				best_permutation[i] = permutation[i];
		}
	}

	// Put permutation[curr_idx] element in a random place and permute 
	// next elements until the a complete permutation is found.
	for (int i = curr_idx; i < vertex_count; i++)
	{
		swap(permutation[curr_idx], permutation[i]);
		test_all_permutations(graph, permutation, best_result, best_permutation, curr_idx + 1);
		swap(permutation[curr_idx], permutation[i]);
	}
}

void TSP_solver::brute_force_solve(const vector< vector<double> >& graph, int start, double& best_result, int **best_permutation)
{
	/*
	Method which wrappes test_all_permutations method and does all the necessery data preparation and data disposal.

	Args:
		graph: Weight matrix.
		start: Index of starting vertex.
		best_result: Sum of weights of edges between visited vertices on the best path.
		best_permutation: Stores the permutation of best trajectory.
	*/
	int vertex_count = graph.size();

	int *permutation = new int[vertex_count];
	int *best_permutation_temp = new int[vertex_count];
	for (int i = 0; i < vertex_count; i++)
		permutation[i] = best_permutation_temp[i] = i;

	// Start must be at start vertex, not 0 vertex. It may not be permuted. 
	swap(permutation[0], permutation[start]);
	swap(best_permutation_temp[0], best_permutation_temp[start]);

	best_result = MAX_DOUBLE;

	test_all_permutations(graph, permutation, best_result, best_permutation_temp);

	*best_permutation = best_permutation_temp;
	delete permutation;
}

void TSP_solver::brute_force_solve(Data_holder& d)
{
	/*
	Overloaded method which makes more intuitive interface.

	Args:
		d: Data_holder object contating data to be processed.
	*/
	brute_force_solve(d.graph, d.start, d.best_result, &d.best_permutation);
}

int TSP_solver::random_number(int interval_start, int interval_end)
{
	return (rand() % (interval_end - interval_start + 1) ) + interval_start;
}

void TSP_solver::get_random_edges_indices(int e[3], int vertex_count)
{
	/*
	Utility method which chooses three random unconnected edges of the graph.

	Args:
	e: Array of pairs representing edges to be chosen by this method and then further processed.
	interval_end: Upper bound on possible index to be chosen as a part of an edge.
	interval_start: Lower bound on possible index to be chosen as a part of an edge.
	*/
	e[0] = random_number(0, vertex_count - 6);

	if (e[0] == 0)
	{
		e[1] = random_number(e[0] + 2, vertex_count - 3);
		e[2] = random_number(e[1] + 2, vertex_count - 1);
	}
	else
	{
		e[1] = random_number(e[0] + 2, vertex_count - 4);
		e[2] = random_number(e[1] + 2, vertex_count - 2);
	}
	
}

void TSP_solver::get_random_edge_starts(Node *head, int edge_indices[3], Node *edges[3])
{
	Node* ptr = head;

	for (int i = 0; i < edge_indices[0]; i++)
		ptr = ptr->next;
	edges[0] = ptr;

	for (int i = 0; i < edge_indices[1] - edge_indices[0]; i++)
		ptr = ptr->next;
	edges[1] = ptr;

	for (int i = 0; i < edge_indices[2] - edge_indices[1]; i++)
		ptr = ptr->next;
	edges[2] = ptr;
}

bool TSP_solver::improve_edges(const vector< vector<double> >& graph, Node *edges[3])
{
	/*
	Method which given three edges tries to replace them with one of seven other options.
	It keeps the best option and returns a flag whether there was an improvement.

	Args:
	graph: Matrix of weights.
	e: Array of 3 edges to be improved.

	Returns:
	A flag whether there was an improvement.
	*/
	bool improved = false;

	// The 7 other possible options for reconnecting 3 chosen edges.
	int possible_vertex_connections[8][6] = {   {2, 3, 4, 5, 6, 1},
												{2, 4, 3, 5, 6, 1},
												{2, 3, 4, 6, 5, 1},
												{2, 4, 3, 6, 5, 1},
												{2, 6, 5, 4, 3, 1},
												{2, 5, 6, 4, 3, 1},
												{2, 6, 5, 3, 4, 1},   
												{2, 5, 6, 3, 4, 1}   };

	int reverses[8][3] = { {0, 0, 0},
							{0, 1, 0},
							{0, 0, 1},
							{0, 1, 1},
							{0, 1, 1},
							{0, 1, 0},
							{0, 0, 1},
							{0, 0, 0} };
	
	int *best_reconnection = nullptr;
	int *need_reverse = nullptr;

	Node *vertices[7] = { nullptr,  edges[2]->next, edges[0],
									edges[0]->next, edges[1],
									edges[1]->next, edges[2] };

	double best_cost = 0;
	for (int j = 0; j < 6; j += 2)
		best_cost += graph[vertices[possible_vertex_connections[0][j]]->data][vertices[possible_vertex_connections[0][j + 1]]->data];

	double curr_cost;
	for (int i = 1; i < 8; i++)
	{
		curr_cost = 0;
		for (int j = 0; j < 6; j += 2)
			curr_cost += graph[vertices[possible_vertex_connections[i][j]]->data][vertices[possible_vertex_connections[i][j + 1]]->data];

		if (curr_cost < best_cost)
		{
			improved = true;
			best_cost = curr_cost;
			best_reconnection = possible_vertex_connections[i];
			need_reverse = reverses[i];
		}
	}

	if (improved)
	{
		if (need_reverse[1])
			reverse_list(vertices[3], vertices[4]);
		if (need_reverse[2])
			reverse_list(vertices[5], vertices[6]);


		for (int j = 0; j < 6; j += 2)
			vertices[best_reconnection[j]]->next = vertices[best_reconnection[j + 1]];
	}
		

	return improved;
}


void TSP_solver::triopt_heuristics_solve(const vector< vector<double> >& graph, int start, double& best_result, int **best_permutation)
{
	/*
	Method which implements 3-opt heuristics for solving TSP problem. It is iterative.
	Stopping criteria are total number of iterations and number of iterations without improvement.
	Those could be implemented as hyper-parameters of algorithms (parameters of this method). Here,
	they are hardcoded.

	Args:
		graph: Weight matrix.
		start: Index of starting vertex.
		best_result: Sum of weights of edges between visited vertices on the best path.
		best_permutation: Stores the permutation of best trajectory.
	*/
	if (graph.size() < 6)
		throw string("Za primenu trioptimalne heuristike mreza mora imati najmanje 6 gradova.");

	int vertex_count = graph.size();

	Node* head = create_list(vertex_count);

	int iter_count = 0;
	int last_update_iter = 0;
	bool improved;
	int edge_indices[3];
	Node *edges[3];

	while (iter_count < 1000000 && iter_count - last_update_iter < 100)
	{
		get_random_edges_indices(edge_indices, vertex_count);
		get_random_edge_starts(head, edge_indices, edges);
		improved = improve_edges(graph, edges);

		iter_count++;

		if (improved)
			last_update_iter = iter_count;
	}

	*best_permutation = new int[vertex_count];
	Node *ptr = head;
	for (int i = 0; i < vertex_count; i++)
	{
		(*best_permutation)[i] = ptr->data;
		ptr = ptr->next;
	}

	delete_list(head, vertex_count);

	best_result = test_permutation(graph, *best_permutation);

}

void TSP_solver::triopt_heuristics_solve(Data_holder& d)
{
	/*
	Overloaded method which makes more intuitive interface.

	Args:
		d: Data_holder object contating data to be processed.
	*/
	triopt_heuristics_solve(d.graph, d.start, d.best_result, &d.best_permutation);
}