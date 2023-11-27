#ifndef _MATRICE
#define _MATRICE

/**
@brief classe Matrice
**/

class Matrice
{
private:
	float ** tab; //correspond à la matrice
	unsigned int dimy; //correspond à la hauteur de la matrice
	unsigned int dimx; //correspond à la largeur de la matrice
	
public:

/**
@brief Matrice : constructeur de la classe Matrice
**/
	Matrice();

/**
@brief Matrice : constructeur de la classe Matrice
@param unsigned int y la taille verticale de la matrice
@param unsigned int x la taille horizontale de la matrice
**/
	Matrice(unsigned int y, unsigned int x);
	
/**
@brief ~Matrice : destructeur de la classe Matrice
**/
	~Matrice();
	
/**
@brief getDimy : accesseur de la taille verticale de la matrice
**/
	unsigned int getDimy()const &;
	
/**
@brief getDimx : accesseur de la taille horizontale de la matrice
**/
	unsigned int getDimx()const &;
	
/**
@brief get : accesseur de la valeur yx de la matrice
@param unsigned int y l'indice verticale de la matrice
@param unsigned int x l'indice horizontale de la matrice
@return float la valeur à l'indice yx de la matrice
**/
	float get(unsigned int y, unsigned int x)const &;
	
/**
@brief set : mutateur de la valeur yx de la matrice
@param unsigned int y l'indice verticale de la matrice
@param unsigned int x l'indice horizontale de la matrice
@param float val la nouvelle valeur
**/
	void set(unsigned int y, unsigned int x, float val);
	
/**
@brief getCol : renvoie la colonne x de la matrice
@param unsigned int x l'indice horizontale de la matrice
@param Matrice& la colonne d'indice x
@return Matrice& la colonne
**/
	Matrice getCol(unsigned int x)const &;
	
/**
@brief getLig : renvoie la ligne y de la matrice
@param unsigned int y l'indice verticale de la matrice
@param Matrice& la ligne d'indice y
@return Matrice& la ligne
**/
	Matrice getLig(unsigned int y)const &;

/**
@brief modifDims : redimmensionne la matrice
@param unsigned int y la taille verticale de la matrice
@param unsigned int x la taille horizontale de la matrice
**/
	void modifDims(unsigned int y, unsigned int x)&;
	
/**
@brief operator + : surcharge de l'operateur d'addition de matrices
@param Matrice mat la matrice à additionner
@return Matrice la somme des 2 matrices
**/
	Matrice operator + (const Matrice & mat)const &;
	
/**
@brief operator - : surcharge de l'operateur de soustraction de matrices
@param Matrice mat la matrice à soustraire
@return Matrice la soustraction des 2 matrices
**/
	Matrice operator - (const Matrice & mat)const &;
	
/**
@brief operator * : surcharge de l'operateur de multiplication de matrices
@param Matrice mat la matrice à multiplier
@return Matrice le produit des 2 matrices
**/
	Matrice operator * (const Matrice & mat)const &;
	
/**
@brief operator * : surcharge de l'operateur de multiplication entre un réel et une matrice
@param float mult le coefficient multiplicateur
@return Matrice le produit du coefficient et de la matrice
**/
	Matrice operator * (float mult)const &;
	
/**
@brief operator * : surcharge de l'operateur de multiplication entre un réel et une matrice
@param float mult le coefficient multiplicateur
@param Matrice mat la matrice à multiplier
@return Matrice le produit du coefficient et de la matrice
**/
	friend Matrice operator * (float mult, const Matrice & mat);
	
/**
@brief operator / : surcharge de l'operateur de division d'une matrice par un réel
@param float div le coefficient divisieur
@return Matrice la division entre la matrice et le coefficient
**/
	Matrice operator / (float div)const &;
	
/**
@brief operator = : surcharge de l'operateur d'assignation
@param Matrice mat la matrice à assigner
**/
	void operator = (const Matrice & mat)&;

/**
@brief prodScal : renvoie le produit scalaire des 2 matrices
@param Matrice mat1 la première matrice
@param Matrice mat2 la deuxième matrice
@return float le produit scalaire des 2 matrices
**/
	friend float prodScal(const Matrice & mat1, const Matrice & mat2);
	
/**
@brief prodScal : renvoie la norme de la matrice
@param Matrice mat la matrice
@return float la norme de la matrice
**/
	friend float norme(const Matrice & mat);
	
/**
@brief prodVect : renvoie le produit vectoriel des 2 matrices
@param Matrice mat1 la première matrice de dim 3,1
@param Matrice mat2 la deuxième matrice de dim 3,1
@return float le produit vectoriel des 2 matrices
**/
	friend Matrice prodVect(const Matrice & mat1, const Matrice & mat2);
	
/**
@brief orthonormalisation : orthonormalise le plan formé  par les 2 vecteurs
@param Matrice mat1 la première matrice de dim 3,1 à orthonormer
@param Matrice mat2 la deuxième matrice de dim 3,1 à orthonormer
**/
	friend void orthonormalisation(Matrice & m1, Matrice & m2);
	
/**
@brief projection : renvoie le projeté de la matrice sur le plan
@param Matrice m la matrice de dim 3,1 à projeter
@param Matrice b1 la première matrice de dim 3,1 du plan
@param Matrice b2 la deuxième matrice de dim 3,1 du plan
@return Matrice le projeté de la matrice sur le plan
**/
	friend Matrice projection(const Matrice & m, const Matrice & b1, const Matrice & b2);
	
/**
@brief distance : renvoie la distance de la matrice au plan formé  par les 2 vecteurs
@param Matrice m la matrice de dim 3,1 à projeter
@param Matrice b1 la première matrice de dim 3,1 du plan
@param Matrice b2 la deuxième matrice de dim 3,1 du plan
@return float la distance de la matrice 
**/
	friend float distance(const Matrice & m, const Matrice & b1, const Matrice & b2);
	
/**
@brief afficher : affiche la matrice
**/
	void afficher()const &;
	
};

#endif
