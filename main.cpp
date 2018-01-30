#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stack>
#include <ostream>

using namespace std;

struct Vertex {	
	// position
	int r;
	int c;
	int l;

	char dir;

	Vertex *prev;
	Vertex *curr;

	vector<Vertex> adj_list;

	// value
	string sequence;

	// status indicators
	bool visited;
	bool start;
	bool end;
};

bool completed;
vector<char> outputs;

void DFS(int l, int r, int c, int endL, int endR, int endC, vector < vector < vector <Vertex> > > maze, char dir) {
	maze[l][r][c].visited = true;
	//if this vertex is finish, then the spider's done
	if ((l == endL) && (r == endR) && (c == endC)) {
		completed = true;
	}

	// the spider keeps going until it WINS
	while (!((l == endL) && (r == endR) && (c == endC)) && !completed) {
		//how the spider checks out its options
		for (int i = 0; i < maze[l][r][c].adj_list.size(); i++) {
			//make sure previous vertex is marked as visited
			if (dir == 'N' && maze[l][r][c].adj_list[i].dir == 'S') {
				maze[l][r][c].adj_list[i].visited = true;
			}
			else if (dir == 'S' && maze[l][r][c].adj_list[i].dir == 'N') {
				maze[l][r][c].adj_list[i].visited = true;
			}
			else if (dir == 'E' && maze[l][r][c].adj_list[i].dir == 'W') {
				maze[l][r][c].adj_list[i].visited = true;
			}
			else if (dir == 'W' && maze[l][r][c].adj_list[i].dir == 'E') {
				maze[l][r][c].adj_list[i].visited = true;
			}
			else if (dir == 'U' && maze[l][r][c].adj_list[i].dir == 'D') {
				maze[l][r][c].adj_list[i].visited = true;
			}
			else if (dir == 'D' && maze[l][r][c].adj_list[i].dir == 'U') {
				maze[l][r][c].adj_list[i].visited = true;
			}

			//the spider hit a dead end!!!
			if (completed) {
				break;
			}
			else if (!maze[l][r][c].adj_list[i].visited) {	
				// otherwise, spider checks out other paths it hasn't taken yet
				maze[l][r][c].adj_list[i].visited = true;
				outputs.push_back(maze[l][r][c].adj_list[i].dir);
				DFS(maze[l][r][c].adj_list[i].l, maze[l][r][c].adj_list[i].r, maze[l][r][c].adj_list[i].c, endL, endR, endC, maze, maze[l][r][c].adj_list[i].dir);
			}
		}

		// the spider's S.O.L. and needs to backtrack
		if (!completed) {
			outputs.pop_back();
		}
		break;
	}
}

