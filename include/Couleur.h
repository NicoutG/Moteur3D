#ifndef _COULEUR
#define _COULEUR

/**
@brief classe Couleur
**/

class Couleur
{
private:
	unsigned char rouge; //correspond à la couleur rouge
	unsigned char vert; //correspond à la couleur vert
	unsigned char bleu; //correspond à la couleur bleu
	unsigned char alpha; //correspond à l'opacité
	
public:

/**
@brief Couleur : constructeur de la classe Couleur
**/
	Couleur();

/**
@brief Couleur : constructeur de la classe Couleur
@param unsigned char r la couleur rouge
@param unsigned char g la couleur verte
@param unsigned char b la couleur bleu
**/
	Couleur(unsigned char r, unsigned char g, unsigned char b);

/**
@brief Couleur : constructeur de la classe Couleur
@param unsigned char r la couleur rouge
@param unsigned char g la couleur verte
@param unsigned char b la couleur bleu
@param unsigned char a l'opacité
**/
	Couleur(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/**
@brief getR : accesseur de rouge
@return unsigned char la couleur rouge
**/
	unsigned char getR()const &;

/**
@brief setR : mutateur de rouge
@param unsigned char r la couleur rouge
**/
	void setR(unsigned char r);

/**
@brief getG : accesseur de vert
@return unsigned char la couleur verte
**/
	unsigned char getG()const &;

/**
@brief setG : mutateur de vert
@param unsigned char g la couleur verte
**/
	void setG(unsigned char g);

/**
@brief getB : accesseur de bleu
@return unsigned char la couleur bleu
**/
	unsigned char getB()const &;

/**
@brief setB : mutateur de bleu
@param unsigned char b la couleur bleu
**/
	void setB(unsigned char b);

/**
@brief getB : accesseur de alpha
@return unsigned char alpha l'opacité
**/
	unsigned char getA()const &;

/**
@brief setA : mutateur de alpha
@param unsigned char a l'opacité
**/
	void setA(unsigned char a);

/**
@brief set : mutateur de la classe
@param unsigned char r la couleur rouge
@param unsigned char g la couleur verte
@param unsigned char b la couleur bleu
**/
	void set(unsigned char r, unsigned char g, unsigned char b);

/**
@brief set : mutateur de la classe
@param unsigned char r la couleur rouge
@param unsigned char g la couleur verte
@param unsigned char b la couleur bleu
@param unsigned char a l'opacité
**/
	void set(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/**
@brief afficher : affiche les couleurs
**/
	void afficher()const &;

};

#endif
