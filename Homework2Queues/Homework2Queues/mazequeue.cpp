#include <queue>
#include <string> //change pls

using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
	queue<Coord> temp;
	temp.push(Coord(sr, sc));
	maze[sr][sc] = 'X';
	int currentr;
	int currentc;
	while (!temp.empty())
	{
		Coord currentCoord = temp.front();
		currentr = currentCoord.r();
		currentc = currentCoord.c();
		temp.pop();
		if (currentr == er && currentc == ec) 
		{
			return true;
		}
		if (currentr + 1 < nRows && maze[currentr + 1][currentc] == '.')
		{
			temp.push(Coord(currentr + 1, currentc));
			maze[currentr + 1][currentc] = 'X';
		}
		if (currentc - 1 >= 0 && maze[currentr][currentc - 1] == '.')
		{
			temp.push(Coord(currentr, currentc - 1));
			maze[currentr][currentc - 1] = 'X';
		}
		if (currentr - 1 >= 0 && maze[currentr - 1][currentc] == '.')
		{
			temp.push(Coord(currentr - 1, currentc));
			maze[currentr - 1][currentc] = 'X';
		}
		if (currentc + 1 < nCols && maze[currentr][currentc + 1] == '.')
		{
			temp.push(Coord(currentr, currentc + 1));
			maze[currentr][currentc + 1] = 'X';
		}
	}
	return false;
}

/*Push the starting coordinate(sr, sc) onto the coordinate stack and
update maze[sr][sc] to indicate that the algorithm has encountered
it(i.e., set maze[sr][sc] to have a value other than '.').
While the stack is not empty,
{ Pop the top coordinate off the stack.This gives you the current
		(r,c) location that your algorithm is exploring.
	If the current(r,c) coordinate is equal to the ending coordinate,
		then we've solved the maze so return true!
	Check each place you can move from the current cell as follows :
		If you can move SOUTH and haven't encountered that cell yet,
			then push the coordinate(r + 1,c) onto the stack and update
			maze[r + 1][c] to indicate the algorithm has encountered it.
		If you can move WEST and haven't encountered that cell yet,
			then push the coordinate(r,c - 1) onto the stack and update
			maze[r][c - 1] to indicate the algorithm has encountered it.
		If you can move NORTH and haven't encountered that cell yet,
			then push the coordinate(r - 1,c) onto the stack and update
			maze[r - 1][c] to indicate the algorithm has encountered it.
		If you can move EAST and haven't encountered that cell yet,
			then push the coordinate(r,c + 1) onto the stack and update
			maze[r][c + 1] to indicate the algorithm has encountered it.
}
There was no solution, so return false */

