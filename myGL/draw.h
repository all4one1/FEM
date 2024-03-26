#pragma once

double colorMax = 1;
double colorMin = 0;
double colorDif = 1;
Pallete Pal("MyGL/out.clr");
#define COLORFUNC(f) (f - colorMin) / colorDif * 100;

void triangle(double x[3], double y[3], double f[3])
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3; i++)
	{
		double r, g, b, F;
		F = (f[i] - colorMin) / colorDif * 100;
		Pal.getColor(F, r, g, b);
		glColor4d(r, g, b, 1);
		//glColor4d(1, (f[i] - colorMin) / colorDif, 0, 0);
		glVertex3d(x[i], y[i], 0);
	}
	glEnd();
	

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 3; i++)
	{
		glColor4d(0.3, 0.3, 0.3, 0);
		glVertex3d(x[i], y[i], 0);
	}
	glEnd();
}


void drawText(double x, double y, string str)
{
	glColor3d(1.0, 1.0, 1.0);
	auto draw_string_bitmap = [](void* font, const char* string)
	{
		while (*string)
			glutBitmapCharacter(font, *string++);
	};
	glRasterPos2d(x, y);
	draw_string_bitmap(GLUT_BITMAP_HELVETICA_18, str.c_str());
}

void drawScale()
{
	double h = 0.05;
	double l = 0.025;

	auto rect = [h, l](double x, double y)
	{
		double eps = 1;
		glBegin(GL_POLYGON);
		glVertex3d(x, y, eps);
		glVertex3d(x + l, y, eps);
		glVertex3d(x + l, y + h, eps);
		glVertex3d(x, y + h, eps);
		glEnd();
	};

	double x = -0.1;
	double y = 0;
	for (double c = colorMin; c <= colorMax; c = c + colorDif * h)
	{
		double r, g, b, F;


		F = COLORFUNC(c);
		Pal.getColor(F, r, g, b); 
		glColor3d(r, g, b);
		rect(x, y); 
		drawText(x+l, y, make_str(c));

		y += h;
	}
}


void display()
{
	glClearColor(rgb(30, 30, 30));
	glClear(GL_COLOR_BUFFER_BIT);

	//____________________________________//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(Xleft, Xright, Ybottom, Ytop);

	//axes
	glBegin(GL_LINES);
	glColor3d(0.0, 0.0, 0.9);
	//glVertex3d(Xleft, Ly / 2, 0); 	 glVertex3d(Xright, Ly/2, 0);
	//glVertex3d(Lx / 2, Ybottom, 0);  	glVertex3d(Lx / 2, Ytop, 0);

	glVertex3d(Xleft, 0, 0); 	 glVertex3d(Xright, 0, 0);
	glVertex3d(0, Ybottom, 0);  	glVertex3d(0, Ytop, 0);
	glEnd();

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	//objects
	glColor4d(0.0, 0.46, 0.89, 0.1);

	//glDisable(GL_BLEND);


	//int type[2] = { GL_TRIANGLES, GL_LINE_LOOP };


	int N = fem.Ne;
	if (iter_draw % 1 == 0)
	{
		fem.findMaxMin(colorMax, colorMin);
		colorDif = colorMax - colorMin;
	}
	//if (colorDif < 1) colorDif = 1;
	for (int I = 0; I < N; I++)
	{
		double x[3] = { fem.getX(I, 0), fem.getX(I, 1), fem.getX(I, 2) };
		double y[3] = { fem.getY(I, 0), fem.getY(I, 1), fem.getY(I, 2) };
		double f[3] = { fem.getValue(I, 0), fem.getValue(I, 1), fem.getValue(I, 2) };
		triangle(x, y, f);
	}
	drawScale();
	
	drawText(0.02, -0.05, "Debug: " + debugStr);
	glutSwapBuffers(); 
}

