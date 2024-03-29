#include "../include/Moteur3D.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>

Moteur3D::Moteur3D()
{
	nbModeles=0;
	modeles=NULL;
	aff.clear();
	points=NULL;
	couleurFond.set(255,255,255);
	distances=NULL;
	transparences=NULL;
	distAffichage=1;
	musique=NULL;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
}

Moteur3D::Moteur3D(const std::string & moteur)
{
	distAffichage=1;
	aff.clear();
	distances=NULL;
	transparences=NULL;
	musique=NULL;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
	charger(moteur);
}

Moteur3D::~Moteur3D()
{
	if (modeles!=NULL) {
		for (unsigned int i=0;i<nbModeles;i++)
			delete modeles[i];
		delete [] modeles;
	}
	aff.clear();
	if (distances!=NULL) {
		for (unsigned int j=0;j<image.getTailley();j++)
			delete [] distances[j];
		delete [] distances;
	}
	if (points!=NULL)
		delete [] points;
	if (musique!=NULL)
		Mix_FreeMusic(musique);
	Mix_CloseAudio();
	if (transparences!=NULL) {
		for (unsigned int j=0;j<image.getTailley();j++) {
			for (unsigned int i=0;i<image.getTaillex();i++)
				if (transparences[j][i].size()>0) {
					transparences[j][i].clear();
				}
			delete [] transparences[j];
		}
		delete [] transparences;
	}
}

bool Moteur3D::charger(const std::string & moteur)&
{
	std::ifstream mot ("./data/moteur3D/"+ moteur +".txt");
	if (mot.is_open()) {
		unsigned int r,g,b;
		assert(mot>>r>>g>>b);
		couleurFond.set(r,g,b);
		float x,y,z,opac;
		Matrice pos(3,1);
		std::string modele, texture;
		while (mot>>modele) {
			mot>>texture>>x>>y>>z;
			pos.set(0,0,x);
			pos.set(1,0,y);
			pos.set(2,0,z);
			addModele(modele,texture,pos);
			assert(mot>>x>>z>>y);
			modeles[nbModeles-1]->setAng(x,z,y);
			assert(mot>>x);
			modeles[nbModeles-1]->setTaille(x);
			assert(mot>>x);
			modeles[nbModeles-1]->setDistAffichage(x);
			assert(mot>>opac);
			modeles[nbModeles-1]->setOpacite(opac);
		}
		mot.close();
		return 1;
	}
	else {
		std::cout<<"Chargement du moteur 3D impossible : Fichier "<<moteur<<" introuvable"<<std::endl;
		return 0;	
	}
}

bool Moteur3D::enregistrer(const std::string & moteur)const &
{
	std::ofstream mot ("./data/moteur3D/"+ moteur +".txt");
	if (mot.is_open()) {
		mot<<int(couleurFond.getR())<<" "<<int(couleurFond.getG())<<" "<<int(couleurFond.getB())<<" "<<std::endl;
		for (unsigned int i=0;i<nbModeles;i++) {
			mot<<modeles[i]->getModele()<<" "<<modeles[i]->getTexture()<<"  ";
			mot<<modeles[i]->getX()<<" "<<modeles[i]->getY()<<" "<<modeles[i]->getZ()<<"  ";
			mot<<modeles[i]->getAngxy()<<" "<<modeles[i]->getAngxz()<<" "<<modeles[i]->getAngyz()<<"  ";
			mot<<modeles[i]->getTaille()<<" "<<modeles[i]->getDistAffichage()<<" "<<modeles[i]->getOpacite()<<std::endl;
		}
		mot.close();
		return 1;
	}
	else {
		std::cout<<"Moteur enregistre dans "<<moteur<<".txt"<<std::endl;
		return 0;
	}
}

const Couleur& Moteur3D::getCouleurFond()const &
{
	return couleurFond;
}

void Moteur3D::setCouleurFond(unsigned char r, unsigned char g, unsigned char b)
{
	couleurFond.set(r,g,b);
}

float Moteur3D::getDistAffichage()const &
{
	return distAffichage;
}

void Moteur3D::setDistAffichage(float dist)
{
	distAffichage=dist;
}

