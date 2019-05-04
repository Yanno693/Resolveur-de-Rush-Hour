all: rushhour rushhour_graphique

rushhour_graphique: rushhour_graphique.cpp voiture.o case.o grille.o jeu.o grabator.o mouvement.o enum.hpp olcPixelGameEngine.h
	g++ -g -Wall -std=c++11 rushhour_graphique.cpp voiture.o case.o grille.o jeu.o grabator.o mouvement.o -o rushhour_graphique -pthread -lpthread -lGL -lpng -lX11

rushhour: rushhour.cpp voiture.o case.o grille.o jeu.o grabator.o mouvement.o enum.hpp
	g++ -g -Wall -std=c++11 rushhour.cpp voiture.o case.o grille.o jeu.o grabator.o mouvement.o -o rushhour

grabator.o: grabator.cpp grabator.hpp mouvement.o jeu.o enum.hpp
	g++ -g -Wall -c grabator.cpp -o grabator.o

mouvement.o: mouvement.cpp mouvement.hpp enum.hpp
	g++ -g -Wall -c mouvement.cpp -o mouvement.o

jeu.o: jeu.cpp jeu.hpp voiture.o grille.o enum.hpp
	g++ -g -c jeu.cpp -o jeu.o

grille.o: grille.cpp grille.hpp case.o
	g++ -g -c grille.cpp -o grille.o

case.o: case.cpp case.hpp voiture.o
	g++ -g -c case.cpp -o case.o

voiture.o: voiture.cpp voiture.hpp enum.hpp
	g++ -g -c voiture.cpp -o voiture.o

clean:
	rm *.o
