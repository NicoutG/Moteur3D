#ifndef _SURFACE
#define _SURFACE

#include "Matrice.h"
#include "Image.h"
#include "Couleur.h"

/**
@brief structure Point
**/

struct Point
{
	Matrice pos{3,1}; //correspond à la position du point
	Matrice posRe{3,1}; //correspond à la position du point relativement au modèle
};

/**
@brief structure Triangle
**/

struct Triangle
{
	unsigned int point1; //correspond à l'indice du premier point du triangle
	unsigned int point2; //correspond à l'indice du deuxième point du triangle
	unsigned int point3; //correspond à l'indice du troisième point du triangle
};

/**
@brief classe Surface
**/

class Surface
{
private:
	unsigned int nbPoints; //correspond au nombre de points qui composent la surface
	Couleur couleur; //correspond à la couleur de la surface
	Matrice normale{3,1}; //correspond à la normale de la surface
	unsigned int * points; //correspond aux indices des points de la surface
	Image * image; //correspond à l'image de la surface
	float posy,posx; //correspondent à la position de l'image sur la surface
	float tailley,taillex; //correspondent à la taille de l'image sur la surface
	float angle; //correspond à l'angle de l'image sur la surface
	unsigned int nbTriangles; //correspond au nombre de triangles qui composent la surface
	Triangle ** triangles; //correspond au nombre de triangles qui composent la surface
	float opacite; //correspond à l'opacité de la surface (0 à 1)
	
public:
	Point centre; //correspond au centre de la surface

/**
@brief Surface : constructeur de la classe Surface
@param unsigned int p1 l'indice du premier point
@param unsigned int p2 l'indice du deuxième point
@param unsigned int p3 l'indice du troisième point
**/
	Surface(unsigned int p1, unsigned int p2, unsigned int p3);

/**
@brief Surface : constructeur de la classe Surface
@param unsigned int * tab le tableau d'indices de points (à ne pas effacer)
@param unsigned int nb la taille du tableau
**/
	Surface(unsigned int * tab, unsigned int nb);

/**
@brief ~Surface : destructeur de la classe Surface
**/
	~Surface();
	
/**
@brief getNbPoints : accesseur de nbPoints
@return unsigned int le nombre de points
**/
	unsigned int getNbPoints()const &;
	
/**
@brief getImage : accesseur de image
@return Image* l'image de la surface
**/
	Image* getImage()const &;

/**
@brief setImage : mutateur de image
@param Image * img l'image
**/
	void setImage(Image * img)&;

/**
@brief setPosition : mutateur de posx et posy
@param float x la position horizontale
@param float y la position verticale
**/
	void setPosition(float x ,float y)&;

/**
@brief getPosx : accesseur de posx
@return float la position horizontale
**/
	float getPosx()const &;

/**
@brief getPosy : accesseur de posy
@return float la position verticale
**/
	float getPosy()const &;

/**
@brief setTaille : mutateur de taillex et tailley
@param float tx l'étirement horizontale de l'image
@param float ty l'étirement verticale de l'image
**/
	void setTaille(float tx ,float ty)&;

/**
@brief getTaillex : accesseur de taillex
@return float l'étirement horizontale de l'image
**/
	float getTaillex()const &;

/**
@brief getTailley : accesseur de tailley
@return float l'étirement verticale de l'image
**/
	float getTailley()const &;

/**
@brief setAngle : mutateur de angle
@param float ang l'angle de l'image
**/
	void setAngle(float ang)&;

/**
@brief getAngle : accesseur de angle
@return float l'angle de l'image
**/
	float getAngle()const &;

/**
@brief addPoint : ajoute le point à la surface
@param unsigned int nb l'indice au quel ajouter le point
@param unsigned int p l'indice du point du modèle à ajouter
**/
	void addPoint(unsigned int nb, unsigned int p)&;

/**
@brief delPoint : supprime un point de la surface
@param unsigned int p l'indice du point du modèle à supprimer
@return unsigned int l'indice du point supprimé
**/
	unsigned int delPoint(unsigned int p)&;

/**
@brief setPoints : redefinie les points de la surface
@param unsigned int * tab le tableau d'indices de points (à ne pas effacer)
@param unsigned int nb la taille du tableau
**/
	void setPoints(unsigned int * tab, unsigned int nb)&;

/**
@brief pointIn : retourne si le point se trouve dans la surface
@param unsigned int p l'indice du point du modèle
@return bool si le point se trouve dans la surface
**/
	bool pointIn(unsigned int p)const &;

/**
@brief get : accesseur de points
@param unsigned int nb l'indice du point dans la surface
@return unsigned int l'indice du point
**/
	unsigned int get(unsigned int nb)const &;

/**
@brief set : mutateur de points
@param unsigned int nb l'indice du point dans la surface
@param unsigned int p l'indice du point du modèle
**/
	void set(unsigned int nb, unsigned int p)&;

/**
@brief getNormale : accesseur de normale
@return Matrice& la normale de la surface
**/
	const Matrice& getNormale()const &;
	
/**
@brief setNormale : mutateur de normale
@param Matrice& la normale de la surface
**/
	void setNormale(const Matrice & mat)&;

/**
@brief addTriangle : ajoute un triangle à la surface
@param unsigned int p1 l'indice du premier point du triangle
@param unsigned int p2 l'indice du deuxième point du triangle
@param unsigned int p3 l'indice du troisième point du triangle
**/
	void addTriangle(unsigned int p1, unsigned int p2, unsigned int p3)&;

/**
@brief delTriangles : efface tout les triangles de la surface
**/
	void delTriangles()&;

/**
@brief getTriangle : renvoie un triangle de la surface
@param unsigned int t l'indice du triangle de la surface
@return Triangle* un triangle de la surface
**/
	Triangle* getTriangle(unsigned int t)&;

/**
@brief getNbTriangles : renvoie le nombre de triangles de la surface
@return unsigned int le nombre de triangles de la surface
**/
	unsigned int getNbTriangles()const &;

/**
@brief getOpacite : accesseur de opacite
@return float opacit& de la surface
**/
	float getOpacite()const &;

/**
@brief setOpacite : mutateur de opacite
@param float opac l'opacité de la surface
**/
	void setOpacite(float opac)&;

};

#endif
