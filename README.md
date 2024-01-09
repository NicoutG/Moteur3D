# Moteur3D

## Description
Ce projet a été puremet réalisé dans le cadre personnel dans le but de mettre en commun mes connaissance en mathématiques et en programmation sans aucune connaissance dans le domaine du 3D.

### Concept du projet
Ce projet permet de généré un espace 3D, de générer une image et de l'afficher.
Les collisions entre les différents modèles 3D peut aussi ètre géré.
Le moteur 3D permet aussi de gérer la musique et les sons émis par les modèles.

Ce projet comporte un main, un éditeur 3D, des modèles, des textures, des images, des sons et des musiques par défaut afin de faciliter l'utillisation.

## Fichiers et Dossiers
README.md : contient les informations sur l'organisation du projet

makefile : contient les liens entre les fichiers néccessaires à la compilation

bin : contient les executables générés après la compilation

obj : contient les fichiers objets générés par la compilation

doc : contient toute la documentation du projet

        /html/index.html : permet d'accéder à la documentation

data : contient les dossiers et fichiers qui permettent de faire générer l'espace 3D

        /images : contient les fichiers images

        /modèles : contient les fichiers textes qui définnissent les modèles 3D

        /moteur3D : contient les fichiers textes qui définnissent l'espace 3D

        /musiques : contient les fichiers audios des musiques

        /sons : contient les fichiers audios des sons

        /textures : contient les fichiers textes qui définnissent les textures des modèles

include : contient tout les fichiers .h

src : contient tout les fichiers .cpp

## Installation
La bibliothèque SDL2 est indispensable pour pouvoir charger des images et gérer les sons et musiques.

### Mise en place
Dans ce projet, vous pouvez trouver le fichier main.cpp qui donne un exemple de 
comment utiliser le moteur 3D.

De plus, le fichier main_editeur3D.cpp permet d'utiliser un éditeur de modèles 3D 
sans avoir à modifier à la main les fichiers textes des modèles.
Les fichiers utiles uniquement pour cet éditeur sont :
- main_editeur3D.cpp
- Editeur3D.cpp
- Editeur3D.h

Un affichage graphique SDL est disponible par défaut.
La classe Moteur3D permet de générer une image sans l'afficher.
Si vous souhaitez utiliser un autre affichage graphique que SDL, vous 
pouvez effacer les fichiers :
- Affichage_SDL.cpp
- Affichage_SDl.h
        
## Compiler et executer
### Compilation
Allez dans le même dossier que le fichier makefile et tapez "make" dans le terminal.

### Execution
Après avoir compilé, dans le même dossier, tapez :

        - bin/moteur3D : pour lancer le main et avoir un exemple d'utilisation du moteur 3D

        - bin/editeur3D : pour lancer l'éditeur de modèles 3D

## Utilisation
### Mise en place
Pour pouvoir utiliser le moteur3D dans un code, il faut inclure les classes suivantes : 
        
        - Moteur3D.h : contient l'ensemble de l'espace 3D

        - Affichage_SDL.h : permet d'afficher l'image en version graphique (optionnel)

### Fichiers importants
Tout les fichiers générant l'espace 3D se trouvent dans le dossier data.

#### Fichiers modèles
Voici à quoi resssemble un fichier qui génere un modèle 3D :

_![Modèle de cube](https://github.com/NiCoutG/Moteur3D/blob/main/doc/ReadMe/modèle.png?raw=true)

Ce fichier représente le modèle 3D d'un cube.

Voici comment ce fichier se décompose :

- Ligne 1 : Nombre de points qui composent le modèle. 8 dans le cas d'un cube.
- Ligne 2-9 : Coordonnées de chaque points sous le format x y z. Le premier point du cube a pour coordonnées -10x -10y 10z.
- Ligne 11 : Nombre de surfaces qui composent le modèle. 6 dans le cas d'un cube.
- Ligne 12-17 : Definis les surfaces du modèle.
  
          - Le premier numéro correspond au nombre de points qui composent la surface. 4 pour chaque face d'un cube
  
          - Les numéro qui suivent correspondent aux indices des points qui composent la surface. La première surface est donc composée des 4 premiers points           définis.

  Attention !!!

  Un point ne peut pas apparaitre 2 fois dans la même surface;
  L'ordre d'écriture des indices des points dans la surface est important.
  Chaque face n'est visible que dans un sens.
  La face est visible si en tracant un vecteur entre 2 points, l'interieur de la surface se trouve à droite du vecteur.
  Si la surface n'a pas d'angle concave, cela revient à définir les points dans le sens horaire.

  Voici un exemple :

  Supposons 5 points définis comme ceci :

          0                  1

                  2
             4             3

  Supposons une surface contenant ses points.
  Voici différentes manières de la définir pour quelle soit visible depuis l'avant de l'écran (votre position actuelle) :

  5  0 1 2 3 4 <=> 5  1 2 3 4 0 <=> 5  2 3 4 0 1 <=> 5  3 4 0 1 2 <=> 5  4 0 1 2 3
  
  5  0 1 3 2 4
  
  5  0 2 1 3 4
  
  5  0 1 3 4 2

  En inversant les indices, elles seront visibles depuis l'arrière de l'écran :

  5  4 3 2 1 0 <=> 5  3 2 1 0 4 <=> ...
  5  4 2 3 1 0
  5  4 3 1 2 0
  5  2 4 3 1 0

  Si certains segments se croisent, l'affichage sera généralement bugué :

  5  0 3 1 2 4
  5  0 2 4 1 3
  4  0 1 2 3
  5  3 4 1 2 0

#### Fichiers textures
