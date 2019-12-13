#include <iostream>
#include <string>
#include <queue>
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
{
	queue<Coord> c;
	c.push(Coord(sr, sc));
	maze[sr][sc] = 'A';
	while (!c.empty())
	{
		Coord curr = c.front();
		c.pop();
		int cr = curr.r();
		int cc = curr.c();
		if (cr == er && cc == ec)
			return true;
		if (maze[cr + 1][cc] == '.')
		{
			c.push(Coord(cr + 1, cc));
			maze[cr + 1][cc] = 'A';
		}
		if (maze[cr][cc - 1] == '.')
		{
			c.push(Coord(cr, cc - 1));
			maze[cr][cc - 1] = 'A';
		}
		if (maze[cr - 1][cc] == '.')
		{
			c.push(Coord(cr - 1, cc));
			maze[cr - 1][cc] = 'A';
		}
		if (maze[cr][cc + 1] == '.')
		{
			c.push(Coord(cr, cc + 1));
			maze[cr][cc + 1] = 'A';
		}
	}
	return false;
}
