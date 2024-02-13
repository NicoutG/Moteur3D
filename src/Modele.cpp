#include "../include/Modele.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstring>

/**
@brief structure Triangulation
**/

struct Triangulation
{
	unsigned int indice;
	bool concave;
};

Modele::Modele()
{
	modele="NULL";
	texture="NULL";
	pos.set(0,0,0);
	pos.set(1,0,0);
	pos.set(2,0,0);
	angle_xy=0;
	angle_xz=0;
	angle_yz=0;
	points=NULL;
	nbPoints=0;
	surfaces=NULL;
	nbSurfaces=0;
	images=NULL;
	nbImages=0;
	taille=1;
	opacite=1;
	distMax=0;
	distAffichage=1;
	son=NULL;
	intensite=50;
	canal=-1;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	if (Mix_AllocateChannels(-1)!=30)
		Mix_AllocateChannels(30);
	calculBase();
}

Modele::Modele(const std::string & model, const std::string & text)
{
	pos.set(0,0,0);
	pos.set(1,0,0);
	pos.set(2,0,0);
	angle_xy=0;
	angle_xz=0;
	angle_yz=0;
	points=NULL;
	nbPoints=0;
	surfaces=NULL;
	nbSurfaces=0;
	images=NULL;
	nbImages=0;
	taille=1;
	opacite=1;
	distAffichage=1;
	son=NULL;
	intensite=50;
	canal=-1;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	if (Mix_AllocateChannels(-1)!=30)
		Mix_AllocateChannels(30);
	calculBase();
	chargerModele(model);
	chargerTexture(text);
	optimiser();
}

Modele::Modele(const std::string & model, const std::string & text,const Matrice & p)
{
	assert(p.getDimy()==3 && p.getDimx()==1);
	pos=p;
	angle_xy=0;
	angle_xz=0;
	angle_yz=0;
	points=NULL;
	nbPoints=0;
	surfaces=NULL;
	nbSurfaces=0;
	images=NULL;
	nbImages=0;
	taille=1;
	opacite=1;
	distAffichage=1;
	son=NULL;
	intensite=50;
	canal=-1;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	if (Mix_AllocateChannels(-1)!=30)
		Mix_AllocateChannels(30);
	calculBase();
	chargerModele(model);
	chargerTexture(text);
	optimiser();
}

Modele::~Modele()
{
	if (son!=NULL) {
		Mix_FreeChunk(son);
		son=NULL;
	}
	Mix_CloseAudio();
	optimiser();
}

bool Modele::charger()&
{
	return (chargerModele(modele) && chargerTexture(texture));
}

bool Modele::chargerModele(const std::string & model)&
{
	optimiser();
	distMax=0;
	if (model.compare("NULL")) {
		std::ifstream mod ("./data/modèles/"+ model +".txt");
		if (mod.is_open()) {
			modele=model;
			std::ifstream tex;
			unsigned int nbP,nbS,nb;
			assert(mod>>nbP);
			float x,y,z;
			Matrice mat(3,1);
			for (unsigned int i=0;i<nbP;i++)
			{
				mod>>x>>y>>z;
				mat.set(0,0,x);
				mat.set(1,0,y);
				mat.set(2,0,z);
				addPoint(mat);
			}
			assert(mod>>nbS);
			unsigned int p1;
			unsigned int * tab=NULL;
			for (unsigned i=0;i<nbS;i++)
			{
				mod>>nb;
				tab=new unsigned int [nb];
				for (unsigned int j=0;j<nb;j++) {
					assert(mod>>p1);
					tab[j]=p1;
				}
				addSurf(tab,nb);
			}
			mod.close();
		}
		else {
			std::cout<<"Chargement du modele impossible : Fichier "<<model<<" introuvable"<<std::endl;
			modele="NULL";
			return 0;
		}
	}
	modele=model;
	return 1;
}

bool Modele::chargerTexture(const std::string & text)&
{
	if (text.compare("NULL")) {
		std::ifstream tex ("./data/textures/"+ text +".txt");
		if (tex.is_open()) {
			unsigned int n;
			float x,y;
			assert(tex>>n);
			std::string nom;
			if (nbSurfaces==n) {
				for (unsigned int i=0;i<nbSurfaces;i++) {
					tex>>nom;
					setImageSurf(i,nom);
					tex>>x>>y;
					surfaces[i]->setPosition(x,y);
					tex>>x>>y;
					surfaces[i]->setTaille(x,y);
					tex>>x;
					surfaces[i]->setAngle(x);
					tex>>x;
					surfaces[i]->setOpacite(x);
				}
				tex.close();
			}
			else {
				std::cout<<"Chargement de la texture impossible : Le fichier "<<text<<" contient "<<n<<" couleurs alors que le modele possede "<<nbSurfaces<<" surfaces"<<std::endl;
				tex.close();
				texture="NULL";
				return 0;
			}
			
		}
		else {
			std::cout<<"Chargement de la texture impossible : Fichier "<<text<<" introuvable"<<std::endl;
			texture="NULL";
			return 0;
		}
	}
	texture=text;
	return 1;
}

bool Modele::enregistrerModele(const std::string & model)const &
{
	std::ofstream mod ("./data/modèles/"+ model +".txt");
	if (mod.is_open()) {
		mod<<nbPoints<<std::endl;
		for (unsigned int i=0;i<nbPoints;i++)
			mod<<points[i]->posRe.get(0,0)<<" "<<points[i]->posRe.get(1,0)<<" "<<points[i]->posRe.get(2,0)<<std::endl;
		mod<<std::endl;
		mod<<nbSurfaces<<std::endl;
		for (unsigned int i=0;i<nbSurfaces;i++) {
			mod<<surfaces[i]->getNbPoints()<<"  ";
			for (unsigned int j=0;j<surfaces[i]->getNbPoints();j++)
				mod<<getIndicePointSurf(i,j)<<" ";
			mod<<std::endl;
		}
		mod.close();
		std::cout<<"Modele enregistre dans "<<model<<".txt"<<std::endl;
		return 1;
	}
	else
		std::cout<<"Enregistrement du modele impossible : Impossible de creer ou modifier le fichier "<<model<<std::endl;
	return 0;
}

bool Modele::enregistrerTexture(const std::string & text)const &
{
	std::ofstream tex ("./data/textures/"+ text +".txt");
	if (tex.is_open()) {
		tex<<nbSurfaces<<std::endl;
		for (unsigned int i=0;i<nbSurfaces;i++) {
			tex<<getNomImageSurf(i)<<"  ";
			tex<<surfaces[i]->getPosx()<<" "<<surfaces[i]->getPosy()<<"  ";
			tex<<surfaces[i]->getTaillex()<<" "<<surfaces[i]->getTailley()<<"  ";
			tex<<surfaces[i]->getAngle()<<"  "<<surfaces[i]->getOpacite()<<std::endl;
		}
		tex.close();
		std::cout<<"Texture enregistre dans "<<text<<".txt"<<std::endl;
		return 1;
	}
	else
		std::cout<<"Enregistrement de la texture impossible : Impossible de creer ou modifier le fichier "<<text<<std::endl;
	return 0;
}

