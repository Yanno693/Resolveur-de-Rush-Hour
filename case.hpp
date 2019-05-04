#ifndef CASE_HPP
#define CASE_HPP

#include "voiture.hpp"

class Case // Une classe case pour representer les differentes cases de la grille du jeu
{
    private:
        Voiture* occupe; // Un pointeur sur la voiture qui occupe la case

    public:
        Case();
        Case(Voiture* v);

        bool estOccupe(); // Renvoie vrai si une voiture occupe actuellement la case

        Voiture* getOccupe(); // Renvoie un pointeur vers la voiture qui occupe la case (si la case est vide, renvoie un pointeur nul)

        void setOccupe(Voiture* v); // Permet de definir la voiture qui occupe la case
};

#endif