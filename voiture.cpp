#include "voiture.hpp"

Voiture::Voiture()
{
    taille = 2;
    posX = 0;
    posY = 0;
    orientation = HORIZONTAL;
    id = 'A';
}

Voiture::Voiture(int _taille, int _posX, int _posY, Orientation _orientation, char _id)
{
    taille = _taille;
    posX = _posX;
    posY = _posY;
    orientation = _orientation;
    id = _id;
}

Voiture::Voiture(const Voiture& v)
{
    taille = v.getTaille();
    posX = v.getPosX();
    posY = v.getPosY();
    orientation = v.getOrientation();
    id = v.getId();
}

int Voiture::getPosX() const
{
    return posX;
}

int Voiture::getPosY() const 
{
    return posY;
}

void Voiture::setPosX(int _posX)
{
    posX = _posX;
}

void Voiture::setPosY(int _posY)
{
    posY = _posY;
}

int Voiture::getTaille() const
{
    return taille;
}

Orientation Voiture::getOrientation() const
{
    return orientation;
}

char Voiture::getId() const
{
    return id;
}