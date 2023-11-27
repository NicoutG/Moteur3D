#include "../include/Couleur.h"
#include <iostream>

Couleur::Couleur()
{
	set(100,100,100,255);
}

Couleur::Couleur(unsigned char r, unsigned char g, unsigned char b)
{
	set(r,g,b,255);
}

Couleur::Couleur(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	set(r,g,b,a);
}

unsigned char Couleur::getR()const &
{
	return rouge;
}

void Couleur::setR(unsigned char r)
{
	rouge=r;
}

unsigned char Couleur::getG()const &
{
	return vert;
}

void Couleur::setG(unsigned char g)
{
	vert=g;
}


unsigned char Couleur::getB()const &
{
	return bleu;
}

void Couleur::setB(unsigned char b)
{
	bleu=b;
}

unsigned char Couleur::getA()const &
{
	return alpha;
}

void Couleur::setA(unsigned char a)
{
	alpha=a;
}

void Couleur::set(unsigned char r, unsigned char g, unsigned char b)
{
	setR(r);
	setG(g);
	setB(b);
}

void Couleur::set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	setR(r);
	setG(g);
	setB(b);
	setA(a);
}

void Couleur::afficher()const &
{
	std::cout<<"r "<<int(rouge)<<"  g "<<int(vert)<<"  b "<<int(bleu)<<" a "<<int(alpha)<<std::endl;
}

