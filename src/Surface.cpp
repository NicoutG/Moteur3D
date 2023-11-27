#include "../include/Surface.h"
#include <cassert>
#include <iostream>

Surface::Surface(unsigned int p1, unsigned int p2, unsigned int p3)
{
	nbPoints=3;
	points=new unsigned int [3];
	points[0]=p1;
	points[1]=p2;
	points[2]=p3;
	image=NULL;
	posx=0;
	posy=0;
	taillex=1;
	tailley=1;
	angle=0;
	nbTriangles=0;
	triangles=NULL;
	opacite=1;
}

Surface::Surface(unsigned int * tab, unsigned int nb)
{
	image=NULL;
	posx=0;
	posy=0;
	taillex=1;
	tailley=1;
	angle=0;
	points=NULL;
	nbTriangles=0;
	triangles=NULL;
	opacite=1;
	setPoints(tab,nb);
}

Surface::~Surface()
{
	if (points!=NULL)
		delete [] points;
	delTriangles();
}

unsigned int Surface::getNbPoints()const &
{
	return nbPoints;
}

Image* Surface::getImage()const &
{
	return image;
}

void Surface::setImage(Image * img)&
{
	image=img;
}

void Surface::setPosition(float x ,float y)&
{
	posy=y;
	posx=x;
}

float Surface::getPosx()const &
{
	return posx;
}

float Surface::getPosy()const &
{
	return posy;
}

void Surface::setTaille(float tx ,float ty)&
{
	assert(tx!=0);
	assert(ty!=0);
	tailley=ty;
	taillex=tx;
}

float Surface::getTaillex()const &
{
	return taillex;
}

float Surface::getTailley()const &
{
	return tailley;
}

void Surface::setAngle(float ang)&
{
	angle=ang-int(ang/360+0.5)*360;
}

float Surface::getAngle()const &
{
	return angle;
}

void Surface::addPoint(unsigned int nb, unsigned int p)&
{
	assert(nb<=nbPoints);
	unsigned int * nouv=new unsigned int [nbPoints+1];
	for (unsigned int i=0;i<nb;i++)
		nouv[i]=points[i];
	nouv[nb]=p;
	for (unsigned int i=nb;i<nbPoints;i++)
		nouv[i+1]=points[i];
	delete [] points;
	nbPoints++;
	points=nouv;
}

unsigned int Surface::delPoint(unsigned int p)&
{
	assert(3<nbPoints);
	unsigned int i=0, res;
	unsigned int * nouv=new unsigned [nbPoints-1];
	while(p!=points[i] && i<nbPoints)
	{
		nouv[i]=points[i];
		i++;
	}
	res=i;
	if (i<nbPoints)
	{
		while(i<nbPoints)
		{
			nouv[i]=points[i+1];
			i++;
		}	
		delete [] points;
		nbPoints--;
		points=nouv;
	}
	else
		delete [] nouv;
	return res;
}

void Surface::setPoints(unsigned int * tab, unsigned int nb)&
{
	assert(nb>=3);
	if (points!=NULL)
		delete [] points;
	nbPoints=nb;
	points=tab;
}

bool Surface::pointIn(unsigned int p)const &
{
	bool b=0;
	unsigned int i=0;
	while (!b && i<nbPoints)
	{
		b=points[i]==p;
		i++;
	}
	return b;
}
	
unsigned int Surface::get(unsigned int nb)const &
{
	assert(nb<nbPoints);
	return points[nb];
}

void Surface::set(unsigned int nb, unsigned int p)&
{
	assert(nb<nbPoints);
	points[nb]=p;
}

const Matrice& Surface::getNormale()const &
{
	return normale;
}

void Surface::setNormale(const Matrice & mat)&
{
	assert(mat.getDimy()==3 && mat.getDimx()==1);
	normale=mat;
}

void Surface::addTriangle(unsigned int p1, unsigned int p2, unsigned int p3)&
{
	assert(p1!=p2 && p2!=p3 && p1!=p3);
	Triangle ** tri=new Triangle * [nbTriangles+1];
	for (unsigned int i=0;i<nbTriangles;i++)
		tri[i]=triangles[i];
	delete [] triangles;
	tri[nbTriangles]=new Triangle;
	tri[nbTriangles]->point1=p1;
	tri[nbTriangles]->point2=p2;
	tri[nbTriangles]->point3=p3;
	triangles=tri;
	nbTriangles++;
}

void Surface::delTriangles()&
{
	if (triangles!=NULL) {
		for (unsigned int i=0;i<nbTriangles;i++)
			delete triangles[i];
		delete [] triangles;
		triangles=NULL;
	}
	nbTriangles=0;
}

Triangle* Surface::getTriangle(unsigned int t)&
{
	assert(t<nbTriangles);
	return triangles[t];
}

unsigned int Surface::getNbTriangles()const &
{
	return nbTriangles;
}

float Surface::getOpacite()const &
{
	return opacite;
}

void Surface::setOpacite(float opac)&
{
	if (opac<0)
		opacite=0;
	else
		if (opacite>1)
			opacite=1;
		else
			opacite=opac;
}
