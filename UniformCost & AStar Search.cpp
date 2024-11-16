/*
Consider the problem of finding the shortest path between two points on a plane that has convex polygonal obstacles.
This is an idealization of the problem that a robot has to solve to navigate in a crowded environment.
he vertices of the graph are the vertices of the polygons and the starting and goal positions.
The shortest path from one polygon vertex to any other in the scene must consist of straight-line segments joining
some of the vertices of the polygons.
You are given a file representing a graph. The vertices of the graph are the vertices of the polygons and the starting and goal positions.
The edges of the graph are all the above pairs of vertices that can be connected by a straight line without going through an obstacle.

File format:
Line 1:
two integers m, n (tab separated) representing the numer of vertices and the number of edges.
Lines 2 to m+1 (list of vertices):
each line has 3 numbers i, x, y (tab separated) : i is an integer, the vertex id; x, y are real numbers, the 'x' and 'y' coordinates of the vertex .
Lines m+2 to m+n+1 (list of edges):
each line has two integers (tab separated) representing the id's of the two nodes connected by the edge.
Example: Here is the file for a graph with 6 vertices and 9 edges
					6	9
					0	0.0	0.0
					1	76.75792122307887	162.26001485048116
					2	11.935677016534685	100.82187428764182
					3	58.69782922720505	14.222944063582222
					4	98.64308647185301	57.25001252255944
					5	100.0	200.0
					0	2
					0	3
					1	2
					1	4
					1	5
					2	3
					2	5
					3	4
					4	5
*/

#include <iostream>   //cin,cout
#include <fstream>   //files
#include <queue>     
#include <vector>
#include <stack>
#include <map>
#include <iomanip>


using namespace std;

struct nodeType    //structure of each node in the tree
{
	int state;          //the index of the vertex
	int parent;         //the state of the parent node
	double costG;          //g(n), path cost 
	double costH;          //h(n), estimated cost by heuristic from present to goal

	//overload the < operator so we can use it for the priority queue
	friend bool operator<(nodeType x, nodeType y)
	{
		return x.costG + x.costH > y.costG + y.costH;
	};
};

struct navigationGraph
{
	int size; //number of vertices
	double** adjM;  //adjacency matrix 
	vector<double> xloc;
	vector<double> yloc;
};


bool readGraphFromFile(string filename, navigationGraph& g1);  //given a file, populate the graph

vector<nodeType> Expand(nodeType curr, navigationGraph g1);
//given a current node curr and the graph, return the nodes you can reach from curr.
vector<nodeType> Solution(int currentId, int startId, map<int, nodeType> exploredNodes);
//Helper function: returns the solution path given the map of explored nodes by the algorithm


vector<nodeType> uniform_cost_search_Nav(navigationGraph g1, int start, int goal);  //uniform cost search between two nodes, returns a path
vector<nodeType> astar_search_Nav(navigationGraph g1, int start, int goal);  //A* search between two nodes, returns a path

int UC = 0; // Variables for counting the nodes visited
int AStar = 0;

//////////////////////////////////////////////////////////////

int main()
{
	string files[10] = { "P2x2m.txt", "P2x4s.txt","P3x3.txt","P3x4s.txt","P3x8s.txt",
		"P4x6.txt","P3x10.txt", "P5x10.txt", "P10x10.txt", "P15x10.txt", };
	string filename;
	navigationGraph g1;
	cout << setw(10) << "Filename" << setw(10) << "Cost" << setw(10) << "UC Size" << setw(10) << "A* Size" << "\tSolution Path (UC/A*)" << endl;
	for (int i = 0; i < 10; i++)
	{
		int su, sa;
		vector<int> pu, pa;
		filename = files[i];
		cout << setw(10) << filename;

		if (!readGraphFromFile(filename, g1))
			exit(1);
		int start = 0, goal = g1.size - 1;
		su = 0;
		sa = 0;
		vector<nodeType> pathu = uniform_cost_search_Nav(g1, start, goal);
		vector<nodeType> patha = astar_search_Nav(g1, start, goal);

		cout << fixed << setprecision(1);
		cout << setw(10) << pathu[0].costG << setw(10) << UC << setw(10) << AStar;

		for (auto x : pathu)
			pu.insert(pu.begin(), x.state);

		for (auto x : patha)
			pa.insert(pa.begin(), x.state);


		cout << setw(5) << " ";
		for (auto x : pu)
			cout << setw(4) << x;
		cout << endl;
		cout << setw(45) << " ";
		for (auto x : pa)
			cout << setw(4) << x;
		cout << endl;


	}
	system("pause");

	return 0;
}




