#pragma once



void keyboard(unsigned char key, int x, int y)
{
	int state = glutGetModifiers();


	switch (key)
	{
	case 'e':
	{

		break;
	}


	case 't':
	{
		cout << "test" << endl;
		fem.readElementsAndMesh();

		break;
	}
	case 'T':
	{

		break;
	}
	case 'r':
	{

		break;
	}
	case 'N':
	{
		break;
	}

	case 's':
	{

		break;
	}
	case 'd':
	{

		break;
	}
	case 'D':
	{

		break;
	}
	default:
		break;
	}


}
