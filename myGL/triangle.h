#pragma once

struct triangle
{
	double x[3], y[3], u[3];
	double c[3];
	int ready = 0;
	vector<double> X, Y, F;
	double A[3][3], det, S;
	double a[3], b[3], g[3];
	double K[3][3], L[3][3];
	triangle()
	{
		u[0] = 0;
		u[1] = 1;
		u[2] = 2;

		ready = 3;
		add(1, 1);
		add(5, 0);
		add(0, 4);
		setA();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cout << i << " " << j << " " << A[i][j] << endl;
	}
	void add(double x, double y)
	{
		X.push_back(x);
		Y.push_back(y);
	}

	void getPoint(double x_, double y_)
	{
		if (ready < 3)
		{
			X.push_back(x_);
			Y.push_back(y_);
			ready++;
		}
		if (ready == 3)
		{
			compCoef();
			cout << c[0] << " " << c[1] << " " << c[2] << endl;
		}
	}

	void clearVec()
	{
		X.clear();
		Y.clear();
		ready = 0;
	}

	void getVal(double* x_, double* y_, double* u_)
	{
		for (int i = 0; i < 3; i++)
		{
			x[i] = x_[i];
			y[i] = y_[i];
			u[i] = u_[i];
		}
	}

	void compCoef()
	{
		for (int i = 0; i < 3; i++)
		{
			x[i] = X[i];
			y[i] = Y[i];
		}
		double det = (x[0] * y[1] - x[0] * y[2] - x[1] * y[0] + x[1] * y[2] + x[2] * y[0] - x[2] * y[1]);
		c[0] = (u[0] * x[1] * y[2] - u[0] * x[2] * y[1] - u[1] * x[0] * y[2]
			+ u[1] * x[2] * y[0] + u[2] * x[0] * y[1] - u[2] * x[1] * y[0]) / det;
		c[1] = (u[0] * y[1] - u[0] * y[2] - u[1] * y[0] + u[1] * y[2] + u[2] * y[0] - u[2] * y[1]) / det;
		c[2] = -(u[0] * x[1] - u[0] * x[2] - u[1] * x[0] + u[1] * x[2] + u[2] * x[0] - u[2] * x[1]) / det;
	}
	void compCoef2()
	{
		c[0] = A[0][0] * u[0] + A[0][1] * u[1] + A[0][2] * u[2];
		c[1] = A[1][0] * u[1] + A[1][1] * u[1] + A[1][2] * u[2];
		c[2] = A[2][0] * u[2] + A[2][1] * u[1] + A[2][2] * u[2];
	}
	void check(double x, double y)
	{
		cout << "U = " << U(x, y) << endl;
		cout << "Sum_phi = " << phi(x, y, 0) << " " << phi(x, y, 1) << " " << phi(x, y, 2) << endl;
		cout << "Sum_dx = " << b[0] << " " << b[1] << " " << b[2] << endl;
		cout << "Sum_dy = " << g[0] << " " << g[1] << " " << g[2] << endl;
		cout << "Det = " << det << endl;
		cout << "Area = " << S << endl;
	}

	void setA()
	{
		for (int i = 0; i < 3; i++)
		{
			x[i] = X[i];
			y[i] = Y[i];
		}
		det = (x[0] * y[1] - x[0] * y[2] - y[0] * x[1] + x[1] * y[2] + x[2] * y[0] - y[1] * x[2]);
		S = abs(0.5 * det);
		//скобки в det как олень сгруппировал
		a[0] = A[0][0] = (x[1] * y[2] - y[1] * x[2]) / det;
		a[1] = A[0][1] = -(x[0] * y[2] - x[2] * y[0]) / det;
		a[2] = A[0][2] = (x[0] * y[1] - y[0] * x[1]) / det;
		b[0] = A[1][0] = (-y[2] + y[1]) / det;
		b[1] = A[1][1] = -(-y[2] + y[0]) / det;
		b[2] = A[1][2] = (-y[1] + y[0]) / det;
		g[0] = A[2][0] = -(-x[2] + x[1]) / det;
		g[1] = A[2][1] = (-x[2] + x[0]) / det;
		g[2] = A[2][2] = -(-x[1] + x[0]) / det;
	}

	void compIntegral()
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
			{
				K[i][j] = S * (b[i] * b[j] + g[i] * g[j]);
				L[i][j] = S * (u[0] + u[1] + u[2]) / 3.0;
			}
		}
	}


	double phi(double x, double y, int i)
	{
		return a[i] + b[i] * x + g[i] * y;
	}
	double U(double x, double y)
	{
		return u[0] * phi(x, y, 0) + u[1] * phi(x, y, 1) + u[2] * phi(x, y, 2);
	}



};