unsigned int Moteur3D::getNbModeles()const &
{
	return nbModeles;
}

void Moteur3D::addModele(Modele * modele)&
{
	Modele ** nouv=new Modele * [nbModeles+1];
	for (unsigned int i=0;i<nbModeles;i++)
		nouv[i]=modeles[i];
	nouv[nbModeles]=modele;
	if (modeles!=NULL)
		delete [] modeles;
	modeles=nouv;
	nbModeles++;
	modeles[nbModeles-1]->optimiser();
}

void Moteur3D::addModele(const std::string & modele, const std::string & texture)&
{
	Matrice pos(3,1);
	addModele(modele,texture,pos);
}

void Moteur3D::addModele(const std::string & modele, const std::string & texture, const Matrice & pos)&
{
	assert(pos.getDimy()==3 && pos.getDimx()==1);
	Modele ** nouv=new Modele * [nbModeles+1];
	for (unsigned int i=0;i<nbModeles;i++)
		nouv[i]=modeles[i];
	nouv[nbModeles]=new Modele(modele,texture,pos);
	if (modeles!=NULL)
		delete [] modeles;
	modeles=nouv;
	nbModeles++;
	modeles[nbModeles-1]->optimiser();
}

void Moteur3D::delModele(unsigned int m)&
{
	assert(m<nbModeles);
	if (nbModeles==1) {
		delete modeles[0];
		delete [] modeles;
		modeles=NULL;
	}
	else {
		Modele ** nouv=new Modele * [nbModeles-1];
		for (unsigned int i=0;i<m;i++)
			nouv[i]=modeles[i];
		delete modeles[m];
		for (unsigned int i=m;i<nbModeles-1;i++)
			nouv[i]=modeles[i+1];
		delete [] modeles;
		modeles=nouv;
	}
	nbModeles--;
}

Modele* Moteur3D::getModele(unsigned int m)&
{
	assert(m<nbModeles);
	return modeles[m];
}

Image* Moteur3D::getImage(Camera & camera,unsigned int tailley, unsigned int taillex)&
{
	cam=&camera;
	creationTableau(tailley,taillex);
	gestionModeles();
	afficherSurfaces();
	appliquerTransparence();
	return &image;
}

Image* Moteur3D::getImage()&
{
	return &image;
}

float**& Moteur3D::getDistances()&
{
	return distances;
}

Point2D* Moteur3D::getPoints2D(const Modele * modele)const
{
	Point2D * p=new Point2D [modele->getNbPoints()];
	float prod;
	for (unsigned int i=0;i<modele->getNbPoints();i++) {
		p[i].posCam=getPosCam(modele->getPoint(i).pos);
		p[i].devant=(p[i].posCam.get(0,0)>0);
		if (p[i].devant) {
			prod=cam->getDistFoc()*cam->getTaillex()/p[i].posCam.get(0,0);
			p[i].pos2D.set(0,0,int(0.5+image.getTaillex()*(cam->getTaillex()/2.0+p[i].posCam.get(1,0)*prod)/cam->getTaillex()));
			p[i].pos2D.set(1,0,int(0.5+image.getTailley()*(cam->getTailley()/2.0-p[i].posCam.get(2,0)*prod)/cam->getTailley()));
		}
	}
	return p;
}

Point2D* Moteur3D::getPoints2D(const Modele * modele, unsigned int s)const
{
	assert(s<modele->getNbSurfaces());
	Point2D * p=new Point2D [modele->getNbPoints()];
	float prod;
	unsigned int ps;
	for (unsigned int i=0;i<modele->getNbPointsSurf(s);i++) {
		ps=modele->getIndicePointSurf(s,i);
		p[ps].posCam=getPosCam(modele->getPointSurf(s,i).pos);
		p[ps].devant=(p[ps].posCam.get(0,0)>0);
		if (p[ps].devant) {
			prod=cam->getDistFoc()*cam->getTaillex()/p[ps].posCam.get(0,0);
			p[ps].pos2D.set(0,0,int(0.5+image.getTaillex()*(cam->getTaillex()/2.0+p[ps].posCam.get(1,0)*prod)/cam->getTaillex()));
			p[ps].pos2D.set(1,0,int(0.5+image.getTailley()*(cam->getTailley()/2.0-p[ps].posCam.get(2,0)*prod)/cam->getTailley()));
		}
	}
	return p;
}

