
//#include "Element.h"
#include <omp.h>
#include "FEM.h"
#include <iostream>
#include <fstream>
using namespace std;
#define pause {cout << "Pause at line " << __LINE__ << ", FILE: " << __FILE__ << endl; system("pause"); }

void MainFreeGlut(int argc, char** argv);


FEM fem;

int main(int argc, char** argv)
{
	fem.readElementsAndMesh();
	//fem.readBoundaryConditions();

	fem.testTemperature(); 
	//fem.testRectangle();
	fem.initMeshValues();
	fem.tau = 5000;
	//fem.tau = 0.1;

	
	fem.computeAllCoefs(); 
	fem.makeStiffnessMatrix();
		
	fem.removeFixedValues();


	//fem.statistics();
	omp_set_num_threads(2);
#pragma omp parallel
	{
		if (omp_get_thread_num() == 0)
		{
			auto check = []()
			{
				double r = 0;
				for (int i = 0; i < fem.Np; i++)
					r += pow(fem.v[i] - fem.T[i], 2);
				r = sqrt(r / fem.Np);
				return r;
			};
			while(true)
			{
				fem.solve();
				double r = check();
				//if (fem.iter % 10000 == 0) cout << r << endl;
				//if (r < 1e-7) break;
				if (fem.iter > 1000000) 
				{
					cout << "iter: = " << fem.iter << endl;
					break;
				}
			}

			fem.writeData();
			
		}




		if (omp_get_thread_num() == 1)
		{
			MainFreeGlut(argc, argv);
		}

	}
	return 0;
}

