#include "jeu.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Jeu::Jeu()
{
    grille = Grille();
    joueur = Voiture(2,0,2,HORIZONTAL, 'x');
    majEtat();
}

Jeu::Jeu(std::vector<Voiture> list_voiture)
{
    grille = Grille();
    joueur = Voiture(2,0,2,HORIZONTAL, 'x');
    voitures = list_voiture;
    majEtat();
}

Jeu::Jeu(std::string src)
{
    std::ifstream fichier;
    fichier.open(src.c_str());
    if(!fichier.is_open())
    {
        std::cout << "ERREUR : IMPOSSIBLE D'OUVRIR LE FICHIER " << src << std::endl;
        exit(1);
    }

    // Les deux premier nombre representent la position de la sortie, on s'en fout vu que la sortie est toujours au même endroit :)
    std::stringstream buffer;
    std::string tmp;
    std::getline(fichier,tmp);

    char id = 'A' - 1;

    int valeur_fichier[4];

    int taille;
    int posx;
    int posy;
    Orientation orientation;

    // Maintenant on ajoute chaque voiture a la liste
    while(!fichier.eof())
    {
        std::getline(fichier,tmp);  // On recupere la ligne dans le fichier
        buffer.clear(); // On vide le buffer
        buffer << tmp; // On met la ligne recupere dans le buffer

        // On recupere les valeur du buffer
        buffer >> valeur_fichier[0] >> valeur_fichier[1] >> valeur_fichier[2] >> valeur_fichier[3];

        taille = valeur_fichier[2];
        posx = valeur_fichier[1];
        posy = valeur_fichier[0];
        orientation = (valeur_fichier[3] == 1 ? HORIZONTAL : VERTICAL);

        // On teste si les valeur sont correctes

        if(!fichier.eof())
        {
           if(taille >= 2 && taille <= 3 && posx >= 0 && posx < 6 && posy >= 0 && posy < 6)
            {
                if(id == 'A' - 1) // La premier voiture est le joueur, dans les faits les deux 1ere ligne du fichier doivent toujours etre les mêmes
                {
                    joueur = Voiture(taille,posx,posy,orientation,'x');
                }
                else
                {
                    Voiture v(taille,posx,posy,orientation,id);
                    voitures.push_back(v);
                }
            }
            else
            {
                std::cout << "ERREUR : DEFINITION INCORRECTE DANS LE FICHIER " << src << std::endl;
                exit(1);
            } 
        }

        id++;
    }

    majEtat(); // Enfin on met a jour la grille par rapport a la position des voitures
}

void Jeu::majEtatVoiture(Voiture* b)
{
    grille.etatGrille[b->getPosX()][b->getPosY()].setOccupe(b);

    if(b->getOrientation() == HORIZONTAL)
    {
        for(int i = 0; i < b->getTaille(); i++)
        {
            if(b->getPosX() + i >= 0 && b->getPosX() + i < 6)
                grille.etatGrille[b->getPosX() + i][b->getPosY()].setOccupe(b);
            else
            {
                std::cout << "ERREUR : VOITURE " << b->getId() << " HORS DU TABLEAU" << std::endl;
                exit(1);
            }
        }
    }
    else
    {
        for(int i = 0; i < b->getTaille(); i++)
        {
            if(b->getPosY() + i >= 0 && b->getPosY() + i < 6)
                grille.etatGrille[b->getPosX()][b->getPosY() + i].setOccupe(b);
            else
            {
                std::cout << "ERREUR : VOITURE " << b->getId() << " HORS DU TABLEAU" << std::endl;
                exit(1);
            }
        }
    }
}

Voiture* Jeu::getVoitureParId(char id)
{
    if(id == 'x')
        return &joueur;
    
    int i = 0;
    Voiture* res = nullptr;
    while(i < voitures.size() || res == nullptr)
    {
        if(voitures[i].getId() == id)
            res = &voitures[i];
        i++;
    }

    return res;
}

void Jeu::majEtat()
{
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 6; j++)
            grille.etatGrille[i][j].setOccupe(nullptr);

    majEtatVoiture(&joueur);

    for(unsigned int i = 0; i < voitures.size(); i++)
        majEtatVoiture(&voitures[i]);
}

