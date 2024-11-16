#include <iostream>
#include <string>
#include <math.h>
#include <time.h>

using namespace std;


void drawBoard(string state);

string play(string state, bool xturn);
string playAI(string state, bool xturn);
string playAIab(string state, bool xturn);


int minimax(string state, bool xturn);
double minvalue(string state, int& move);
double maxvalue(string state, int& move);

int minimaxAB(string state, bool xturn);
double minvalueAB(string state, double alpha, double beta, int& move);
double maxvalueAB(string state, double alpha, double beta, int& move);

int eval(string state);  //given a state s, return 1 if x wins, -1 if o wins, 0  undetermined (may or may not be a leaf)

int main()
{
	string state;
	bool xturn;
	
	cout << "Testing eval function ...\n";
	system("pause");

	string s[] = { ".........", "o..xooxXx","xoxxoxoO.", "x.oxOxo..","......xox","xxooxxoxo",".x..o...x" };

	for (int i = 0; i < 7; i++)
	{
		system("cls");
		drawBoard(s[i]);
		cout << "Evaluation : " << eval(s[i]) << endl;
		system("pause");
	}

	

	cout << "Testing minimax by game playing ...\n";
	system("pause");


	//AI goes first
	state = ".........";
	drawBoard(state);
	xturn = true;
	for (int i = 0; i < 9; i++)
	{
		if (xturn)
			state = playAI(state, xturn);
		else
			state = play(state, xturn);
		if (eval(state) > 0)
		{
			cout << "AI (x) wins !" << endl;
			break;
		}
		if (eval(state) < 0)
		{
			cout << "Player 2 (o) wins !" << endl;
			break;
		}
		if (eval(state) == 0 && i == 8)
		{
			cout << "Draw!" << endl;
			break;
		}
		xturn = !xturn;
	}

	system("pause");
	xturn = true;
	state = ".........";

	//AI goes second
	for (int i = 0; i < 9; i++)
	{
		if (xturn)
			state = play(state, xturn);
		else
			state = playAI(state, xturn);
		if (eval(state) < 0)
		{
			cout << "AI (o) wins !" << endl;
			break;
		}
		if (eval(state) > 0)
		{
			cout << "Player 1 (x) wins !" << endl;
			break;
		}
		if (eval(state) == 0 && i == 8)
		{
			cout << "Draw!" << endl;
			break;
		}
		xturn = !xturn;
	}
	system("pause");




	cout << "Testing minimax alpha-beta by game playing ...\n";
	system("pause");



	//AI goes first
	state = ".........";
	drawBoard(state);
	xturn = true;
	for (int i = 0; i < 9; i++)
	{
		if (xturn)
			state = playAIab(state, xturn);
		else
			state = play(state, xturn);
		if (eval(state) > 0)
		{
			cout << "AI (x) wins !" << endl;
			break;
		}
		if (eval(state) < 0)
		{
			cout << "Player 2 (o) wins !" << endl;
			break;
		}
		if (eval(state) == 0 && i == 8)
		{
			cout << "Draw!" << endl;
			break;
		}
		xturn = !xturn;
	}

	system("pause");
	xturn = true;
	state = ".........";

	//AI goes second
	for (int i = 0; i < 9; i++)
	{
		if (xturn)
			state = play(state, xturn);
		else
			state = playAIab(state, xturn);
		if (eval(state) < 0)
		{
			cout << "AI (o) wins !" << endl;
			break;
		}
		if (eval(state) > 0)
		{
			cout << "Player 1 (x) wins !" << endl;
			break;
		}
		if (eval(state) == 0 && i == 8)
		{
			cout << "Draw!" << endl;
			break;
		}
		xturn = !xturn;
	}

	system("pause");
	return 0;
}


/********************************************************/
///Drawing and game play functions////////////////////////
void drawBoard(string state)
{
	string board[] = { ".....|.....|.....",
	".....|.....|.....",
	".....|.....|.....",
	"-----+-----+-----",
	".....|.....|.....",
	".....|.....|.....",
	".....|.....|.....",
	"-----+-----+-----",
	".....|.....|.....",
	".....|.....|.....",
	".....|.....|.....", };
	cout << endl;

	for (int i = 0; i < 9; i++)
	{
		int a = i / 3, b = i % 3;
		board[a * 4 + 1][b * 6 + 2] = state[i];
	}

	cout << "      0     1     2" << endl;
	cout << endl;

	for (int i = 0; i < 11; i++)
	{
		if (i % 4 == 1)
			cout << i / 4 << "   ";
		else
			cout << "    ";
		for (int j = 0; j < 17; j++)
		{
			char ch = toupper(board[i][j]);
			switch (ch)
			{
			case '.': cout << ' '; break;
			case '|': cout << char(186); break;
			case '-': cout << char(205); break;
			case '+': cout << char(206); break;
			case 'X': cout << 'X'; break;
			case 'O': cout << 'O'; break;
			}

		}
		cout << endl;
	}
	cout << endl;
}

string play(string state, bool xturn)
{
	int r, c;
	system("cls");
	drawBoard(state);
	if (xturn)
	{
		cout << "X-player, enter the coordinates of where you want to play (row then column):";
		cin >> r >> c;
		int ind = r * 3 + c;
		while (r > 2 || r < 0 || c>2 || c < 0 || state[ind] == 'x' || state[ind] == 'X' || state[ind] == 'o' || state[ind] == 'O')
		{
			cout << "Illegal move.  Try again : ";
			cin >> r >> c;
			ind = r * 3 + c;
		}
		state[ind] = 'x';
	}
	else
	{
		cout << "O-player, enter the coordinates of where you want to play (row then column):";
		cin >> r >> c;
		int ind = r * 3 + c;
		while (r > 2 || r < 0 || c>2 || c < 0 || state[ind] == 'x' || state[ind] == 'X' || state[ind] == 'o' || state[ind] == 'O')
		{
			cout << "Illegal move.  Try again : ";
			cin >> r >> c;
			ind = r * 3 + c;
		}
		state[ind] = 'o';
	}
	system("cls");
	drawBoard(state);
	return state;
}

