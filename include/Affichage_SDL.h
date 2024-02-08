#ifndef _AFFICHAGE_SDL
#define _AFFICHAGE_SDL

#include "../lib/SDL2/include/SDL.h"
#include "Image.h"

/**
@brief classe Affichage_SDL
**/

class Affichage_SDL
{
private:
	SDL_Window * window; //correspond à la fenètre SDL
	SDL_Renderer * renderer; //correspond au renderer
	unsigned int tailley, taillex; //correspondent aux dimensions de la fenètre
	
public:

/**
@brief Affichage_SDL : constructeur de la classe Affichage_SDL
@param unsigned int ty la taille verticale de l'écran
@param unsigned int tx la taille horizontale de l'écran
**/
	Affichage_SDL(unsigned int ty, unsigned int tx);

/**
@brief ~Affichage_SDL : destructeur de la classe Affichage_SDL
**/
	~Affichage_SDL();

/**
@brief effacer : reinitialise le renderer
**/
	void effacer();

/**
@brief afficher : affiche le renderer à l'écran
**/
	void afficher();

/**
@brief ajouter : ajoute une image au renderer
@param Image img l'image à ajouter au renderer
@param unsigned int posy la coordonnée verticale du coin supérieur gauche de l'image sur le renderer
@param unsigned int posy la coordonnée horizontale du coin supérieur gauche de l'image sur le renderer
@param unsigned int hauteur la hauteur de l'image sur le renderer
@param unsigned int largeur la largeur de l'image sur le renderer
**/
	void ajouter(Image const * img, unsigned int posy, unsigned int posx, unsigned int hauteur, unsigned int largeur);
	
};

#endif
