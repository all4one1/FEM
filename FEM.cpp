#include "FEM.h"

FEM::FEM()
{

}

void FEM::newMeshPoint(double x, double y, int t)
{
	mesh.emplace_back(x, y, t);
	Np++;
}
void FEM::finalizeMeshMaking()
{
	allocation();
}

void FEM::initMeshValues()
{
	for (int i = 0; i < Np; i++)
	{
		double x = mesh[i].x;
		double y = mesh[i].y;
		//T[i] = y;
		//v[i] = (1 - y)*10;
		v[i] = 0;
		if (mesh[i].t == 1) v[i] = mesh[i].Tin;
		T0[i] = T[i] = 0;
	}
}

void FEM::newElement()
{
	E.emplace_back(mesh);
	Ne++;
}
void FEM::newElement(int i, int j, int k, double FluxEdge01, double FluxEdge02, double FluxEdge12)
{
	E.emplace_back(mesh, i, j, k, Ne);
	Ne++;
	if (FluxEdge01 != 0) E.back().addFlux(FluxEdge01, i, j);
	if (FluxEdge02 != 0) E.back().addFlux(FluxEdge02, i, k);
	if (FluxEdge12 != 0) E.back().addFlux(FluxEdge12, j, k);
}

void FEM::readElementsAndMesh()
{
	std::ifstream vertexfile("..\\MeshTriangleDraw\\verticesSaved.dat");
	std::ifstream indexfile("..\\MeshTriangleDraw\\triangleIndicesSaved.dat");
	if (!vertexfile.is_open() || !indexfile.is_open())
	{
		cout << "files for reading are not opened " << endl;
		return;
	}
	std::stringstream ss;
	std::string str;
	
	double x, y;
	getline(vertexfile, str); //skip first line
	while (std::getline(vertexfile, str))
	{
		ss.clear();	ss << str;
		ss >> x;
		ss >> y;
		newMeshPoint(x, y);
	}
	finalizeMeshMaking();

	int i, j, k;
	getline(indexfile, str); //skip first line
	while (std::getline(indexfile, str))
	{
		ss.clear();	ss << str;
		ss >> i;
		ss >> j;
		ss >> k;
		newElement(i, j, k);
	}

}
void FEM::readBoundaryConditions()
{
	std::ifstream bc1("..\\MeshTriangleDraw\\bc1Saved.dat");
	std::ifstream bc2("..\\MeshTriangleDraw\\bc2Saved.dat");
	if (!bc1.is_open() || !bc2.is_open())
	{
		cout << "files for reading are not opened " << endl;
		return;
	}
	std::stringstream ss;
	std::string str;

	int i, i1, i2;
	double f, flux;
	getline(bc1, str); //skip first line
	while (std::getline(bc1, str))
	{
		ss.clear();	ss << str;
		ss >> i;
		ss >> f;
		mesh[i].t = 1;
		mesh[i].Tin = f;
	}

	getline(bc2, str); //skip first line
	while (std::getline(bc2, str))
	{
		ss.clear();	ss << str;
		ss >> i1;
		ss >> i2;
		ss >> flux;
		//newElement(i, j, k);
	}
}

void FEM::addFluxToEdge(int p1, int p2, double q, int id)
{
	if (id == -1) id = Ne - 1;
	E[id].addFlux(q, p1, p2);
}

void FEM::addFixedEdge(int p1, int p2)
{
	mesh[p1].t = 1;
	mesh[p2].t = 1;
}



void FEM::computeAllCoefs()
{
	for (unsigned int i = 0; i < E.size(); i++)
	{
		E[i].computeK();
		E[i].computeL(1.0/tau);
	}
}
void FEM::makeStiffnessMatrix()
{
	for (int l = 0; l < Ne; l++)
	{
		for (int i = 0; i < 3; i++)
		{
			int I = E[l].n[i];

			for (int j = 0; j < 3; j++)
			{
				int J = E[l].n[j];
				K[I][J] += E[l].K[i][j] + E[l].L[i][j];
				L[I][J] += E[l].L[i][j];
			}
		}
	}
}

void FEM::temporalRHS(double *b)
{
	for (int l = 0; l < Ne; l++)
	{
		for (int i = 0; i < 3; i++)
		{
			int I = E[l].n[i];
			double f = 0;
			for (int j = 0; j < 3; j++)
			{
				int J = E[l].n[j];
				b[J] += E[l].L[i][j] * T[J];
			}
		}
	}
}



