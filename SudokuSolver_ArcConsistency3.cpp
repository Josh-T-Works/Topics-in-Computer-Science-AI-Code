#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <chrono>
#include <queue>


using namespace std;
using namespace std::chrono;

///////////////////////////////////////////////////////
//functions for drawing the board//////////////////////
void getSudokuBoard(string table[]);
void displayBoard(string table[]);
void fillDisplayBoard(string board[], int sudokuM[][9]);
void fillDisplayBoard(string board[], vector<int> assignment);
void displayBoard(string table[], int T[][9]);
void displaySudokuCSP(vector<int> assignment, vector<int> domain[81]);
void displaySudokuSimple(vector<int> assignment);
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
///Helper functions and structures///////////////////////
void fillConst(bool C[81][81]);   //creates the constraint graph
void getSudokuCSP(string puzzle, vector<int>& assignment, vector<int> domain[81]);
//given a puzzle as a string of 81 characters 0-9 
//return the apropriate assigment and domain

struct arcType   //a structure for a binary constraint, i.e., an edge in the graph
{
	int source;
	int destination;
};


queue<arcType> initQueue(bool C[81][81]); // FRONTIER INITIALIZER

vector<int> backtrackCSP(vector<int> assignment, int count, vector<int> domain[81], const bool C[81][81]);
vector<int> backtrackCSP_MRV(vector<int> assignment, int count, vector<int> domain[81], const bool C[81][81]);

bool AC3(vector<int> domain[81], const bool C[81][81]);
bool revise(int xi, int xj, vector<int> domain[81], const bool C[81][81]);