void Modele::optimiser()&
{
	if (surfaces!=NULL) {
		delete [] surfaces;
		surfaces=NULL;
	}
	nbSurfaces=0;
	if (points!=NULL) {
		for (unsigned int i=0;i<nbPoints;i++)
			delete points[i];
		delete [] points;
		points=NULL;
	}
	nbPoints=0;
	if (images!=NULL) {
		for (unsigned int i=0;i<nbImages;i++)
			if (images[i].img!=NULL)
				delete images[i].img;
		delete [] images;
		images=NULL;
	}
	nbImages=0;
	if (canal!=-1) {
		Mix_HaltChannel(canal);
		canal=-1;
	}
	if (son!=NULL) {
		Mix_FreeChunk(son);
		son=NULL;
	}
}

bool Modele::verif()&
{
	afficher();
	std::cout<<std::endl;
	if (triangulations()) {
		std::cout<<"Modele OK"<<std::endl;
		return 1;
	}
	return 0;
}

void Modele::calculBase()&
{
	float cosxy=cs.Cos(angle_xy);
	float sinxy=cs.Sin(angle_xy);
	float cosxz=cs.Cos(angle_xz);
	float sinxz=cs.Sin(angle_xz);
	float cosyz=cs.Cos(angle_yz);
	float sinyz=cs.Sin(angle_yz);
	base.set(0,0,cosxy*cosxz);
	base.set(0,1,sinxy*cosxz);
	base.set(0,2,-sinxz);
	base.set(1,0,-cosxy*sinxz*sinyz-sinxy*cosyz);
	base.set(1,1,cosxy*cosyz-sinxy*sinxz*sinyz);
	base.set(1,2,-cosxz*sinyz);
	base.set(2,0,-sinxy*sinyz+cosxy*sinxz*cosyz);
	base.set(2,1,cosxy*sinyz+sinxy*sinxz*cosyz);
	base.set(2,2,cosxz*cosyz);
}

void Modele::calculDistMax()&
{
	distMax=0.00001;
	if (nbPoints>=2)
	{
		float dist;
		for (unsigned int p1=0;p1<nbPoints-1;p1++)
			for (unsigned int p2=p1+1;p2<nbPoints;p2++)
			{
				dist=norme(points[p1]->posRe-points[p2]->posRe);
				if (distMax<dist)
					distMax=dist;
			}
	}
}

void Modele::majPoint(unsigned int p)&
{
	assert(p<nbPoints);
	points[p]->pos=pos+taille*(base*points[p]->posRe);
}

void Modele::majPoints()&
{
	for (unsigned int i=0;i<nbPoints;i++)
		majPoint(i);
}

void Modele::calculCentre(unsigned int s)&
{
	assert(s<nbSurfaces);
	Matrice moy(3,1);
	unsigned int nbps=surfaces[s]->getNbPoints();
	assert(nbps>0);
	for (unsigned int i=0;i<nbps;i++)
		moy=moy+points[surfaces[s]->get(i)]->posRe;
	moy=moy/nbps;
	surfaces[s]->centre.posRe=moy;
	majCentre(s);
}

void Modele::calculCentres()&
{
	for (unsigned int i=0;i<nbSurfaces;i++)
		calculCentre(i);
}

void Modele::majCentre(unsigned int s)&
{
	assert(s<nbSurfaces);
	surfaces[s]->centre.pos=pos+taille*(base*surfaces[s]->centre.posRe);
}

void Modele::majCentres()&
{
	for (unsigned int i=0;i<nbSurfaces;i++)
		majCentre(i);
}

void Modele::majNormales()&
{
	for (unsigned int i=0;i<nbSurfaces;i++)
		majNormale(i);
}

void Modele::majNormale(unsigned int s)&
{
	assert(s<nbSurfaces);
	assert(surfaces[s]->getNbTriangles()>0);
	Triangle * tri=surfaces[s]->getTriangle(0);
	Matrice mat(3,1);
	mat=prodVect(points[tri->point2]->pos-points[tri->point1]->pos, points[tri->point3]->pos-points[tri->point2]->pos);
	float norm=norme(mat);
	assert(norm!=0);
	mat=mat/norm;
	surfaces[s]->setNormale(mat);
}

bool Modele::calculNormale(unsigned int s)&
{
	assert(s<nbSurfaces);
	unsigned int nbps=getNbPointsSurf(s);
	Matrice * arretes=new Matrice [nbps];
	float norm;
	for (unsigned int i=0;i<nbps;i++) {
		arretes[i].modifDims(3,1);
		arretes[i]=getPointSurf(s,(i+1)%nbps).pos-getPointSurf(s,i).pos;
		norm=norme(arretes[i]);
		if (norm==0) {
			delete [] arretes;
			std::cout<<"Le point "<<getIndicePointSurf(s,(i+1)%nbps)<<" et "<<getIndicePointSurf(s,i)<<" sont identiques"<<std::endl;
			return 0;
		}
		arretes[i]=arretes[i]/norm;
	}
	unsigned int k=0;
	bool b=1;
	Matrice normale(3,1);
	while (b && k<nbps) {
		normale=prodVect(arretes[(k+1)%nbps],arretes[k]);
		norm=norme(normale);
		if (norm>0.01) {
			normale=normale/norm;
			b=0;
		}
	}
	if (b) {
		delete [] arretes;
		std::cout<<"Tous les points de la surface "<<s<<" sont allignés"<<std::endl;
		return 0;
	}
	for (unsigned int i=0;i<nbps;i++) {
		if (abs(prodScal(normale,arretes[i]))>0.001) {
			delete [] arretes;
			std::cout<<"Les points de la surface "<<s<<" ne sont pas tous sur le meme plan"<<std::endl;
			return 0;
		}
	}
	float angles=0;
	for (unsigned int i=0;i<nbps;i++) {
		norm=acos(prodScal(arretes[(i+1)%nbps],arretes[i]));
		if (prodScal(arretes[(i+1)%nbps],prodVect(arretes[i],normale))<0)
			angles=angles-norm;
		else
			angles=angles+norm;
	}
	delete [] arretes;
	if (abs(angles)<6) {
		std::cout<<"Les points de la surface "<<s<<" ne forment pas un polygone correct"<<std::endl;
		return 0;
	}
	if (angles>0)
		normale=-1*normale;
	surfaces[s]->setNormale(normale);
	return 1;
}

