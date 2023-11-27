#include "../include/Cosin.h"
#include <math.h>
#include <cassert>

Cosin::Cosin()
{
	for (unsigned int i=0;i<1000;i++)
	{
		tab[i]=cos(PI*i/2000);
		tab[2000-i]=-tab[i];
	}
	tab[1000]=0;
}

float Cosin::Cos(float angle)const&
{
	int indice=abs(2000*angle/180);
	return tab[abs(indice-int(indice/4000.0+0.5)*4000)];
}

float Cosin::Sin(float angle)const&
{
	return Cos(angle-90);
}

float Cosin::Tan(float angle)const&
{
	float cs=Cos(angle);
	assert(cs!=0);
	return Sin(angle)/cs;
}
