#include "../include/Editeur3D.h"
#include<cassert>
#include<iostream>
#include <chrono>
using namespace std;

void Editeur3D::Lancement()
{
	string model,text;
	Matrice pos(3,1);
	moteur.setDistAffichage(100000000);
	moteur.addModele("axe","axe",pos);
	axe=moteur.getModele(0);
	moteur.addModele("axe","axe",pos);
	boussole=moteur.getModele(1);
	boussole->setTaille(0.01);
	modele=new Modele;
	cout<<"Nom du modele a ouvrir :"<<endl;
	cin>>model;
	cout<<"Nom de la texture a ouvrir :"<<endl;
	cin>>text;
	modele->chargerModele(model);
	modele->chargerTexture(text);
	moteur.addModele(modele);
	cam.setX(-2*modele->getDistMax());
	Affichage_SDL aff(800,1520);
	touche='-';
	auto start = std::chrono::steady_clock::now();
	typedef std::chrono::duration<float> fsec;
	fsec temp;
	unsigned int fps=16;
	int t=0;
	image=new Image;
	while (action())
	{
		aff.effacer();
		image=moteur.getImage(cam,400,760);
		placementPointsModele(moteur.getPoints2D(modele),modele->getNbPoints());
		aff.ajouter(image,0,0,800,1520);
		aff.afficher();
		temp=std::chrono::steady_clock::now()-start;
		if (temp.count()<(t+1)/fps)
			SDL_Delay(1000/fps-int(1000*temp.count())%(1000/fps));
		for (unsigned int i=0;i<30;i++)
			cout<<endl;
		t++;
	}
}

bool Editeur3D::evenement()
{
	SDL_Event event;
	bool p=1, quitter=1;
	presse=0;
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
				case SDL_MOUSEBUTTONDOWN:{
					presse=1;
				}break;
				default:break;
			}
		}while (SDL_PollEvent(&event));
    		if (!p)
    			touche='-';
    		SDL_GetMouseState(&xs,&ys);
		xs=xs*image->getTaillex()/1520;
		ys=ys*image->getTailley()/800;
    	}
    	return quitter;
}

bool Editeur3D::action()
{
	bool b=evenement();
	float vitesse=10;
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
	boussole->setPosRe(cam,2,-0.56,-0.25);
	switch (menu) {
		case 0: {
			afficherMenuChoix();
			menuChoix();
		}break;
		case 1: {
			afficherAjoutPoint();
			ajoutPoint();
		}break;
		case 2:{
			point=getPoint(xs,ys);
			afficherSelectionPoint();
			selectionPoint();
		}break;
		case 3:{
			point=getPoint(xs,ys);
			afficherAjoutSurface();
			ajoutSurface();
		}break;
		case 4:{
			surface=getSurface(xs,ys);
			afficherSelectionSurface();
			selectionSurface();
		}break;
		case 5:enregistrerModele();break;
		case 6:enregistrerTexture();break;
		case 7:{
			afficherMenuPoint();
			menuPoint();
		}break;
		case 8:{
			afficherMenuSurface();
			menuSurface();
		}break;
		case 9:{
			point=getPoint(xs,ys);
			afficherAjoutPointSurface();
			ajoutPointSurface();
		}break;
		case 10:{
			point=getPoint(xs,ys);
			afficherSupPointSurface();
			supPointSurface();
		}break;
		case 11:{
			afficherMenuTextureSurface();
			menuTextureSurface();
		}break;
		default:break;
	}
	return b;
}

