#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "../myLibs/Sparse/SparseMatrix.h"
#include "IterativeSolver.h"
#include "Element.h"
using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::ofstream;

struct FEM
{
	vector<Element> E;
	vector<Point> mesh;
	vector<double> value;
	int Np = 0, Ne = 0;
	//Np = Number of points
	//Ne = Number of elements
	double** K, **L, * b, *bt, * bs, * T, * T0, * v;
	int iter = 0;
	double tau, timeq;
	string** D;
	SparseMatrix SM;
	IterativeSolver LinSys;

	FEM();

	void newMeshPoint(double x, double y, int t = 0);
	void finalizeMeshMaking();
	void initMeshValues();

	void newElement();
	void newElement(int i, int j, int k, double FluxEdge01 = 0, double FluxEdge02 = 0, double FluxEdge12 = 0);
	void readElementsAndMesh();
	void readBoundaryConditions();
	void addFluxToEdge(int p1, int p2, double q, int id = -1);
	void addFixedEdge(int p1, int p2);

	void computeAllCoefs();


	void makeStiffnessMatrix();

	void temporalRHS(double *b);

	void removeFixedValues();

	void formRHS();

	void solve();
	void swap();



	double getX(int I, int i);
	double getY(int I, int i);
	double getValue(int I, int i);

	void findMaxMin(double &max, double &min);

	int findElementByPoint(double x, double y);
	double getInterpolatedValue(double x, double y);

	void writeData();

	void checkStiffnessMatrix(int t = 0);
	void checkStiffnessMatrix2();

	void testRectangle();
	void testTemperature();
	void statistics();
private:
	void allocation();

};