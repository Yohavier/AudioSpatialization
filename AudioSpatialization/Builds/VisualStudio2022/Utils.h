#pragma once
# define M_PI 3.14159265358979323846

struct Position
{
	Position(double xCoord, double yCoord)
		: x(xCoord)
		, y(yCoord)
	{}

	void Set(double xCoord, double yCoord)
	{
		x = xCoord;
		y = yCoord;
	}

	double x = 0;
	double y = 0;
};