void Jeu::ajouterVoiture(Voiture b)
{
    voitures.push_back(b);
    majEtat();
}

void Jeu::afficher()
{
    std::cout << grille.encode() << std::endl;
}

bool Jeu::peutDeplacerVoiture(Voiture& b, Direction direction)
{
    return peutDeplacerVoiture(b,direction,1);
}

bool Jeu::peutDeplacerVoiture(Voiture& b, Direction direction, int iteration)
{
    if(iteration < 1)
        return false;

    int offset;
    for(int i = 0; i < iteration; i++)
    {
        if(b.getOrientation() == HORIZONTAL)
        {
            if(direction == GAUCHE)
            {
                offset = b.getPosX() - 1 - i;
                if(offset >= 0 && offset < 6)
                {
                    if(grille.etatGrille[offset][b.getPosY()].estOccupe())
                        return false;
                }
                else
                    return false;

            }
            else if(direction == DROITE)
            {
                offset = b.getPosX() + b.getTaille() + i;
                if(offset >= 0 && offset < 6)
                {
                    if(grille.etatGrille[offset][b.getPosY()].estOccupe())
                        return false;
                }
                else
                    return false;
            }
            else if(direction == HAUT || direction == BAS)
            {
                return false;
            }
        }
        else
        {
            if(direction == HAUT)
            {
                offset = b.getPosY() - 1 - i;
                if(offset >= 0 && offset < 6)
                {
                    if(grille.etatGrille[b.getPosX()][offset].estOccupe())
                        return false;
                }
                else
                    return false;

            }
            else if(direction == BAS)
            {
                offset = b.getPosY() + b.getTaille() + i;
                if(offset >= 0 && offset < 6)
                {
                    if(grille.etatGrille[b.getPosX()][offset].estOccupe())
                        return false;
                }
                else
                    return false;
            }
            else if(direction == GAUCHE || direction == DROITE)
                return false;
        }
    }

    return true;
}

void Jeu::deplacerVoiture(Voiture& b, Direction direction)
{
    if(peutDeplacerVoiture(b,direction))
    {
        if(b.getOrientation() == HORIZONTAL)
        {
            if(direction == GAUCHE)
                b.setPosX(b.getPosX() - 1);
            else if(direction == DROITE)
                b.setPosX(b.getPosX() + 1);
            else
            {
                std::cout << "ERREUR : DEPLACEMENT VERTICAL IMPOSSIBLE" << std::endl;
                exit(1);
            }

        }
        else
        {
            if(direction == HAUT)
                b.setPosY(b.getPosY() - 1);
            else if(direction == BAS)
                b.setPosY(b.getPosY() + 1);
            else
            {
                std::cout << "ERREUR : DEPLACEMENT HORIZONTAL IMPOSSIBLE" << std::endl;
                exit(1);
            }
        }
    }
    else
    {
        std::cout << "ERREUR : DEPLACEMENT VERS " << direction << " IMPOSSIBLE" << std::endl;
        exit(1);
    }

    majEtat();
}

void Jeu::deplacerJoueur()
{
    if(!peutDeplacerVoiture(joueur,DROITE))
    {
        std::cout << "ERREUR : DEPLACEMENT HORIZONTAL IMPOSSIBLE" <<std::endl;
        exit(1);
    }
    else
        while(peutDeplacerVoiture(joueur,DROITE))
            joueur.setPosX(joueur.getPosX() + 1);
    
    majEtat();
}
void Jeu::ajouterEnnemiAleatoire()
{
    int x,y;
    bool valide;

    do
    {
        valide = true;
        x = rand()%5;
        y = rand() % 5;
        for(int i = 0; i < 2; i++)
            valide = valide & !grille.etatGrille[x][y+i].estOccupe();

    }while(!valide);
    
    Voiture ennemi;
    if(voitures.size() == 0)
    {
        ennemi = Voiture(2,x,y,VERTICAL,'A');
        voitures.push_back(ennemi);
    }
    else
    {
        ennemi = Voiture(2,x,y,VERTICAL,voitures[voitures.size()-1].getId()+1);
        voitures.push_back(ennemi);
    }

    majEtat();
}

void Jeu::InitialisationPartie()
{
    for(int i = 0; i<4;i++)
        ajouterEnnemiAleatoire();
}
