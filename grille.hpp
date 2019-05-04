#ifndef GRILLE_HPP
#define GRILLE_HPP

#include "case.hpp"

class Grille // Une grille qui contenant les differentes cases du jeu
{
    public:
        Case etatGrille[6][6]; // Un tableau 2d de Case

        Grille();
        std::string encode(); // Renvoie un string facilitant l'affichage de la grille
};

#endif