Matrice Moteur3D::getPosCam(const Matrice & pos)const
{
	assert(pos.getDimx()==1 && pos.getDimy()==3);
	return cam->getBase()*(pos-cam->getPos());
}

Camera* Moteur3D::getCam()const
{
	return cam;
}

unsigned int Moteur3D::getNbAff()const
{
	return aff.size();
}

void Moteur3D::pauseSons()
{
	Mix_Pause(-1);
}
	
void Moteur3D::reprendreSons()
{
	Mix_Resume(-1);
}

void Moteur3D::arreterSons()
{
	Mix_HaltChannel(-1);
}

bool Moteur3D::chargerMusique(const std::string & nom)
{
	if (musique!=NULL)
		Mix_FreeMusic(musique);
	std::string chemin="./data/musiques/"+ nom;
	if (nom[nom.size()-4]!='.')
		chemin=chemin+".mp3";
	char nom2[100];
	strcpy(nom2, chemin.c_str());
	musique=Mix_LoadMUS(nom2);
	if (musique==NULL) {
		std::cout<<"Chargement de la musique impossible : Fichier "<<chemin<<" introuvable"<<std::endl;
		return 0;
	}
	return 1;
}

bool Moteur3D::jouerMusique(int repet)
{
	assert(repet>=-1);
	if (musique==NULL)
		return 0;
	if (Mix_PlayingMusic())
		Mix_HaltMusic();
	Mix_PlayMusic(musique, repet);
	return 1;
}

void Moteur3D::pauseMusique()
{
	Mix_PauseMusic();
}

int Moteur3D::isPauseMusique()
{
	if (!Mix_PlayingMusic())
		return 2;
	return Mix_PausedMusic();
}

void Moteur3D::reprendreMusique()
{
	Mix_ResumeMusic();
}

void Moteur3D::arreterMusique()
{
	Mix_HaltMusic();
}

void Moteur3D::recommencerMusique()
{
	Mix_RewindMusic();
}

void Moteur3D::setVolume(float volume)
{
	if (volume<0)
		Mix_VolumeMusic(0);
	else
		if (volume>100)
			Mix_VolumeMusic(MIX_MAX_VOLUME);
		else
			Mix_VolumeMusic(MIX_MAX_VOLUME*volume/100.0);
}
	
float Moteur3D::getVolume()const
{
	return 100.0*Mix_VolumeMusic(-1)/MIX_MAX_VOLUME;
}

void Moteur3D::setPosLumiere(const Matrice & pos)
{
	assert(pos.getDimy()==3 && pos.getDimx()==1);
	posLumiere=pos;
}

void Moteur3D::setPosLumiere(float x, float y, float z)
{
	posLumiere.set(0,0,x);
	posLumiere.set(1,0,y);
	posLumiere.set(2,0,z);
}

const Matrice& Moteur3D::getPosLumiere()const &
{
	const Matrice & res=posLumiere;
	return res;
}

void Moteur3D::creationTableau(unsigned int y, unsigned int x)&
{
	assert(y!=0 && x!=0);
	if (y!=image.getTailley() || x!=image.getTaillex()) {
		if (distances!=NULL) {
			for (unsigned int j=0;j<image.getTailley();j++)
				delete [] distances[j];
			delete [] distances;
		}
		if (transparences!=NULL) {
			for (unsigned int j=0;j<image.getTailley();j++) {
				for (unsigned int i=0;i<image.getTaillex();i++)
					if (transparences[j][i].size()>0) {
						transparences[j][i].clear();
					}
				delete [] transparences[j];
			}
			delete [] transparences;
		}
		image.setImage(y,x);
		distances=new float * [y];
		for (unsigned int j=0;j<y;j++)
			distances[j]=new float [x];
		transparences=new std::vector<Couche> * [y];
		for (unsigned int j=0;j<y;j++)
			transparences[j]=new std::vector<Couche> [x];
	}
	initialiserTableau();
}
	