bool Modele::triangulation(unsigned int s)&
{
	assert(s<nbSurfaces);
	if (!calculNormale(s))
		return 0;
	surfaces[s]->delTriangles();
	unsigned int nbps=getNbPointsSurf(s);
	unsigned int nbTri=nbps;
	Triangulation * sommets=new Triangulation [nbTri];
	for (unsigned int i=0;i<nbTri;i++) {
		sommets[i].indice=surfaces[s]->get(i);
		sommets[i].concave=prodScal(surfaces[s]->getNormale(),prodVect(getPointSurf(s,(i+1)%nbps).pos-getPointSurf(s,i).pos,getPointSurf(s,i).pos-getPointSurf(s,(i+nbps-1)%nbps).pos))>=0;
	}
	unsigned int p=0,k;
	bool in;
	while (p<nbTri && nbTri>3) {
		if (!sommets[p].concave) {
			in=1;
			k=0;
			while (in && k<nbTri-3) {
				if (sommets[(p+2+k)%nbTri].concave) {
					in=!pointInTriangle(points[sommets[(p+2+k)%nbTri].indice]->pos, points[sommets[p].indice]->pos, points[sommets[(p+1)%nbTri].indice]->pos, points[sommets[(p+nbTri-1)%nbTri].indice]->pos);
				}
				k++;
			}
			if (in) {
				surfaces[s]->addTriangle(sommets[p].indice, sommets[(p+1)%nbTri].indice, sommets[(p+nbTri-1)%nbTri].indice);
				for (unsigned int i=p;i<nbTri-1;i++)
					sommets[i]=sommets[i+1];
				nbTri--;
				if (sommets[p].concave)
					sommets[p].concave=prodScal(surfaces[s]->getNormale(),prodVect(points[sommets[(p+1)%nbTri].indice]->pos-points[sommets[p].indice]->pos, points[sommets[p].indice]->pos-points[sommets[(p+nbTri-1)%nbTri].indice]->pos))>=0;
				if (sommets[(p+nbTri-1)%nbTri].concave)
					sommets[(p+nbTri-1)%nbTri].concave=prodScal(surfaces[s]->getNormale(),prodVect(points[sommets[p].indice]->pos-points[sommets[(p+nbTri-1)%nbTri].indice]->pos, points[sommets[(p+nbTri-1)%nbTri].indice]->pos-points[sommets[(p+nbTri-2)%nbTri].indice]->pos))>=0;
				p=0;
			}
			else
				p++;
		}
		else
			p++;
	}
	if (nbTri==3) {
		if (!sommets[0].concave && !sommets[0].concave && !sommets[0].concave)
			surfaces[s]->addTriangle(sommets[0].indice, sommets[1].indice, sommets[2].indice);
		delete [] sommets;
		return surfaces[s]->getNbTriangles()>0;
	}
	else {
		delete [] sommets;
		std::cout<<"Triangulation de la surface "<<s<<" impossible"<<std::endl;
		return 0;
	}
}

bool Modele::triangulations()&
{
	bool b=1;
	for (unsigned int i=0;i<nbSurfaces;i++)
		b=(b && triangulation(i));
	return b;
}

Image* Modele::addImage(std::string nom)&
{
	for (unsigned int i=0;i<nbImages;i++)
		if (nom==images[i].nomImg)
			return images[i].img;
	Image * img=new Image;
	if (img->charger("./data/images/"+ nom)) {
		ImageText * nouv=new ImageText [nbImages+1];
		for (unsigned int i=0;i<nbImages;i++)
			nouv[i]=images[i];
		delete [] images;
		images=nouv;
		images[nbImages].nomImg=nom;
		images[nbImages].img=img;
		nbImages++;
		return img;
	}
	return NULL;
}

int Modele::addPoint(const Matrice & matRe)&
{
	assert(matRe.getDimy()==3 && matRe.getDimx()==1);
	Matrice mat(3,1);
	for (unsigned int i=0;i<nbPoints;i++) {
		mat=getPoint(i).posRe;
		if (matRe.get(0,0)==mat.get(0,0) && matRe.get(1,0)==mat.get(1,0) && matRe.get(2,0)==mat.get(2,0)) {
			std::cout<<"Ajout du point impossible : Ce point est le point "<<i<<std::endl;
			return i;
		}
	}
	Point ** nouv=new Point * [nbPoints+1];
	for (unsigned int i=0;i<nbPoints;i++)
		nouv[i]=points[i];
	nouv[nbPoints]=new Point;
	nouv[nbPoints]->posRe=matRe;
	if (points!=NULL)
		delete [] points;
	points=nouv;
	nbPoints++;
	majPoint(nbPoints-1);
	if (nbPoints==1)
		calculDistMax();
	else {
		float dist;
		for (unsigned int i=0;i<nbPoints-1;i++)
		{
			dist=norme(points[nbPoints-1]->posRe-points[i]->posRe);
			if (distMax<dist)
				distMax=dist;
		}
	}
	return -1;
}

bool Modele::delPoint(unsigned int p)&
{
	assert(p<nbPoints);
	if (nbPoints==1) {
		delete points[0];
		delete [] points;
		points=NULL;
		nbPoints=0;
		distMax=0;
		return 1;
	}
	for (unsigned int i=0;i<nbSurfaces;i++)
		if (surfaces[i]->pointIn(p)) {
			if (surfaces[i]->getNbPoints()==3) {
				delSurf(i);
				i--;
			}
			else {
				if (!delPointSurf(i,p)) {
					std::cout<<"Suppression du point "<<p<<" impossible"<<std::endl;
					return 0;
				}
			}
		}
	Point ** nouv=new Point * [nbPoints-1];
	for (unsigned int i=0;i<p;i++)
		nouv[i]=points[i];
	delete points[p];
	for (unsigned int i=p;i<nbPoints-1;i++)
		nouv[i]=points[i+1];
	delete [] points;
	points=nouv;
	nbPoints--;
	for (unsigned int i=0;i<nbSurfaces;i++)
		for (unsigned int j=0;j<surfaces[i]->getNbPoints();j++)
			if (surfaces[i]->get(j)>p)
				surfaces[i]->set(j,surfaces[i]->get(j)-1);
	calculDistMax();
	triangulations();
	return 1;
}

const Point& Modele::getPoint(unsigned int p)const &
{
	assert(p<nbPoints);
	const Point& res=*points[p];
	return res; 
}

bool Modele::setPoint(unsigned int p, const Matrice & matRe)&
{
	assert(p<nbPoints);
	assert(matRe.getDimy()==3 && matRe.getDimx()==1);
	Matrice mat(3,1);
	mat=points[p]->posRe;
	points[p]->posRe=matRe;
	majPoint(p);
	bool b=1;
	for (unsigned int i=0;i<nbSurfaces;i++)
	{
		if (surfaces[i]->pointIn(p)) {
			if (!triangulation(i)) {
				b=0;
				std::cout<<"Modification du point "<<p<<" impossible"<<std::endl;
			}
		}
	}
	if (b) {
		points[p]->pos=pos+base*points[p]->posRe;
		calculDistMax();
		calculCentres();
	}
	else {
		points[p]->posRe=mat;
		majPoint(p);
		triangulations();
	}
	return b;
}

bool Modele::addPointSurf(unsigned int s, unsigned int p, unsigned int pi)&
{
	assert(s<nbSurfaces);
	assert(p<=surfaces[s]->getNbPoints());
	assert(pi<nbPoints);
	if (surfaces[s]->pointIn(pi)){
		std::cout<<"Ajout du point a la surface impossible : Le point "<<pi<<" appartient deja a la surface "<<s<<std::endl;
		return 0;
	}
	else {
		surfaces[s]->addPoint(p,pi);
		if (!triangulation(s)) {
			surfaces[s]->delPoint(pi);
			std::cout<<"Ajout du point "<<pi<<" a la surface "<<s<<" impossible"<<std::endl;
			return 0;
		}
	}
	calculCentre(s);
	return 1;	
}

bool Modele::addPointSurf(unsigned int s, unsigned int p,const Matrice & matRe)&
{
	assert(s<nbSurfaces);
	assert(p<=surfaces[s]->getNbPoints());
	assert(matRe.getDimy()==3 && matRe.getDimx()==1);
	int n=addPoint(matRe);
	if (-1==n)
		if(addPointSurf(s,p,nbPoints-1))
			return 1;
		else {
			delPoint(nbPoints-1);
			return 0;
		}
	else
		return addPointSurf(s,p,n);
}

