#ifndef ENUM_HPP
#define ENUM_HPP

#include <iostream>

enum Direction : char // Une direction dans laquelle une voiture peut se déplacer
{
    HAUT,BAS,GAUCHE,DROITE
};

static std::ostream& operator<<(std::ostream& flux, Direction d)
{
    switch (d)
    {
        case HAUT: flux << "HAUT"; break;
        case BAS: flux << "BAS"; break;
        case GAUCHE: flux << "GAUCHE"; break;
        case DROITE: flux << "DROITE"; break;
        default: break;
    }
    return flux;
}

enum Orientation : char // Une orientation pour les voitures
{
    HORIZONTAL, VERTICAL
};

static std::ostream& operator<<(std::ostream& flux, Orientation o)
{
    switch (o)
    {
        case HORIZONTAL: flux << "HORIZONTAL"; break;
        case VERTICAL: flux << "VERTICAL"; break;
        default: break;
    }
    return flux;
}

#endif