#include "mouvement.hpp"

Mouvement::Mouvement()
{
    id = '-';
    direction = HAUT;
}

Mouvement::Mouvement(char _id, Direction _direction)
{
    id = _id;
    direction = _direction;
}

bool Mouvement::operator==(Mouvement m)
{
    return (id == m.id && direction == m.direction);
}

char Mouvement::getId()
{
    return id;
}

Direction Mouvement::getDirection()
{
    return direction;
}