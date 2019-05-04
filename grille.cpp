#include "grille.hpp"

std::string Grille::encode()
{   
    std::string s = "";

    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 6; j++)
            (etatGrille[j][i].getOccupe() != nullptr ? s+= (etatGrille[j][i].getOccupe()->getId()) : s += '-');
        s+='\n';
    }

    return s;
}

Grille::Grille()
{
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 6; j++)
            etatGrille[i][j] = Case(nullptr);
}