void solveMaze() {
	ifstream infile;
	ofstream outfile;

	infile.open("input.txt");
	outfile.open("output.txt");

	// input.txt can't be found
	if (!infile.is_open()) {
		cout << "THE SPIDER HAS MISPLACED INPUT.TXT!!!" << endl;
		return;
	}

	// input.txt found, save data and perform calculations
	if (infile.is_open()) {
		// important variables
		string junk;
		int numInstances;

		int levels;
		int rows;
		int cols;

		int startingLvl;
		int startingRow;
		int startingCol;

		int endingLvl;
		int endingRow;
		int endingCol;

		// organize what's needed
		infile >> numInstances;
		getline(infile, junk);
		
		// now calculations are performed for each maze

		cout << "Number of Mazes: " << numInstances << endl;
		for (int instCtr = 0; instCtr < numInstances; instCtr++) {
			cout << "\nMaze Number: " << instCtr + 1 << endl;
			
			infile >> levels;
			infile >> rows;
			infile >> cols;
			getline(infile, junk);

			// let's make a maze for the spider to solve!
			vector <vector <vector <Vertex> > > Maze(levels, vector < vector < Vertex > >(rows, vector< Vertex >(cols)));
			
			// set starting and ending locations
			infile >> startingLvl;
			infile >> startingRow;
			infile >> startingCol;
			getline(infile, junk);

			infile >> endingLvl;
			infile >> endingRow;
			infile >> endingCol;
			getline(infile, junk);

			// initialize the important things we can do something about right now
			int ctr = 0;
			for (int l = 0; l < levels; l++) {
				for (int r = 0; r < rows; r++) {
					for (int c = 0; c < cols; c++) {
						infile >> Maze[l][r][c].sequence;
						cout << Maze[l][r][c].sequence << " ";
						Maze[l][r][c].visited = false;
						Maze[l][r][c].start = false;
						Maze[l][r][c].end = false;
						Maze[l][r][c].l = l;
						Maze[l][r][c].r = r;
						Maze[l][r][c].c = c;
						Maze[l][r][c].adj_list.resize(0);
						Maze[l][r][c].prev = NULL;
					}
					getline(infile, junk);
					cout << endl;
				}
			}
			getline(infile, junk);
			cout << endl << endl;

			Maze[startingLvl][startingRow][startingCol].start = true;
			Maze[endingLvl][endingRow][endingCol].end = true;
			/*
			Connect each valid pathway with an edge. 
			Each pass through an individual cell's determines its legal directions.
			each legal direction is assigned to an adjacency list.
			*/
			for (int z = 0; z < levels; z++) {
				for (int x = 0; x < rows; x++) {
					for (int y = 0; y < cols; y++) {
						// check each direction
						// if true, save direction into the list 
						int ctr = 0;
						//north						
						if (Maze[z][x][y].sequence[0] == '1') {	
							Maze[z][x][y].adj_list.push_back(Maze[z][x - 1][y]);
							Maze[z][x][y].adj_list[0].prev = &Maze[z][x][y];
							Maze[z][x][y].adj_list[0].dir = 'N';
							Maze[z][x][y].adj_list[0].prev->dir = 'S';
							cout << Maze[z][x][y].adj_list[0].dir;
							ctr++;
						}
						
						//east						
						if (Maze[z][x][y].sequence[1] == '1') {
							Maze[z][x][y].adj_list.push_back(Maze[z][x][y + 1]);
							Maze[z][x][y].adj_list[ctr].prev = &Maze[z][x][y];
							Maze[z][x][y].adj_list[ctr].dir = 'E';
							Maze[z][x][y].adj_list[ctr].prev->dir = 'W';
							cout << Maze[z][x][y].adj_list[ctr].dir;
							ctr++;
						}
						
						//south						
						if (Maze[z][x][y].sequence[2] == '1') {
							Maze[z][x][y].adj_list.push_back(Maze[z][x + 1][y]);
							Maze[z][x][y].adj_list[ctr].prev = &Maze[z][x][y];
							Maze[z][x][y].adj_list[ctr].dir = 'S';
							Maze[z][x][y].adj_list[ctr].prev->dir = 'N';
							cout << Maze[z][x][y].adj_list[ctr].dir;
							ctr++;
						}
						
						//west						
						if (Maze[z][x][y].sequence[3] == '1') {
							Maze[z][x][y].adj_list.push_back(Maze[z][x][y - 1]);
							Maze[z][x][y].adj_list[ctr].prev = &Maze[z][x][y];
							Maze[z][x][y].adj_list[ctr].dir = 'W';
							Maze[z][x][y].adj_list[ctr].prev->dir = 'E';
							cout << Maze[z][x][y].adj_list[ctr].dir;
							ctr++;
						}
						
						//up					
						if (Maze[z][x][y].sequence[4] == '1') {
							Maze[z][x][y].adj_list.push_back(Maze[z + 1][x][y]);
							Maze[z][x][y].adj_list[ctr].prev = &Maze[z][x][y];
							Maze[z][x][y].adj_list[ctr].dir = 'U';
							Maze[z][x][y].adj_list[ctr].prev->dir = 'D';
							cout << Maze[z][x][y].adj_list[ctr].dir;
							ctr++;
						}
						
						//down						
						if (Maze[z][x][y].sequence[5] == '1') {					
							Maze[z][x][y].adj_list.push_back(Maze[z - 1][x][y]);							
							Maze[z][x][y].adj_list[ctr].prev = &Maze[z][x][y];
							Maze[z][x][y].adj_list[ctr].dir = 'D';
							Maze[z][x][y].adj_list[ctr].prev->dir = 'U';
							cout << Maze[z][x][y].adj_list[ctr].dir;
							ctr++;
						}	
						cout << " ";
						ctr = 0;
					}
					cout << endl;
				}
			}

			// solve maze here
			// DFS
			// put each direction to outfile

			//DFS(startingLvl, startingRow, startingCol, Maze);
			DFS(startingLvl, startingRow, startingCol, endingLvl, endingRow, endingCol, Maze, 'J');

			//write path to output
			for (int i = 0; i < outputs.size(); i++)
			{
				outfile << outputs[i] << " ";
			}
			outfile << endl;
			outputs.clear();
			completed = false;

		}
	}

	// file is formatted incorrectly
	if (!infile.eof()) {
		cout << endl << "ERROR: THE SPIDER RUINED THE CONTENTS OF INPUT.TXT!!!\n" << endl;
	}

	// success! time to leave
	
	infile.close();
	outfile.close();
	cout << "\nHA HA HA!! The spider wins again!!!" << endl;
}

int main() {
	cout << "Analysis of Algorithms" << endl;
	cout << "***** Project 3 *****\n" << endl;
	cout << "Spider's Revenge:" << endl;
	cout << "Solving a 3D Maze\n" << endl;
	cout << "by Jon Gierzak\n" << endl;

	solveMaze();

	cin.get();
	return 0;
}

