#include "../include/Image.h"
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstring>

Image::Image()
{
	tailley=0;
	taillex=0;
	couleurs=NULL;
}

Image::Image(unsigned int ty, unsigned int tx)
{
	assert(ty>0);
	assert(tx>0);
	tailley=ty;
	taillex=tx;
	couleurs=new Couleur * [tailley];
	for (unsigned int i=0;i<tailley;i++)
		couleurs[i]=new Couleur [taillex];
}

Image::~Image()
{
	if (couleurs!=NULL) {
		for (unsigned int i=0;i<tailley;i++)
			delete [] couleurs[i];
		delete [] couleurs;
	}
}

Couleur Image::getCouleur(unsigned int y,unsigned int x)const &
{
	assert(y<tailley);
	assert(x<taillex);
	return couleurs[y][x];
}

void Image::setCouleurRGB(unsigned int y,unsigned int x,const Couleur & couleur)
{
	assert(y<tailley);
	assert(x<taillex);
	couleurs[y][x].set(couleur.getR(),couleur.getG(),couleur.getB());
}

void Image::setCouleurRGBA(unsigned int y,unsigned int x,const Couleur & couleur)
{
	assert(y<tailley);
	assert(x<taillex);
	couleurs[y][x].set(couleur.getR(),couleur.getG(),couleur.getB(),couleur.getA());
}

void Image::setCouleurA(unsigned int y, unsigned int x, unsigned char a)
{
	assert(y<tailley);
	assert(x<taillex);
	couleurs[y][x].setA(a);
}

unsigned int Image::getTailley()const &
{
	return tailley;
}

unsigned int Image::getTaillex()const &
{
	return taillex;
}

void Image::setImage(unsigned int ty,unsigned int tx)
{
	if (couleurs!=NULL) {
		for (unsigned int i=0;i<tailley;i++)
			delete [] couleurs[i];
		delete [] couleurs;
	}
	assert(ty>0);
	assert(tx>0);
	tailley=ty;
	taillex=tx;
	couleurs=new Couleur * [tailley];
	for (unsigned int i=0;i<tailley;i++)
		couleurs[i]=new Couleur [taillex];
}

void Image::effacer(const Couleur & couleur)
{
	unsigned char r=couleur.getR();
	unsigned char g=couleur.getG();
	unsigned char b=couleur.getB();
	unsigned char a=couleur.getA();
	for (unsigned int j=0;j<tailley;j++)
		for (unsigned int i=0;i<taillex;i++)
			couleurs[j][i].set(r,g,b,a);
}

void Image::colorier(const Couleur & couleur)
{
	float lum;
	for (unsigned int j=0;j<tailley;j++)
		for (unsigned int i=0;i<taillex;i++) {
			lum=(couleurs[j][i].getR()+couleurs[j][i].getG()+couleurs[j][i].getB())/765.0;
			couleurs[j][i].set(couleur.getR()*lum,couleur.getG()*lum,couleur.getB()*lum);
		}
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	switch (bpp)
	{
		case 1: return *p;break;
		case 2: return *(Uint16 *)p;break;
		case 3: {
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
		}break;
		case 4: return *(Uint32 *)p;break;
		default: return 0;
      }
}

bool Image::charger(const std::string & nom)
{
	std::string nomf=nom;
	if (nomf.size()<5 ||(nomf[nomf.size()-4]!='.' && nomf[nomf.size()-5]!='.'))
		nomf=nomf+".png";
	char nom2[100];
	strcpy(nom2, nomf.c_str());
	SDL_Surface * surface;
	surface=IMG_Load(nom2);
	if (surface==NULL) {
		std::cout<<"Chargement de l'image "<<nomf<<" impossible"<<std::endl;
		if (couleurs!=NULL) {
			for (unsigned int i=0;i<tailley;i++)
				delete [] couleurs[i];
			delete [] couleurs;
		}
		couleurs=NULL;
		taillex=0;
		tailley=0;
		return 0;
	}
	if (surface->h!=int(tailley) ||surface->w!=int(taillex))
		setImage(surface->h,surface->w);
	Uint8 r,g,b,a;
	for (unsigned int j=0;j<tailley;j++)
		for (unsigned int i=0;i<taillex;i++)	{
			SDL_GetRGBA(getpixel(surface,i,j),surface->format,&r,&g,&b,&a);
			couleurs[j][i].set(r,g,b,a);
		}
	SDL_FreeSurface(surface);
	return 1;
}

bool Image::enregistrer(const std::string & nom)const &
{
	assert(tailley!=0 && taillex!=0);
	SDL_Surface * surface = SDL_CreateRGBSurfaceWithFormat(0, taillex, tailley, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_LockSurface(surface);
	Uint32 * pixels=(Uint32 *)surface->pixels;
	Uint32 couleur;
	int indice=0;
	for (unsigned int j=0;j<tailley;j++)
		for (unsigned int i=0;i<taillex;i++) {
			couleur=SDL_MapRGBA(surface->format, couleurs[j][i].getR(), couleurs[j][i].getG(), couleurs[j][i].getB(), couleurs[j][i].getA());
			pixels[indice]=couleur;
			indice++;
		}
	char nom2[100];
	strcpy(nom2, nom.c_str());
	return (0==SDL_SaveBMP(surface,nom2));
}