void Editeur3D::placementPointsModele(Point2D * points, unsigned int nbPoints)
{
	Matrice pos(2,1);
	Couleur rouge,rouge2;
	rouge.set(200,0,0);
	rouge2.set(50,0,0);
	float ** distances=moteur.getDistances();
	float dist;
	float taille=10;
	float taillex=taille*image->getTaillex()/3040;
	float tailley=taille*image->getTailley()/1600;
	for (unsigned int i=0;i<nbPoints;i++) {
		if (points[i].devant) {
			pos=points[i].pos2D;
			if (0<=pos.get(0,0) && pos.get(0,0)<image->getTaillex() && 0<=pos.get(1,0) && pos.get(1,0)<image->getTailley()) {
				dist=norme(points[i].posCam);
				if (distances[int(pos.get(1,0))][int(pos.get(0,0))]==-1) {
					for (int y=std::max(0,int(pos.get(1,0)-tailley));y<std::min(int(image->getTailley()),int(pos.get(1,0)+tailley));y++)
						for (int x=std::max(0,int(pos.get(0,0)-taillex));x<std::min(int(image->getTaillex()),int(pos.get(0,0)+taillex));x++) {
							image->setCouleurRGB(y,x,rouge);
							distances[y][x]=dist*dist;
						}
				}
				else {
					if (dist<sqrt(distances[int(pos.get(1,0))][int(pos.get(0,0))])+1) {
						for (int y=std::max(0,int(pos.get(1,0)-tailley));y<std::min(int(image->getTailley()),int(pos.get(1,0)+tailley));y++)
							for (int x=std::max(0,int(pos.get(0,0)-taillex));x<std::min(int(image->getTaillex()),int(pos.get(0,0)+taillex));x++) {
								image->setCouleurRGB(y,x,rouge);
								distances[y][x]=dist*dist;
							}
					}
					else
						for (int y=std::max(0,int(pos.get(1,0)-tailley));y<std::min(int(image->getTailley()),int(pos.get(1,0)+tailley));y++)
							for (int x=std::max(0,int(pos.get(0,0)-taillex));x<std::min(int(image->getTaillex()),int(pos.get(0,0)+taillex));x++) {
								image->setCouleurRGB(y,x,rouge2);
							}
				}
			}
		}
	}
}

void Editeur3D::afficherPoint()const
{
	if (point!=-1)
		cout<<"Point "<<point<<" | coordonnees : x "<<modele->getPoint(point).pos.get(0,0)<<" y "<<modele->getPoint(point).pos.get(1,0)<<" z "<<modele->getPoint(point).pos.get(2,0)<<endl;
	else
		cout<<"Aucun point selectionne"<<endl;
}

void Editeur3D::afficherSurface()const
{
	if (surface!=-1) {
		cout<<"Surface "<<surface<<" | Nb points : "<<modele->getNbPointsSurf(surface)<<" | Points : ";
		for (unsigned int i=0;i<modele->getNbPointsSurf(surface);i++)
			cout<<modele->getIndicePointSurf(surface,i)<<" ";
		cout<<endl;
	}
	else
		cout<<"Aucune surface selectionnee"<<endl;
}

void Editeur3D::afficherImage()const
{
	cout<<"Image : "<<modele->getNomImageSurf(surface)<<" | Position : x "<<modele->getPosxSurf(surface)<<" y "<<modele->getPosySurf(surface)<<" | Angle : "<<modele->getAngleSurf(surface)<<" | Taille : "<<modele->getTaillexSurf(surface)<<" x "<<modele->getTailleySurf(surface)<<" y"<<endl;
}

void Editeur3D::afficherMenuChoix()const
{
	cout<<"Coordonnees : "<<cam.getX()<<" x "<<cam.getY()<<" y "<<cam.getZ()<<" z "<<endl;
	cout<<"Nb points : "<<modele->getNbPoints()<<" | Nb surfaces : "<<modele->getNbSurfaces()<<endl;
	cout<<"1 : ajouter un point"<<endl;
	cout<<"2 : selectionner un point"<<endl;
	cout<<"3 : ajouter une surface"<<endl;
	cout<<"4 : selectionner une surface"<<endl;
	cout<<"5 : enregistrer le modele"<<endl;
	cout<<"6 : enregistrer la texture"<<endl;
	cout<<endl;
}

void Editeur3D::menuChoix()
{
	nbp=0;
	axe->setOpacite(0);
	switch (touche) {
		case 1073741913:menu=1;break; //1
		case 1073741914:menu=2;break; //2
		case 1073741915:menu=3;break; //3
		case 1073741916:menu=4;break; //4
		case 1073741917:menu=5;break; //5
		case 1073741918:menu=6;break; //6
		default:break;
	}
}

