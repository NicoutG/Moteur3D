#ifndef _IMAGE
#define _IMAGE

#include "Couleur.h"
#include <iostream>

/**
@brief classe Image
**/

class Image
{
private:
	unsigned int tailley, taillex; //correspondent au dimensions de l'image
	Couleur ** couleurs; //correspond aux pixels de l'image
	
public:

/**
@brief Image : constructeur de la classe Image
**/
	Image();

/**
@brief Image : constructeur de la classe Image
@param unsigned int ty la hauteur de l'image
@param unsigned int tx la largeur de l'image
**/
	Image(unsigned int ty, unsigned int tx);

/**
@brief ~Image : destructeur de la classe Image
**/
	~Image();

/**
@brief getCouleur : accesseur de couleurs
@param unsigned int y la hauteur de la couleur
@param unsigned int x la largeur de la couleur
@return Couleur la couleur de l'image aux coordonnées y,x
**/
	Couleur getCouleur(unsigned int y, unsigned int x)const &;

/**
@brief setCouleurRGB : modifie seulement la couleur du pixel
@param unsigned int y la hauteur de la couleur
@param unsigned int x la largeur de la couleur
@param Couleur couleur la couleur
**/
	void setCouleurRGB(unsigned int y, unsigned int x, const Couleur & couleur);

/**
@brief setCouleurRGBA : modifie la couleur du pixel et sa transparence
@param unsigned int y la hauteur de la couleur
@param unsigned int x la largeur de la couleur
@param Couleur couleur la couleur
**/
	void setCouleurRGBA(unsigned int y, unsigned int x, const Couleur & couleur);

/**
@brief setCouleurA : modifie seulement la transparence du pixel
@param unsigned int y la hauteur de la couleur
@param unsigned int x la largeur de la couleur
@param unsigned char a la transparence
**/
	void setCouleurA(unsigned int y, unsigned int x, unsigned char a);

/**
@brief getTailley : accesseur de tailley
@return unsigned int la hauteur de l'image
**/
	unsigned int getTailley()const &;

/**
@brief getTaillex : accesseur de taillex
@return unsigned int la largeur de l'image
**/
	unsigned int getTaillex()const &;

/**
@brief setImage : permet de créer une nouvelle image 
@param unsigned int ty la hauteur de l'image
@param unsigned int tx la largeur de l'image
**/
	void setImage(unsigned int ty, unsigned int tx);

/**
@brief effacer : remplie l'image avec la couleur
@param Couleur couleur la couleur de remplissage
**/
	void effacer(const Couleur & couleur);

/**
@brief colorier : colorie l'image avec la couleur en conservant la luminosité de chaque pixels
@param Couleur couleur la couleur de coloriage
**/
	void colorier(const Couleur & couleur);

/**
@brief charger : charge une image (png par défaut)
@param string nom le chemin vers l'image
@return bool si le chargement à bien fonctionné
**/
	bool charger(const std::string & nom);

/**
@brief enregister : enregistre une image au format bmp
@param string nom le chemin vers l'image
@return bool si l'enregistrement à bien fonctionné
**/
	bool enregistrer(const std::string & nom)const &;

};

#endif
