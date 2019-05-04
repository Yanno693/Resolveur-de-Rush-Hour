#ifndef GRABATOR_HPP
#define GRABATOR_HPP

/* Quand on y pense on essaye de creer un algo pour resoudre notre Rush Hour
C'est un peu comme creer une sorte d'intelligence artificielle (même si ca reste juste un algo qui resout un Rush Hour comme un bourrin)
les IA ont toujours eu des noms un peu etranges, du coup celui ci va s'appeler Grabator (ce nom est suseptible de changer ulterieurement) */

#include "jeu.hpp"
#include "mouvement.hpp"
#include <iostream>
#include <utility>
#include <queue> 
#include <map>

class Grabator // Une classe qui va essayer de resoudre un Jeu donné
{
    public:
		Grabator();
		std::vector<std::queue<Mouvement>> solutions; // Les solutions trouves pour un jeu

		static int nombreMouvement(std::queue<Mouvement> file_mouvement); // Calcule le nombre de mouvment de jeu (différents du nombre d'appel à la fonction deplacer() ) d'une sequence de mouvement
        
		void afficherSolutions(); // Afficher les solution trouvées par Grabator
        void resoudre_recursive(Jeu j); // Cherche un maximum de solution dans un jeu donnée de facon recursive
		void resoudre_largeur(Jeu j); // Cherche le moyen le plus court de finir le jeu avec un minimum de mouvement, en parcourant les solutions en largeur

        Jeu genererGrille(int n); // Génère une grille qui se résout en 'n' coups a partir de la grille donnée dans le sujet

	private:
        
		bool deplacerJoueur_recursive(Jeu j); // Teste si le joueur peut se deplacer jusqu'a la ligne d'arrivée

        /* Degager_recursive: Permet de deplacer une voiture qui en gene une autre
            parents : Une liste de couples de (gené,geneur) qui se remplit au fur et a mesure que des voitures demande a d'autres voiture de se deplacer
            j : une copie du jeu (permet de simuler le comportement de certain deplacement sans changer la grille initial)
            gener : L'identifiant de la voiture qui est genée
            geneur : L'identifiant de la voiture qui gene
            direction : La direction vers laquelle la voiture qui gene doit se deplacer
            xCase : la coordonnée en x de la case que le geneur doit degager
            yCase : la coordonnée en y de la case que le geneur doit degager

            Valeur de retour : un couple de deux liste :
            - Une liste des enchainement de mouvement possible pour degager la case demandée
            - Une liste de liste des couples (gené,geneur) qui a ete demander pour degager la case demandée
            
            Propriétés : 
            - Normalement, les deux listes de retour ont la même taille
            - Si il est impossible de degager une case, on renvois des listes vides
        */
        std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > degager_recursive(std::vector<std::pair<char,char>> parents, Jeu j, char gener, char geneur, Direction direction, int xCase, int yCase);
        
        std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > degager_haut_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase);
        std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > degager_bas_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase);
        std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > degager_gauche_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase);
        std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > degager_droite_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase);
};

#endif