int main()
{

	string puzzles[] = { "000500002700600800001000300076050000002000000080000700390072600020000090050018030",
		"800201040000000107709006003006340000003060000002000008020903000000080090000007080",
		"100003400000600020008005300001004070406800100000000005065080002802001600000000000",
		"809402037020090500360000800500000000080037000076008000000000093700300000900054000",
		"050800607000600520100509080040001700007006490000000000921000070000005100560000000",
		"001200000000047000789006000060008701070002400100000906036000000800090000400070000",
		"400070009008605070005000026000800704670000500000000000069000000000590010020010040",
		"080006070002407010003000000000010800201060037300000240106000000034070000000090000",
		"000000040000000906350100800510030400004910008060004000030000160200000059100000000",
		"000000007000050060057000824000005782090000600008070000400009050015068200009000408",
		"000639710000000340900000000603700008000081000000500000005370600009040800700000100",
		"000000030000004060000090400002006000106300500097500004001005900049200007070010040",
		"000000003029003500800900006600000190500009000001600057000300000080000020176200300",
		"300000000901000000004701000080020073003050092000008001490500000100300007008000109",
		"012060000900200860005400120670004000401000090000820040096100000000090000004000700",
		"400000805030000000000700000020000060000080400000010000000603070500200000104000000",
		"520006000000000701300000000000400800600000050000000000041800000000030020008700000",
		"600000803040700000000000000000504070300200000106000000020000050000080600000010000",
		"480300000000000071020000000705000060000200800000000000001076000300000400000050000",
		"000014000030000200070000000000900030601000000000000080200000104000050600000708000" };

	bool C[81][81];
	vector<int> assignment;
	vector <int> domain[81];
	string table[20];  //for display purposes
	fillConst(C);
	getSudokuCSP(puzzles[0], assignment, domain);
	getSudokuBoard(table);
	fillDisplayBoard(table, assignment);
	cout << "The puzzle:\n";
	displayBoard(table);
	cout << endl << "The CSP domain:\n";
	displaySudokuCSP(assignment, domain);

	cout << "Testing the revise function...\n";
	for (int i = 0; i < 3; i++)
	{
		int xi, xj;
		cout << "Enter the two cells you want to resolve (indexed 0-80 as discussed) : ";
		cin >> xi >> xj;
		if (revise(xi, xj, domain, C))
		{
			cout << "The domain of cell #" << xi << " has been revised.  Here is the new domain: \n{";
			for (auto v : domain[xi])
				cout << v << ", ";
			cout << "}.\n";
		}
		else
			cout << "No revisions.\n";
	}
	system("pause");
	system("cls");
	cout << "The status of the overall domain after the revisions is :\n";
	cout << endl << "The CSP domain:\n";
	displaySudokuCSP(assignment, domain);
	system("pause");


	//AC3 Testing
	for (int i = 0; i < 20; i++)
	{
		system("cls");
		cout << "Testing the AC3 function...\n";
		getSudokuCSP(puzzles[i], assignment, domain);
		getSudokuBoard(table);
		fillDisplayBoard(table, assignment);
		cout << "The puzzle:\n";
		displayBoard(table);
		cout << endl << "The CSP domain:\n";
		displaySudokuCSP(assignment, domain);
		AC3(domain, C);
		cout << endl << "The CSP domain after AC3:\n";
		displaySudokuCSP(assignment, domain);
		system("pause");

	}

	//Backtrack testing
	double avg = 0;
	for (int i = 0; i < 20; i++)
	{
		system("cls");
		cout << "Testing  backtracking ...\n";
		getSudokuCSP(puzzles[i], assignment, domain);
		getSudokuBoard(table);
		fillDisplayBoard(table, assignment);
		cout << "The puzzle:\n";
		displayBoard(table);
		cout << endl << "The solution:\n";
		int count = 0;
		for (int i = 0; i < 81; i++)
			if (assignment[i] > 0)
				count++;

		auto start = high_resolution_clock::now();
		vector<int> result = backtrackCSP(assignment, count, domain, C);
		auto end = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(end - start);

		fillDisplayBoard(table, result);
		displayBoard(table);

		cout << endl << "Time Taken: " << duration.count() << " milliseconds" << endl;
		avg += duration.count();
		system("pause");
	}

	avg = avg / 20;
	cout << "Average: " << avg << " milliseconds" << endl;
	system("pause");


	//Backtrack + MRV testing
	avg = 0;
	for (int i = 0; i < 20; i++)
	{
		system("cls");
		cout << "Testing the backTracking with the MRV heuristic function...\n";
		getSudokuCSP(puzzles[i], assignment, domain);
		getSudokuBoard(table);
		fillDisplayBoard(table, assignment);
		cout << "The puzzle:\n";
		displayBoard(table);
		cout << endl << "The solution:\n";
		int count = 0;
		for (int i = 0; i < 81; i++)
			if (assignment[i] > 0)
				count++;

		auto start = high_resolution_clock::now();
		vector<int> result = backtrackCSP_MRV(assignment, count, domain, C);
		auto end = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(end - start);

		fillDisplayBoard(table, result);
		displayBoard(table);

		cout << endl << "Time Taken: " << duration.count() << " milliseconds" << endl;
		avg += duration.count();
		system("pause");
	}

	avg = avg / 20;
	cout << "Average: " << avg << " milliseconds" << endl;
	system("pause");

	//Backtrack + AC3 testing
	avg = 0;
	for (int i = 0; i < 20; i++)
	{
		system("cls");
		cout << "Testing backtracking + AC3...\n";
		getSudokuCSP(puzzles[i], assignment, domain);
		getSudokuBoard(table);
		fillDisplayBoard(table, assignment);
		cout << "The puzzle:\n";
		displayBoard(table);
		cout << endl << "The solution:\n";
		int count = 0;
		for (int i = 0; i < 81; i++)
			if (assignment[i] > 0)
				count++;
		AC3(domain, C);
		auto start = high_resolution_clock::now();
		vector<int> result = backtrackCSP(assignment, count, domain, C);
		auto end = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(end - start);

		fillDisplayBoard(table, result);
		displayBoard(table);

		cout << endl << "Time Taken: " << duration.count() << " milliseconds" << endl;
		avg += duration.count();
		system("pause");
	}

	avg = avg / 20;
	cout << "Average: " << avg << " milliseconds" << endl;
	system("pause");

	//Backtrack + MRV testing + AC3
	avg = 0;
	for (int i = 0; i < 20; i++)
	{
		system("cls");
		cout << "Testing the backTracking with the MRV heuristic function + AC3...\n";
		getSudokuCSP(puzzles[i], assignment, domain);
		getSudokuBoard(table);
		fillDisplayBoard(table, assignment);
		cout << "The puzzle:\n";
		displayBoard(table);
		cout << endl << "The solution:\n";
		int count = 0;
		for (int i = 0; i < 81; i++)
			if (assignment[i] > 0)
				count++;
		AC3(domain, C);
		auto start = high_resolution_clock::now();
		vector<int> result = backtrackCSP_MRV(assignment, count, domain, C);
		auto end = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(end - start);

		fillDisplayBoard(table, result);
		displayBoard(table);

		cout << endl << "Time Taken: " << duration.count() << " milliseconds" << endl;
		avg += duration.count();
		system("pause");
	}

	avg = avg / 20;
	cout << "Average: " << avg << " milliseconds" << endl;
	system("pause");

	return 0;
}

////////Drawing functions////////////////////////////