string playAI(string state, bool xturn)
{
	int action = minimax(state, xturn);// assign action based on algorithm

	if (xturn)
	{
		state[action] = 'x';
	}
	else
	{
		state[action] = 'o';
	}
	system("cls");
	drawBoard(state);
	cout << "AI played : (" << action / 3 << ", " << action % 3 << ")\n";
	system("pause");
	return state;
}

string playAIab(string state, bool xturn)
{
	int action = minimaxAB(state, xturn);// assign action based on algorithm

	if (xturn)
	{
		state[action] = 'x';
	}
	else
	{
		state[action] = 'o';
	}
	system("cls");
	drawBoard(state);
	cout << "AI played : (" << action / 3 << ", " << action % 3 << ")\n";
	system("pause");
	return state;
}

/*********************************************************/

int minimax(string state, bool xturn)
{
	double val;
	int action;
	for (int i = 0; i < 9; i++)
		state[i] = tolower(state[i]);

	if (xturn)  //max players turn
		val = maxvalue(state, action);
	else
		val = minvalue(state, action);
	return action;
}

/*************Functions to be implemented by the students ********************************************/

int eval(string state) //given a state s, return 1 if x wins, -1 if o wins, 0  undetermined (may or may not be a leaf)
{
	int wins[8][3] = {
		{0,1,2}, // HW 1
		{3,4,5}, // HW 2
		{6,7,8}, // HW 3
		{0,3,6}, // VW 1
		{1,4,7}, // VW 2
		{2,5,8}, // VW 3
		{0,4,8}, // DW 1
		{2,4,6}  // DW 2
	};

	for (auto w : wins) // Checks all possible combinations of positions for wins
	{
		char a = tolower(state[w[0]]); 
		if (a != '.' && a != ' ' && a == tolower(state[w[1]]) && a == tolower(state[w[2]])) // If a win is found
		{
			if (a == 'x') return 1;
			else return -1;
		}
	}
	return 0; // No win found
}

double minvalue(string state, int& move) {

	if (state.find('.') == string::npos || eval(state) != 0) { // If the state is terminal
		if (state.find(' ') == string::npos) return eval(state);
	}

	double v = INT_MAX; // Sets initial value
	for (int a = 0; a < 9; a++) {
		if (state[a] == '.' || state[a] == ' ') { // Ignores filled spots
			state[a] = 'o'; // Sets new state
			int nextMove;
			double v2 = maxvalue(state, nextMove); // Calls max after placing an o
			state[a] = '.'; // Reverts back to try other states
			if (v2 < v) { // If the value returned is more minimized, take it
				v = v2;
				move = a;
			}
		}
	}
	return v;
}

double maxvalue(string state, int& move) {

	if (state.find('.') == string::npos || eval(state) != 0) { // If the state is terminal
		if (state.find(' ') == string::npos) return eval(state);
	}

	double v = INT_MIN; // Sets initial value
	for (int a = 0; a < 9; a++) {
		if (state[a] == '.' || state[a] == ' ') { // Ignores filled spots
			state[a] = 'x'; // Sets new state
			int nextMove;
			double v2 = minvalue(state, nextMove); // Calls min after placing an x
			state[a] = '.'; // Reverts back to try other states
			if (v2 > v) { // If the value returned is more maximized, take it
				v = v2;
				move = a;
			}
		}
	}
	return v;
}

int minimaxAB(string state, bool xturn)
{
	double val;
	int action;
	double alpha = INT_MIN; // Sets initial alpha and beta values
	double beta = INT_MAX;
	for (int i = 0; i < 9; i++)
		state[i] = tolower(state[i]);

	if (xturn)  //max players turn
		val = maxvalueAB(state, alpha, beta, action);
	else  //min players turn
		val = minvalueAB(state, alpha, beta, action);
	return action;
}

double minvalueAB(string state, double alpha, double beta, int& move) 
{
	if (state.find('.') == string::npos || eval(state) != 0) { // If state is terminal
		if (state.find(' ') == string::npos) return eval(state);
	}

	double v = INT_MAX; // Sets initial value
	for (int a = 0; a < 9; a++) {
		if (state[a] == '.' || state[a] == ' ') { // Ignores filled spots
			state[a] = 'o';
			int nextMove;
			double v2 = maxvalueAB(state, alpha, beta, nextMove); // Calls max after placing an o
			state[a] = '.';
			if (v2 < v) { // If the value returned is more minimized, take it
				v = v2;
				move = a;
				beta = min(beta, v); // Updates beta
			}
			if (v <= alpha) return v; // Prune the branch
		}
	}
	return v;
}

double maxvalueAB(string state, double alpha, double beta, int& move)
{
	if (state.find('.') == string::npos || eval(state) != 0) { // If state is terminal
		if (state.find(' ') == string::npos) return eval(state);
	}

	double v = INT_MIN; // Sets initial value
	for (int a = 0; a < 9; a++) {
		if (state[a] == '.' || state[a] == ' ') { // Ignores filled spots
			state[a] = 'x';
			int nextMove;
			double v2 = minvalueAB(state, alpha, beta, nextMove); // Calls min after placing an x
			state[a] = '.';
			if (v2 > v) { // If the value returned is more maximized, take it
				v = v2;
				move = a;
				alpha = max(alpha, v); // Updates alpha
			}
			if (v >= beta) return v; // Prune the branch
		}
	}
	return v;
}