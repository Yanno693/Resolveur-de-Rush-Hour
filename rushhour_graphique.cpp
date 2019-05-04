#include "grabator.hpp"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <algorithm>
#include <string> 

static int min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

const std::string S_PAUSE = "p : Demmarer/Mettre le jeu en pause";
const std::string S_ORIENTATION = "O : Changer l'orientation";
const std::string S_TAILLE = "T : Changer la taille";
const std::string S_MODIFICATION = "Clique Gauche/Droit : Ajouter/Supprimer une voiture";
const std::string S_SOLUTION1 = "Espace : Calculer les solutions (Largeur)";
const std::string S_SOLUTION2 = "Shift : Calculer les solutions (Recursive)";
const std::string S_RECHARGER = "R : Recharger la situation initiale";

class GameEngine : public olc::PixelGameEngine
{
    private:
		Jeu copie; // Est sens� etre const/final mais j'sais plus trop comment on fait ca dans une classe
        Jeu j;
		std::vector< std::queue<Mouvement> > solutions;
        std::queue<Mouvement> file_mouvement;
        float delta;

		Orientation creation_orientation;
		int creation_taille;

		bool pause; // Met le jeu en pause ou pas, et demmare le jeu au debut
		bool creation; // Indique si on est en train de creer une nouvelle grille ou pas
		bool calcul; // Indique si on est en train de calculer les solutions
		int selection_solution;

		int taille_fenetre;
		int nb_mouvement;
    
        void afficherGrille()
        {
            for(int i = 0; i < 7; i++)
            {
                DrawLine( ( taille_fenetre / 6) * i, 0, (taille_fenetre / 6) * i, taille_fenetre, olc::BLACK );
                DrawLine( 0, (taille_fenetre / 6) * i , taille_fenetre, (taille_fenetre / 6) * i, olc::BLACK );
            }
        }

		void afficherVoiture(char _id) // Afficher une voiture par identifiant
		{
			olc::Pixel couleur;
			if (_id == j.joueur.getId())
				couleur = olc::RED;
			else
				couleur = olc::BLUE;

			afficherVoiture(j.getVoitureParId(_id)->getPosX(), j.getVoitureParId(_id)->getPosY(), j.getVoitureParId(_id)->getTaille(), j.getVoitureParId(_id)->getOrientation(), couleur);
		}

		void afficherVoiture(int posX, int posY, int taille, Orientation orientation, olc::Pixel couleur) // Afficher une voiture par ses parametre
		{
			if (orientation == VERTICAL)
				FillRect(
					posX * (taille_fenetre / 6) + 10,
					posY * (taille_fenetre / 6) + 10,
					(taille_fenetre / 6) - 20,
					(taille_fenetre / 6) * taille - 20,
					couleur
				);
			else
				FillRect(
					posX * (taille_fenetre / 6) + 10,
					posY * (taille_fenetre / 6) + 10,
					(taille_fenetre / 6) * taille - 20,
					(taille_fenetre / 6) - 20,
					couleur
				);
		}

        void afficherVoitures()
        {
            for(int i = -1; i < (int)j.voitures.size(); i++)
                if(i == -1)
                    afficherVoiture(j.joueur.getId());
                else
                    afficherVoiture(j.voitures[i].getId());
        }

		bool peutPlacerVoiture(int posX, int posY, int taille, Orientation orientation)
		{
			bool accu = true;
			
			if (orientation == VERTICAL) // Premiere verification pour voir qu'on est pas en dehors 
			{
				if (posY + taille - 1 > 5)
					return false;

				for (int i = posY; i < posY + taille; i++)
					accu = accu && !j.grille.etatGrille[posX][i].estOccupe();
			}
			else
			{
				if (posX + taille - 1 > 5)
					return false;

				for (int i = posX; i < posX + taille; i++)
					accu = accu && !j.grille.etatGrille[i][posY].estOccupe();
			}

			return accu;
		}