void Editeur3D::afficherAjoutPoint()const
{
	cout<<"Ajout d'un point"<<endl;
}

void Editeur3D::ajoutPoint()
{
	float point;
	Matrice pos(3,1);
	cout<<"Coordonnee x :"<<endl;
	cin>>point;
	pos.set(0,0,point);
	cout<<"Coordonnee y :"<<endl;
	cin>>point;
	pos.set(1,0,point);
	cout<<"Coordonnee z :"<<endl;
	cin>>point;
	pos.set(2,0,point);
	modele->addPoint(pos);
	menu=0;
}

void Editeur3D::afficherSelectionPoint()const
{
	cout<<"Selectionnez un point"<<endl;
	afficherPoint();
	cout<<endl;
}
	
void Editeur3D::selectionPoint()
{
	if (presse) {
		if (point!=-1)
			menu=7;
		else
			menu=0;
	}
}

void Editeur3D::afficherMenuPoint()const
{
	afficherPoint();
	cout<<"Action a faire :"<<endl;
	cout<<"1 : Deplacer le point"<<endl;
	cout<<"2 : Supprimer le point"<<endl;
	cout<<"0 : Retour"<<endl;
	cout<<endl;
}

void Editeur3D::menuPoint()
{
	switch (touche) {
		case 1073741913:{
			afficherDepPoint();
			depPoint();
			touche='-';
			menu=0;
		}break; //1
		case 1073741914:{
			modele->delPoint(point);
			touche='-';
			menu=0;
		}break; //2
		case 1073741922:{
			touche='-';
			menu=0;
		}break; //0
		default:break;
	}
}

void Editeur3D::afficherDepPoint()const
{
	for (unsigned int i=0;i<30;i++)
			cout<<endl;
	afficherPoint();
	cout<<"Deplacement d'un point"<<endl;
}

void Editeur3D::depPoint()
{
	Matrice pos(3,1);
	float p;
	cout<<"Deplacement x :"<<endl;
	cin>>p;
	pos.set(0,0,modele->getPoint(point).posRe.get(0,0)+p);
	cout<<"Deplacement y :"<<endl;
	cin>>p;
	pos.set(1,0,modele->getPoint(point).posRe.get(1,0)+p);
	cout<<"Deplacement z :"<<endl;
	cin>>p;
	pos.set(2,0,modele->getPoint(point).posRe.get(2,0)+p);
	modele->setPoint(point,pos);
}

void Editeur3D::afficherAjoutSurface()const
{
	cout<<"Ajout surface"<<endl;
	if (nbp<3) {
		cout<<"Nb points : "<<nbp<<" | Points : ";
		for (unsigned int i=0;i<nbp;i++)
			cout<<pointsSurf[i]<<" ";
		cout<<endl;
	}
	else {
		cout<<"Nb points : "<<modele->getNbPointsSurf(modele->getNbSurfaces()-1)<<" | Points : ";
		for (unsigned int i=0;i<modele->getNbPointsSurf(modele->getNbSurfaces()-1);i++)
			cout<<modele->getIndicePointSurf(modele->getNbSurfaces()-1,i)<<" ";
		cout<<endl;
	}
	cout<<"Selectionnez des points a ajouter a la surface"<<endl;
	afficherPoint();
	cout<<"0 : Retour";
	cout<<endl;
}
	
void Editeur3D::ajoutSurface()
{
	if (touche==1073741922) { //0
		touche='-';
		menu=0;
	}
	else
		if (presse)
			if (point!=-1) {
				if (nbp==0) {
					pointsSurf[0]=point;
					nbp=1;
				}
				else
					if (nbp==1) {
						if (point!=int(pointsSurf[0])) {
							pointsSurf[1]=point;
							nbp=2;
						}
					}
					else
						if (nbp==2) {
							if (modele->addSurf(pointsSurf[0],pointsSurf[1],point))
								nbp=3;
						}
						else
							modele->addPointSurf(modele->getNbSurfaces()-1,modele->getNbPointsSurf(modele->getNbSurfaces()-1),point);
			}
}

