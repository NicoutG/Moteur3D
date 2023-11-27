#include "../include/Matrice.h"
#include <iostream>
#include <cassert>
#include <math.h>

Matrice::Matrice()
{
	dimy=0;
	dimx=0;
	tab=NULL;
}

Matrice::Matrice(unsigned int y, unsigned int x)
{
	dimy=y;
	dimx=x;
	tab=new float * [y];
	for (unsigned int i=0;i<y;i++)
		tab[i]=new float [x]{0};
}

Matrice::~Matrice()
{
	for (unsigned int i=0;i<dimy;i++)
		delete [] tab[i];
	delete tab;
}

unsigned int Matrice::getDimy()const &
{
	return dimy;
}

unsigned int Matrice::getDimx()const &
{
	return dimx;
}

float Matrice::get(unsigned int y, unsigned int x)const &
{
	assert(y<dimy);
	assert(x<dimx);
	return tab[y][x];
}

void Matrice::set(unsigned int y, unsigned int x, float val)
{
	assert(y<dimy);
	assert(x<dimx);
	tab[y][x]=val;
}

Matrice Matrice::getCol(unsigned int x)const &
{
	assert(x<dimx);
	Matrice res(dimy,1);
	for (unsigned int j=0;j<dimy;j++)
		res.set(j,0,get(j,x));
	return res;
}

Matrice Matrice::getLig(unsigned int y)const &
{
	assert(y<dimy);
	Matrice res(1,dimx);
	for (unsigned int i=0;i<dimx;i++)
		res.set(0,i,get(y,i));
	return res;
}

void Matrice::modifDims(unsigned int y, unsigned int x)&
{
	for (unsigned int i=0;i<dimy;i++)
		delete [] tab[i];
	delete tab;
	dimy=y;
	dimx=x;
	tab=new float * [y];
	for (unsigned int i=0;i<y;i++)
		tab[i]=new float [x]{0};
}

Matrice Matrice::operator + (const Matrice & mat)const &
{
	assert(dimy==mat.getDimy() && dimx==mat.getDimx());
	Matrice res(dimy,dimx);
	for (unsigned int j=0;j<dimy;j++)
		for (unsigned int i=0;i<dimx;i++)
			res.set(j,i,get(j,i)+mat.get(j,i));
	return res;
}

Matrice Matrice::operator - (const Matrice & mat)const &
{
	assert(dimy==mat.getDimy() && dimx==mat.getDimx());
	Matrice res(dimy,dimx);
	for (unsigned int j=0;j<dimy;j++)
		for (unsigned int i=0;i<dimx;i++)
			res.set(j,i,get(j,i)-mat.get(j,i));
	return res;
}

Matrice Matrice::operator * (const Matrice & mat)const &
{
	assert(dimx==mat.getDimy());
	Matrice res(dimy,mat.getDimx());
	for (unsigned int j=0;j<dimy;j++)
		for (unsigned int i=0;i<mat.getDimx();i++)
		{
			res.set(j,i,0);
			for(unsigned int k=0;k<dimx;k++)
				res.set(j,i,res.get(j,i)+get(j,k)*mat.get(k,i));
		}
	return res;
}

Matrice Matrice::operator * (float mult)const &
{
	Matrice res(dimy,dimx);
	for (unsigned int j=0;j<dimy;j++)
		for (unsigned int i=0;i<dimx;i++)
			res.set(j,i,mult*get(j,i));
	return res;
}

Matrice operator * (float mult, const Matrice & mat)
{
	return mat*mult;
}

Matrice Matrice::operator / (float div)const &
{
	assert(div!=0);
	Matrice res(dimy,dimx);
	for (unsigned int j=0;j<dimy;j++)
		for (unsigned int i=0;i<dimx;i++)
			res.set(j,i,get(j,i)/div);
	return res;
}

void Matrice::operator = (const Matrice & mat)&
{
	assert(dimy==mat.getDimy());
	assert(dimx==mat.getDimx());
	for (unsigned int j=0;j<dimy;j++)
		for (unsigned int i=0;i<dimx;i++)
			set(j,i,mat.get(j,i));
}

float prodScal(const Matrice & mat1, const Matrice & mat2)
{
	assert(mat1.getDimy()==mat2.getDimy() && mat1.getDimx()==mat2.getDimx());
	float res=0;
	for (unsigned int j=0;j<mat1.getDimx();j++)
		for (unsigned int i=0;i<mat1.getDimy();i++)
			res+=mat1.get(i,j)*mat2.get(i,j);
	return res;
}

float norme(const Matrice & mat)
{
	return sqrt(prodScal(mat,mat));
}

Matrice prodVect(const Matrice & mat1, const Matrice & mat2)
{
	assert(mat1.getDimx()==1 && mat1.getDimy()==3 && mat2.getDimx()==1 && mat2.getDimy()==3);
	Matrice res(3,1);
	res.set(0,0,mat1.get(1,0)*mat2.get(2,0)-mat1.get(2,0)*mat2.get(1,0));
	res.set(1,0,mat1.get(2,0)*mat2.get(0,0)-mat1.get(0,0)*mat2.get(2,0));
	res.set(2,0,mat1.get(0,0)*mat2.get(1,0)-mat1.get(1,0)*mat2.get(0,0));
	return res;
}

void orthonormalisation(Matrice & m1, Matrice & m2)
{
	assert(m1.getDimy()==3 && m1.getDimx()==1);
	assert(m2.getDimy()==3 && m2.getDimx()==1);
	assert(&m1!=&m2);
	float n1=norme(m1);
	assert(n1!=0);
	m1=m1/n1;
	m2=m2-prodScal(m1,m2)*m1;
	float n2=norme(m2);
	assert(n2!=0);
	m2=m2/n2;
}

Matrice projection(const Matrice & m, const Matrice & b1, const Matrice & b2)
{
	assert(m.getDimy()==3 && m.getDimx()==1);
	assert(b1.getDimy()==3 && b1.getDimx()==1);
	assert(b2.getDimy()==3 && b2.getDimx()==1);
	Matrice b1n(3,1);
	b1n=b1;
	Matrice b2n(3,1);
	b2n=b2;
	orthonormalisation(b1n,b2n);
	Matrice res(3,1);
	res=prodScal(b1n,m)*b1n+prodScal(b2n,m)*b2n;
	return res;
}

float distance(const Matrice & m, const Matrice & b1, const Matrice & b2)
{
	return norme(m-projection(m,b1,b2));
}

void Matrice::afficher()const &
{
	for (unsigned int j=0;j<dimy;j++)
	{
		for (unsigned int i=0;i<dimx;i++)
			std::cout<<get(j,i)<<" ";
		std::cout<<std::endl;
	}
}