void Moteur3D::initialiserTableau()&
{
	for (unsigned int j=0;j<image.getTailley();j++)
		for (unsigned int i=0;i<image.getTaillex();i++) {
			distances[j][i]=-1;
			if (transparences[j][i].size()>0) {
				transparences[j][i].clear();
			}
		}
	image.effacer(couleurFond);
}

void Moteur3D::gestionModeles()&
{
	for (unsigned int i=0;i<aff.size();i++) {
		delete aff.at(i);
	}
	aff.clear();
	std::vector <SurfaceAff*> affTransp;
	for (unsigned int i=0;i<nbModeles;i++) {
		if (modeles[i]->getNbPoints()==0) {
			if (norme(cam->getPos()-modeles[i]->getPos())<200*modeles[i]->getDistMax()*modeles[i]->getTaille()*distAffichage*modeles[i]->getDistAffichage()) {
				modeles[i]->charger();
			}
		}
		else
			if (norme(cam->getPos()-modeles[i]->getPos())>100+200*modeles[i]->getDistMax()*modeles[i]->getTaille()*distAffichage*modeles[i]->getDistAffichage())
				modeles[i]->optimiser();
		if (modeles[i]->getNbPoints()>0) {
			if (modeles[i]->getOpacite()>0) {
				majPoints(modeles[i]);
				for (unsigned int j=0;j<modeles[i]->getNbSurfaces();j++)
					if (modeles[i]->getOpaciteSurf(j)>0)
						if (0>prodScal(cam->getBase()*modeles[i]->getNormaleSurf(j),points[modeles[i]->getIndicePointSurf(j,0)].posCam)) {
							SurfaceAff * nouvSurf=new SurfaceAff;
							nouvSurf->modele=modeles[i];
							nouvSurf->surface=j;
							nouvSurf->distance=norme(cam->getPos()-modeles[i]->getCentreSurf(j));
							if (modeles[i]->getOpacite()<1 || modeles[i]->getOpaciteSurf(j)<1)
								affTransp.push_back(nouvSurf);
							else
								aff.push_back(nouvSurf);
						}
			}
			modeles[i]->majSon(getPosCam(modeles[i]->getPos()));
		}
	}
	if (aff.size()!=0)
		aff=affTri(aff,0,aff.size()-1);
	if (affTransp.size()!=0)
		affTransp=affTri(affTransp,0,affTransp.size()-1);
	for (unsigned int i=0;i<affTransp.size();i++) {
		aff.push_back(affTransp.at(i));
	}
}

std::vector <SurfaceAff*> Moteur3D::affTri(std::vector <SurfaceAff*> tab, unsigned int deb, unsigned int fin)&
{
	assert(deb<=fin && fin<tab.size());
	std::vector <SurfaceAff*> res;
	if (deb==fin) {
		res.push_back(tab.at(deb));
		return res;
	} 
	unsigned int milieu=(fin+deb)/2;
	std::vector <SurfaceAff*> tab1=affTri(tab,deb,milieu);
	std::vector <SurfaceAff*> tab2=affTri(tab,milieu+1,fin);
	unsigned int indice1=0;
	unsigned int indice2=0;
	unsigned int taille1=tab1.size();
	unsigned int taille2=tab2.size();
	while (indice1<taille1 || indice2<taille2) {
		if (indice1==taille1) {
			res.push_back(tab2.at(indice2));
			indice2++;
		}
		else {
			if (indice2==taille2) {
				res.push_back(tab1.at(indice1));
				indice1++;
			}
			else {
				if (tab1.at(indice1)->distance<tab2.at(indice2)->distance) {
					res.push_back(tab1.at(indice1));
					indice1++;
				}
				else {
					res.push_back(tab2.at(indice2));
					indice2++;
				}
			}
		}
	}
	return res;
}

void Moteur3D::afficherSurfaces()&
{
	for (unsigned int i=0;i<aff.size();i++) {
		majPoints(aff.at(i)->modele,aff.at(i)->surface);
		afficherSurface(aff.at(i)->modele,aff.at(i)->surface);
	}
	if (points!=NULL) {
		delete [] points;
		points=NULL;
	}
}

