#ifndef _CAMERA
#define _CAMERA

#include "Matrice.h"
#include "Cosin.h"

/**
@brief classe Camera
**/

class Camera
{
private:
	Matrice pos{3,1}; //correspond à la position x y z de la caméra
	float angle_xy=0, angle_xz=0, angle_yz=0; //correspondent aux angles de la caméra en degrés
	Matrice base{3,3}; //correspond à la base orthonormée relative à la caméra
	unsigned int tailley, taillex; //correspond aux dimensions de la caméra
	float distFoc; //correspond à la distance focale de la caméra
	
/**
@brief calculBase : recalcul la base de la caméra
**/
	void calculBase()&;
	
public:
	Cosin cs; //permet d'utiliser des formules de trigonométrie précalculées

/**
@brief Camera : constructeur de la classe Camera
@param unsigned int ty la hauteur de la caméra
@param unsigned int tx la largeur de la caméra
**/
	Camera(unsigned int ty, unsigned int tx);

/**
@brief getPos : accesseur de pos
@return Matrice& la position
**/
	const Matrice& getPos()const &;
	
/**
@brief setPos : mutateur de pos
@param float x la position x
@param float y la position y
@param float z la position z
**/
	void setPos(float x, float y, float z);
	
/**
@brief getX : renvoie la position x
@return float la position x
**/
	float getX()const &;
	
/**
@brief setX : modifie la position x
@param float x la position x
**/
	void setX(float x);

/**
@brief getY : renvoie la position y
@return float la position y
**/
	float getY()const &;
	
/**
@brief setY : modifie la position y
@param float y la position y
**/
	void setY(float y);
	
/**
@brief getZ : renvoie la position z
@return float la position z
**/
	float getZ()const &;
	
/**
@brief setZ : modifie la position z
@param float z la position z
**/
	void setZ(float z);
	
/**
@brief getAngxy : accesseur de angle_xy
@return float l'angle
**/
	float getAngxy()const &;
	
/**
@brief setAngxy : mutateur de angle_xy
@param float angxy l'angle
**/
	void setAngxy(float angxy);
	
/**
@brief getAngxz : accesseur de angle_xz
@return float l'angle
**/
	float getAngxz()const &;
	
/**
@brief setAngxz : mutateur de angle_xz
@param float angxz l'angle
**/
	void setAngxz(float angxz);
	
/**
@brief getAngyz : accesseur de angle_yz
@return float l'angle
**/
	float getAngyz()const &;
	
/**
@brief setAngyz : mutateur de angle_yz
@param float angyz l'angle
**/
	void setAngyz(float angyz);
	
/**
@brief setAng : modifie les angles
@param float angxy l'angle xy
@param float angxz l'angle xz
@param float angyz l'angle yz
**/
	void setAng(float angxy, float angxz, float angyz);
	
/**
@brief getBase : accesseur de base
@return Matrice& la base
**/
	const Matrice& getBase()const &;
	
/**
@brief getTailley : accesseur de tailley
@return unsigned int la taille verticale de l'écran
**/
	unsigned int getTailley()const &;
	
/**
@brief getTaillex : accesseur de taillex
@return unsigned int la taille horizontale de l'écran
**/
	unsigned int getTaillex()const &;
	
/**
@brief getDistFoc : accesseur de distFoc
@return float la distance focale
**/
	float getDistFoc()const &;

/**
@brief setDistFoc : mutateur de distFoc
@rparam float dist la distance focale
**/
	void setDistFoc(float dist);
	
/**
@brief getNormale : renvoie la normale de l'ecran de la camera
@return Matrice la normale de l'ecran de la camera
**/
	Matrice getNormale()const &;
	
};

#endif
