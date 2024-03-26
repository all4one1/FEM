#pragma once





void mouseClick(int key, int state, int x, int y) {

	int stat = glutGetModifiers();
	ms.detectPressed(key, state, (float)x, (float)y);
	double X = ms.realX;
	double Y = ms.realY;

	if (state == GLUT_DOWN)
	{
		switch (stat)
		{
		case 0:
			if (key == LeftButton) //left click
			{
				//fem.findElementByPoint(ms.realX, ms.realY);
				double v = fem.getInterpolatedValue(ms.realX, ms.realY);
				//cout << ms.realX << " " << ms.realY << endl;
				cout << "T= " << v << endl;
				debugStr = "T(" + make_str(ms.realX) + "," + make_str(ms.realY)
					+ ") = " + make_str(v);
			}
			if (key == MiddleButton)  //middle click
			{
				//tria.check(X, Y);
			}

			if (key == RightButton) //right click
			{
				//tria.clearVec();
				//ms.cursorInfo();
			}

			if (key == 3) //scroll up
			{

			}

			if (key == 4) { //scroll down

			}

			break;

		case GLUT_ACTIVE_SHIFT:
			break;

		case GLUT_ACTIVE_ALT:

			break;
		default:
			break;
		}
	}

	if (state == GLUT_DOWN)
	{
		//cout << "pressed down" << endl;
	}
	if (state == GLUT_UP)
	{
		//cout << "released" << endl;
	}

	
}

void mouseClickMove(int x, int y)
{
	int stat = glutGetModifiers();
	//if (!(x > 0 && x < width && y > 0 && y < height)) return;
	float xpos = (float)x;
	float ypos = (float)y;

	ms.mouseShift(xpos, ypos);

	if (stat == 0)
	{
		if (ms.onPressed[LeftButton])
		{

			//ms.onPressedTick++;
			//cout << "Tick: " << ms.onPressedTick << endl;
		}

		if (ms.onPressed[RightButton])
		{
			//cout << ms.shiftX << " " << ms.shiftY << endl;
		}
	}


}