bool Modele::delPointSurf(unsigned int s, unsigned int p)&
{
	assert(s<nbSurfaces);
	assert(p<nbPoints);
	assert(surfaces[s]->getNbPoints()>=3);
	if (surfaces[s]->pointIn(p)) {
		unsigned int num=surfaces[s]->delPoint(p);
		if (!triangulation(s)) {
			surfaces[s]->addPoint(num,p);
			triangulation(s);
			std::cout<<"Suppression du point "<<p<<" de la surface "<<s<<" impossible"<<std::endl;
			return 0;
		}
		calculCentre(s);
		return 1;
	}
	else {
		std::cout<<"Suppression du point de la surface impossible : Le point "<<p<<" n'appartient pas a la surface "<<s<<std::endl;
		return 0;
	}
}

unsigned int Modele::getIndicePointSurf(unsigned int s, unsigned int p)const &
{
	assert(s<nbSurfaces);
	assert(p<surfaces[s]->getNbPoints());
	return surfaces[s]->get(p);
}

const Point& Modele::getPointSurf(unsigned int s, unsigned int p)const &
{
	assert(s<nbSurfaces);
	assert(p<nbPoints);
	return getPoint(surfaces[s]->get(p));
}

bool Modele::setPointSurf(unsigned int s, unsigned int p, unsigned int pi)&
{
	assert(s<nbSurfaces);
	assert(pi<nbPoints);
	if (!surfaces[s]->pointIn(pi))
	{
		unsigned int ps=surfaces[s]->get(p);
		surfaces[s]->set(p,pi);
		if (!triangulation(s)) {
			surfaces[s]->set(p,ps);
			triangulation(s);
			std::cout<<"Modification du point de la surface "<<s<<" impossible"<<std::endl;
			return 0;
		}
		calculCentre(s);
		return 1;
	}
	std::cout<<"Modification du point de la surface impossible : Le point "<<pi<<" appartient deja a la surface "<<s<<std::endl;
	return 0;
}

bool Modele::setPointSurf(unsigned int s, unsigned int p,const Matrice & matRe)&
{
	assert(s<nbSurfaces);
	assert(p<surfaces[s]->getNbPoints());
	assert(matRe.getDimy()==3 && matRe.getDimx()==1);
	if (setPoint(getIndicePointSurf(s,p),matRe))
		return 1;
	else {
		int n=addPoint(matRe);
		if (n==-1)
			if (setPointSurf(s,p,nbPoints-1)) {
				std::cout<<"Modification du point "<<p<<" de la surface "<<s<<" effectuee : Ajout d'un nouveau point"<<std::endl;
				return 1;
			}
			else {
				delPoint(nbPoints-1);
				return 0;
			}
		else
			return setPointSurf(s,p,n);
	}
}

bool Modele::addSurf(unsigned int p1, unsigned int p2, unsigned int p3)&
{
	assert(p1<nbPoints && p2<nbPoints && p3<nbPoints);
	Surface ** nouv=new Surface * [nbSurfaces+1];
	for (unsigned int i=0;i<nbSurfaces;i++)
		nouv[i]=surfaces[i];
	nouv[nbSurfaces]=new Surface(p1,p2,p3);
	delete [] surfaces;
	surfaces=nouv;
	nbSurfaces++;
	if (!triangulation(nbSurfaces-1)) {
		std::cout<<"Ajout de la surface "<<nbSurfaces-1<<" impossible"<<std::endl;
		delSurf(nbSurfaces-1);
		return 0;
	}
	calculCentre(nbSurfaces-1);
	return 1;
}

bool Modele::addSurf(unsigned int * tab, unsigned int nb)&
{
	for (unsigned int i=0;i<nb;i++)
		if (tab[i]>=nbPoints) {
			std::cout<<"Ajout de la surface "<<nbSurfaces-1<<" impossible : Le point "<<tab[i]<<" n existe pas"<<std::endl;
			return 0;
		}
	for (unsigned int j=0;j<nb-1;j++)
		for (unsigned int i=j+1;i<nb;i++) {
			if (tab[j]==tab[i]) {
				std::cout<<"Ajout de la surface "<<nbSurfaces-1<<" impossible : Le point "<<tab[i]<<" apparait plusieurs fois"<<std::endl;
				return 0;
			}
		}
	Surface ** nouv=new Surface * [nbSurfaces+1];
	for (unsigned int i=0;i<nbSurfaces;i++)
		nouv[i]=surfaces[i];
	nouv[nbSurfaces]=new Surface(tab,nb);
	delete [] surfaces;
	surfaces=nouv;
	nbSurfaces++;
	if (!triangulation(nbSurfaces-1)) {
		std::cout<<"Ajout de la surface "<<nbSurfaces-1<<" impossible"<<std::endl;
		delSurf(nbSurfaces-1);
		return 0;
	}
	calculCentre(nbSurfaces-1);
	return 1;
}

void Modele::delSurf(unsigned int s)&
{
	assert(s<nbSurfaces);
	if (nbSurfaces==1){
		delete surfaces[0];
		delete [] surfaces;
		surfaces=NULL;
		nbSurfaces=0;
	}
	else {
		Surface ** nouv=new Surface * [nbSurfaces-1];
		for (unsigned int i=0;i<s;i++)
			nouv[i]=surfaces[i];
		delete surfaces[s];
		for (unsigned int i=s;i<nbSurfaces-1;i++)
			nouv[i]=surfaces[i+1];
		delete [] surfaces;
		nbSurfaces--;
		surfaces=nouv;
	}
}

bool Modele::depSurf(unsigned int s, const Matrice & dep)&
{
	assert(s<nbSurfaces);
	assert(dep.getDimy()==3 && dep.getDimx()==1);
	for (unsigned int p=0;p<surfaces[s]->getNbPoints();p++)
		points[surfaces[s]->get(p)]->posRe=points[surfaces[s]->get(p)]->posRe+dep;
	for (unsigned int p=0;p<surfaces[s]->getNbPoints();p++)
		majPoint(surfaces[s]->get(p));
	if (!triangulations()) {
		for (unsigned int p=0;p<surfaces[s]->getNbPoints();p++)
			points[surfaces[s]->get(p)]->posRe=points[surfaces[s]->get(p)]->posRe-dep;
		for (unsigned int p=0;p<surfaces[s]->getNbPoints();p++)
			majPoint(surfaces[s]->get(p));
		triangulations();
		std::cout<<"Deplacement de la surface "<<s<<" impossible"<<std::endl;
		return 0;
	}
	calculCentres();
	return 1;
}

bool Modele::depSurf(unsigned int s, float x, float y, float z)&
{
	Matrice dep(3,1);
	dep.set(0,0,x);
	dep.set(1,0,y);
	dep.set(2,0,z);
	return depSurf(s,dep);
}

Image* Modele::getImageSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getImage();
}

void Modele::setImageSurf(unsigned int s, std::string nom)&
{
	assert(s<nbSurfaces);
	if (nom=="NULL")
		surfaces[s]->setImage(NULL);
	else
		surfaces[s]->setImage(addImage(nom));
}

void Modele::setImageSurf(unsigned int s, Image * img)&
{
	assert(s<nbSurfaces);
	surfaces[s]->setImage(img);
}

std::string Modele::getNomImageSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	Image * img=surfaces[s]->getImage();
	if (img==NULL)
		return "NULL";
	for (unsigned int i=0;i<nbImages;i++)
		if (img==images[i].img)
			return images[i].nomImg;
	return "NULL";
}