void getSudokuBoard(string table[])
{
	string table1[] = {
	"+===========+===========+===========+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+---*---*---+---*---*---+---*---*---+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+---*---*---+---*---*---+---*---*---+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+===========+===========+===========+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+---*---*---+---*---*---+---*---*---+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+---*---*---+---*---*---+---*---*---+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+===========+===========+===========+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+---*---*---+---*---*---+---*---*---+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"+---*---*---+---*---*---+---*---*---+",
	"] 0 | 0 | 0 ] 0 | 0 | 0 ] 0 | 0 | 0 ]",
	"=====================================", };

	for (int i = 0; i < 19; i++)
	{
		table[i] = table1[i];
		for (int j = 0; j < table[i].length(); j++)
		{
			switch (table[i][j])
			{
			case '+': table[i][j] = char(219); break;
			case ']': table[i][j] = char(219); break;
			case '=': table[i][j] = char(219); break;
			case '-': table[i][j] = char(196); break;
			case '|': table[i][j] = char(179); break;
			case '*': table[i][j] = char(197); break;
			case '0': table[i][j] = ' '; break;
			}

		}
	}
}

void displayBoard(string table[])
{
	for (int i = 0; i < 19; i++)
	{
		cout << table[i] << endl;
	}
}

void fillDisplayBoard(string board[], int sudokuM[][9])
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (sudokuM[i][j] > 0)
				board[1 + 2 * i][2 + 4 * j] = char(sudokuM[i][j] + '0');
		}
}

void fillDisplayBoard(string board[], vector<int> assignment)
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (assignment[i * 9 + j] > 0)
				board[1 + 2 * i][2 + 4 * j] = char(assignment[i * 9 + j] + '0');
		}
}


void displayBoard(string table[], int T[][9])
{
	//https://en.wikipedia.org/wiki/ANSI_escape_code
	char blue[] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };
	char normal[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < table[i].length(); j++)
		{
			if (i < 18 && T[i / 2][j / 4]>0 && j % 4 == 2 && i % 2 == 1)
				cout << blue << table[i][j] << normal;
			else
				cout << table[i][j];

		}
		cout << endl;
	}
}

void displaySudokuCSP(vector<int> assignment, vector<int> domain[81])
{
	string s1 = " . . .  . . .  . . .    . . .  . . .  . . .    . . .  . . .  . . .";
	string s2 = "                                                                  ";
	string board[] = { s1,s1,s1,s2,s1,s1,s1,s2,s1,s1,s1,s2,s2, s1,s1,s1,s2,s1,s1,s1,s2,s1,s1,s1,s2,s2, s1,s1,s1,s2,s1,s1,s1,s2,s1,s1,s1, };
	for (int a = 0; a < 81; a++)
	{
		int i = a / 9, j = a % 9;
		if (assignment[a] > 0.5)
		{
			int v = assignment[a] - 1;
			int r1 = v / 3, c1 = v % 3;
			int x = i * 4 + r1 + i / 3;
			int y = j * 7 + c1 * 2 + j / 3 * 2 + 1;
			board[x][y] = char(v + '1');

		}
		else
		{
			for (int k = 0; k < domain[a].size(); k++)
			{
				int v = domain[a][k] - 1;
				int r1 = v / 3, c1 = v % 3;
				int x = i * 4 + r1 + i / 3;
				int y = j * 7 + c1 * 2 + j / 3 * 2 + 1;
				board[x][y] = char(v + '1');

			}
		}
	}
	for (int i = 0; i < 37; i++)
		cout << board[i] << endl;
}

void displaySudokuSimple(vector<int> assignment)
{
	string s1 = " . . .  . . .  . . .";
	string s2 = "                    ";
	string board[] = { s1,s1,s1,s2,s1,s1,s1,s2,s1,s1,s1 };
	for (int a = 0; a < 81; a++)
	{
		int i = a / 9, j = a % 9;
		int x = i + i / 3;
		int y = j * 2 + j / 3 + 1;

		if (assignment[a] > 0.5)
		{
			int v = assignment[a];
			board[x][y] = char(v + '0');
		}
	}
	for (int i = 0; i < 11; i++)
		cout << board[i] << endl;
}

//////////////////////////////////////////////////////

