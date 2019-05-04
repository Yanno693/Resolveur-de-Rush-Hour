#ifndef MOUVEMENT_HPP
#define MOUVEMENT_HPP

#include "enum.hpp"

class Mouvement // Cette classe est en gros un couple (id,mouvement)
{
    public:
        char id; // L'identifiant de la voiture qui effectue le mouvement
        Direction direction; // La direction dans la quelle la voiture fait le mouvement

        Mouvement();
        Mouvement(char _id, Direction _direction);
        bool operator==(Mouvement m); // Renvoie true si les id et les directions sont les memes
        char getId(); // Recuperer l'ID de la voiture qui effectue le mouvement
        Direction getDirection(); // Recuperer la direction de la voiture qui effectue la mouvement
};

#endif