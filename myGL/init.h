#pragma once

#include <iostream> 
#include <string>       
#include <iostream>     
#include <sstream> 
#include <fstream>
#include <map>	
#include <vector>

#include "../GL/freeglut.h"

using std::cout;
using std::endl;
using std::vector;


int iter_draw = 0;
double Xleft, Xright, Ybottom, Ytop;
double Lx, Ly;
double* glPosX, * glPosY;
int width, height;

#define pause system("pause");
#define rgb(r,g,b) (GLclampf)r/255.0f, (GLclampf)g/255.0f, (GLclampf)b/255.0f, (GLclampf)1.0f
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//width = w;
	//height = h;
}
void idle()
{
	//this function is doing something between rendering
	glutReshapeFunc(reshape);
	glutPostRedisplay();
	iter_draw++;
	//if (iter_draw % 100 == 0) cout << iter_draw << endl;
	//cout << "mouse: "; 	for (int i = 0; i < 3; i++) 	cout << ms.onPressed[i] << " "; 	cout << endl;
}



using namespace std;
string debugStr = "";
struct Pallete
{
	struct fRGB { double f = 0, r = 0, g = 0, b = 0; };
	vector<fRGB> v;
	int N = 0;
	enum pal { value, R, G, B };

	Pallete() {};
	Pallete(string fileName)
	{
		if (fileName != "")
		{
			clrFileRead(fileName);
		}
	}

	void printRGB(int i)
	{
		cout << "value: " << v[i].f << ", RGB: " << " " << v[i].r * 255 << " " << v[i].g * 255 << " " << v[i].b * 255 << endl;
	}

	void clrFileRead(string fileName = "out.clr")
	{
		ifstream f(fileName);
		if (!f.is_open()) cout << "Pallete is not read" << endl;
		
		//istringstream iss;
		string line;
	
		getline(f, line); //skip 1st line
		while (getline(f, line))
		{
			stringstream ss;
			ss << line;
			v.emplace_back();
			ss >> v.back().f;	
			ss >> v.back().r;
			ss >> v.back().g;	
			ss >> v.back().b;
			N++;
		}
		for (int i = 0; i < N; i++)
		{
			v[i].r /= 255;
			v[i].g /= 255;
			v[i].b /= 255;
		}

		if (0) //print read colors
		{
			cout << "Color read: " << endl;
			for (int i = 0; i < N; i++)
			{
				cout << i << " ";
				this->printRGB(i);
			}
		}
	}

	void getColor(double f, double &r, double &g, double &b)
	{
		//кривовато
		//upd: вроде чуть лучше
		for (int i = 0; i < N - 1; i++)
		{

			if (f >= v[i].f && f <= v[i+1].f)
			{
				double d1 = abs(v[i].f - f);
				double d2 = abs(v[i + 1].f - f);
				int k;
				if (d1 < d2) k = i;
				else k = i + 1;

				r = v[k].r;
				g = v[k].g;
				b = v[k].b;
				
				break;
			}
		}
		//pause
	}

};

template <typename T>
std::string make_str(const T a_value, const int n = 2)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::move(out).str();
}