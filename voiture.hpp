#ifndef VOITURE_HPP
#define VOITURE_HPP

#include "enum.hpp"

class Voiture // Un voiture pouvant etre le joueur ou une autre voiture sur la grille
{

    private:
        int taille, posX, posY; // Respectivement la taille de la voiture, sa position en X et sa position en Y
        // /!\ Les positions X et Y representent la case soit tout en haut, soit tout a gauche de la voiture suivant son orientation. La voiture est libre d'occuper d'autres cases suivant sa taille

        char id; // L'identifiant de la voiture
        Orientation orientation; // L'orientation horizontale ou verticale de la voiture

    public:
        Voiture();
        Voiture(int _taille, int _posX, int _posY, Orientation _orientation, char _id);
        Voiture(const Voiture& v);
        int getPosX() const;
        int getPosY() const;
        void setPosX(int _posX);
        void setPosY(int _posY);
        int getTaille() const;
        Orientation getOrientation() const;
        char getId() const;
};

#endif