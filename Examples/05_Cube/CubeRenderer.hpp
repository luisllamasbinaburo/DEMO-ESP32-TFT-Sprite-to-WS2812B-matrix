#include "cube.h"

class Point3d
{
public:
	Point3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{
	}

	int x;
	int y;
	int z;
};

class Point2d
{
public:
	Point2d(float _x, float _y) : x(_x), y(_y)
	{
	}

	int x;
	int y;
};


class Line3d
{
public:
	Line3d(Point3d _p0, Point3d _p1) : p0(_p0), p1(_p1)
	{
	}

	Point3d p0;
	Point3d p1;
};



class Line2d
{
public:
	Line2d(Point2d _p0, Point2d _p1) : p0(_p0), p1(_p1)
	{
	}

	Point2d p0;
	Point2d p1;
};


float xx, xy, xz;
float yx, yy, yz;
float zx, zy, zz;

const float fact = 180 / 3.14159259;

int Xan, Yan;

int Xoff;
int Yoff;
int Zoff;


std::vector<Line3d> Lines;
std::vector<Line2d> Render;


void SetVars(void)
{
	float Xan2, Yan2, Zan2;
	float s1, s2, s3, c1, c2, c3;

	Xan2 = Xan / fact; // convert degrees to radians.
	Yan2 = Yan / fact;

	// Zan is assumed to be zero

	s1 = sin(Yan2);
	s2 = sin(Xan2);

	c1 = cos(Yan2);
	c2 = cos(Xan2);

	xx = c1;
	xy = 0;
	xz = -s1;

	yx = (s1 * s2);
	yy = c2;
	yz = (c1 * s2);

	zx = (s1 * c2);
	zy = -s2;
	zz = (c1 * c2);
}


void ProcessLine(Line2d& ret, Line3d& vec)
{
	float zvt1;
	int xv1, yv1, zv1;

	float zvt2;
	int xv2, yv2, zv2;

	int rx1, ry1;
	int rx2, ry2;

	int x1, y1, z1;
	int x2, y2, z2;

	int Ok;

	x1 = vec.p0.x;
	y1 = vec.p0.y;
	z1 = vec.p0.z;

	x2 = vec.p1.x;
	y2 = vec.p1.y;
	z2 = vec.p1.z;

	Ok = 0; // defaults to not OK

	xv1 = (x1 * xx) + (y1 * xy) + (z1 * xz);
	yv1 = (x1 * yx) + (y1 * yy) + (z1 * yz);
	zv1 = (x1 * zx) + (y1 * zy) + (z1 * zz);

	zvt1 = zv1 - Zoff;


	if (zvt1 < -5)
	{
		rx1 = 256 * (xv1 / zvt1) + Xoff;
		ry1 = 256 * (yv1 / zvt1) + Yoff;
		Ok = 1; // ok we are alright for point 1.
	}


	xv2 = (x2 * xx) + (y2 * xy) + (z2 * xz);
	yv2 = (x2 * yx) + (y2 * yy) + (z2 * yz);
	zv2 = (x2 * zx) + (y2 * zy) + (z2 * zz);

	zvt2 = zv2 - Zoff;


	if (zvt2 < -5)
	{
		rx2 = 256 * (xv2 / zvt2) + Xoff;
		ry2 = 256 * (yv2 / zvt2) + Yoff;
	}
	else
	{
		Ok = 0;
	}

	if (Ok == 1)
	{

		ret.p0.x = rx1;
		ret.p0.y = ry1;

		ret.p1.x = rx2;
		ret.p1.y = ry2;
	}
}


void InitCube()
{
	Xoff = 80;
	Zoff = 250;

	for (int i = 0; i < cube_count / 6; i++)
	{
		auto p0 = Point3d(cube[i * 6 + 0], cube[i * 6 + 1], cube[i * 6 + 2]);
		auto p1 = Point3d(cube[i * 6 + 3], cube[i * 6 + 4], cube[i * 6 + 5]);
		auto line3d = Line3d(p0, p1);
		Lines.push_back(line3d);
		Render.push_back({ {0,0},{0,0} });
	}
}

void UpdateCube()
{
	Zoff = 600;
	Xan = -45.0;
	Yoff = 35;
	Xoff = +32;

	Yan += 2;
	Yan = Yan % 360;

	SetVars(); //sets up the global vars to do the conversion.
}


void RenderCube(TFT_eSprite& sprite)
{
	sprite.fillSprite(TFT_BLACK);
	for (int i = 0; i < Lines.size(); i++)
	{
		ProcessLine(Render[i], Lines[i]);
	}

	for (auto& line : Render)
	{
		sprite.drawLine(line.p0.y, line.p0.x, line.p1.y, line.p1.x, WHITE); // erase the old lines.
	}

	sprite.pushSprite(0, 0);
}