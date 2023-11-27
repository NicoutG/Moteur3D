#include "../include/Camera.h"
#include <cassert>

Camera::Camera(unsigned int ty, unsigned int tx)
{
	pos.set(0,0,0);
	pos.set(1,0,0);
	pos.set(2,0,0);
	angle_xy=0;
	angle_xz=0;
	angle_yz=0;
	calculBase();
	assert(ty!=0);
	tailley=ty;
	assert(tx!=0);
	taillex=tx;
	distFoc=1.5;
	assert(distFoc>0);
}

const Matrice& Camera::getPos()const &
{
	return pos;
}

void Camera::setPos(float x, float y, float z)
{
	setX(x);
	setY(y);
	setZ(z);
}

float Camera::getX()const &
{
	return pos.get(0,0);
}
	
void Camera::setX(float x)
{
	pos.set(0,0,x);
}
	
float Camera::getY()const &
{
	return pos.get(1,0);
}
	
void Camera::setY(float y)
{
	pos.set(1,0,y);
}
	
float Camera::getZ()const &
{
	return pos.get(2,0);
}
	
void Camera::setZ(float z)
{
	pos.set(2,0,z);
}

float Camera::getAngxy()const &
{
	return angle_xy;
}

void Camera::setAngxy(float angxy)
{
	angle_xy=angxy-int(angxy/360+0.5)*360;
	calculBase();
}

float Camera::getAngxz()const &
{
	return angle_xz;
}

void Camera::setAngxz(float angxz)
{
	float angmax=90;
	if (angxz>angmax)
		angle_xz=angmax;
	else
		if (angxz<-angmax)
			angle_xz=-angmax;
		else
			angle_xz=angxz;
	calculBase();
}

float Camera::getAngyz()const &
{
	return angle_yz;
}

void Camera::setAngyz(float angyz)
{
	angle_yz=angyz-int(angyz/360+0.5)*360;
	calculBase();
}

void Camera::setAng(float angxy, float angxz, float angyz)
{
	angle_xy=angxy-int(angxy/360+0.5)*360;
	float angmax=90;
	if (angxz>angmax)
		angle_xz=angmax;
	else
		if (angxz<-angmax)
			angle_xz=-angmax;
		else
			angle_xz=angxz;
	angle_yz=angyz-int(angyz/360+0.5)*360;
	calculBase();
}

const Matrice& Camera::getBase()const &
{
	const Matrice& res=base;
	return res;
}

unsigned int Camera::getTailley()const &
{
	return tailley;
}

unsigned int Camera::getTaillex()const &
{
	return taillex;
}

float Camera::getDistFoc()const &
{
	return distFoc;
}
	
void Camera::setDistFoc(float dist)
{
	assert(dist>0);
	distFoc=dist;
}

Matrice Camera::getNormale()const &
{
	Matrice res(3,1);
	res.set(0,0,base.get(0,0));
	res.set(1,0,base.get(0,1));
	res.set(2,0,base.get(0,2));
	return res;
}

void Camera::calculBase()&
{
	float cosxy=cs.Cos(angle_xy);
	float sinxy=cs.Sin(angle_xy);
	float cosxz=cs.Cos(angle_xz);
	float sinxz=cs.Sin(angle_xz);
	float cosyz=cs.Cos(angle_yz);
	float sinyz=cs.Sin(angle_yz);
	base.set(0,0,cosxy*cosxz);
	base.set(0,1,-sinxy*cosxz);
	base.set(0,2,sinxz);
	base.set(1,0,-cosxy*sinxz*sinyz+sinxy*cosyz);
	base.set(1,1,cosxy*cosyz+sinxy*sinxz*sinyz);
	base.set(1,2,cosxz*sinyz);
	base.set(2,0,-sinxy*sinyz-cosxy*sinxz*cosyz);
	base.set(2,1,-cosxy*sinyz+sinxy*sinxz*cosyz);
	base.set(2,2,cosxz*cosyz);
}
