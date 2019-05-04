#include "case.hpp"

Case::Case()
{
    occupe = nullptr;
}

Case::Case(Voiture* v)
{
    occupe = v;
}

bool Case::estOccupe()
{
    return occupe != nullptr;
}

Voiture* Case::getOccupe()
{
    return occupe;
}

void Case::setOccupe(Voiture* v)
{
    occupe = v;
}