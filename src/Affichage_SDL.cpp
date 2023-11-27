#include "../include/Affichage_SDL.h"
#include <cassert>
#include <iostream>

Affichage_SDL::Affichage_SDL(unsigned int ty, unsigned int tx)
{
	tailley=ty;
	taillex=tx;
	assert(SDL_Init(SDL_INIT_VIDEO)==0);
	window=SDL_CreateWindow("Fenetre",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,taillex,tailley,SDL_WINDOW_SHOWN);
	assert(NULL!=window);
	renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	assert(NULL!=renderer);
	effacer();
	afficher();
}

Affichage_SDL::~Affichage_SDL()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window=NULL;
	renderer=NULL;
	SDL_Quit();
}

void Affichage_SDL::effacer()
{
	SDL_RenderClear(renderer);
	SDL_Rect rect;
	rect={0, 0, int(taillex), int(tailley)};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
}

void Affichage_SDL::afficher()
{
	SDL_RenderPresent(renderer);
}

void Affichage_SDL::ajouter(const Image * img, unsigned int posy, unsigned int posx, unsigned int hauteur, unsigned int largeur)
{
	assert(img->getTailley()!=0 && img->getTaillex()!=0);
	SDL_Surface * surface = SDL_CreateRGBSurfaceWithFormat(0, img->getTaillex(), img->getTailley(), 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_LockSurface(surface);
	Uint32 * pixels=(Uint32 *)surface->pixels;
	Uint32 couleur;
	int indice=0;
	for (unsigned int j=0;j<img->getTailley();j++)
		for (unsigned int i=0;i<img->getTaillex();i++) {
			couleur=SDL_MapRGB(surface->format, img->getCouleur(j,i).getR(), img->getCouleur(j,i).getG(), img->getCouleur(j,i).getB());
			pixels[indice]=couleur;
			indice++;
		}
	SDL_Texture *texture=NULL;
	texture=SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);
	SDL_Rect position={int(posx),int(posy),int(largeur),int(hauteur)};
	SDL_RenderCopy(renderer,texture,NULL,&position);
	SDL_DestroyTexture(texture);
}