void Modele::setPositionSurf(unsigned int s, float x, float y)&
{
	assert(s<nbSurfaces);
	surfaces[s]->setPosition(x,y);
}

float Modele::getPosxSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getPosx();
}

float Modele::getPosySurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getPosy();
}

void Modele::setTailleSurf(unsigned int s, float tx, float ty)&
{
	assert(s<nbSurfaces);
	surfaces[s]->setTaille(tx,ty);
}

float Modele::getTaillexSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getTaillex();
}

float Modele::getTailleySurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getTailley();
}

void Modele::setAngleSurf(unsigned int s, float ang)&
{
	assert(s<nbSurfaces);
	surfaces[s]->setAngle(ang);
}

float Modele::getAngleSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getAngle();
}

const Matrice& Modele::getNormaleSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getNormale();
}

bool Modele::pointInTriangle(const Matrice & p, const Matrice & t1, const Matrice & t2, const Matrice & t3)const &
{
	assert(p.getDimy()==3 && p.getDimx()==1);
	assert(t1.getDimy()==3 && t1.getDimx()==1);
	assert(t2.getDimy()==3 && t2.getDimx()==1);
	assert(t3.getDimy()==3 && t3.getDimx()==1);
	if (prodScal(prodVect(t1-t2,t1-p),prodVect(t1-p,t1-t3))<0)
		return 0;
	if (prodScal(prodVect(t2-t1,t2-p),prodVect(t2-p,t2-t3))<0)
		return 0;
	return (prodScal(prodVect(t3-t1,t3-p),prodVect(t3-p,t3-t2))>=0);
}

bool Modele::pointInSurf(unsigned int s, const Matrice & p)&
{
	Triangle * tri;
	for (unsigned int i=0;i<surfaces[s]->getNbTriangles();i++) {
		tri=surfaces[s]->getTriangle(i);
		if (pointInTriangle(p, points[tri->point1]->pos, points[tri->point2]->pos, points[tri->point3]->pos))
			return 1;
	}
	return 0;
}

Triangle* Modele::getTriangleSurf(unsigned int s, unsigned int t)&
{
	assert(s<nbSurfaces);
	assert(t<surfaces[s]->getNbTriangles());
	return surfaces[s]->getTriangle(t);
}

unsigned int Modele::getNbTrianglesSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getNbTriangles();
}

const Matrice& Modele::getPos()const &
{
	const Matrice& res=pos;
	return res;
}

void Modele::setPos(float x, float y, float z)
{
	pos.set(0,0,x);
	pos.set(1,0,y);
	pos.set(2,0,z);
	majPoints();
	majCentres();
}

void Modele::setPos(const Matrice & pos)
{
	assert(pos.getDimy()==3 && pos.getDimx()==1);
	setPos(pos.get(0,0),pos.get(1,0),pos.get(2,0));
}

void Modele::setPosRe(const Camera & cam, float x, float y, float z)
{
	Matrice pos(3,1);
	pos.set(0,0,x);
	pos.set(1,0,y);
	pos.set(2,0,z);
	setPosRe(cam,pos);
}

void Modele::setPosRe(const Camera & cam, const Matrice & pos)
{
	assert(pos.getDimy()==3 && pos.getDimx()==1);
	Matrice rot(3,3);
	float cosxy=cs.Cos(cam.getAngxy());
	float sinxy=cs.Sin(cam.getAngxy());
	float cosxz=cs.Cos(cam.getAngxz());
	float sinxz=cs.Sin(cam.getAngxz());
	float cosyz=cs.Cos(cam.getAngyz());
	float sinyz=cs.Sin(cam.getAngyz());
	rot.set(0,0,cosxy*cosxz);
	rot.set(0,1,-cosxy*sinxz*sinyz+sinxy*cosyz);
	rot.set(0,2,-sinxy*sinyz-cosxy*sinxz*cosyz);
	rot.set(1,0,-sinxy*cosxz);
	rot.set(1,1,cosxy*cosyz+sinxy*sinxz*sinyz);
	rot.set(1,2,-cosxy*sinyz+sinxy*sinxz*cosyz);
	rot.set(2,0,sinxz);
	rot.set(2,1,cosxz*sinyz);
	rot.set(2,2,cosxz*cosyz);
	setPos(rot*pos+cam.getPos());
}

float Modele::getX()const &
{
	return pos.get(0,0);
}

void Modele::setX(float x)
{
	pos.set(0,0,x);
	majPoints();
	majCentres();
}

float Modele::getY()const &
{
	return pos.get(1,0);
}

void Modele::setY(float y)
{
	pos.set(1,0,y);
	majPoints();
	majCentres();
}

float Modele::getZ()const &
{
	return pos.get(2,0);
}

void Modele::setZ(float z)
{
	pos.set(2,0,z);
	majPoints();
	majCentres();
}

float Modele::getAngxy()const &
{
	return angle_xy;
}

void Modele::setAngxy(float angxy)
{
	angle_xy=angxy-int(angxy/360+0.5)*360;
	calculBase();
	majPoints();
	majCentres();
	majNormales();
}

float Modele::getAngxz()const &
{
	return angle_xz;
}

void Modele::setAngxz(float angxz)
{
	angle_xz=angxz-int(angxz/360+0.5)*360;
	calculBase();
	majPoints();
	majCentres();
	majNormales();
}

float Modele::getAngyz()const &
{
	return angle_yz;
}

void Modele::setAngyz(float angyz)
{
	angle_yz=angyz-int(angyz/360+0.5)*360;
	calculBase();
	majPoints();
	majCentres();
	majNormales();
}

void Modele::setAng(float angxy, float angxz, float angyz)
{
	angle_xy=angxy-int(angxy/360+0.5)*360;
	angle_xz=angxz-int(angxz/360+0.5)*360;
	angle_yz=angyz-int(angyz/360+0.5)*360;
	calculBase();
	majPoints();
	majCentres();
	majNormales();
}

unsigned int Modele::getNbPoints()const &
{
	return nbPoints;
}
	
unsigned int Modele::getNbPointsSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getNbPoints();
}
	
unsigned int Modele::getNbSurfaces()const &
{
	return nbSurfaces;
}

unsigned int Modele::getNbImages()const &
{
	return nbImages;
}

Image* Modele::getImage(unsigned int i)const &
{
	assert(i<nbImages);
	return images[i].img;
}

float Modele::getDistMax()const &
{
	return distMax;
}

float Modele::getTaille()const &
{
	return taille;
}
	
void Modele::setTaille(float t)
{
	assert(t>0);
	taille=t;
	majPoints();
	majCentres();
}

float Modele::getOpacite()const &
{
	return opacite;
}
	
void Modele::setOpacite(float opac)
{
	if (opac<0)
		opacite=0;
	else
		if (opac>1)
			opacite=1;
		else
			opacite=opac;
}

float Modele::getOpaciteSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->getOpacite();
}

void Modele::setOpaciteSurf(unsigned int s, float opac)
{
	assert(s<nbSurfaces);
	surfaces[s]->setOpacite(opac);
}

float Modele::getDistAffichage()const &
{
	return distAffichage;
}

void Modele::setDistAffichage(float dist)
{
	distAffichage=dist;
}

const Matrice& Modele::getBase()const &
{
	const Matrice& res=base;
	return res;
}

std::string Modele::getModele()const &
{
	return modele;
}