void Editeur3D::afficherSelectionSurface()const
{
	cout<<"Selectionnez une surface"<<endl;
	afficherSurface();
	cout<<endl;
}
	
void Editeur3D::selectionSurface()
{
	if (presse) {
		if (surface!=-1)
			menu=8;
		else
			menu=0;
	}
}

void Editeur3D::afficherMenuSurface()const
{
	afficherSurface();
	cout<<"Action a faire :"<<endl;
	cout<<"1 : Deplacer la surface"<<endl;
	cout<<"2 : Supprimer la surface"<<endl;
	cout<<"3 : Ajouter des points a la surface"<<endl;
	cout<<"4 : Supprimer des points de la surface"<<endl;
	cout<<"5 : Modifier la texture de la surface"<<endl;
	cout<<"0 : Retour"<<endl;
	cout<<endl;
}
	
void Editeur3D::menuSurface()
{
	switch (touche) {
		case 1073741913:{
			afficherDepSurface();
			depSurface();
			touche='-';
			menu=0;
		}break; //1
		case 1073741914:{
			modele->delSurf(surface);
			touche='-';
			menu=0;
		}break; //2
		case 1073741915:{
			touche='-';
			menu=9;
		}break; //3
		case 1073741916:{
			touche='-';
			menu=10;
		}break; //4
		case 1073741917:{
			touche='-';
			menu=11;
		}break; //5
		case 1073741922:{
			touche='-';
			menu=0;
		}break; //0
		default:break;
	}
}

void Editeur3D::afficherDepSurface()const
{
	for (unsigned int i=0;i<30;i++)
			cout<<endl;
	afficherSurface();
	cout<<"Deplacement d'une surface"<<endl;
}

void Editeur3D::depSurface()
{
	Matrice pos(3,1);
	float p;
	cout<<"Deplacement x :"<<endl;
	cin>>p;
	pos.set(0,0,p);
	cout<<"Deplacement y :"<<endl;
	cin>>p;
	pos.set(1,0,p);
	cout<<"Deplacement z :"<<endl;
	cin>>p;
	pos.set(2,0,p);
	modele->depSurf(surface,pos);
}

void Editeur3D::afficherAjoutPointSurface()const
{
	afficherSurface();
	cout<<"Selecionnez un point a ajouter a la surface"<<endl;
	afficherPoint();
	cout<<"0 : Retour"<<endl;
	cout<<endl;
}
	
void Editeur3D::ajoutPointSurface()
{
	if (touche==1073741922) { //0
		touche='-';
		menu=8;
	}
	else
		if (presse)
			if (point!=-1)
				modele->addPointSurf(surface,modele->getNbPointsSurf(surface),point);
}

void Editeur3D::afficherSupPointSurface()const
{
	afficherSurface();
	cout<<"Selecionnez un point a supprimer de la surface"<<endl;
	afficherPoint();
	cout<<"0 : Retour"<<endl;
	cout<<endl;
}
	
void Editeur3D::supPointSurface()
{
	if (touche==1073741922) { //0
		touche='-';
		menu=8;
	}
	else
		if (presse && point!=-1) {
			if (modele->getNbPointsSurf(surface)==3) {
				if (int(modele->getIndicePointSurf(surface,0))==point||int(modele->getIndicePointSurf(surface,1))==point||int(modele->getIndicePointSurf(surface,2))==point) {
					modele->delSurf(surface);
					menu=0;
				}
			}
			else
				modele->delPointSurf(surface,point);
		}
}

void Editeur3D::afficherMenuTextureSurface()const
{
	afficherSurface();
	afficherImage();
	cout<<"Action a faire :"<<endl;
	cout<<"1 : Modifier l'image"<<endl;
	cout<<"2 : Deplacer l'image"<<endl;
	cout<<"3 : Tourner l'image"<<endl;
	cout<<"4 : Redimensionner l'image"<<endl;
	cout<<"0 : Retour"<<endl;
}