void Moteur3D::majPoints(const Modele * modele)&
{
	if (points!=NULL)
		delete [] points;
	points=getPoints2D(modele);
}

void Moteur3D::majPoints(const Modele * modele,unsigned int s)&
{
	assert(s<modele->getNbSurfaces());
	if (points!=NULL)
		delete [] points;
	points=getPoints2D(modele,s);
}

void Moteur3D::afficherSurface(Modele * modele, unsigned int s)&
{
	float ang=0.1;
	Matrice surf (3,1);
	surf=modele->getNormaleSurf(s);
	Matrice lumiere (3,1);
	lumiere=modele->getCentreSurf(s)-posLumiere;
	float norm=norme(lumiere);
	if (norm>0) {
		lumiere=lumiere/norm;
		ang=0.4+0.6*(1-prodScal(lumiere,surf))/2.0;

	}
	unsigned int i1,i2,i3;
	for (unsigned int i=0;i<modele->getNbTrianglesSurf(s);i++) {
		i1=modele->getTriangleSurf(s,i)->point1;
		i2=modele->getTriangleSurf(s,i)->point2;
		i3=modele->getTriangleSurf(s,i)->point3;
		traitementTriangle(modele,s,&points[i1],&points[i2],&points[i3],ang);
	}
}

void Moteur3D::traitementTriangle(const Modele * modele, unsigned int s, Point2D * p1, Point2D * p2, Point2D * p3, float ang)&
{
	if (p1->devant || p2->devant || p3->devant) {
		Point2D * point1;
		Point2D * point2;
		Point2D * point3;
		if (!p1->devant && !p2->devant) {
				point1=p3;
				point2=p1;
				point3=p2;
			}
			else
				if (!p1->devant && !p3->devant) {
					point1=p2;
					point2=p1;
					point3=p3;
				}
				else
					if (!p3->devant) {
						point1=p1;
						point2=p2;
						point3=p3;
					}
					else
						if (!p2->devant) {
							point1=p1;
							point2=p3;
							point3=p2;
						}
						else {
							point1=p2;
							point2=p3;
							point3=p1;
						}
		float distMin;
		if (point3->devant) {
			distMin=std::min(point1->posCam.get(0,0),std::min(point2->posCam.get(0,0),point3->posCam.get(0,0)));
			afficherTriangle(modele,s,point1,point2,point3,ang,distMin);
		}
		else {
			float prod;
			float dist=1;
			Point2D p4,p5;
			unsigned int imgTaillex=image.getTaillex();
			unsigned int imgTailley=image.getTailley();
			unsigned int camTaillex=cam->getTaillex();
			unsigned int camTailley=cam->getTailley();
			float distFoc=cam->getDistFoc();
			if (point2->devant) {
				p4.devant=1;
				p4.posCam=point3->posCam+(point3->posCam.get(0,0)-dist)*(point1->posCam-point3->posCam)/(point3->posCam.get(0,0)-point1->posCam.get(0,0));
				prod=distFoc*cam->getTaillex()/p4.posCam.get(0,0);
				p4.pos2D.set(0,0,int(0.5+imgTaillex*(camTaillex/2.0+p4.posCam.get(1,0)*prod)/camTaillex));
				p4.pos2D.set(1,0,int(0.5+imgTailley*(camTailley/2.0-p4.posCam.get(2,0)*prod)/camTailley));
				p5.devant=1;
				p5.posCam=point3->posCam+(point3->posCam.get(0,0)-dist)*(point2->posCam-point3->posCam)/(point3->posCam.get(0,0)-point2->posCam.get(0,0));
				prod=distFoc*cam->getTaillex()/p5.posCam.get(0,0);
				p5.pos2D.set(0,0,int(0.5+imgTaillex*(camTaillex/2.0+p5.posCam.get(1,0)*prod)/camTaillex));
				p5.pos2D.set(1,0,int(0.5+imgTailley*(camTailley/2.0-p5.posCam.get(2,0)*prod)/camTailley));
				distMin=std::min(point1->posCam.get(0,0),std::min(point2->posCam.get(0,0),p5.posCam.get(0,0)));
				afficherTriangle(modele,s,point1,point2,&p5,ang,distMin);
			}
			else {
				p4.devant=1;
				p4.posCam=point2->posCam+(point2->posCam.get(0,0)-dist)*(point1->posCam-point2->posCam)/(point2->posCam.get(0,0)-point1->posCam.get(0,0));
				prod=distFoc*camTaillex/p4.posCam.get(0,0);
				p4.pos2D.set(0,0,int(0.5+imgTaillex*(camTaillex/2.0+p4.posCam.get(1,0)*prod)/camTaillex));
				p4.pos2D.set(1,0,int(0.5+imgTailley*(camTailley/2.0-p4.posCam.get(2,0)*prod)/camTailley));
				p5.devant=1;
				p5.posCam=point3->posCam+(point3->posCam.get(0,0)-dist)*(point1->posCam-point3->posCam)/(point3->posCam.get(0,0)-point1->posCam.get(0,0));
				prod=distFoc*camTaillex/p5.posCam.get(0,0);
				p5.pos2D.set(0,0,int(0.5+imgTaillex*(camTaillex/2.0+p5.posCam.get(1,0)*prod)/camTaillex));
				p5.pos2D.set(1,0,int(0.5+imgTailley*(camTailley/2.0-p5.posCam.get(2,0)*prod)/camTailley));
			}
			distMin=std::min(point1->posCam.get(0,0),std::min(p4.posCam.get(0,0),p5.posCam.get(0,0)));
			afficherTriangle(modele,s,point1,&p4,&p5,ang,distMin);
		}
	}
}