void Modele::setModele(const std::string & mod)
{
	modele=mod;
}
	
std::string Modele::getTexture()const &
{
	return texture;
}

void Modele::setTexture(const std::string & tex)
{
	texture=tex;
}

bool Modele::chargerSon(const std::string & nom)
{
	if (son!=NULL) {
		Mix_FreeChunk(son);
		son=NULL;
	}
	if (canal!=-1) {
		if (Mix_Playing(canal))
			Mix_HaltChannel(canal);
		canal=-1;
	}
	std::string chemin="./data/sons/"+ nom;
	if (nom[nom.size()-4]!='.')
		chemin=chemin+".wav";
	char nom2[100];
	strcpy(nom2, chemin.c_str());
	son = Mix_LoadWAV(nom2);
	if (son==NULL) {
		std::cout<<"Chargement du son impossible : Fichier "<<chemin<<".wav introuvable"<<std::endl;
		return 0;
	}
	return 1;
}

bool Modele::jouerSon(int repet)
{
	assert(repet>=-1);
	if (son==NULL)
		return 0;
	if (canal==-1)
		canal=Mix_PlayChannel(-1,son,repet);
	else
		if (!Mix_Playing(canal))
			canal=Mix_PlayChannel(-1,son,repet);
		else {
			Mix_HaltChannel(canal);
			canal=Mix_PlayChannel(canal,son,repet);
		}
	return 1;
}

void Modele::pauseSon()
{
	if (canal!=-1)
		Mix_Pause(canal);
}

int Modele::isPauseSon()
{
	if (canal!=-1) {
		if (Mix_Playing(canal))
			return Mix_Paused(canal);
		else {
			Mix_HaltChannel(canal);
			canal=-1;
		}
	}
	return 2;
}

void Modele::reprendreSon()
{
	if (canal!=-1)
		Mix_Resume(canal);
}

void Modele::arreterSon()
{
	if (canal!=-1) {
		if (Mix_Playing(canal))
			Mix_HaltChannel(canal);
		canal=-1;
	}
}

float Modele::getIntensite()const
{
	return intensite;
}

void Modele::setIntensite(float intens)
{
	if (intens<0)
		intensite=0;
	else
		if (intens>100)
			intensite=100;
		else
			intensite=intens;
}

void Modele::majSon(const Matrice & position)
{
	if (canal!=-1) {
		if (!Mix_Playing(canal)) {
			Mix_HaltChannel(canal);
			canal=-1;
		}
		else {
			Mix_Volume(canal, intensite*MIX_MAX_VOLUME/100.0);
			float angle=0;
			if (position.get(0,0)>0)
				angle=180*atan(position.get(1,0)/position.get(0,0))/3.1415926;
			else
				if (position.get(0,0)<0)
					angle=180+180*atan(position.get(1,0)/position.get(0,0))/3.1415926;
				else
					if (position.get(1,0)<0)
						angle=180;
			Mix_SetPosition(canal,angle,std::min(int(norme(position)/100),255));
		}
	}
}

Matrice& Modele::getCentreSurf(unsigned int s)const &
{
	assert(s<nbSurfaces);
	return surfaces[s]->centre.pos;
}

Matrice* Modele::croisSegPlan(const Matrice & s1, const Matrice & s2, const Matrice & t1, const Matrice & t2, const Matrice & t3)const &
{
	assert(s1.getDimy()==3 && s1.getDimx()==1);
	assert(s2.getDimy()==3 && s2.getDimx()==1);
	assert(t1.getDimy()==3 && t1.getDimx()==1);
	assert(t2.getDimy()==3 && t2.getDimx()==1);
	assert(t3.getDimy()==3 && t3.getDimx()==1);
	Matrice m0(3,1),m1(3,1),m2(3,1),s(3,1);
	m0=t1-s1;
	m1=t2-t1;
	m2=t3-t1;
	s=s2-s1;
	if (prodScal(s,prodVect(m1,m2))==0)
		return NULL;
	float a0d,a1d,a2d,in1,jn1,n1,in2,jn2,n2,id,jd,d,denom;
	float l1,l2,alpha;
	if (s.get(0,0)!=0) {
		a0d=1.0*m0.get(0,0)/s.get(0,0);
		a1d=1.0*m1.get(0,0)/s.get(0,0);
		a2d=1.0*m2.get(0,0)/s.get(0,0);
		in1=a0d*m2.get(2,0)-a2d*m0.get(2,0);
		jn1=a2d*m0.get(1,0)-a0d*m2.get(1,0);
		n1=m2.get(1,0)*m0.get(2,0)-m0.get(1,0)*m2.get(2,0);
		in2=a1d*m0.get(2,0)-a0d*m1.get(2,0);
		jn2=a0d*m1.get(1,0)-a1d*m0.get(1,0);
		n2=m0.get(1,0)*m1.get(2,0)-m1.get(1,0)*m0.get(2,0);
		id=a2d*m1.get(2,0)-a1d*m2.get(2,0);
		jd=a1d*m2.get(1,0)-a2d*m1.get(1,0);
		d=m1.get(1,0)*m2.get(2,0)-m2.get(1,0)*m1.get(2,0);
		denom=id*s.get(1,0)+jd*s.get(2,0)+d;
		if (denom==0)
			return NULL;
		l1=(in1*s.get(1,0)+jn1*s.get(2,0)+n1)/denom;
		l2=(in2*s.get(1,0)+jn2*s.get(2,0)+n2)/denom;
	}
	else
		if (s.get(1,0)!=0) {
			a0d=1.0*m0.get(1,0)/s.get(1,0);
			a1d=1.0*m1.get(1,0)/s.get(1,0);
			a2d=1.0*m2.get(1,0)/s.get(1,0);
			in1=a0d*m2.get(2,0)-a2d*m0.get(2,0);
			jn1=a2d*m0.get(0,0)-a0d*m2.get(0,0);
			n1=m2.get(0,0)*m0.get(2,0)-m0.get(0,0)*m2.get(2,0);
			in2=a1d*m0.get(2,0)-a0d*m1.get(2,0);
			jn2=a0d*m1.get(0,0)-a1d*m0.get(0,0);
			n2=m0.get(0,0)*m1.get(2,0)-m1.get(0,0)*m0.get(2,0);
			id=a2d*m1.get(2,0)-a1d*m2.get(2,0);
			jd=a1d*m2.get(0,0)-a2d*m1.get(0,0);
			d=m1.get(0,0)*m2.get(2,0)-m2.get(0,0)*m1.get(2,0);
			denom=id*s.get(0,0)+jd*s.get(2,0)+d;
			if (denom==0)
				return NULL;
			l1=(in1*s.get(0,0)+jn1*s.get(2,0)+n1)/denom;
			l2=(in2*s.get(0,0)+jn2*s.get(2,0)+n2)/denom;
		}
		else {
			a0d=1.0*m0.get(2,0)/s.get(2,0);
			a1d=1.0*m1.get(2,0)/s.get(2,0);
			a2d=1.0*m2.get(2,0)/s.get(2,0);
			in1=a0d*m2.get(0,0)-a2d*m0.get(0,0);
			jn1=a2d*m0.get(1,0)-a0d*m2.get(1,0);
			n1=m2.get(1,0)*m0.get(0,0)-m0.get(1,0)*m2.get(0,0);
			in2=a1d*m0.get(0,0)-a0d*m1.get(0,0);
			jn2=a0d*m1.get(1,0)-a1d*m0.get(1,0);
			n2=m0.get(1,0)*m1.get(0,0)-m1.get(1,0)*m0.get(0,0);
			id=a2d*m1.get(0,0)-a1d*m2.get(0,0);
			jd=a1d*m2.get(1,0)-a2d*m1.get(1,0);
			d=m1.get(1,0)*m2.get(0,0)-m2.get(1,0)*m1.get(0,0);
			denom=id*s.get(1,0)+jd*s.get(0,0)+d;
			if (denom==0)
				return NULL;
			l1=(in1*s.get(1,0)+jn1*s.get(0,0)+n1)/denom;
			l2=(in2*s.get(1,0)+jn2*s.get(0,0)+n2)/denom;
		}
	alpha=l1*a1d+l2*a2d+a0d;
	if (alpha<=0 || alpha>=1)
		return NULL;
	Matrice * posr=new Matrice(3,1);
	posr->set(0,0,l1*m1.get(0,0)+l2*m2.get(0,0)+m0.get(0,0)+s1.get(0,0));
	posr->set(1,0,l1*m1.get(1,0)+l2*m2.get(1,0)+m0.get(1,0)+s1.get(1,0));
	posr->set(2,0,l1*m1.get(2,0)+l2*m2.get(2,0)+m0.get(2,0)+s1.get(2,0));
	return posr;
}

