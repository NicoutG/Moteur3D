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


## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Support
Tell people where they can go to for help. It can be any combination of an issue tracker, a chat room, an email address, etc.

## Roadmap
If you have ideas for releases in the future, it is a good idea to list them in the README.

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
For open source projects, say how it is licensed.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.