/////////////////////////////////////////////////////////////////////////////////



bool readGraphFromFile(string filename, navigationGraph& g1)
{
	ifstream gfile(filename);

	if (!gfile)
	{
		cout << "Error opening file ..." << endl;
		return false;
	}

	int m, n;  //#of vertices, #of edges, 
	gfile >> m >> n;

	g1.size = m; //set the size of the graph

	//allocate memory for the adjacacy matrix
	g1.adjM = new double* [n];
	for (int i = 0; i < n; i++)
		g1.adjM[i] = new double[n] {0};

	int id; //vertex id
	double x, y;//x-y coordinates
	for (int i = 0; i < m; i++) //read the vertex information
	{
		gfile >> id >> x >> y;
		g1.xloc.push_back(x);
		g1.yloc.push_back(y);
	}

	int a, b; //read the edges and fill out the adjacency matrix

	for (int i = 0; i < n; i++)
	{
		gfile >> a >> b;
		g1.adjM[a][b] = 1;
		g1.adjM[b][a] = 1;
	}
	gfile.close();
	return true;
}

vector<nodeType> Expand(nodeType curr, navigationGraph g1)
{
	vector<nodeType> children;
	int id = curr.state; //the id of current node
	nodeType newnode;
	int numChildren = 0;
	newnode.parent = id;
	for (int i = 0; i < g1.size; i++)
	{
		if (g1.adjM[id][i] > 0)  //there is a connection between vertices id and i
		{
			double d = sqrt(pow(g1.xloc[id] - g1.xloc[i], 2) + pow(g1.yloc[id] - g1.yloc[i], 2));

			newnode.costG = curr.costG + d; //update the cost g(n)=cost of parent+ cost of transition
			newnode.state = i;
			newnode.costH = 0;    //the heuristic cost is assumed 0 (the informed searches will update this themselves)
			children.push_back(newnode);
		}
	}
	return children;
}


vector<nodeType> Solution(int currentId, int startId, map<int, nodeType> exploredNodes)
{
	vector<nodeType> path;

	nodeType curr = exploredNodes[currentId];
	path.push_back(curr);
	while (curr.parent >= 0)
	{
		curr = exploredNodes[curr.parent];
		path.push_back(curr);

	}
	return path;
}

vector<nodeType> uniform_cost_search_Nav(navigationGraph g1, int start, int goal)
{
	UC = 0; // variable for counting visited nodes
	priority_queue<nodeType>frontier; // initialize queue for storing all nodes 
	map<int, nodeType> reached; // initialize map for storing already visited paths
	nodeType curr = { start, -1, 0, -1}; // starting state

	frontier.push(curr);
	reached[curr.state] = curr;

	while (!frontier.empty())
	{
		curr = frontier.top(); frontier.pop();
		if (curr.state == goal)
		{
			return Solution(curr.state, start, reached); // return solution path
		}
		for (auto x : Expand(curr, g1)) // get all possible paths from current node
		{
			int s = x.state;
			if (reached.find(s) == reached.end() || x.costG < reached[s].costG) // Add the lowest cost nodes to the frontier
			{
				reached[s] = x;
				frontier.push(x);
			}
		}
		UC = reached.size();
	}
}

vector<nodeType> astar_search_Nav(navigationGraph g1, int start, int goal)
{
	AStar = 0; // Variable for counting visited nodes
	priority_queue<nodeType>frontier; // initialize queue for storing all nodes 
	map<int, nodeType> reached; // initialize map for storing already visited paths
	nodeType curr = { start, -1, 0, -1 }; // starting state

	frontier.push(curr);
	reached[curr.state] = curr;

	while (!frontier.empty())
	{
		curr = frontier.top(); frontier.pop();
		if (curr.state == goal)
		{
			return Solution(curr.state, start, reached); // return solution path
		}
		for (auto x : Expand(curr, g1)) // get all possible paths from current node
		{
			int s = x.state;
			x.costH = sqrt(pow(g1.xloc[goal] - g1.xloc[s], 2) + pow(g1.yloc[goal] - g1.yloc[s], 2)); // SLD Heuristic
			if (reached.find(s) == reached.end() || x.costG + x.costH < reached[s].costG + reached[s].costH) // Add the lowest cost nodes to the frontier
			{
				reached[s] = x;
				frontier.push(x);
			}
		}
		AStar = reached.size();
	}
}