void FEM::removeFixedValues()
{
	for (int I = 0; I < Np; I++)
	{
		if (mesh[I].t == 1)
		{
			bs[I] = v[I];
			continue;
		}
		for (int J = 0; J < Np; J++)
		{
			if (mesh[J].t == 1 && J != I)
			{

				bs[I] = bs[I] - K[I][J] * v[J];
			}
		}
	}


	for (int I = 0; I < Np; I++)
	{
		if (mesh[I].t == 1)
		{
			for (int J = 0; J < Np; J++)
			{
				if (I != J)
				{
					K[I][J] = 0;
					K[J][I] = 0;
				}
				else K[I][I] = 1;
			}
		}

	}
}


void FEM::formRHS()
{
	for (int i = 0; i < Np; i++)
		bt[i] = 0;

	for (int I = 0; I < Np; I++)
	{
		if (mesh[I].t == 1)
		{
			//for those which are fixed 
			//bs[I] = v[I];
		}
		else
		{
			double s = 0;
			for (int J = 0; J < Np; J++)
			{
				s += L[I][J] * T[J];
			}
			bt[I] += s;
		}
	}


	for (int l = 0; l < Ne; l++)
	{
		int elementType = E[l].type;
		
		if (elementType == 0)
		{
			
		}
		if (elementType == 1)
		{
			E[l].fluxToB(bt);
		}
	}

	for (int i = 0; i < Np; i++)
		b[i] = bs[i] + bt[i];

	
}

void FEM::solve()
{
	iter++;
	timeq += tau;


	formRHS();
	LinSys.solveGS(T, T0, b, Np, K);
	//LinSys.solveJacobi(T, T0, b, Np, K);

	swap();
}

void FEM::swap()
{
	for (int i = 0; i < Np; i++)
	{
		T0[i] = T[i];
	}
}

double FEM::getX(int I, int i)
{
	return E[I].mesh[E[I].n[i]].x;
}

double FEM::getY(int I, int i)
{
	return E[I].mesh[E[I].n[i]].y;
}

double FEM::getValue(int I, int i)
{
	return T[E[I].n[i]];
}

void FEM::findMaxMin(double& max, double& min)
{
	max = min = T[0];
	for (int i = 0; i < Np; i++)
	{
		if (T[i] > max) max = T[i];
		if (T[i] < min) min = T[i];
	}
}

int FEM::findElementByPoint(double x, double y)
{
	for (int I = 0; I < Ne; I++)
	{
		if (E[I].insideCheck(x, y) == true) return I;
	}
	return -1;
}

double FEM::getInterpolatedValue(double x, double y)
{
	int I = findElementByPoint(x, y);
	if (I != -1) return	E[I].getInterpolatedValue(x, y, T);
	else return NAN;
}

void FEM::writeData()
{
	ofstream w("field.dat");

	for (int l = 0; l < Np; l++)
	{
		double x = mesh[l].x;
		double y = mesh[l].y;
		double f = T[l];
		w << x << " " << y << " " << f << endl;
	}

}