void Editeur3D::menuTextureSurface()
{
	switch (touche) {
		case 1073741913:{
			afficherModifImageSurface();
			modifImageSurface();
			touche='-';
		}break; //1
		case 1073741914:{
			afficherDepImageSurface();
			depImageSurface();
			touche='-';
		}break; //2
		case 1073741915:{
			afficherTourneImageSurface();
			tourneImageSurface();
			touche='-';
		}break; //3
		case 1073741916:{
			afficherTailleImageSurface();
			tailleImageSurface();
			touche='-';
		}break; //4
		case 1073741922:{
			touche='-';
			menu=8;
		}break; //0
		default:break;
	}
}

void Editeur3D::afficherModifImageSurface()const
{
	for (unsigned int i=0;i<30;i++)
		cout<<endl;
	afficherSurface();
	afficherImage();
	cout<<"Modification de l'image"<<endl;
}

void Editeur3D::modifImageSurface()
{
	string img;
	cout<<"Nom de l'image :"<<endl;
	cin>>img;
	modele->setImageSurf(surface,img);
}

void Editeur3D::afficherDepImageSurface()const
{
	for (unsigned int i=0;i<30;i++)
		cout<<endl;
	afficherSurface();
	afficherImage();
	cout<<"Deplacement de l'image"<<endl;
}

void Editeur3D::depImageSurface()
{
	float x,y;
	cout<<"Position x de l'image :"<<endl;
	cin>>x;
	cout<<"Position y de l'image :"<<endl;
	cin>>y;
	modele->setPositionSurf(surface,x,y);
}

void Editeur3D::afficherTourneImageSurface()const
{
	for (unsigned int i=0;i<30;i++)
		cout<<endl;
	afficherSurface();
	afficherImage();
	cout<<"Rotation de l'image"<<endl;
}

void Editeur3D::tourneImageSurface()
{
	float ang;
	cout<<"Angle de l'image :"<<endl;
	cin>>ang;
	modele->setAngleSurf(surface,ang);
}

void Editeur3D::afficherTailleImageSurface()const
{
	for (unsigned int i=0;i<30;i++)
		cout<<endl;
	afficherSurface();
	afficherImage();
	cout<<"Redimmensionnement de l'image"<<endl;
}

void Editeur3D::tailleImageSurface()
{
	float tx,ty;
	cout<<"Taille x de l'image :"<<endl;
	cin>>tx;
	cout<<"Taille y de l'image :"<<endl;
	cin>>ty;
	modele->setTailleSurf(surface,tx,ty);
}

void Editeur3D::enregistrerModele()
{
	string mod;
	cout<<"Saisissez le nom du modele a enregistrer :"<<endl;
	cin>>mod;
	modele->enregistrerModele(mod);
	menu=0;
}

void Editeur3D::enregistrerTexture()
{
	string tex;
	cout<<"Saisissez le nom de la texture a enregistrer :"<<endl;
	cin>>tex;
	modele->enregistrerTexture(tex);
	menu=0;
}

int Editeur3D::getPoint(int x,int y)
{
	Point2D * points=moteur.getPoints2D(modele);
	float distanceMin=-1, distance;
	int p=-1;
	float taille=10;
	float taillex=taille*image->getTaillex()/(3040);
	float tailley=taille*image->getTailley()/1200;
	axe->setOpacite(0);
	for (int j=std::max(0,int(y-tailley));j<std::min(int(image->getTailley()),int(y+tailley));j++)
		for (int i=std::max(0,int(x-taillex));i<std::min(int(image->getTaillex()),int(x+taillex));i++) {
			for (unsigned int k=0;k<modele->getNbPoints();k++) {
				if (i==points[k].pos2D.get(0,0) && j==points[k].pos2D.get(1,0)) {
					distance=prodScal(points[k].posCam, points[k].posCam);
					if (distanceMin==-1 ||distanceMin>distance) {
						distanceMin=distance;
						p=k;
						axe->setOpacite(1);
						axe->setPos(modele->getPoint(p).pos);
					}
				}
			}
	}
	if (points!=NULL)
		delete [] points;
	return p;
}