int collisionCube(const Modele * mod1, const Modele * mod2)
{
	assert(mod1!=NULL && mod2!=NULL);
	assert(mod1!=mod2);
	return mod1->testCollisionCube(*creationCubeModele(mod1),*creationCubeModele(mod2));
}

int Modele::testCollisionCube(const Matrice & cube1, const Matrice & cube2)const &
{
	assert(cube1.getDimy()==6 && cube1.getDimx()==1);
	assert(cube2.getDimy()==6 && cube2.getDimx()==1);
	int x,y,z;
	if (cube2.get(0,0)<=cube1.get(0,0) && cube1.get(1,0)<=cube2.get(1,0))
		x=2;
	else
		if (cube1.get(0,0)<=cube2.get(0,0) && cube2.get(1,0)<=cube1.get(1,0))
			x=3;
		else
			if ((cube1.get(0,0)<=cube2.get(0,0) && cube2.get(0,0)<=cube1.get(1,0) && cube2.get(1,0)>=cube1.get(1,0))||(cube1.get(0,0)<=cube2.get(1,0) && cube2.get(1,0)<=cube1.get(1,0) && cube2.get(0,0)<=cube1.get(0,0)))
				x=1;
			else
				return 0;
	if (cube2.get(2,0)<=cube1.get(2,0) && cube1.get(3,0)<=cube2.get(3,0))
		y=2;
	else
		if (cube1.get(2,0)<=cube2.get(2,0) && cube2.get(3,0)<=cube1.get(3,0))
			y=3;
		else
			if ((cube1.get(2,0)<=cube2.get(2,0) && cube2.get(2,0)<=cube1.get(3,0) && cube2.get(3,0)>=cube1.get(3,0))||(cube1.get(2,0)<=cube2.get(3,0) && cube2.get(3,0)<=cube1.get(3,0) && cube2.get(2,0)<=cube1.get(2,0)))
				y=1;
			else
				return 0;
	if (cube2.get(4,0)<=cube1.get(4,0) && cube1.get(5,0)<=cube2.get(5,0))
		z=2;
	else
		if (cube1.get(4,0)<=cube2.get(4,0) && cube2.get(5,0)<=cube1.get(5,0))
			z=3;
		else
			if ((cube1.get(4,0)<=cube2.get(4,0) && cube2.get(4,0)<=cube1.get(5,0) && cube2.get(5,0)>=cube1.get(5,0))||(cube1.get(4,0)<=cube2.get(5,0) && cube2.get(5,0)<=cube1.get(5,0) && cube2.get(4,0)<=cube1.get(4,0)))
				z=1;
			else
				return 0;
	if (x==y && y==z)
		return x;
	return 1;
}

Matrice* creationCubeModele(const Modele * mod)
{
	assert(mod!=NULL);
	Matrice * res=new Matrice (6,1);
	if (mod->getNbPoints()==0)
		return res;
	res->set(0,0,mod->getPoint(0).pos.get(0,0));
	res->set(1,0,mod->getPoint(0).pos.get(0,0));
	res->set(2,0,mod->getPoint(0).pos.get(1,0));
	res->set(3,0,mod->getPoint(0).pos.get(1,0));
	res->set(4,0,mod->getPoint(0).pos.get(2,0));
	res->set(5,0,mod->getPoint(0).pos.get(2,0));
	for (unsigned int i=1;i<mod->getNbPoints();i++) {
		if (res->get(0,0)>mod->getPoint(i).pos.get(0,0))
			res->set(0,0,mod->getPoint(i).pos.get(0,0));
		else
			if (res->get(1,0)<mod->getPoint(i).pos.get(0,0))
				res->set(1,0,mod->getPoint(i).pos.get(0,0));
		if (res->get(2,0)>mod->getPoint(i).pos.get(1,0))
			res->set(2,0,mod->getPoint(i).pos.get(1,0));
		else
			if (res->get(3,0)<mod->getPoint(i).pos.get(1,0))
				res->set(3,0,mod->getPoint(i).pos.get(1,0));
		if (res->get(4,0)>mod->getPoint(i).pos.get(2,0))
			res->set(4,0,mod->getPoint(i).pos.get(2,0));
		else
			if (res->get(5,0)<mod->getPoint(i).pos.get(2,0))
				res->set(5,0,mod->getPoint(i).pos.get(2,0));
	}
	return res;
}

Matrice* creationCubeSurf(const Modele * mod,unsigned int s)
{
	assert(mod!=NULL);
	assert(s<mod->getNbSurfaces());
	Matrice*res=new Matrice (6,1);
	res->set(0,0,mod->getPointSurf(s,0).pos.get(0,0));
	res->set(1,0,mod->getPointSurf(s,0).pos.get(0,0));
	res->set(2,0,mod->getPointSurf(s,0).pos.get(1,0));
	res->set(3,0,mod->getPointSurf(s,0).pos.get(1,0));
	res->set(4,0,mod->getPointSurf(s,0).pos.get(2,0));
	res->set(5,0,mod->getPointSurf(s,0).pos.get(2,0));
	for (unsigned int i=1;i<mod->getNbPointsSurf(s);i++) {
		if (res->get(0,0)>mod->getPointSurf(s,i).pos.get(0,0))
			res->set(0,0,mod->getPointSurf(s,i).pos.get(0,0));
		else
			if (res->get(1,0)<mod->getPointSurf(s,i).pos.get(0,0))
				res->set(1,0,mod->getPointSurf(s,i).pos.get(0,0));
		if (res->get(2,0)>mod->getPointSurf(s,i).pos.get(1,0))
			res->set(2,0,mod->getPointSurf(s,i).pos.get(1,0));
		else
			if (res->get(3,0)<mod->getPointSurf(s,i).pos.get(1,0))
				res->set(3,0,mod->getPointSurf(s,i).pos.get(1,0));
		if (res->get(4,0)>mod->getPointSurf(s,i).pos.get(2,0))
			res->set(4,0,mod->getPointSurf(s,i).pos.get(2,0));
		else
			if (res->get(5,0)<mod->getPointSurf(s,i).pos.get(2,0))
				res->set(5,0,mod->getPointSurf(s,i).pos.get(2,0));
	}
	return res;
}