		void afficherHUD()
		{
			afficherGrille();
			afficherVoitures();
			
			DrawString(10, taille_fenetre + 10, S_PAUSE, olc::BLACK, 1);
			DrawString(10, taille_fenetre + 30, S_ORIENTATION, olc::BLACK, 1);
			DrawString(10, taille_fenetre + 50, S_TAILLE, olc::BLACK, 1);
			DrawString(10, taille_fenetre + 70, S_MODIFICATION, olc::BLACK, 1);
			DrawString(10, taille_fenetre + 90, S_SOLUTION1, olc::BLACK, 1);
			DrawString(10, taille_fenetre + 110, S_SOLUTION2, olc::BLACK, 1);
			DrawString(10, taille_fenetre + 130, S_RECHARGER, olc::BLACK, 1);

			if (solutions.size() > 0)
			{
				std::stringstream ss_solutions;
				ss_solutions << "Solutions trouvees : " << solutions.size();
				DrawString(10, ScreenHeight() - 30, ss_solutions.str(), olc::BLACK, 1);

				std::stringstream ss_selection;
				ss_selection << "Solution selectionnees : <" << (selection_solution+1) << ">";
				DrawString(10, ScreenHeight() - 10, ss_selection.str(), olc::BLACK, 1);

				std::stringstream ss_nb_mouvement;
				ss_nb_mouvement << "Deplacement : " << nb_mouvement;
				DrawString(ScreenWidth() / 2 + 20, ScreenHeight() - 10, ss_nb_mouvement.str(), olc::BLUE, 1);
			}

			if (IsFocused() && creation && GetMouseX() < taille_fenetre && GetMouseX() >= 0 && GetMouseY() < taille_fenetre && GetMouseY() >= 0)
				if (peutPlacerVoiture(GetMouseX() * 6 / taille_fenetre, GetMouseY() * 6 / taille_fenetre, creation_taille, creation_orientation))
					afficherVoiture(
						GetMouseX() * 6 / taille_fenetre,
						GetMouseY() * 6 / taille_fenetre,
						creation_taille,
						creation_orientation,
						olc::GREEN
					);
				else
				{
					Voiture* occupe = j.grille.etatGrille[GetMouseX() * 6 / taille_fenetre][GetMouseY() * 6 / taille_fenetre].getOccupe();

					if(occupe != nullptr)
						if(occupe->getId() != j.joueur.getId())
							afficherVoiture(
								occupe->getPosX(),
								occupe->getPosY(),
								occupe->getTaille(),
								occupe->getOrientation(),
								olc::YELLOW
							);
				}

			if(solutions.size() > 0)

			if (calcul)
				DrawString(10, 20, "Calcul des solutions \nen cours ...", olc::MAGENTA, 2);
		}

    public : 
		
		GameEngine()
		{
			sAppName = "Rush Hour";
			copie = Jeu();
			j = Jeu();
        }

		GameEngine(char* src)
		{
			sAppName = "LIFAP6 : Rush Hour";
			copie = Jeu(src);
			j = copie;
		}

        bool OnUserCreate() override // Creation de la fenetre
        {
			pause = true;
			creation = true;
			calcul = false;
			creation_orientation = HORIZONTAL;
			creation_taille = 2;
			selection_solution = -1;
			nb_mouvement = 0;

            delta = 0;

			if (ScreenWidth() < ScreenHeight())
				taille_fenetre = ScreenWidth();
			else
				taille_fenetre = ScreenHeight();

            return true;
        }

        bool OnUserUpdate(float elapsedTime) override
        {
            FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::WHITE);
			delta += elapsedTime;

			if (!pause)
				if(delta > 0.5)
				{
					if(file_mouvement.size() > 0)
					{
						Mouvement m = file_mouvement.front();
						j.deplacerVoiture( *j.getVoitureParId(m.id), m.direction );
                    
						file_mouvement.pop();
					}
                
					delta = 0.0;
				}

