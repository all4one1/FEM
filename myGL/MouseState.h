#pragma once
enum MouseClick { LeftButton, MiddleButton, RightButton };
struct MouseState
{
	float lastX, lastY; //previous position pixels
	float PosX, PosY; //position pixels
	float shiftX, shiftY; //difference between former two 
	float PosXn, PosYn; //normalized coords
	float realX, realY, realZ; //physical coords

	float xRealClicked, yRealClicked;
	float xRealReleased, yRealReleased;

	bool firstMouse = true;
	bool onPressed[3] = { false, false, false };
	int onPressedTick = 0;
	float X0, Y0, X1, Y1, LX, LY;

	void getCursorPosition(float xpos, float ypos)
	{
		normalized(xpos, ypos);

		realX = X0 + (PosXn + 1) * 0.5f * LX;
		realY = Y0 + (PosYn + 1) * 0.5f * LY;
	}

	void normalized(float xpos, float ypos)
	{
		PosXn = (-1.0f + 2.0f * xpos / width);
		PosYn = (1.0f - 2.0f * ypos / height);
	}

	void mouseShift(float xpos, float ypos)
	{
		//if (xpos > 0 && xpos < width && ypos > 0 && ypos < height)
		{
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			shiftX = xpos - lastX;
			shiftY = -(ypos - lastY);

			lastX = xpos;
			lastY = ypos;
			//double sensibility = 0.5;
			//mouseDx += sensibility * Dx;
			//mouseDy += sensibility * Dy;
		}
	}


	void detectPressed(int key, int state, float xpos, float ypos)
	{
		getCursorPosition((xpos), (ypos));
		if (state == GLUT_DOWN)
		{
			lastX = xpos;
			lastY = ypos;
			onPressed[key] = 1;
			xRealClicked = realX;
			yRealClicked = realY;

			//cout << "Clicked: " << xRealClicked << " " << yRealClicked << endl;
		}
		if (state == GLUT_UP)
		{
			onPressed[key] = 0;
			xRealReleased = realX;
			yRealReleased = realY;
			//cout << "Released: " << xRealReleased << " " << yRealReleased << endl;
		}
	}

	void cursorInfo()
	{
		cout << "pixel: " << lastX << " " << lastY << endl;
		cout << "norm: " << PosXn << " " << PosYn << endl;
		cout << "real: " << realX << " " << realY << endl;
		//if (onPressed[LeftButton]) cout << "LeftButton pressed" << endl;
		//if (onPressed[MiddleButton]) cout << "MiddleButton pressed" << endl;
		//if (onPressed[RightButton]) cout << "RightButton pressed" << endl;
	}

};

