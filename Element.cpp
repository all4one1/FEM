#include "Element.h"



Element::Element(vector<Point>& p) 
	: mesh(p)
{
}

Element::Element(vector<Point>& p, int i, int j, int k, int id_)
	: mesh(p), n{ i,j,k }, id(id_)
{

}

void Element::fluxToB(double* b)
{
	this->edge->fluxToB(b);
}

void Element::addFlux(double q, int i1, int i2)
{
	if (type == 0)
	{
		edge = new BoundaryEdge;
		type = 1;
	}
	double L = Point::distance(mesh[i1], mesh[i2]);
	edge->add(q, i1, i2, L);
}


void Element::setIndex(int i, int j, int k)
{
	n[0] = i;
	n[1] = j;
	n[2] = k;
}

void Element::computeK()
{
	det = (M0.x * M1.y - M0.x * M2.y - M0.y * M1.x + M1.x * M2.y + M2.x * M0.y - M1.y * M2.x);
	S = abs(0.5 * det);

	//скобки в det как олень сгруппировал
	a[0] = A[0][0] = (M1.x * M2.y - M1.y * M2.x) / det;
	a[1] = A[0][1] = -(M0.x * M2.y - M2.x * M0.y) / det;
	a[2] = A[0][2] = (M0.x * M1.y - M0.y * M1.x) / det;
	b[0] = A[1][0] = (-M2.y + M1.y) / det;
	b[1] = A[1][1] = -(-M2.y + M0.y) / det;
	b[2] = A[1][2] = (-M1.y + M0.y) / det;
	g[0] = A[2][0] = -(-M2.x + M1.x) / det;
	g[1] = A[2][1] = (-M2.x + M0.x) / det;
	g[2] = A[2][2] = -(-M1.x + M0.x) / det;

	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
		{
			K[i][j] = S * (b[i] * b[j] + g[i] * g[j]);
			//L[i][j] = S * (p[0]->u + p[1]->u + p[2]->u) / 3.0;
		}
	}
	//coefsDebug();
}

void Element::computeL(double coef)
{
	if (S == 0) { cout << "S, Error" << endl; system("pause"); }
	//page 312 in Reddy's book
	double X_ = (M0.x + M1.x + M2.x) / 3.0;
	double Y_ = (M0.y + M1.y + M2.y) / 3.0;
	double I20 = S / 12.0 * (M0.x * M0.x + M1.x * M1.x + M2.x * M2.x + 9.0 * X_);
	double I02 = S / 12.0 * (M0.y * M0.y + M1.y * M1.y + M2.y * M2.y + 9.0 * Y_);
	double I11 = S / 12.0 * (M0.x * M0.y + M1.x * M1.y + M2.x * M2.y + 9.0 * X_ * Y_);

	auto S_00 = [this, I20, I02, I11, X_, Y_](int i, int j)
	{
		return (a[i] * a[j]
			+ (a[i] * b[j] + a[j] * b[i]) * X_
			+ (a[i] * g[j] + a[j] * g[i]) * Y_)
			+ (I11 * (g[i] * b[j] + g[j] * b[i])
				+ I20 * b[i] * b[j]
				+ I02 * g[i] * g[j]) / (S);
	};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
		{
			L[i][j] = coef*S_00(i, j);
		}
	}
}


double Element::phi(double x, double y, int i)
{
	return a[i] + b[i] * x + g[i] * y;
}

double Element::sumPhi(double x, double y)
{
	return phi(x, y, 0) + phi(x, y, 1) + phi(x, y, 2);
}

bool Element::insideCheck(double x, double y)
{
	if (phi(x, y, 0) < 0) return false;
	if (phi(x, y, 1) < 0) return false;
	if (phi(x, y, 2) < 0) return false;
	return true;
}

double Element::getInterpolatedValue(double x, double y, double* f)
{
	double v = 0;
	for (int i = 0; i < 3; i++)
		v += f[n[i]] * phi(x, y, i);
	return v;
}


void Element::printPhi()
{
	auto toStr = [](double f)
	{
		std::stringstream ss; ss << f; return ss.str();
	};
	auto toStrI = [](int f)
	{
		std::stringstream ss; ss << f; return ss.str();
	};
	for (int i = 0; i < 3; i++)
	{
		string sx, sy, s0;
		s0 = "phi[" + toStrI(n[i]) + "] = ";
		if (a[i] != 0) s0 += toStr(a[i]) + " ";

		if (b[i] > 0) sx = "+" + toStr(b[i]) + "x";
		else if (b[i] < 0) sx = toStr(b[i]) + "x";
		else sx = "";
		sx.append(" ");

		if (g[i] > 0) sy = "+" + toStr(g[i]) + "y";
		else if (g[i] < 0) sy = toStr(g[i]) + "y";
		else sy = "";

		cout << s0 + sx + sy << endl;
	}
}


void Element::coefsDebug()
{
	auto toStr = [](double f)
	{
		std::stringstream ss; ss << f; return ss.str();
	};
	auto toStrI = [](int f)
	{
		std::stringstream ss; ss << f; return ss.str();
	};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
		{
			int I = n[i];
			int J = n[j];
			debug[i][j] = toStrI(id) + "K_" + toStrI(I) + "," + toStrI(J) + "=" + toStr(K[i][j]);
		}
	}
}