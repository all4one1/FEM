#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
using std::cout;
using std::endl;
using std::ofstream;

struct IterativeSolver
{
	int k = 0, write_i = 0;
	double eps_iter = 1e-6;
	ofstream w;
	IterativeSolver()
	{

	}
	void solveGS(double* f, double* f0, double* bb, int NN, double** M)
	{
		k = 0;
		for (k = 0; k < 100; k++)
		{

			auto line = [NN, M](int q, double* y)
			{
				double l = 0;
				for (int j = 0; j < NN; j++)
					l += M[q][j] * y[j];
				return l;
			};
			double s = 0;
			for (int j = 0; j < NN; j++)
			{
				s = line(j, f);
				f[j] = f[j] + (bb[j] - s) / M[j][j];
			}


			double max = 0;
			double dif;
			for (int i = 0; i < NN; i++)
			{
				dif = abs(f0[i] - f[i]);
				if (dif > max)
					max = dif;
			}
			for (int j = 0; j < NN; j++)
				f0[j] = f[j];

			if (max < eps_iter)	break;
		}
	}
	void solveJacobi(double* f, double* f0, double* bb, int NN, double** M)
	{
		k = 0;
		for (k = 0; k < 200; k++)
		{

			auto line = [NN, M](int q, double* y)
			{
				double l = 0;
				for (int j = 0; j < NN; j++)
					l += M[q][j] * y[j];
				return l;
			};
			double s = 0;
			for (int j = 0; j < NN; j++)
			{
				s = line(j, f0);
				f[j] = f0[j] + (bb[j] - s) / M[j][j];
			}


			double max = 0;
			double dif;
			for (int i = 0; i < NN; i++)
			{
				dif = abs(f0[i] - f[i]);
				if (dif > max)
					max = dif;
			}
			for (int j = 0; j < NN; j++)
				f0[j] = f[j];

			if (max < eps_iter)	break;
		}
	}



	void write()
	{
		if (write_i == 0) w.open("iter_solver.dat");
		w << " " << k << endl;
		write_i++;
	}
	void auto_test()
	{
		double A[6][6] =
		{
			{ 30,3,4,0,0,0 },
			{ 4,22,1,3,0,0 },
			{ 5,7,33,6,7,0 },
			{ 0,1,2,42,3,3 },
			{ 0,0,2,11,52,2 },
			{ 0,0,0,3,9,26 },
		};
		double b[6] = { 1, 2, 3, 3, 2, 1 };
		int n = 6;
		double* x = new double[6];
		double* x0 = new double[6];
		double** SM;
		SM = new double* [n];
		for (int i = 0; i < n; i++)
			SM[i] = new double[n];

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				SM[i][j] = A[i][j];

		for (int i = 0; i < n; i++)
		{
			x[i] = x0[i] = 0;
		}

		solveGS(x, x0, b, n, SM);

		double cg[6];
		cg[0] = 0.1826929218e-1;
		cg[1] = 0.7636750835e-1;
		cg[2] = 0.5570467736e-1;
		cg[3] = 0.6371099009e-1;
		cg[4] = 0.2193724104e-1;
		cg[5] = 0.2351661001e-1;
		cout << "x should be: ";
		for (int i = 0; i < n; i++)
			cout << cg[i] << " ";
		cout << endl << "auto test:   ";
		for (int i = 0; i < n; i++)
			cout << x[i] << " ";
		cout << endl;

		cout << "b should be: 1 2 3 3 2 1" << endl;
		cout << "auto test:   ";
		for (int i = 0; i < n; i++)
		{
			double s = 0;
			for (int j = 0; j < n; j++)
			{
				s += A[i][j] * x[j];
			}
			cout << s << " ";
		} cout << endl;
	}

};
