#ifndef _MODELE
#define _MODELE

#include "Surface.h"
#include "Camera.h"
#include "Cosin.h"
#include <string>
#include "../lib/SDL2/include/SDL_mixer.h"

/**
@brief structure ImageText
**/

struct ImageText
{
	std::string nomImg; //correspond au nom de l'image
	Image * img; //correspond au pointeur vers l'image
	bool transparent; //correspond à si l'image possède des pixels transparents
};

/**
@brief classe Modele
**/

class Modele
{
private:
	unsigned int nbSurfaces; //correspond au nombre de surfaces qui composent le modèle
	Surface ** surfaces; //correspond aux surfaces du modèle
	unsigned int nbPoints; //correspond au nombre de points qui composent le modèle
	Point ** points; //correspond aux points du modèle
	unsigned int nbImages;
	ImageText * images;
	Matrice pos{3,1}; //correspond à la position du modèle
	float angle_xy=0, angle_xz=0, angle_yz=0; //correspondent aux angles du modèle en degrés
	Matrice base{3,3}; //correspond à la matrice de rotation du modèle
	float taille; //correspond à la taille du modèle
	float opacite; //correspond à l'opacité du modèle
	float distMax; //correspond à la distance maximum entre les points du modèle
	float distAffichage; //correspond au coefficient de la distance d'affichage proportionnelle à la taille du modèle et sa distMax
	std::string modele; //corresond au nom du fichier texte qui contient le modèle 3D
	std::string texture; //corresond au nom du fichier qui contient les couleurs du modèle
	Mix_Chunk * son; //correspond au son du modèle
	float intensite; //correspond à l'intensité sonore du modèle (0 à 100)
	int canal; //correspond au canal qui joue le son du modèle
	
/**
@brief calculBase : recalcul la matrice de rotation du modèle
**/
	void calculBase()&;
	
/**
@brief calculDistMax : calcul la distance maximale entre les points du modèle
**/
	void calculDistMax()&;

/**
@brief majPoint : met à jour la position du point
@param unsigned int p l'indice du point à mettre à jour
**/
	void majPoint(unsigned int p)&;

/**
@brief majPoints : met à jour la position des points
**/
	void majPoints()&;

/**
@brief calculCentre : calcule le centre de la surface
@param unsigned int s l'indice de la surface
**/
	void calculCentre(unsigned int s)&;

/**
@brief calculCentres : calcule le centre des surfaces
**/
	void calculCentres()&;

/**
@brief majCentre : met à jour le centre de la surface
@param unsigned int s l'indice de la surface
**/
	void majCentre(unsigned int s)&;

/**
@brief majCentres : met à jour le centre des surfaces
**/
	void majCentres()&;

/**
@brief majNormales : met à jour les normales des surfaces
**/
	void majNormales()&;

/**
@brief majNormale : met à jour la normale d'une surface
@param unsigned int s l'indice de la surface
**/
	void majNormale(unsigned int s)&;

/**
@brief calculNormale : calcule la normale d'une surface
@param unsigned int s l'indice de la surface
@return bool si le calcul de la normale et la surface sont valides
**/
	bool calculNormale(unsigned int s)&;
	
/**
@brief triangulation : triangule la surface
@param unsigned int s l'indice de la surface
@return bool si la triangulation de la surface est valide
**/
	bool triangulation(unsigned int s)&;

/**
@brief triangulations : triangule les surfaces
@return bool si les triangulations des surfaces sont valide
**/
	bool triangulations()&;

/**
@brief addImage : renvoie le pointeur vers l'image et la créé si besoin
@param string nom le nom de l'image
@return Image* l'addresse de l'image
**/
	Image* addImage(std::string nom)&;
	
public:
	Cosin cs; //permet d'utiliser des formules de trigonométrie précalculées

/**
@brief Modele : constructeur de la classe Modele
**/
	Modele();

/**
@brief Modele : constructeur de la classe Modele
@param string model nom du fichier texte qui contient le modèle 3D
@param string text nom du fichier qui contient les couleurs du modèle
**/
	Modele(const std::string & model, const std::string & text);
	
/**
@brief Modele : constructeur de la classe Modele
@param string model nom du fichier texte qui contient le modèle 3D
@param string text nom du fichier qui contient les couleurs du modèle
@param Matrice p la position du modèle
**/
	Modele(const std::string & model, const std::string & text, const Matrice & p);

/**
@brief ~Modele : destructeur de la classe Modele
**/
	~Modele();

/**
@brief charger : charge les points et les surfaces du modele et les textures du modèle
@return bool si le chargement a bien fonctionné
**/
	bool charger()&;
	
/**
@brief chargerModele : charge les points et les surfaces du modele
@param string model le nom du fichier qui contient le modele 3D
@return bool si le chargement a bien fonctionné
**/
	bool chargerModele(const std::string & model)&;
	
/**
@brief chargerTexture : charge les textures des surfaces
@param string text le nom du fichier qui contient la texture
@return bool si le chargement a bien fonctionné
**/
	bool chargerTexture(const std::string & text)&;
	
/**
@brief enregistrerModele : enregistre les points et les surfaces du modele
@param string model le nom du fichier qui va contenir le modele 3D
@return bool si l'enregistrement a bien fonctionné
**/
	bool enregistrerModele(const std::string & model)const &;

/**
@brief enregistrerTexture : enregistre les couleurs des surfaces du modele
@param string text le nom du fichier qui va contenir les couleurs
@return bool si l'enregistrement a bien fonctionné
**/
	bool enregistrerTexture(const std::string & text)const &;
	
/**
@brief optimiser : efface les surfaces du modèle
**/
	void optimiser()&;

/**
@brief verif : renvoie si le modèle est valide
@return si le modèle est valide
**/
	bool verif()&;
	
/**
@brief addPoint : ajoute le point au modèle
@param Matrice matRe la position relative au modèle
@return int l'indice du point si il est déja dans le modèle, -1 sinon
**/
	int addPoint(const Matrice & matRe)&;

/**
@brief delPoint : supprime un point du modèle
@param unsigned int p l'indice du point du modèle
@return bool si la suppression a bien été effectué
**/
	bool delPoint(unsigned int p)&;

/**
@brief getPoint : accesseur des points du modèle
@param unsigned int p l'indice du point du modèle
@return Point& le point
**/
	const Point& getPoint(unsigned int p)const &;

/**
@brief setPoint : mutateur des points du modèle
@param unsigned int p l'indice du point du modèle
@param Matrice matRe la position relative au modèle
@return bool si la modification a bien été effectué
**/
	bool setPoint(unsigned int p, const Matrice & matRe)&;

/**
@brief addPointSurf : ajoute un point à une surface
@param unsigned int s l'indice de la surface
@param unsigned int p l'indice auquel ajouter le point dans la surface
@param unsigned int pi l'indice du point du modèle
@return bool si l'ajout a bien été effectué
**/
	bool addPointSurf(unsigned int s, unsigned int p, unsigned int pi)&;

/**
@brief addPointSurf : ajoute un point à une surface
@param unsigned int s l'indice de la surface
@param unsigned int p l'indice auquel ajouter le point dans la surface
@param Matrice matRe la position relative au modèle
@return bool si l'ajout a bien été effectué
**/
	bool addPointSurf(unsigned int s, unsigned int p,const Matrice & matRe)&;

/**
@brief delPointSurf : supprime un point à une surface
@param unsigned int s l'indice de la surface
@param unsigned int p l'indice du point du modèle à supprimer de la surface
@return bool si la suppression a bien été effectué
**/
	bool delPointSurf(unsigned int s, unsigned int p)&;

/**
@brief getIndicePoint : renvoie l'indice du point du modèle present dans la surface
@param unsigned int s l'indice de la surface
@param unsigned int p l'indice du point de la surface
@return unsigned int l'indice du point du modèle
**/
	unsigned int getIndicePointSurf(unsigned int s, unsigned int p)const &;

/**
@brief getPointSurf : renvoie le point du modèle present dans la surface
@param unsigned int s l'indice de la surface
@param unsigned int p l'indice du point de la surface
@return Point& le point du modèle
**/
	const Point& getPointSurf(unsigned int s, unsigned int p)const &;

/**
@brief setPointSurf : modifie l'indice du point du modèle de la surface
@param unsigned int s l'indice de la surface
@param unsigned int p l'indice auquel modifier le point dans la surface
@param unsigned int pi l'indice du point du modèle
@return bool si la modification a bien été effectué
**/
	bool setPointSurf(unsigned int s, unsigned int p, unsigned int pi)&;

/**
@brief setPointSurf : modifie le point du modèle present dans la surface
@param unsigned int s l'indice de la surface
@param unsigned int p l'indice auquel modifier le point dans la surface
@param Matrice matRe la position relative au modèle
@return bool si la modification a bien été effectué
**/
	bool setPointSurf(unsigned int s, unsigned int p, const Matrice & matRe)&;

/**
@brief addSurf : ajoute une surface au modèle
@param unsigned int p1 l'indice du point du modèle
@param unsigned int p2 l'indice du point du modèle
@param unsigned int p3 l'indice du point du modèle
@return bool si l'ajout a bien été effectué
**/
	bool addSurf(unsigned int p1, unsigned int p2, unsigned int p3)&;

/**
@brief addSurf : ajoute une surface au modèle
@param unsigned int * tab le tableau d'indices de points (à ne pas effacer)
@param unsigned int nb la taille du tableau
@return bool si l'ajout a bien été effectué
**/
	bool addSurf(unsigned int * tab, unsigned int nb)&;

/**
@brief delSurf : supprime une surface du modèle
@param unsigned int s l'indice de la surface
**/
	void delSurf(unsigned int s)&;

/**
@brief depSurf : déplace une surface du modèle
@param unsigned int s l'indice de la surface
@param Matrice dep le deplacement de la surface
@return bool si le deplacement de la surface a fonctionné
**/
	bool depSurf(unsigned int s, const Matrice & dep)&;

/**
@brief depSurf : déplace une surface du modèle
@param unsigned int s l'indice de la surface
@param float x la coordonnée x à ajouter à la surface
@param float y la coordonnée y à ajouter à la surface
@param float z la coordonnée z à ajouter à la surface
@return bool si le deplacement de la surface a fonctionné
**/
	bool depSurf(unsigned int s, float x, float y, float z)&;

/**
@brief getImageSurf : accesseur de l'image d'une surface
@param unsigned int s l'indice de la surface
@return Image* l'image de la surface
**/
	Image* getImageSurf(unsigned int s)const &;

/**
@brief setImageSurf : mutateur de l'image d'une surface
@param unsigned int s l'indice de la surface
@param string nom le nom de l'image
**/
	void setImageSurf(unsigned int s, std::string nom)&;

/**
@brief setImageSurf : mutateur de l'image d'une surface
@param unsigned int s l'indice de la surface
@param Image* img pointeur vers une image
**/
	void setImageSurf(unsigned int s, Image * img)&;

/**
@brief getNomImageSurf : accesseur du nom de l'image d'une surface
@param unsigned int s l'indice de la surface
@return string nom le nom de l'image
**/
	std::string getNomImageSurf(unsigned int s)const &;

/**
@brief setPositionSurf : mutateur de posx et posy d'une surface
@param unsigned int s l'indice de la surface
@param float x la position horizontale
@param float y la position verticale
**/
	void setPositionSurf(unsigned int s, float x, float y)&;

/**
@brief getPosxSurf : accesseur de posx d'une surface
@param unsigned int s l'indice de la surface
@return float la position horizontale
**/
	float getPosxSurf(unsigned int s)const &;

/**
@brief getPosySurf : accesseur de posy d'une surface
@param unsigned int s l'indice de la surface
@return float la position verticale
**/
	float getPosySurf(unsigned int s)const &;

/**
@brief setTailleSurf : mutateur de taillex et tailley d'une surface
@param unsigned int s l'indice de la surface
@param float tx l'étirement horizontale de l'image
@param float ty l'étirement verticale de l'image
**/
	void setTailleSurf(unsigned int s, float tx, float ty)&;

/**
@brief getTaillexSurf : accesseur de taillex d'une surface
@param unsigned int s l'indice de la surface
@return float l'étirement horizontale de l'image
**/
	float getTaillexSurf(unsigned int s)const &;

/**
@brief getTailleySurf : accesseur de tailley d'une surface
@param unsigned int s l'indice de la surface
@return float l'étirement verticale de l'image
**/
	float getTailleySurf(unsigned int s)const &;

/**
@brief setAngleSurf : mutateur de angle d'une surface
@param unsigned int s l'indice de la surface
@param float ang l'angle de l'image
**/
	void setAngleSurf(unsigned int s, float ang)&;

/**
@brief getAngleSurf : accesseur de angle d'une surface
@param unsigned int s l'indice de la surface
@return float l'angle de l'image
**/
	float getAngleSurf(unsigned int s)const &;

/**
@brief getNormaleSurf : accesseur de la normale d'une surface
@param unsigned int s l'indice de la surface
@return Matrice& la normale de la surface
**/
	const Matrice& getNormaleSurf(unsigned int s)const &;

/**
@brief pointInTriangle : renvoie si le point appartient au triangle
@param Matrice p le point à tester qui appartient au meme plan que le triangle
@param Matrice t1 le premier point du triangle
@param Matrice t2 le deuxième point du triangle
@param Matrice t3 le troisième point du triangle
@return bool si le point appartient au triangle
**/
	bool pointInTriangle(const Matrice & p, const Matrice & t1, const Matrice & t2, const Matrice & t3)const &;

/**
@brief pointInSurf : renvoie si le point appartient à la surface
@param unsigned int s l'indice de la surface
@param Matrice p le point à tester qui appartient au meme plan que le triangle
@return bool si le point appartient à la surface
**/
	bool pointInSurf(unsigned int s, const Matrice & p)&;

/**
@brief getTriangleSurf : renvoie un triangle de la surface
@param unsigned int s l'indice de la surface
@param unsigned int t l'indice du triangle de la surface
@return Triangle* un triangle de la surface
**/
	Triangle* getTriangleSurf(unsigned int s, unsigned int t)&;

/**
@brief getNbTrianglesSurf : renvoie le nombre de triangles de la surface
@param unsigned int s l'indice de la surface
@return unsigned int le nombre de triangles de la surface
**/
	unsigned int getNbTrianglesSurf(unsigned int s)const &;

/**
@brief getPos : accesseur de pos
@return Matrice& pos la position du modèle
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
@brief setPos : mutateur de pos
@param Matrice pos la position du modèle
**/
	void setPos(const Matrice & pos);

/**
@brief setPosRe : mutateur de pos relativement a une caméra
@param Camera cam la caméra par rapport à laquelle on modifie la position
@param float x la position x du modèle relativement à la caméra
@param float y la position y du modèle relativement à la caméra
@param float z la position z du modèle relativement à la caméra
**/
	void setPosRe(const Camera & cam,float x, float y, float z);

/**
@brief setPosRe : mutateur de pos relativement a une caméra
@param Camera cam la caméra par rapport à laquelle on modifie la position
@param Matrice pos la position du modèle relativement à la caméra
**/
	void setPosRe(const Camera & cam, const Matrice & pos);
	
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
@brief getNbPoints : accesseur de nbPoints
@return unsigned int nbPoints le nombre de points
**/
	unsigned int getNbPoints()const &;

/**
@brief getNbPointSurf : accesseur de nbPoints de la surface
@param unsigned int s le numéro de la surface
@return unsigned int le nombre de points de la surfaces
**/
	unsigned int getNbPointsSurf(unsigned int s)const &;
	
/**
@brief getNbSurfaces : accesseur de nbSurfaces
@return unsigned int nbSurfaces le nombre de surfaces
**/
	unsigned int getNbSurfaces()const &;
	
/**
@brief getNbImages : accesseur de nbImages
@return unsigned int nbImages le nombre d'images
**/
	unsigned int getNbImages()const &;

/**
@brief getImage : accesseur d'une image
@param unsigned int i l'indice de l'image
@return Image* le pointeur vers l'image
**/
	Image* getImage(unsigned int i)const &;
	
/**
@brief getDistmax : accesseur de distMax
@return float distMax la distance maximale entre les points du modèle
**/
	float getDistMax()const &;
	
/**
@brief getTaille : accesseur de taille
@return float taille la taille du modèle
**/
	float getTaille()const &;

/**
@brief setTaille : mutateur de taille
@param float t la taille
**/
	void setTaille(float t);

/**
@brief getOpacite : accesseur de opacite
@return float opacité du modèle
**/
	float getOpacite()const &;

/**
@brief setOpacite : mutateur de opacite
@param float opac l'opacité du modèle
**/
	void setOpacite(float opac);

/**
@brief getOpaciteSurf : accesseur de opacite d'une surface
@param unsigned int s l'indice de la surface
@return float opacité de la surface
**/
	float getOpaciteSurf(unsigned int s)const &;

/**
@brief setOpaciteSurf : mutateur de opacite d'une surface
@param unsigned int s l'indice de la surface
@param float opac l'opacité de la surface
**/
	void setOpaciteSurf(unsigned int s, float opac);

/**
@brief getDistAffichage : accesseur de distAffichage
@return float distAffichage la distance d'affichage
**/
	float getDistAffichage()const &;

/**
@brief setVisible : mutateur de visible
@param float dist la distance d'affichage
**/
	void setDistAffichage(float dist);

/**
@brief getBase : accesseur de base
@return Matrice& base la matrice de rotation du modèle
**/
	const Matrice& getBase()const &;

/**
@brief getModele : accesseur de modele
@return string modele le nom du fichier texte du modèle
**/
	std::string getModele()const &;

/**
@brief setModele : mutateur de modele
@param string mod le nom du fichier texte du modèle
**/
	void setModele(const std::string & mod);

/**
@brief getTexture : accesseur de texture
@return string texture le nom du fichier texte de la texture
**/
	std::string getTexture()const &;

/**
@brief setTexture : mutateur de texture
@param string tex le nom du fichier texte de la texture
**/
	void setTexture(const std::string & tex);

/**
@brief chargerSon : charge un son dans le modèle (wav par défaut)
@param string nom le nom du fichier à charger
@return bool si le chargement a bien fonctionné
**/
	bool chargerSon(const std::string & nom);

/**
@brief jouerSon : joue le son chargé dans le modèle
@param int repet le nombre de répétitions du son supplémentaires (-1 pour une boucle infini)
@return bool si le son a bien été joué
**/
	bool jouerSon(int repet=0);

/**
@brief pauseSon : met en pause le son du modèle
**/
	void pauseSon();

/**
@brief isPauseSon : renvoie si le son est en pause
@return int renvoie 2 si aucun son n'est joué, 1 si le son est en pause, 0 sinon
**/
	int isPauseSon();

/**
@brief reprendreSon : retire la pause du son du modèle
**/
	void reprendreSon();

/**
@brief arreterSon : arrête le son du modèle
**/
	void arreterSon();

/**
@brief getIntensite : accesseur de intensite
@return float intensite la valeur de l'intensité sonore du modèle (0 à 100)
**/
	float getIntensite()const;

/**
@brief setIntensite : mutateur de intensite
@param float intens la valeur de l'intensité sonore du modèle (0 à 100)
**/
	void setIntensite(float intens);

/**
@brief majSon : met à jour le volume et le panning du modèle en fonction de la positon relative à la caméra
@param Matrice position la position du modèle relativement à la caméra
**/
	void majSon(const Matrice & position);

/**
@brief getCentreSurf : renvoie le centre d'une surface
@param unsigned int s le numéro de la surface
@return Matrice& le centre de la surface
**/
	Matrice& getCentreSurf(unsigned int s)const &;
	
/**
@brief croisSegPlan : renvoie le point d'intersection entre un segment et un plan
@param Matrice s1 le premier point du segment
@param Matrice s2 le premier point du segment
@param Matrice t1 le premier point du plan
@param Matrice t2 le deuxième point du plan
@param Matrice t3 le troisième point du plan	/les points doivent former une base d'un plan
@return Matrice* le point de croisement entre le plan et le segment, NULL sinon
**/
	Matrice* croisSegPlan(const Matrice & s1, const Matrice & s2, const Matrice & t1, const Matrice & t2, const Matrice & t3)const &;

/**
@brief collisionCube : test les collisions cubiques des modeles
@param Modele * mod1 le premier modèle
@param Modele * mod2 le deuxième modèle
@return int retourne 1 si les cubes se croisent, 2 si le cube 1 est dans le 2, 3 si le cube 2 est dans le 1, 0 sinon
**/
	friend int collisionCube(const Modele * mod1, const Modele * mod2);

/**
@brief testCollisionCube : renvoie si les cubes se croisent
@param Matrice cube1 une matrice de taille 6x1 avec xmin,xmax,ymin,ymax,zmin,zmax du premier cube
@param Matrice cube2 une matrice de taille 6x1 avec xmin,xmax,ymin,ymax,zmin,zmax du deuxième cube
@return int retourne 1 si les cubes se croisent, 2 si le cube 1 est dans le 2, 3 si le cube 2 est dans le 1, 0 sinon
**/
	int testCollisionCube(const Matrice & cube1, const Matrice & cube2)const &;

/**
@brief creationCubeModele : renvoie les coordonnées du cube englobant du modèle
@param Modele* mod le modèle dont on veut récupérer le cube englobant
@return Matrice* retourne une matrice de taille 6x1 avec xmin,xmax,ymin,ymax,zmin,zmax
**/
	friend Matrice* creationCubeModele(const Modele * mod);

/**
@brief creationCubeSurf : renvoie les coordonnées du cube englobant de la surface
@param Modele* mod le modèle
@param unsigned int s le numéro de la surface dont on veut les coordonnées du cube englobant
@return Matrice* retourne une matrice de taille 6x1 avec xmin,xmax,ymin,ymax,zmin,zmax
**/
	friend Matrice* creationCubeSurf(const Modele * mod,unsigned int s);

/**
@brief collision : test les collisions des modeles
@param Modele * mod1 le premier modèle
@param Modele * mod2 le deuxième modèle
@return bool retourne 1 si les modeles se croisent, 0 sinon
**/
	friend bool collision(Modele * mod1, Modele * mod2);

/**
@brief collisionSurf : test les collisions des surfaces
@param Modele * mod1 le premier modèle
@param unsigned int s1 le numéro de la surface du premier modèle
@param Modele * mod2 le deuxième modèle
@param unsigned int s2 le numéro de la surface du deuxième modèle
@return bool retourne 1 si les surfaces se croisent, 0 sinon
**/
	friend bool collisionSurf(Modele * mod1, unsigned int s1, Modele * mod2, unsigned int s2);

/**
@brief afficher : affiche les informations du modèle
**/
	void afficher()const &;

};

#endif
