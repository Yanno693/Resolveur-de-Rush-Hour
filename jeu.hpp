#ifndef JEU_HPP
#define JEU_HPP

#include "enum.hpp"
#include "grille.hpp"
#include "voiture.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class Jeu
{
    public:
        Grille grille; // Un grille avec dans les cases un pointeur sur la voiture qui l'occupe
        std::vector<Voiture> voitures; // Un liste des voitures dans le jeu
        Voiture joueur; // Une instance de la classe "Voiture" representant la voiture que le joueur doit sortir de la grille pour remporter la partie (souvent representee en rouge dans les versions de RushHour/UnblockMe comprenant une interface grahique), si possible en bougeant le moins de vehicules possible

        Jeu(); // Constructeur a partir de RIEEEEEEEEEN
        Jeu(std::vector<Voiture> list_voiture); // Constructeur a partir d'une liste de voitures
        Jeu(std::string src); // Constructeur a partir d'un fichier texte (comme celui du sujet)

        void majEtatVoiture(Voiture* b); // Met a jour la position d'une voitures dans la grille
        void majEtat(); // Met a jour la position des voitures et du joueur dans la grille (à appeler apres un deplacement)
        void ajouterVoiture(Voiture b); // Ajoute une voiture dans le grille du jeu
        void afficher(); // Afficher la grille du jeu
        bool peutDeplacerVoiture(Voiture& b, Direction direction); // Renvoit vrai si la voiture peut etre deplacer une fois dans une direction donnée
        bool peutDeplacerVoiture(Voiture& b, Direction direction, int iteration); // Renvoit vrai si la voiture peut etre deplacer "iteration" fois dans une direction donnee
        void deplacerVoiture(Voiture& b, Direction direction); // Deplace une voiture dans une direction donnee
        Voiture* getVoitureParId(char id); // Permet de trouver une voiture par son id, pratique pour bouger une meme voiture dans des instances de jeu différentes, renvoi un pointeur nul si la voiture n'est pas trouvee
        void deplacerJoueur(); // Euh ...
        void ajouterEnnemiAleatoire(); // Ajoute une voiture a une position aléatoire dans la grille
        void InitialisationPartie(); // Initialise une partie en ajoutant des voitures dans la grille
};

#endif
