#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ofstream;

enum PointType
{
	inside,
	bc1,
	bc2
};

struct Point
{
	double x, y;
	double Tin;
	char t;
	Point(double x_, double y_, int t_) : x(x_), y(y_), t(t_)
	{};
	Point()
	{
		x = y = 0;
		t = 0;
	};
	static double distance(Point &P1, Point &P2)
	{
		return sqrt(pow(P2.x - P1.x, 2) + pow(P2.y - P1.y, 2));
	}
};
struct BoundaryEdge
{
	vector<double> flux;
	vector<int> i1;
	vector<int> i2;
	char n = 0;
	void add(double f, int i1_, int i2_, double s)
	{
		i1.push_back(i1_);
		i2.push_back(i2_);
		n++;
		flux.push_back(f * s * 0.5);
	}
	void fluxToB(double *b)
	{
		for (int k = 0; k < n; k++)
		{
			b[i1[k]] += flux[k];
			b[i2[k]] += flux[k];
		}
	}
};

struct Element
{
	vector<Point> &mesh;
	double A[3][3], det, S;
	double a[3], b[3], g[3];
	double K[3][3], L[3][3];
	int n[3];
	char type = 0;
	int id;
	string debug[3][3];
	BoundaryEdge* edge;

	#define M0 mesh[n[0]]
	#define M1 mesh[n[1]]
	#define M2 mesh[n[2]]

	Element(vector<Point>& p);

	Element(vector<Point>& p, int i, int j, int k, int id_);

	void addFlux(double q, int i1, int i2);

	void fluxToB(double* b);

	void setIndex(int i, int j, int k);

	void computeK();
	void computeL(double coef = 1);

	double phi(double x, double y, int i);
	double sumPhi(double x, double y);
	bool insideCheck(double x, double y);

	double getInterpolatedValue(double x, double y, double* f);

	void printPhi();
	void coefsDebug();
};