void FEM::checkStiffnessMatrix(int t)
{
	ofstream w("K_matrix.dat");
	ofstream rhs("rhs.dat");
	w << std::fixed << std::setprecision(2);

	if (t == 0)
	{
		for (int I = 0; I < Np; I++)
		{
			for (int J = 0; J < Np; J++)
			{
				w << K[I][J] << " ";
			}
			w << endl;
		}
	}

	if (t == 1)
	{
		for (int I = 0; I < Np; I++)
		{
			for (int J = 0; J < Np; J++)
			{
				if (K[I][J] != 0) w << "x ";
				else w << "  ";
			}
			w << endl;
		}
	}


	for (int I = 0; I < Np; I++)
	{
		rhs << b[I] << endl;
	}
}
void FEM::checkStiffnessMatrix2()
{
	auto toStr = [](double f)
	{
		std::stringstream ss; ss << f; return ss.str();
	};
	auto toStrI = [](int f)
	{
		std::stringstream ss; ss << f; return ss.str();
	};

	ofstream w("K_check.dat");
	ofstream s("K_check2.dat");
	//w << std::fixed << std::setprecision(2);
	for (int l = 0; l < Ne; l++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int I = E[l].n[i];
				int J = E[l].n[j];
				w << E[l].debug[i][j] << " ;";
				D[I][J] += E[l].debug[i][j] + ", ";
			}
			w << endl;
		}
	}

	for (int I = 0; I < Np; I++)
	{
		for (int J = 0; J < Np; J++)
		{
			string pos = "Cell:" + toStrI(I) + "," + toStrI(J);
			s << pos << ": " << D[I][J] << " =" << K[I][J] << " ;";
		}
		s << endl;
	}

}
void FEM::testRectangle()
{
	double Lx = 1;
	double Ly = 1;
	int Nx = 3;
	int Ny = 3;
	int N = (Nx + 1) * (Ny + 1);
	double h = 1.0 / 3.0;


	for (int j = 0; j <= Ny; j++)
	{
		for (int i = 0; i <= Nx; i++)
		{
			int t = 0;
			double x = i * h;
			double y = j * h;
			//if (i == 0 || i == Nx) t = 1;
			//if (j == 0) t = 1;
			//if (j == Ny) t = 1;
			newMeshPoint(x, y, t);
		}
	}

	finalizeMeshMaking();

	initMeshValues();
	{
		newElement(0, 1, 5);
		newElement(0, 4, 5);
		newElement(1, 2, 6);
		newElement(1, 5, 6);
		newElement(2, 3, 7);
		newElement(2, 6, 7);

		newElement(4, 5, 9);
		newElement(4, 8, 9);
		newElement(5, 6, 10);
		newElement(5, 9, 10);
		newElement(6, 7, 11);
		newElement(6, 10, 11);

		double q = -0.5;
		newElement(8, 9, 13);
		newElement(8, 12, 13); //addFluxToEdge(12, 13, q);
		newElement(9, 10, 14);
		newElement(9, 13, 14); //addFluxToEdge(13, 14, q);
		newElement(10, 11, 15);
		newElement(10, 14, 15); //addFluxToEdge(14, 15, q);
	}

}
void FEM::testTemperature()
{
	if (mesh.size() == 0) return;
	int imin = 0, imax = 0;
	double ymin = mesh[0].y, ymax = mesh[0].y;

	for (unsigned int i = 0; i < mesh.size(); i++)
	{
		double y = mesh[i].y;
		if (y > ymax)
		{
			ymax = y;
			imax = i;
		}
		if (y < ymin)
		{
			ymin = y;
			imin = i;
		}
	}
	mesh[imin].t = 1;
	mesh[imax].t = 1;
 
}
void FEM::statistics()
{
	int q = -1;
	double min = 1000000;
	for (unsigned int i = 0; i < E.size(); i++)
	{
		if (E[i].S < min) 
		{
			min = E[i].S;
			q = i;
		}
	}
	int I[3] = { E[q].n[0], E[q].n[1], E[q].n[2] };
	cout << "Smin = " << min << " " << endl;
	cout << E[q].mesh[I[0]].x << " " << E[q].mesh[I[0]].y << endl;
	cout << E[q].mesh[I[1]].x << " " << E[q].mesh[I[1]].y << endl;
	cout << E[q].mesh[I[2]].x << " " << E[q].mesh[I[2]].y << endl;
}
void FEM::allocation()
{
	auto alloc1 = [this](double **f)
	{
		*f = (double*)malloc(sizeof(double) * Np);
		for (int l = 0; l < Np; l++)
			(*f)[l] = 0.0;
	};
	auto alloc2 = [this](double*** f)
	{
		(*f) = new double* [Np];
		for (int i = 0; i < Np; i++)
			(*f)[i] = new double [Np];
		for (int i = 0; i < Np; i++)
			for (int j = 0; j < Np; j++)
				(*f)[i][j] = 0.0;

	};

	alloc2(&K);
	alloc2(&L);

	for (int i = 0; i < Np; i++)
		D = new string * [Np];
	for (int i = 0; i < Np; i++)
		D[i] = new string[Np];

	for (int i = 0; i < Np; i++)
		for (int j = 0; j < Np; j++)
			D[i][j] = "";

	alloc1(&b);
	alloc1(&bt);
	alloc1(&bs);

	alloc1(&T);
	alloc1(&T0);
	alloc1(&v);

}