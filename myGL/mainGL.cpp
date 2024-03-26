#include "init.h"



#include "../FEM.h"
extern FEM fem;



#include "MouseState.h"
MouseState ms;


#include "draw.h"
#include "keyboard.h"
#include "mouse.h"

void MainFreeGlut(int argc, char** argv)
{

	Xleft = -0.1;
	Xright = 1.1;
	Ybottom = -0.1;
	Ytop = 1.1;
	Lx = Xright - Xleft;
	Ly = Ytop - Ybottom;

	ms.X0 = (float)Xleft;
	ms.Y0 = (float)Ybottom;
	ms.LX = (float)(Xright - Xleft);
	ms.LY = (float)(Ytop - Ybottom);


	height = 400;
	width = 600;
	double aspect = Lx / Ly;
	height = (int)(width / aspect);
	width = (int)(height * aspect);



	//cout << "L= " << Lx << " " << Ly << endl;

	int pixel_mod = 4;
	width = int(ceil(width / (0.0 + pixel_mod)) * pixel_mod);
	height = int(ceil(height / (0.0 + pixel_mod)) * pixel_mod);

	//init gl functions
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(400, 20);
		glutCreateWindow("Window_freeglut");

		glutReshapeFunc(reshape);
		glutDisplayFunc(display);
		glutIdleFunc(idle);

		//glutPassiveMotionFunc(mouseMovePassive);
		glutMotionFunc(mouseClickMove);
		glutMouseFunc(mouseClick);
		glutKeyboardFunc(keyboard);

		glutMainLoop();
	}
	
}