bool collision(Modele * mod1, Modele * mod2)
{
	assert(mod1!=NULL && mod2!=NULL);
	Matrice cube1(6,1), cube2(6,1);
	if (mod1->getNbSurfaces()>0 && mod2->getNbSurfaces()>0) {
		cube1=*creationCubeModele(mod1);
		cube2=*creationCubeModele(mod2);
		if (mod1->testCollisionCube(cube1,cube2)!=0) {
			Matrice ** cubes1=new Matrice* [mod1->getNbSurfaces()];
			for (unsigned int i=0;i<mod1->getNbSurfaces();i++)
				cubes1[i]=creationCubeSurf(mod1,i);
			Matrice ** cubes2=new Matrice* [mod2->getNbSurfaces()];
			for (unsigned int i=0;i<mod2->getNbSurfaces();i++)
				cubes2[i]=creationCubeSurf(mod2,i);
			for (unsigned int j=0;j<mod1->getNbSurfaces();j++)
				if (mod1->testCollisionCube(*cubes1[j],cube2)!=0)
					for (unsigned int i=0;i<mod2->getNbSurfaces();i++)
						if (mod1->testCollisionCube(*cubes1[j],*cubes2[i])!=0)
							if (collisionSurf(mod1,j,mod2,i)) {
								for (unsigned int k=0;k<mod1->getNbSurfaces();k++)
									delete cubes1[k];
								delete [] cubes1;
								for (unsigned int k=0;k<mod2->getNbSurfaces();k++)
									delete cubes2[k];
								delete [] cubes2;
								return 1;
							}
			for (unsigned int k=0;k<mod1->getNbSurfaces();k++)
				delete cubes1[k];
			delete [] cubes1;
			for (unsigned int k=0;k<mod2->getNbSurfaces();k++)
				delete cubes2[k];
			delete [] cubes2;
		}
	}
	return 0;
}

bool collisionSurf(Modele * mod1, unsigned int s1, Modele * mod2, unsigned int s2)
{
	assert(mod1!=NULL && mod2!=NULL);
	assert(s1<mod1->getNbSurfaces() && s2<mod2->getNbSurfaces());
	Matrice *pos1;
	Triangle * tri1;
	Triangle * tri2=mod2->getTriangleSurf(s2,0);
	for (unsigned int i=0;i<mod1->getNbTrianglesSurf(s1);i++) {
		tri1=mod1->getTriangleSurf(s1,i);
		pos1=mod1->croisSegPlan(mod1->getPoint(tri1->point1).pos,mod1->getPoint(tri1->point3).pos,mod2->getPoint(tri2->point1).pos,mod2->getPoint(tri2->point2).pos,mod2->getPoint(tri2->point3).pos);
		if (pos1!=NULL) {
			if (mod2->pointInSurf(s2,*pos1)) {
				delete pos1;
				return 1;
			}
			delete pos1;
		}
		pos1=mod1->croisSegPlan(mod1->getPoint(tri1->point2).pos,mod1->getPoint(tri1->point1).pos,mod2->getPoint(tri2->point1).pos,mod2->getPoint(tri2->point2).pos,mod2->getPoint(tri2->point3).pos);
		if (pos1!=NULL) {
			if (mod2->pointInSurf(s2,*pos1)) {
				delete pos1;
				return 1;
			}
			delete pos1;
		}
		pos1=mod1->croisSegPlan(mod1->getPoint(tri1->point3).pos,mod1->getPoint(tri1->point2).pos,mod2->getPoint(tri2->point1).pos,mod2->getPoint(tri2->point2).pos,mod2->getPoint(tri2->point3).pos);
		if (pos1!=NULL) {
			if (mod2->pointInSurf(s2,*pos1)) {
				delete pos1;
				return 1;
			}
			delete pos1;
		}
	}
	tri1=mod1->getTriangleSurf(s1,0);
	for (unsigned int i=0;i<mod2->getNbTrianglesSurf(s2);i++) {
		tri2=mod2->getTriangleSurf(s2,i);
		pos1=mod1->croisSegPlan(mod2->getPoint(tri2->point1).pos,mod2->getPoint(tri2->point3).pos,mod1->getPoint(tri1->point1).pos,mod1->getPoint(tri1->point2).pos,mod1->getPoint(tri1->point3).pos);
		if (pos1!=NULL) {
			if (mod1->pointInSurf(s1,*pos1)) {
				delete pos1;
				return 1;
			}
			delete pos1;
		}
		pos1=mod1->croisSegPlan(mod2->getPoint(tri2->point2).pos,mod2->getPoint(tri2->point2).pos,mod1->getPoint(tri1->point1).pos,mod1->getPoint(tri1->point2).pos,mod1->getPoint(tri1->point3).pos);
		if (pos1!=NULL) {
			if (mod1->pointInSurf(s1,*pos1)) {
				delete pos1;
				return 1;
			}
			delete pos1;
		}
		pos1=mod1->croisSegPlan(mod2->getPoint(tri2->point3).pos,mod2->getPoint(tri2->point1).pos,mod1->getPoint(tri1->point1).pos,mod1->getPoint(tri1->point2).pos,mod1->getPoint(tri1->point3).pos);
		if (pos1!=NULL) {
			if (mod1->pointInSurf(s1,*pos1)) {
				delete pos1;
				return 1;
			}
			delete pos1;
		}
	}
	return 0;
}

void Modele::afficher()const &
{
	std::cout<<std::endl<<"Nom modele : "<<modele<<std::endl;
	std::cout<<"Nom texture : "<<texture<<std::endl;
	std::cout<<"Position x : "<<pos.get(0,0)<<" | y : "<<pos.get(1,0)<<" | z : "<<pos.get(2,0)<<std::endl;
	std::cout<<"Angle xy : "<<angle_xy<<" | xz : "<<angle_xz<<" | yz : "<<angle_yz<<std::endl;
	std::cout<<"Base : "<<std::endl;
	base.afficher();
	std::cout<<"Taille : "<<taille<<std::endl;
	std::cout<<"Opacite : "<<opacite<<std::endl;
	std::cout<<"Distance maximale : "<<distMax<<std::endl;
	std::cout<<"Distance affichage : "<<distAffichage<<std::endl;
	std::cout<<std::endl<<"Nombre de points : "<<nbPoints<<std::endl;
	for (unsigned int i=0;i<nbPoints;i++)
		std::cout<<"Point "<<i<<" | Position x : "<<points[i]->posRe.get(0,0)<<" | y : "<<points[i]->posRe.get(1,0)<<" | z : "<<points[i]->posRe.get(2,0)<<std::endl;
	std::cout<<std::endl<<"Nombre de surfaces : "<<nbSurfaces<<std::endl;
	for (unsigned int i=0;i<nbSurfaces;i++) {
		std::cout<<"Surface "<<i<<" | Nombre de points : "<<surfaces[i]->getNbPoints();
		for (unsigned int j=0;j<surfaces[i]->getNbPoints();j++)
			std::cout<<getIndicePointSurf(i,j)<<" ";
		std::cout<<" | Nombre de triangles : "<<surfaces[i]->getNbTriangles()<<std::endl;
	}
}