int Editeur3D::getSurface(int x,int y)
{
	Point2D * points=moteur.getPoints2D(modele);
	int s=-1;
	axe->setOpacite(0);
	Matrice m0(3,1),m1(3,1),m2(3,1),pos(3,1);
	float a0d,a1d,a2d,distMin=-1,dist,l1,l2,d;
	float it,jt;
	it=(x-image->getTaillex()/2.0)*cam.getTaillex()/image->getTaillex();
	jt=(image->getTailley()/2.0-y)*cam.getTailley()/image->getTailley();
	for (unsigned int i=0;i<modele->getNbSurfaces();i++) {
		if (SurfaceIn(i,points)) {
			m0=points[modele->getIndicePointSurf(i,0)].posCam;
			m1=points[modele->getIndicePointSurf(i,1)].posCam-m0;
			m2=points[modele->getIndicePointSurf(i,2)].posCam-m0;
			a0d=1.0*m0.get(0,0)/(cam.getDistFoc()*cam.getTaillex());
			a1d=1.0*m1.get(0,0)/(cam.getDistFoc()*cam.getTaillex());
			a2d=1.0*m2.get(0,0)/(cam.getDistFoc()*cam.getTaillex());
			d=(a2d*m1.get(2,0)-a1d*m2.get(2,0))*it+(a1d*m2.get(1,0)-a2d*m1.get(1,0))*jt+m1.get(1,0)*m2.get(2,0)-m2.get(1,0)*m1.get(2,0);
			l1=((a0d*m2.get(2,0)-a2d*m0.get(2,0))*it+(a2d*m0.get(1,0)-a0d*m2.get(1,0))*jt+m2.get(1,0)*m0.get(2,0)-m0.get(1,0)*m2.get(2,0))/d;
			l2=((a1d*m0.get(2,0)-a0d*m1.get(2,0))*it+(a0d*m1.get(1,0)-a1d*m0.get(1,0))*jt+m0.get(1,0)*m1.get(2,0)-m1.get(1,0)*m0.get(2,0))/d;
			pos=l1*m1+l2*m2+m0;
			dist=prodScal(pos,pos);
			if ((distMin==-1) ||(dist<distMin)) {
				distMin=dist;
				s=i;
				axe->setOpacite(1);
				axe->setPosRe(cam,pos);
			}
		}
	}
	if (points!=NULL)
		delete [] points;
	return s;
}

bool Editeur3D::SurfaceIn(unsigned int s, Point2D * points)
{
	assert(s<modele->getNbSurfaces());
	Matrice pos(2,1);
	pos.set(0,0,xs);
	pos.set(1,0,ys);
	Matrice pos1(2,1),pos2(2,1);
	Triangle * tri;
	unsigned int k;
	for (unsigned int i=0;i<modele->getNbTrianglesSurf(s);i++) {
		tri=modele->getTriangleSurf(s,i);
		if (points[tri->point1].devant && points[tri->point2].devant && points[tri->point3].devant) {
			k=0;
			pos1=points[tri->point2].pos2D-points[tri->point1].pos2D;
			pos2=pos-points[tri->point1].pos2D;
			if (pos1.get(0,0)*pos2.get(1,0)-pos1.get(1,0)*pos2.get(0,0)>0)
				k++;
			pos1=points[tri->point3].pos2D-points[tri->point2].pos2D;
			pos2=pos-points[tri->point2].pos2D;
			if (pos1.get(0,0)*pos2.get(1,0)-pos1.get(1,0)*pos2.get(0,0)>0)
				k++;
			pos1=points[tri->point1].pos2D-points[tri->point3].pos2D;
			pos2=pos-points[tri->point3].pos2D;
			if (pos1.get(0,0)*pos2.get(1,0)-pos1.get(1,0)*pos2.get(0,0)>0)
				k++;
			if (k==3)
				return 1;
		}
	}
	return 0;
}