void Moteur3D::interpolationLigne(Point2D * p1, Point2D * p2, unsigned int dep, unsigned int fin, std::vector <int> tab [])&
{
	int p1y=p1->pos2D.get(1,0);
	int p2y=p2->pos2D.get(1,0);
	int p1x=p1->pos2D.get(0,0);
	int p2x=p2->pos2D.get(0,0);
	int dif=p2y-p1y;
	int indice, x;
	if (dif>0) {
		int debut=std::max((int)dep,p1y);
		int final=std::min((int)fin,p2y);
		float pentX=(p2x-p1x)/(1.0*dif);
		float constanteX=p1x-pentX*p1y;
		for (int i=debut;i<=final;i++) {
			indice=i-dep;
			if (tab[indice].size()<2) {
				x=pentX*i+constanteX;
				tab[indice].push_back(x);
			}
		}
	}
}

void Moteur3D::interpolationTriangle(Point2D * p1, Point2D * p2, Point2D * p3, unsigned int dep, unsigned int fin, std::vector <int> tab [])&
{
	interpolationLigne(p1,p2,dep,fin,tab);
	interpolationLigne(p1,p3,dep,fin,tab);
	interpolationLigne(p2,p3,dep,fin,tab);
}

void Moteur3D::afficherTriangle(const Modele * modele, unsigned int s, Point2D * p1, Point2D * p2, Point2D * p3, float ang, float distMin)&
{
	Point2D *p;
	if (!(p1->pos2D.get(1,0)<p2->pos2D.get(1,0))) {
		p=p1;
		p1=p2;
		p2=p;
	}
	if (!(p2->pos2D.get(1,0)<p3->pos2D.get(1,0))) {
		p=p3;
		p3=p2;
		if (p1->pos2D.get(1,0)<p->pos2D.get(1,0))
			p2=p;
		else {
			p2=p1;
			p1=p;
		}
	}
	int dep=std::max(0,(int)p1->pos2D.get(1,0));
	int fin=std::min((int)image.getTailley(),(int)p3->pos2D.get(1,0));
	int longueur=fin-dep+1;
	if (dep<=fin) {
		std::vector <int> tab [longueur];
		interpolationTriangle(p1,p2,p3,dep,fin,tab);
		int imageTaillex=image.getTaillex(), imageTailley=image.getTailley();
		Matrice v0(3,1),v1(3,1),v2(3,1);
		const Image * img=modele->getImageSurf(s);
		float cosAng,sinAng,constx,consty;
		float x0,x1,x2,x3,y0,y1,y2,y3;
		Couleur couleur,couleurImg;
		float opacite=modele->getOpacite()*modele->getOpaciteSurf(s);
		unsigned char alpha;
		int imgTaillex, imgTailley;
		if (img!=NULL) {
			imgTaillex=img->getTaillex();
			imgTailley=img->getTailley();
			if (imgTaillex>1 || imgTailley>1) {
				v0=points[modele->getIndicePointSurf(s,0)].posCam;
				v1=(points[modele->getIndicePointSurf(s,1)].posCam-v0);
				v2=(points[modele->getIndicePointSurf(s,2)].posCam-v0);
				orthonormalisation(v1,v2);
				v1=v1/(modele->getTailleySurf(s)*modele->getTaille());
				v2=v2/(modele->getTaillexSurf(s)*modele->getTaille());
				cosAng=modele->cs.Cos(modele->getAngleSurf(s));
				sinAng=modele->cs.Sin(modele->getAngleSurf(s));
				constx=-v2.get(0,0)*v0.get(0,0)-v2.get(1,0)*v0.get(1,0)-v2.get(2,0)*v0.get(2,0)-modele->getPosxSurf(s)/(modele->getTaillexSurf(s)*modele->getTaille());
				consty=-v1.get(0,0)*v0.get(0,0)-v1.get(1,0)*v0.get(1,0)-v1.get(2,0)*v0.get(2,0)-modele->getPosySurf(s)/(modele->getTailleySurf(s)*modele->getTaille());
				x0=cosAng*constx+sinAng*consty;
				y0=imgTailley+sinAng*constx-cosAng*consty;
				x1=cosAng*v2.get(0,0)+sinAng*v1.get(0,0);
				y1=sinAng*v2.get(0,0)-cosAng*v1.get(0,0);
				x2=cosAng*v2.get(1,0)+sinAng*v1.get(1,0);
				y2=sinAng*v2.get(1,0)-cosAng*v1.get(1,0);
				x3=cosAng*v2.get(2,0)+sinAng*v1.get(2,0);
				y3=sinAng*v2.get(2,0)-cosAng*v1.get(2,0);
			}
			else
				if (imgTaillex==1 && imgTailley==1)
					couleur.set(ang*img->getCouleur(0,0).getR(),ang*img->getCouleur(0,0).getG(),ang*img->getCouleur(0,0).getB(),opacite*img->getCouleur(0,0).getA());
		}
		else
			couleur.set(100*ang,100*ang,100*ang,255*opacite);
		int i2,i3;
		float m0x,m0y,m0z,m1x,m1y,m1z,m2x,m2y,m2z;
		m0x=p1->posCam.get(0,0);
		m0y=p1->posCam.get(1,0);
		m0z=p1->posCam.get(2,0);
		m1x=p2->posCam.get(0,0)-m0x;
		m1y=p2->posCam.get(1,0)-m0y;
		m1z=p2->posCam.get(2,0)-m0z;
		m2x=p3->posCam.get(0,0)-m0x;
		m2y=p3->posCam.get(1,0)-m0y;
		m2z=p3->posCam.get(2,0)-m0z;
		float posx,posy,posz;
		float a0d,a1d,a2d,in1,jn1,n1,in2,jn2,n2,id,jd,d;
		float camTaillex=cam->getTaillex();
		float camTailley=cam->getTailley();
		float camDistFoc=cam->getDistFoc();
		a0d=1.0*m0x/(camDistFoc*camTaillex);
		a1d=1.0*m1x/(camDistFoc*camTaillex);
		a2d=1.0*m2x/(camDistFoc*camTaillex);
		in1=a0d*m2z-a2d*m0z;
		jn1=a2d*m0y-a0d*m2y;
		n1=m2y*m0z-m0y*m2z-in1/2.0*camTaillex+jn1/2.0*camTailley;
		in1=in1*camTaillex/imageTaillex;
		jn1=-jn1*camTailley/imageTailley;
		in2=a1d*m0z-a0d*m1z;
		jn2=a0d*m1y-a1d*m0y;
		n2=m0y*m1z-m1y*m0z-in2/2.0*camTaillex+jn2/2.0*camTailley;
		in2=in2*camTaillex/imageTaillex;
		jn2=-jn2*camTailley/imageTailley;
		id=a2d*m1z-a1d*m2z;
		jd=a1d*m2y-a2d*m1y;
		d=m1y*m2z-m2y*m1z-id/2.0*camTaillex+jd/2.0*camTailley;
		id=id*camTaillex/imageTaillex;
		jd=-jd*camTailley/imageTailley;
		float intn1,intn2,intd,denom,l1,l2;
		int x,y,indice,xtab1,xtab2;

		for (int j=dep;j<fin;j++) {
			indice=j-dep;
			if (tab[indice].size()==2) {
				intn1=jn1*j+n1;
				intn2=jn2*j+n2;
				intd=jd*j+d;
				xtab1=tab[indice].at(0);
				xtab2=tab[indice].at(1);
				i2=std::max(0,std::min(xtab1,xtab2));
				i3=std::min(imageTaillex,std::max(xtab1,xtab2));
				for (int i=i2;i<i3;i++) {
					if (distances[j][i]==-1 || distances[j][i]>distMin) {
						denom=id*i+intd;
						l1=(in1*i+intn1)/denom;
						l2=(in2*i+intn2)/denom;
						posx=l1*m1x+l2*m2x+m0x;
						if (distances[j][i]==-1 || distances[j][i]>posx) {
							if (img!=NULL) {
								if (imgTaillex>1 || imgTailley>1) {
									posy=l1*m1y+l2*m2y+m0y;
									posz=l1*m1z+l2*m2z+m0z;
									x=x1*posx+x2*posy+x3*posz+x0;
									if (x>=0)
										x=x%imgTaillex;
									else
										x=imgTaillex-(-x)%imgTaillex-1;
									y=y1*posx+y2*posy+y3*posz+y0;
									if (y>=0)
										y=y%imgTailley;
									else
										y=imgTailley-(-y)%imgTailley-1;
									couleurImg=img->getCouleur(y,x);
									couleur.set(ang*couleurImg.getR(),ang*couleurImg.getG(),ang*couleurImg.getB(),opacite*couleurImg.getA());
								}
							}
							alpha=couleur.getA();
							if (alpha==255) {
								distances[j][i]=posx;
								image.setCouleurRGB(j,i,couleur);
							}
							else
								if (alpha>0)
									addTransparent(i,j,couleur,posx);
						}
					}
				}
			}
		}
	}
}

