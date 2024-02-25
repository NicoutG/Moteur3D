#ifndef _MOTEUR3D
#define _MOTEUR3D

#include "Modele.h"
#include "Camera.h"
#include "Image.h"
#include <vector>

/**
@brief structure Point2D
**/

struct Point2D
{
	Matrice pos2D{2,1};
	Matrice posCam{3,1};
	bool devant;
};

/**
@brief structure SurfaceAff
**/

struct SurfaceAff
{
	Modele * modele;
	unsigned int surface;
	bool transparent;
	float distance;
};

/**
@brief structure Couche
**/

struct Couche
{
	Couleur couleur; 
	float distance;
};

/**
@brief classe Moteur3D
**/

class Moteur3D
{
private:
	unsigned int nbModeles; //correspond au nombre de modèles présent dans le moteur 3D
	Modele ** modeles; //correspond au modèles présent dans le moteur 3D
	std::vector <SurfaceAff*> aff; //les surfaces à afficher
	Point2D * points; //correspond aux points du modèle à afficher
	Couleur couleurFond; //correspond à la couleur du fond du moteur 3D
	Image image; //correspond à l'image du moteur
	float ** distances; //correspond à la distance au carré des points de l'image
	std::vector <Couche> ** transparences; //correspond aux différentes couches de transparence
	float distAffichage; //correspond au coefficient de la distance d'affichage proportionnelle à la taille des modèles, leur distMax et leur distance d'affichage
	Camera * cam; //correspond à la camera
	Mix_Music * musique; //correspond à la musique du moteur
	Matrice posLumiere{3,1}; //correspond à la position de la lumière

/**
@brief creationTableau : redimensionne distances et image si neccessaire 
@param unsigned int y la taille verticale de l'image demandée
@param unsigned int x la taille horizontale de l'image demandée
**/
	void creationTableau(unsigned int y, unsigned int x)&;

/**
@brief initialiserTableau : remplie distances avec -1 et image avec couleurFond
**/
	void initialiserTableau()&;

/**
@brief gestionModeles : optimise les modèles trop éloignés de la camera et charge les modèles proche de la camera
**/
	void gestionModeles()&;

/**
@brief affTri : tri les faces à afficher de la plus proche à la plus éloignée
@param unsigned int deb le debut du tableau à trier
@param unsigned int fin la fin du tableau à trier
@return std::vector <SurfaceAff*> le tableau trié
**/
	std::vector <SurfaceAff*> affTri(unsigned int deb, unsigned int fin)&;

/**
@brief afficherSurfaces : Fait apparaitre les surfaces sur l'image
**/
	void afficherSurfaces()&;

/**
@brief majPoints : calcul la position sur l'image des points du modèle
@param Modele* modele le pointeur vers le modèle à faire apparaitre sur l'image
**/
	void majPoints(const Modele * modele)&;

/**
@brief majPoints : calcul la position sur l'image des points d'une surface
@param Modele* modele le pointeur vers le modèle à faire apparaitre sur l'image
@param unsigned int s l'indice de la surface du modèle
**/
	void majPoints(const Modele * modele, unsigned int s)&;

/**
@brief afficherSurface : Fait apparaitre les surfaces sur l'image
@param Modele* modele le pointeur vers le modèle à faire apparaitre sur l'image
@param unsigned int s l'indice de la surface du modèle
**/
	void afficherSurface(Modele * modele, unsigned int s)&;

/**
@brief traitementTriangle : projete les points du triangle devant la caméra
@param Modele* modele le pointeur vers le modèle à faire apparaitre sur l'image
@param unsigned int s l'indice de la surface du modèle
@param Point2D * p1 pointeur vers le point 1
@param Point2D * p2 pointeur vers le point 2
@param Point2D * p3 pointeur vers le point 3
@param float ang l'angle de la surface par rapport à la caméra
**/
	void traitementTriangle(const Modele * modele, unsigned int s, Point2D * p1, Point2D * p2, Point2D * p3, float ang)&;

/**
@brief interpolationTriangle : Insère les coordonnées x des points entre p1, p2 et p3 pour chaque coordonnées y
@param Point2D * p1 pointeur vers le point 1
@param Point2D * p2 pointeur vers le point 2
@param Point2D * p2 pointeur vers le point 3
@param unsigned int dep la première coordonnée y du tableau
@param unsigned int fin la dernière coordonnée y du tableau
@param vector <int> tab [] le tableau de coordonnées x à remplir
**/
	void interpolationTriangle(Point2D * p1, Point2D * p2, Point2D * p3, unsigned int dep, unsigned int fin, std::vector <int> tab [])&;

/**
@brief interpolationLigne : Insère les coordonnées x des points entre p1 et p2 pour chaque coordonnées y
@param Point2D * p1 pointeur vers le point 1
@param Point2D * p2 pointeur vers le point 2
@param unsigned int dep la première coordonnée y du tableau
@param unsigned int fin la dernière coordonnée y du tableau
@param vector <int> tab [] le tableau de coordonnées x à remplir
**/
	void interpolationLigne(Point2D * p1, Point2D * p2, unsigned int dep, unsigned int fin, std::vector <int> tab [])&;

/**
@brief afficherTriangle : Fait apparaitre les triangles sur l'image
@param Modele* modele le pointeur vers le modèle à faire apparaitre sur l'image
@param unsigned int s l'indice de la surface du modèle
@param Point2D * p1 pointeur vers le point 1
@param Point2D * p2 pointeur vers le point 2
@param Point2D * p3 pointeur vers le point 3
@param float ang l'angle de la surface par rapport à la caméra
@param float distMin la distance entre la caméra et le plan de la surface
**/
	void afficherTriangle(const Modele * modele, unsigned int s, Point2D * p1, Point2D * p2, Point2D * p3, float ang, float distMin)&;

/**
@brief addTransparent : ajoute une couche transparente au pixel
@param unsigned int x la coordonnée x du pixel
@param unsigned int y la coordonnée y du pixel
@param Couleur coul la couleur de la couche transparente
@param float dist la distance de la couche transparente
**/
	void addTransparent(unsigned int x, unsigned int y, const Couleur & coul, float dist)&;

/**
@brief appliquerTransparence : applique les couches transparentes sur l'image
**/
	void appliquerTransparence()&;
	
public:

/**
@brief Moteur3D : constructeur de la classe Moteur3D
**/
	Moteur3D();

/**
@brief Moteur3D : constructeur de la classe Moteur3D
@param string moteur le fichier texte qui contient les informations sur le moteur3D et les modèles
**/
	Moteur3D(const std::string & moteur);

/**
@brief ~Moteur3D : destructeur de la classe Moteur3D
**/
	~Moteur3D();

/**
@brief charger : charge les informations sur le moteur3D et les modèles
@param string moteur le fichier texte qui contient les informations sur le moteur3D et les modèles
@return bool si le chargement a bien fonctionné
**/
	bool charger(const std::string & moteur)&;

/**
@brief enregistrer : enregistre les informations sur le moteur3D et les modèles
@param string moteur le fichier texte qui contient les informations sur le moteur3D et les modèles
@return bool si l'enregistrement a bien fonctionné
**/
	bool enregistrer(const std::string & moteur)const &;

/**
@brief getCouleurFond : accesseur de couleurFond
@return Couleur& couleurFond la couleur du fond
**/
	const Couleur& getCouleurFond()const &;

/**
@brief setCouleurFond : mutateur de couleurFond
@param unsigned int r la couleur rouge
@param unsigned int g la couleur verte
@param unsigned int b la couleur bleue
**/
	void setCouleurFond(unsigned char r, unsigned char g, unsigned char b);
	
/**
@brief getDistAffichage : accesseur de distAffichage
@return float distAffichage la distance d'affichage
**/
	float getDistAffichage()const &;

/**
@brief setDistAffichage : mutateur de distAffichage
@param float dist la distance d'affichage
**/
	void setDistAffichage(float dist);

/**
@brief getNbModeles : accesseur de nbModeles
@return unsigned int nbModeles le nombre de modèles du moteur 3D
**/
	unsigned int getNbModeles()const &;

/**
@brief addModele : ajoute un modèle au moteur 3D
@param Modele * modele l'adrese du modèle à ajouter
**/
	void addModele(Modele * modele)&;

/**
@brief addModele : ajoute un modèle au moteur 3D
@param string modele nom du fichier texte qui contient le modèle 3D
@param string texture nom du fichier qui contient les couleurs du modèle
**/
	void addModele(const std::string & modele, const std::string & texture)&;

/**
@brief addModele : ajoute un modèle au moteur 3D
@param string modele nom du fichier texte qui contient le modèle 3D
@param string texture nom du fichier qui contient les couleurs du modèle
@param Matrice pos la position du modèle
**/
	void addModele(const std::string & modele, const std::string & texture, const Matrice & pos)&;

/**
@brief delModele : supprime un modèle du moteur 3D
@param unsigned int m l'indice du modèle à supprimer
**/
	void delModele(unsigned int m)&;

/**
@brief getModele : renvoie le modèle
@param unsigned int m l'indice du modèle
@return Modele* le pointeur vers le modèle
**/
	Modele* getModele(unsigned int m)&;

/**
@brief getImage : renvoie l'image des modèles vue par la caméra
@param Camera camera la caméra
@param unsigned int tailley la taille verticale de l'image
@param unsigned int taillex la taille horizontale de l'image
@return Image l'image
**/
	Image* getImage(Camera & camera,unsigned int tailley, unsigned int taillex)&;

/**
@brief getImage : accesseur de image
@return Image l'image
**/
	Image* getImage()&;

/**
@brief getDistances : accesseur de distances
@return float**& distances le tableau des distances au carré des points de l'images
**/
	float**& getDistances()&;

/**
@brief getPoints2D : renvoie les les coordonnées relatives au moteur des points du modele
@param Modele * modele le modele dont on veut calculer les positions des points
@return Point2D * les coordonnées relatives au moteur des points du modele 
**/
	Point2D* getPoints2D(const Modele * modele)const;

/**
@brief getPoints2D : renvoie les coordonnées relatives au moteur des points du modele (calcul seulement les points de la surface)
@param Modele * modele le modele dont on veut calculer les positions des points
@param unsigned int s l'indice de la surface du modèle
@return Point2D * les coordonnées relatives au moteur des points du modele (calcul seulement les points de la surface)
**/
	Point2D* getPoints2D(const Modele * modele, unsigned int s)const;

/**
@brief getPosCam : renvoie les coordonnées en fonction de la caméra
@param Matrice pos les coordonnées de l'objet
@return Matrice les coordonnées de l'objet relativement à la caméra 
**/
	Matrice getPosCam(const Matrice & pos)const;

/**
@brief getCam : accesseur de cam
@return Camera * la camera du moteur
**/
	Camera* getCam()const;

/**
@brief getNbAff: accesseur de nbAff
@return unsigned int le nombre de surfaces afficher
**/
	unsigned int getNbAff()const;

/**
@brief pauseSons : met en pause tout les sons du moteur de jeu
**/
	void pauseSons();

/**
@brief reprendreSons : retire la pause de tout les sons du moteur de jeu
**/
	void reprendreSons();

/**
@brief arreterSons : arrête tout les sons du moteur de jeu
**/
	void arreterSons();

/**
@brief chargerMusique : charge une musique dans le moteur de jeu (mp3 par défaut)
@param string nom le nom du fichier à charger
@return bool si le chargement a bien fonctionné
**/
	bool chargerMusique(const std::string & nom);

/**
@brief jouerMusique : joue la musique chargé dans le moteur de jeu
@param int repet le nombre de répétitions de la musique supplémentaires (-1 pour une boucle infini)
@return bool si la musique a bien été joué
**/
	bool jouerMusique(int repet=-1);

/**
@brief pauseMusique : met en pause la musique du moteur de jeu
**/
	void pauseMusique();

/**
@brief isPauseMusique : renvoie si le musique est en pause
@return int renvoie 2 si la musique n'est joué, 1 si la musique est en pause, 0 sinon
**/
	int isPauseMusique();

/**
@brief reprendreMusique : retire la pause de la musique du moteur de jeu
**/
	void reprendreMusique();

/**
@brief arreterMusique : arrête la musique du moteur de jeu
**/
	void arreterMusique();

/**
@brief recommencerMusique : recommence la musique depuis le début
**/
	void recommencerMusique();

/**
@brief setVolume : mutateur du volume de la musique du moteur de jeu
@param float volume la nouvelle valeur du volume (0 à 100)
**/
	void setVolume(float volume);

/**
@brief getVolume : accesseur du volume de la musique du moteur de jeu
@return float la valeur du volume de la musique du moteur de jeu (0 à 100)
**/
	float getVolume()const;

/**
@brief setPosLumiere : mutateur de posLumiere
@param const Matrice& pos la position de la lumière
**/
	void setPosLumiere(const Matrice & pos);

/**
@brief setPosLumiere : mutateur de posLumiere
@param float x la position x de la lumière
@param float y la position y de la lumière
@param float z la position z de la lumière
**/
	void setPosLumiere(float x, float y, float z);

/**
@brief getPosLumiere : accesseur de posLumiere
@return const Matrice& la position de la lumière
**/
	const Matrice& getPosLumiere()const &;

};

#endif