			if (IsFocused() && !calcul) // Partie souris et clavier
			{
				if (GetKey(olc::P).bPressed)
					if (solutions.size() > 0)
					{
						pause = !pause;

						if (creation)
						{
							file_mouvement = solutions[selection_solution];

							std::cout << Grabator::nombreMouvement(file_mouvement) << std::endl;
							creation = false;
						}
					}

				if (GetKey(olc::R).bPressed) // Recharger la situation du debut
				{
					j = copie;
					pause = true;
					creation = true;
					solutions.clear();
					while (!file_mouvement.empty())
						file_mouvement.pop();
				}

				if (pause && creation) // Mode creation, avant le premier lancement
				{
					if (GetKey(olc::O).bPressed) // Changer l'Orientation
						if (creation_orientation == HORIZONTAL)
							creation_orientation = VERTICAL;
						else
							creation_orientation = HORIZONTAL;

					if (GetKey(olc::T).bPressed) // Changer la taille de la voiture
						if (creation_taille == 2)
							creation_taille = 3;
						else
							creation_taille = 2;

					if (GetKey(olc::LEFT).bPressed)
						if (solutions.size() > 0)
						{
							selection_solution = (selection_solution - 1 + (int)solutions.size()) % solutions.size();
							nb_mouvement = Grabator::nombreMouvement(solutions[selection_solution]);
						}

					if (GetKey(olc::RIGHT).bPressed)
						if (solutions.size() > 0)
						{
							selection_solution = (selection_solution + 1 + (int)solutions.size() ) % solutions.size();
							nb_mouvement = Grabator::nombreMouvement(solutions[selection_solution]);
						}

					if (GetKey(olc::SPACE).bPressed) // Calcul des solutions dans la grille
					{
						std::cout << "Largeur : Calcul des solutions en cours ... (peut prendre quelques secondes)" << std::endl;

						std::thread t([](Jeu* _j, std::vector<std::queue<Mouvement>>* _solutions, bool* _calcul) 
						{
								*_calcul = true;

								Grabator g;
								g.resoudre_largeur(*_j);
								*_solutions = g.solutions;

								std::cout << (*_solutions).size() << " SOLUTIONS TROUVEES" << std::endl;

								*_calcul = false;

						}, &j, &solutions, &calcul);
						
						t.detach();
						selection_solution = 0;
					}

					if (GetKey(olc::SHIFT).bPressed) // Calcul des solutions dans la grille
					{
						std::cout << "Recursive : Calcul des solutions en cours ... (peut prendre quelques secondes)" << std::endl;

						std::thread t([](Jeu * _j, std::vector<std::queue<Mouvement>> * _solutions, bool* _calcul)
							{
								*_calcul = true;

								Grabator g;
								g.resoudre_recursive(*_j);
								*_solutions = g.solutions;

								std::cout << (*_solutions).size() << " SOLUTIONS TROUVEES" << std::endl;

								*_calcul = false;

							}, &j, &solutions, &calcul);

						t.detach();
						selection_solution = 0;
					}

					if (GetMouse(0).bPressed) // Creation de voiture
					{
						solutions.clear();
						if (peutPlacerVoiture(GetMouseX() * 6 / taille_fenetre, GetMouseY() * 6 / taille_fenetre, creation_taille, creation_orientation) && j.voitures.size() < 16)
						{
							char id_creation;

							if (j.voitures.size() > 0)
								id_creation = j.voitures[j.voitures.size() - 1].getId() + 1;
							else
								id_creation = 'A';
							
							Voiture v = Voiture(creation_taille, GetMouseX() * 6 / taille_fenetre, GetMouseY() * 6 / taille_fenetre, creation_orientation, id_creation);

							j.voitures.push_back(v);
							//j.ajouterVoiture(v); Ne marche pas ?
							j.majEtat();
						}
					}

					if (GetMouse(1).bPressed) // Suppresion de voiture
					{
						solutions.clear();
						if (IsFocused() && creation && GetMouseX() < taille_fenetre && GetMouseX() >= 0 && GetMouseY() < taille_fenetre && GetMouseY() >= 0)
						{
							Voiture* occupe = j.grille.etatGrille[GetMouseX() * 6 / taille_fenetre][GetMouseY() * 6 / taille_fenetre].getOccupe();

							if (occupe != nullptr)
								if (occupe->getId() != j.joueur.getId())
								{
									std::vector<Voiture>::iterator it = j.voitures.begin();
									while (it->getId() != occupe->getId())
										it++;

									if (it != j.voitures.end())
										j.voitures.erase(it);

									j.majEtat();
								}
						}
					}
				}
			}

			afficherHUD();

            return true;
        }
};

int main(int argc, char** argv) // On peut passer un chemin de fichier en argument
{

	GameEngine ge;

	if (argc > 1)
		ge = GameEngine(argv[1]);

	if (ge.Construct(500, 800, 1, 1))
		ge.Start();
	else
        std::cout << "Erreur lors de la creation de la fenetre" << std::endl;

    return 0;
}
