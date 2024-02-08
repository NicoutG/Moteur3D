DIRCPP=src/
DIRH=include/
DIRO=obj/
OBJ=$(DIRO)Couleur.o $(DIRO)Image.o $(DIRO)Matrice.o $(DIRO)Cosin.o $(DIRO)Camera.o $(DIRO)Surface.o $(DIRO)Modele.o $(DIRO)Moteur3D.o $(DIRO)Affichage_SDL.o $(DIRO)Editeur3D.o  
CC=g++
FLAG= -Wall
FLAGS= -c -Wall
NOM=./bin/moteur3D
NOM2=./bin/editeur3D

MAIN=main
MAIN2=main_editeur3D
SDLPATH=./lib/SDL2/lib/
LIBS=-L $(SDLPATH) -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image

all: $(NOM) $(NOM2)

$(NOM): $(DIRO)$(MAIN).o $(OBJ)
	$(CC) $(FLAG) $(DIRO)$(MAIN).o $(OBJ) -o $(NOM) $(LIBS)
	
$(NOM2): $(DIRO)$(MAIN2).o $(OBJ)
	$(CC) $(FLAG) $(DIRO)$(MAIN2).o $(OBJ) -o $(NOM2) $(LIBS)
	
$(DIRO)$(MAIN).o: $(DIRCPP)$(MAIN).cpp $(DIRH)Moteur3D.h $(DIRH)Affichage_SDL.h 
	$(CC) $(FLAGS) $(DIRCPP)$(MAIN).cpp -o $(DIRO)$(MAIN).o
	
$(DIRO)$(MAIN2).o: $(DIRCPP)$(MAIN2).cpp $(DIRH)Editeur3D.h
	$(CC) $(FLAGS) $(DIRCPP)$(MAIN2).cpp -o $(DIRO)$(MAIN2).o
	
$(DIRO)Couleur.o: $(DIRCPP)Couleur.cpp $(DIRH)Couleur.h
	$(CC) $(FLAGS) $(DIRCPP)Couleur.cpp -o $(DIRO)Couleur.o
	
$(DIRO)Image.o: $(DIRCPP)Image.cpp $(DIRH)Image.h $(DIRH)Couleur.h
	$(CC) $(FLAGS) $(DIRCPP)Image.cpp -o $(DIRO)Image.o
	
$(DIRO)Matrice.o: $(DIRCPP)Matrice.cpp $(DIRH)Matrice.h
	$(CC) $(FLAGS) $(DIRCPP)Matrice.cpp -o $(DIRO)Matrice.o
	
$(DIRO)Cosin.o: $(DIRCPP)Cosin.cpp $(DIRH)Cosin.h
	$(CC) $(FLAGS) $(DIRCPP)Cosin.cpp -o $(DIRO)Cosin.o
	
$(DIRO)Camera.o: $(DIRCPP)Camera.cpp $(DIRH)Camera.h $(DIRH)Cosin.h $(DIRH)Matrice.h
	$(CC) $(FLAGS) $(DIRCPP)Camera.cpp -o $(DIRO)Camera.o
	
$(DIRO)Surface.o: $(DIRCPP)Surface.cpp $(DIRH)Surface.h $(DIRH)Couleur.h $(DIRH)Matrice.h $(DIRH)Image.h
	$(CC) $(FLAGS) $(DIRCPP)Surface.cpp -o $(DIRO)Surface.o
	
$(DIRO)Modele.o: $(DIRCPP)Modele.cpp $(DIRH)Modele.h $(DIRH)Surface.h $(DIRH)Matrice.h $(DIRH)Camera.h
	$(CC) $(FLAGS) $(DIRCPP)Modele.cpp -o $(DIRO)Modele.o
	
$(DIRO)Moteur3D.o: $(DIRCPP)Moteur3D.cpp $(DIRH)Moteur3D.h $(DIRH)Modele.h $(DIRH)Camera.h $(DIRH)Image.h
	$(CC) $(FLAGS) $(DIRCPP)Moteur3D.cpp -o $(DIRO)Moteur3D.o
	
$(DIRO)Affichage_SDL.o: $(DIRCPP)Affichage_SDL.cpp $(DIRH)Affichage_SDL.h $(DIRH)Image.h
	$(CC) $(FLAGS) $(DIRCPP)Affichage_SDL.cpp -o $(DIRO)Affichage_SDL.o
	
$(DIRO)Editeur3D.o: $(DIRCPP)Editeur3D.cpp $(DIRH)Editeur3D.h $(DIRH)Camera.h $(DIRH)Moteur3D.h $(DIRH)Affichage_SDL.h
	$(CC) $(FLAGS) $(DIRCPP)Editeur3D.cpp -o $(DIRO)Editeur3D.o



clean:
	rm $(DIRO)*.o
	rm -r bin/*


veryclean: clean
	rm *.out
