#include "../include/Moteur3D.h"
#include "../include/Affichage_SDL.h"
#include <iostream>
#include <ctime>
#include <chrono>
using namespace std;

bool evenement(int & touche)
{
	SDL_Event event;
	bool p=1, quitter=1;
	if (SDL_PollEvent(&event)){
		do
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:{
					touche=event.key.keysym.sym;
            				p=1;
            				if (event.key.keysym.sym==SDLK_ESCAPE)
            					quitter=0;
				}break;
				case SDL_KEYUP:{
					if (touche==event.key.keysym.sym)
						p=0;
				}break;
				case SDL_WINDOWEVENT:{
					if (event.window.event == SDL_WINDOWEVENT_CLOSE)
						quitter=0;
				}break;
				default:break;
			}
		}while (SDL_PollEvent(&event));
    		if (!p)
    			touche='-';
    	}
    	return quitter;
}

bool deplacement(Camera & cam, int & touche)
{
	bool b=evenement(touche);
	float vitesse=20;
	switch (touche) {
		case 'z':{
			cam.setX(cam.getX()+vitesse*cam.cs.Cos(cam.getAngxy()));
			cam.setY(cam.getY()-vitesse*cam.cs.Sin(cam.getAngxy()));
		}break;
		case 's':{
			cam.setX(cam.getX()-vitesse*cam.cs.Cos(cam.getAngxy()));
			cam.setY(cam.getY()+vitesse*cam.cs.Sin(cam.getAngxy()));
		}break;
		case 'q': {
			cam.setX(cam.getX()-vitesse*cam.cs.Sin(cam.getAngxy()));
			cam.setY(cam.getY()-vitesse*cam.cs.Cos(cam.getAngxy()));
		}break;
		case 'd': {
			cam.setX(cam.getX()+vitesse*cam.cs.Sin(cam.getAngxy()));
			cam.setY(cam.getY()+vitesse*cam.cs.Cos(cam.getAngxy()));
		}break;
		case 'a':cam.setZ(cam.getZ()+vitesse);break;
		case 'e':cam.setZ(cam.getZ()-vitesse);break;
		case 1073741906:cam.setAngxz(cam.getAngxz()+6);break;
		case 1073741905:cam.setAngxz(cam.getAngxz()-6);break;
		case 1073741904:cam.setAngxy(cam.getAngxy()+6);break;
		case 1073741903:cam.setAngxy(cam.getAngxy()-6);break;
		case 'w':cam.setAngyz(cam.getAngyz()+6);break;
		case 'c':cam.setAngyz(cam.getAngyz()-6);break;
	}
	return b;
}

int main ()
{
	Camera cam(20,38);
	Moteur3D moteur("cubes");
	moteur.setCouleurFond(176,224,230);
	Affichage_SDL aff(800,1520);
	moteur.getModele(4)->charger();
	moteur.getModele(4)->depSurf(3,100,0,0);
	int touche='-';
	int t=0;
	time_t temps;
	temps=time(0);
	auto start = std::chrono::steady_clock::now();
	typedef std::chrono::duration<float> fsec;
	fsec temp;
	unsigned int fps=20;
	while (deplacement(cam,touche)) {
		aff.effacer();
		aff.ajouter(moteur.getImage(cam,400,760),0,0,800,1520);
		aff.afficher();
		moteur.getModele(0)->setAng(moteur.getModele(0)->getAngxy()+6,moteur.getModele(0)->getAngxz()+6,0);
		moteur.getModele(1)->setAng(moteur.getModele(1)->getAngxy()+3,moteur.getModele(1)->getAngxz(),moteur.getModele(1)->getAngyz()+6);
		moteur.getModele(2)->setAng(moteur.getModele(2)->getAngxy(),moteur.getModele(2)->getAngxz()-6,moteur.getModele(2)->getAngyz()+11);
		moteur.getModele(3)->setAng(moteur.getModele(3)->getAngxy()+11,moteur.getModele(3)->getAngxz()+3,0);
		temp=std::chrono::steady_clock::now()-start;
		/*if (temp.count()<(t+1)/fps)
			SDL_Delay(1000/fps-int(1000*temp.count())%(1000/fps));*/
		if (t%300==0) {
			cout<<difftime(time(0),temps)<<endl;
			temps=time(0);
		}
		t++;
	}
	return 0;
}