/////Helper Functions///////////////////////////////////
void fillConst(bool C[81][81])
{
	for (int i = 0; i < 81; i++)
		for (int j = 0; j < 81; j++)
			C[i][j] = false;

	for (int a = 0; a < 9; a++)
	{
		for (int b = 0; b < 9; b++)
		{
			int z = 9 * a + b;
			for (int i = 0; i < 9; i++)
			{
				int z1 = 9 * i + b;  //same column
				int z2 = 9 * a + i;  //same row
				if (z != z1)
					C[z][z1] = true;
				if (z != z2)
					C[z][z2] = true;
			}
			int r = a / 3, c = b / 3;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					int z3 = 9 * (3 * r + i) + (3 * c + j);
					if (z != z3)
						C[z][z3] = true;  //same cell

				}
		}

	}
}

void getSudokuCSP(string puzzle, vector<int>& assignment, vector<int> domain[81])
{
	vector<int>  d = { 1,2,3,4,5,6,7,8,9 };
	vector<int> assignment2(81, 0);

	for (int a = 0; a < 81; a++)
	{
		int v = int(puzzle[a] - '0');
		assignment2[a] = v;
		domain[a].clear();
		if (v > 0)
			domain[a].push_back(v);
		else
			domain[a] = d;
	}
	assignment = assignment2;
}

////////////////////////////////////////////////////////


//Base bactrack implementation
vector<int> backtrackCSP(vector<int> assignment, int count, vector<int> domain[81], const bool C[81][81])
{
	vector<int> result;
	if (count >= 81) //if assignment complete
		return assignment;
	//pick the next unassigned variable
	int r = 0;
	while (assignment[r] > 0)
		r++;
	int n = domain[r].size();
	for (int i = 0; i < n; i++)
	{
		int value = domain[r][i];  //pick the next value

		//check for if value is consitent with the past
		bool consistent = true;
		for (int j = 0; j < 81; j++)
			if (j != r && C[r][j] && assignment[j] == value)
			{
				consistent = false;
				break;
			}
		if (consistent)
		{
			assignment[r] = value;
			result = backtrackCSP(assignment, count + 1, domain, C);
			if (result.size() > 0)
				return result;
			assignment[r] = 0;
		}
	}
	return result;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//To be implemented by the students

bool AC3(vector<int> domain[81], const bool C[81][81])
{
	queue<arcType> frontier; // Creates queue full of arcs
	for (int i = 0; i < 81; i++)
	{
		for (int j = 0; j < 81; j++)
		{
			if (C[i][j])
			{
				frontier.push({ i, j });
			}
		}
	}

	while (!frontier.empty()) // Goes through all arcs & checks if they are consistent
	{
		arcType curr = frontier.front();
		int xi = curr.source;
		int xj = curr.destination;
		frontier.pop();

		if (revise(xi, xj, domain, C))
		{
			if (domain[xi].size() == 0) // if no valid value exists in xi, we have a constraint
			{
				return false;
			}
			for (int xk = 0; xk < 81; xk++)
			{
				if (C[xi][xk] && xk != xj)
				{
					frontier.push({ xk, xi });
				}
			}
		}
	}
	return true;
}

bool revise(int xi, int xj, vector<int> domain[81], const bool C[81][81])
{
	bool revised = false;
	vector<int> temp;
	for (auto x : domain[xi]) // Checks domain of xi
	{
		bool a = false;
		for (auto y : domain[xj]) // Checks respective xj domain
		{
			if (x != y)
			{
				a = true;
				break;
			}
		}
		if (a) // saves current value if it satisfies the constraint
		{
			temp.push_back(x);
		}
		else revised = true;
	}
	if (revised)
	{
		domain[xi] = temp;
	}
	return revised;
}


vector<int> backtrackCSP_MRV(vector<int> assignment, int count, vector<int> domain[81], const bool C[81][81])
{
	vector<int> result;
	if (count >= 81) //if assignment complete
		return assignment;
	//pick the next unassigned variable
	int r = 0;
	int min = 0;
	int d = 10;
	for (int r = 0; r < 81; r++) // MRV MODIFICATION
	{
		if (assignment[r] == 0 && domain[r].size() < d) // UPDATES THE MOST CONSTRAINED VARIABLE
		{
			min = r;
			d = domain[r].size();
		}
	}
	int n = domain[min].size();
	for (int i = 0; i < n; i++)
	{
		int value = domain[min][i];  //pick the next value

		//check for if value is consitent with the past
		bool consistent = true;
		for (int j = 0; j < 81; j++)
			if (j != min && C[min][j] && assignment[j] == value)
			{
				consistent = false;
				break;
			}
		if (consistent)
		{
			assignment[min] = value;
			result = backtrackCSP_MRV(assignment, count + 1, domain, C);
			if (result.size() > 0)
				return result;
			assignment[min] = 0;
		}
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
