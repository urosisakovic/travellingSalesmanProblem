#include "TSP_solver.h"

int main()
{
	Data_holder data;
	TSP_solver solver;
	try
	{
		cout << "Za rucno unosenje podataka unesite 1." << endl;
		cout << "Za unosenje podataka iz fajla unesite 2." << endl;
		int input_option; cin >> input_option;

		if (input_option == 1)
			data.manual_load();
		else if (input_option == 2)
			data.file_load();
		else
			throw 1;


		cout << "Za resavanje problema proveravanjem svih permutacija unesite 1." << endl;
		cout << "Za resavanje problema trioptimalnom heuristikom unesite 2." << endl;
		int algorithm_option; cin >> algorithm_option;


		if (algorithm_option == 1)
			solver.brute_force_solve(data);
		else if (algorithm_option == 2)
			solver.triopt_heuristics_solve(data);
		else
			throw 1;
	}
	catch (int)
	{
		cout << "Izabrali ste nepostojecu opciju. Kraj programa." << endl;
		system("pause");
		exit(1);
	}
	catch (string s)
	{
		cout << "GRESKA: " << s << " Kraj programa." << endl;
		system("pause");
		exit(1);
	}

	data.print_result();

	system("pause");
}