void Moteur3D::addTransparent(unsigned int x, unsigned int y, const Couleur & coul, float dist)&
{
	assert(x<image.getTaillex());
	assert(y<image.getTailley());
	unsigned int k=transparences[y][x].size();
	while (k>0 && dist<=transparences[y][x].at(k-1).distance) {
		k--;
	}
	Couche couche;
	couche.couleur=coul;
	couche.distance=dist;
	transparences[y][x].insert(transparences[y][x].begin()+k,couche);
}

void Moteur3D::appliquerTransparence()&
{
	unsigned int tx=image.getTaillex();
	unsigned int ty=image.getTailley();
	float alpha,beta;
	Couleur couleur,couleurIm,couleurTrans;
	for (unsigned int j=0;j<ty;j++)
		for (unsigned int i=0;i<tx;i++)
			for (unsigned int k=0;k<transparences[j][i].size();k++)
				if (distances[j][i]==-1 || transparences[j][i].at(transparences[j][i].size()-k-1).distance<distances[j][i]) {
					couleurIm=image.getCouleur(j,i);
					couleurTrans=transparences[j][i].at(transparences[j][i].size()-k-1).couleur;
					alpha=couleurTrans.getA()/255.0;
					beta=1-alpha;
					couleur.setR(alpha*couleurTrans.getR()+beta*couleurIm.getR());
					couleur.setG(alpha*couleurTrans.getG()+beta*couleurIm.getG());
					couleur.setB(alpha*couleurTrans.getB()+beta*couleurIm.getB());
					image.setCouleurRGB(j,i,couleur);
				}
}
