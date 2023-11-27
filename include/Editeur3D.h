#ifndef _EDITEUR3D
#define _EDITEUR3D

#include "../include/Camera.h"
#include "../include/Moteur3D.h"
#include "../include/Affichage_SDL.h"

/**
@brief classe Editeur3D
**/

class Editeur3D
{
public:
	
	void Lancement();

private:
	int menu=0;
	Camera cam{20,38};
	Moteur3D moteur;
	Image * image;
	Modele * modele;
	Modele * axe;
	Modele * boussole;
	int touche;
	int xs;
	int ys;
	bool presse;
	int point;
	int surface;
	unsigned int pointsSurf[2];
	unsigned int nbp;
	
	bool evenement();
	
	bool action();
	
	void placementPointsModele(Point2D * points, unsigned int nbPoints);
	
	void afficherPoint()const;
	
	void afficherSurface()const;
	
	void afficherImage()const;
	
	void afficherMenuChoix()const;
	
	void menuChoix();
	
	void afficherAjoutPoint()const;
	
	void ajoutPoint();
	
	void afficherSelectionPoint()const;
	
	void selectionPoint();
	
	void afficherMenuPoint()const;
	
	void menuPoint();
	
	void afficherDepPoint()const;
	
	void depPoint();
	
	void afficherAjoutSurface()const;
	
	void ajoutSurface();
	
	void afficherSelectionSurface()const;
	
	void selectionSurface();
	
	void afficherMenuSurface()const;
	
	void menuSurface();
	
	void afficherDepSurface()const;
	
	void depSurface();
	
	void afficherAjoutPointSurface()const;
	
	void ajoutPointSurface();
	
	void afficherSupPointSurface()const;
	
	void supPointSurface();
	
	void afficherMenuTextureSurface()const;
	
	void menuTextureSurface();
	
	void afficherModifImageSurface()const;
	
	void modifImageSurface();
	
	void afficherDepImageSurface()const;
	
	void depImageSurface();
	
	void afficherTourneImageSurface()const;
	
	void tourneImageSurface();
	
	void afficherTailleImageSurface()const;
	
	void tailleImageSurface();
	
	void enregistrerModele();
	
	void enregistrerTexture();
	
	int getPoint(int x, int y);
	
	int getSurface(int x, int y);
	
	bool SurfaceIn(unsigned int s, Point2D * points);
	
};

#endif
