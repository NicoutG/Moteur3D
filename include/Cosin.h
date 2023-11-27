#ifndef _COSIN
#define _COSIN

/**
@brief classe Cosin
**/

class Cosin
{
private:
	float tab[2001]; //correspond aux valeurs précalculées de cosinus
	
public:
	const float PI=3.14159; //correspond à la valeur de PI

/**
@brief Cosin : constructeur de la classe Cosin
**/
	Cosin();
	
/**
@brief Cos : renvoie le cosinus de l'angle
@param float angle un angle en degrés
@return float la valeur du cosinus de l'angle
**/
	float Cos(float angle)const&;
	
/**
@brief Sin : renvoie le sinus de l'angle
@param float angle un angle en degrés
@return float la valeur du sinus de l'angle
**/
	float Sin(float angle)const&;
	
/**
@brief Tan : renvoie la tangente de l'angle
@param float angle un angle en degrés
@return float la valeur de la tangente de l'angle
**/
	float Tan(float angle)const&;
	
};

#endif
