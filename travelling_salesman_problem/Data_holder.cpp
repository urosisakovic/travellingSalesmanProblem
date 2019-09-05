#include "Data_holder.h"

void Data_holder::manual_load()
{
	/*
	Method which interacts with user and stores all the needed data.
	*/
	int vertex_count;
	cout << "Unesite broj gradova: ";
	cin >> vertex_count;

	cout << "Unesite matricu tezina izmedju gradova: " << endl;
	double buffer;
	for (int i = 0; i < vertex_count; i++)
	{
		graph.push_back(vector<double>());
		for (int j = 0; j < vertex_count; j++)
		{
			cin >> buffer;
			graph[i].push_back(buffer);
		}
	}

	cout << "Unesite indeks pocetnog grada(0-indeksiran): ";
	cin >> start;
}

void Data_holder::file_load() try
{
	/*
	Method which interacts with user and, given a filepath,
	loads all the needed data from the file.
	*/
	string filepath;
	cout << "Unesite relativnu putanju do fajla: ";
	cin >> filepath;

	ifstream file;
	file.open(filepath);

	if (!file.is_open())
		throw "Fajl nije uspesno otvoren.";

	int vertex_count;
	file >> vertex_count;
	double buffer;
	for (int i = 0; i < vertex_count; i++)
	{
		graph.push_back(vector<double>());
		for (int j = 0; j < vertex_count; j++)
		{
			file >> buffer;
			graph[i].push_back(buffer);
		}
	}

	file >> start;
}
catch (string message)
{
	cout << "GRESKA: " << message << " Kraj programa." << endl;
	system("pause");
	exit(1);
}


void Data_holder::print_result()
{
	/*
	Method used for printing found result in nice form.
	*/
	cout << "Najkraci predjeni put: " << best_result << endl;
	cout << "Redosled gradova kojim se moze postici takav rezultat: " << endl;
	for (int i = 0; i < graph.size(); i++) cout << best_permutation[i] << " ";
	cout << endl;
}