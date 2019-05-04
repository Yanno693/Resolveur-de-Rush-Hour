#include "grabator.hpp"

Grabator::Grabator(){}

int Grabator::nombreMouvement(std::queue<Mouvement> file_mouvement)
{
	char c = 0;
	int accu = 0;

	while (!file_mouvement.empty())
	{
		Mouvement m = file_mouvement.front();
		char tmp = m.id;

		if (c != tmp)
			accu++;

		c = tmp;

		file_mouvement.pop();
	}
	
	return accu;
}

void Grabator::resoudre_recursive(Jeu j)
{
    std::vector<std::pair<char,char>> parents;
    bool resolu = false;

    if(deplacerJoueur_recursive(j))
    {
        std::queue<Mouvement> res;
        
        resolu = true;
        for(int i = 0; i < 6 - j.joueur.getPosX() - j.joueur.getTaille(); i++)
        {
            res.push(Mouvement(j.joueur.getId(), DROITE));
        }

        solutions.push_back(res);
    }
    else
    {
        Voiture* geneur = nullptr;

        int offset = j.joueur.getTaille() + j.joueur.getPosX();

        while(offset < 6 && geneur == nullptr)
        {
            geneur = j.grille.etatGrille[offset][j.joueur.getPosY()].getOccupe();
            if(geneur == nullptr)
                offset++;
        }

        if(geneur == nullptr)
        {
            std::cout << "pas resolu" << std::endl;
        }
        else
        {
            auto copie_parents = parents;
            //copie_parents.push_back(std::pair<char,char>(j.joueur.getId(), geneur->getId()));
            
            std::queue<std::queue<Mouvement>> file_mouvement;
            std::queue<std::vector<std::pair<char,char>>> file_parents;
            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1, res2;
            std::vector<std::queue<Mouvement>> mouv1, mouv2;

            std::vector< std::vector<std::pair<char,char>> > res_p1, res_p2;

            if(geneur->getOrientation() == VERTICAL)
            {   
                res1 = degager_recursive(copie_parents, j, j.joueur.getId(), geneur->getId(), HAUT, offset, j.joueur.getPosY());
                res2 = degager_recursive(copie_parents, j, j.joueur.getId(), geneur->getId(), BAS, offset, j.joueur.getPosY());
            }
            else
            {
                res1 = degager_recursive(copie_parents, j, j.joueur.getId(), geneur->getId(), GAUCHE, offset, j.joueur.getPosY());
                res2 = degager_recursive(copie_parents, j, j.joueur.getId(), geneur->getId(), DROITE, offset, j.joueur.getPosY());
            }

            mouv1 = res1.first;
            mouv2 = res2.first;
            res_p1 = res1.second;
            res_p2 = res2.second;

            mouv1.insert(mouv1.end(), mouv2.begin(), mouv2.end());
            res_p1.insert(res_p1.end(), res_p2.begin(), res_p2.end());

            for(int i = 0; i < mouv1.size(); i++)
            {
                file_mouvement.push(mouv1[i]);
                file_parents.push(res_p1[i]);
            }

            while(file_mouvement.size() > 0)
            {
                Jeu copie_jeu = j;
                std::queue<Mouvement> copie_solution = file_mouvement.front();
                
                if(copie_solution.size() > 0)
                {
                    while(copie_solution.size() > 0)
                    {
                        Mouvement m = copie_solution.front();
                        copie_jeu.deplacerVoiture(*copie_jeu.getVoitureParId(m.getId()), m.getDirection());
                        copie_solution.pop();
                    }

                    //copie_jeu.afficher();

                    if(deplacerJoueur_recursive(copie_jeu))
                    {
                        copie_solution = file_mouvement.front();
                        int offset2 = 6 - copie_jeu.joueur.getTaille() - copie_jeu.joueur.getPosX();
                        for(int i = 0; i < offset2; i++)
                            copie_solution.push(Mouvement(copie_jeu.joueur.getId(),DROITE));
                        
                        solutions.push_back(copie_solution);
                        //std::cout << "SOLUTIONS TROUVEE : " << solutions.size() << std::endl;
                    }
                    else
                    {
                        Voiture* geneur2 = nullptr;
                        int offset2 = copie_jeu.joueur.getTaille() + copie_jeu.joueur.getPosX();

                        while(offset2 < 6 && geneur2 == nullptr)
                        {
                            geneur2 = copie_jeu.grille.etatGrille[offset2][copie_jeu.joueur.getPosY()].getOccupe();
                            if(geneur2 == nullptr)
                                offset2++;
                        }

                        if(geneur2 != nullptr)
                        {
                            auto copie_parents2 = file_parents.front();
                            //copie_parents2.push_back(std::pair<char,char>(j.joueur.getId(), geneur2->getId()));
                            
                            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1bis, res2bis;
                            std::vector<std::queue<Mouvement>> mouv1bis, mouv2bis;

                            std::vector< std::vector<std::pair<char,char>> > res_p1bis, res_p2bis;

                            if(geneur2->getOrientation() == VERTICAL)
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, copie_jeu.joueur.getId(), geneur2->getId(), HAUT, offset2, copie_jeu.joueur.getPosY());
                                res2bis = degager_recursive(copie_parents2, copie_jeu, copie_jeu.joueur.getId(), geneur2->getId(), BAS, offset2, copie_jeu.joueur.getPosY());
                            }
                            else
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, copie_jeu.joueur.getId(), geneur2->getId(), GAUCHE, offset2, copie_jeu.joueur.getPosY());
                                res2bis = degager_recursive(copie_parents2, copie_jeu, copie_jeu.joueur.getId(), geneur2->getId(), DROITE, offset2, copie_jeu.joueur.getPosY());
                            }

                            mouv1bis = res1bis.first;
                            mouv2bis = res2bis.first;
                            res_p1bis = res1bis.second;
                            res_p2bis = res2bis.second;

                            mouv1bis.insert(mouv1bis.end(), mouv2bis.begin(), mouv2bis.end());
                            res_p1bis.insert(res_p1bis.end(), res_p2bis.begin(), res_p2bis.end());

                            for(int i = 0; i < mouv1bis.size() ; i++)
                            {
                                copie_solution = file_mouvement.front();
                                std::queue<Mouvement> copie_a_concatener = mouv1bis[i];
                                while(copie_a_concatener.size() > 0)
                                {
                                    copie_solution.push(copie_a_concatener.front());
                                    copie_a_concatener.pop();
                                }

                                file_mouvement.push(copie_solution);
                                file_parents.push(res_p1bis[i]);
                            }
                        }
                    }
                }

                file_mouvement.pop();
                file_parents.pop();
            }
        }
    }
}

bool Grabator::deplacerJoueur_recursive(Jeu j)
{ 
    if(j.peutDeplacerVoiture(j.joueur, DROITE, 6 - j.joueur.getTaille() - j.joueur.getPosX()))
    {
        int deplacement = 6 - j.joueur.getTaille() - j.joueur.getPosX();
        for(int i = 0; i < deplacement; i++)
            j.deplacerVoiture(j.joueur, DROITE);

        return true;
    }
    else
        return false;
}

std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > Grabator::degager_recursive(std::vector<std::pair<char,char>>parents, Jeu j, char gener, char geneur, Direction direction, int xCase, int yCase)
{    
    std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > s;

    if(gener == geneur)
        return s;

    //std::cout << direction << " " << gener << geneur << " PARENTS : " << parents.size() << std::endl;
    //std::cout << "xCase " << xCase << " yCase " << yCase << std::endl;
    //j.afficher();

    //for(int i = 0; i < parents.size(); i++)
    //    std::cout << parents[i].first << " " << parents[i].second << std::endl;
    
    j.majEtat();
    
    switch (direction)
    {
        case HAUT:
            return degager_haut_recursive(parents, j, gener,geneur,xCase,yCase);
            break;
        case BAS:
            return degager_bas_recursive(parents, j, gener,geneur,xCase,yCase);
            break;
        case GAUCHE:
            return degager_gauche_recursive(parents, j, gener,geneur,xCase,yCase);
            break;
        case DROITE:
            return degager_droite_recursive(parents, j, gener,geneur,xCase,yCase);
            break;
        default:
            return s;
            break;
    }
}

std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > Grabator::degager_haut_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase)
{
    std::vector<std::queue<Mouvement>> vec_res; // Le vecteur-resultat qui contiendra plusieurs sequence de mouvement pour degager une case
    std::vector<std::vector<std::pair<char,char>>> vec_parents; // Le appels <gené,geneur> consecutifs de chaque associées a chaque sequance de mouvement
    std::vector<std::pair<char,char>> copie_parents = parents;

    if(parents.size() > 50) // On s'arrete a 50 appeles de degagement consecutifs
        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);

    int offset = j.getVoitureParId(geneur)->getPosY() - yCase + j.getVoitureParId(geneur)->getTaille(); // Le decalage que la voiture doit effectuer pour degager la case
    //std::cout << geneur << " OFFSET : " << offset << std::endl;
    
    if(j.peutDeplacerVoiture(*j.getVoitureParId(geneur), HAUT, offset)) // Si on peut effectuer le mouvement, un creer une nouvelle sequence de mouvement que l'on renvoie a celui qui demande un degagement
    {
        std::queue<Mouvement> res;

        for(int i = 0; i < offset; i++)
            res.push(Mouvement(geneur,HAUT));
        
        vec_parents.push_back(parents);
        vec_res.push_back(res);
    }
    else // Sinon il y a on problème, on verifie si c'est un mur ou une voiture
    {
        Voiture* nouveau_geneur = nullptr; // Possible voiture qui nous gene
        offset = j.getVoitureParId(geneur)->getPosY() - 1; // Position a partie de laquelle je doit partir pour verifier tout la ligne

        while(offset >= 0 && nouveau_geneur == nullptr) // On ne s'arrete pas tant qu'on a pas trouver de geneur ou qu'on n'est pas au niveau du mur
        {
            nouveau_geneur = j.grille.etatGrille[j.getVoitureParId(geneur)->getPosX()][offset].getOccupe();
            if(nouveau_geneur == nullptr)
                offset--;
        }

        if(nouveau_geneur != nullptr) // Si on est gene par une voiture
        {
            std::pair<char,char> test(geneur,nouveau_geneur->getId());
            std::pair<char,char> test2(nouveau_geneur->getId(),geneur);

            if(parents.size() > 0) // Ici on test si la voiture qui nous gene est dans la meme orientation que nous, ce qui peut entrainer des appels de degagement répétés a l'infini
                if(test == parents[parents.size() - 1] || test2 == parents[parents.size() - 1])
                    if((nouveau_geneur)->getOrientation() == j.getVoitureParId(geneur)->getOrientation())
                        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);

            bool accu = true;

            for(int i = 0; i < (int)(parents.size() - 1); i++) // Ici, on verifie si la voiture qui gene n'a pas deja géné par le passé
            {
                accu = accu & (test != parents[i]);// & (test2 != parents[parents.size() - 1]);
                //accu = accu && test != parents[i] && test2 != parents[i];
            }

            if(!accu)
                return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
            
			// A partir de cette etape , on a identifier la voiture qui nous gene, on va lui de mander de bouger et on va recuperer les resultats dans les variables si dessous

            std::queue<std::queue<Mouvement>> file_mouvement; // Une variable tampon de sequence de mouvemnt qui nous permetra de simuler des situation et de verifier dans ces simulations, si on peut se degager
            std::queue<std::vector<std::pair<char,char>>> file_parents; // une variable tampon des appaels gené,geneur
            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1, res2; // Les resultats des appels de degagement
            std::vector<std::queue<Mouvement>> mouv1, mouv2; // Les sequences de mouvement des resultats
            std::vector< std::vector<std::pair<char,char>> > res_p1, res_p2; // Les appels appaels gené,geneur des resultats

            auto copie_parents = parents;
            copie_parents.push_back(std::pair<char,char>(geneur, nouveau_geneur->getId())); // On ajoute a notre file de parents celui qui gene la voiture actuelle

            if(nouveau_geneur->getOrientation() == VERTICAL)
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), HAUT, j.getVoitureParId(geneur)->getPosX() , offset);
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), BAS, j.getVoitureParId(geneur)->getPosX(), offset);
            }
            else
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), GAUCHE, j.getVoitureParId(geneur)->getPosX() , offset);
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), DROITE, j.getVoitureParId(geneur)->getPosX(), offset);
            }
			
			// Ici, je pousse tout dans une seul variable pour le traiter plus facilement
            mouv1 = res1.first;
            mouv2 = res2.first;
            res_p1 = res1.second;
            res_p2 = res2.second;

            mouv1.insert(mouv1.end(), mouv2.begin(), mouv2.end());
            res_p1.insert(res_p1.end(), res_p2.begin(), res_p2.end());

            for(int i = 0; i < mouv1.size(); i++)
            {
                file_mouvement.push(mouv1[i]);
                file_parents.push(res_p1[i]);
            }

            while(file_mouvement.size() > 0) // Chaque sequance de mouvement renvoyé par la voiture qui gene a ete enfilé dans cette file, on va simuler chaque sequance de mouvement et verifier si on peut se deplacer dedans
            {
                Jeu copie_jeu = j;

                std::queue<Mouvement> copie_solution = file_mouvement.front();
                auto copie_parents2 = file_parents.front();

                if(copie_solution.size() > 0) // Si ce qu'on nous a envoyez n'est pas vide, alors ont simule dans notre copie du jeu
                {
                    while(copie_solution.size() > 0)
                    {
                        Mouvement m = copie_solution.front();
                        copie_jeu.deplacerVoiture(*copie_jeu.getVoitureParId(m.getId()), m.getDirection());
                        copie_solution.pop();
                    }

                    if(copie_jeu.peutDeplacerVoiture(*copie_jeu.getVoitureParId(geneur), HAUT, copie_jeu.getVoitureParId(geneur)->getPosY() - yCase + copie_jeu.getVoitureParId(geneur)->getTaille()))
                    {
                        copie_solution = file_mouvement.front();
                        int offset2 = copie_jeu.getVoitureParId(geneur)->getPosY() - yCase + copie_jeu.getVoitureParId(geneur)->getTaille();
                        for(int i = 0; i < offset2; i++)
                            copie_solution.push(Mouvement(geneur,HAUT));
                        
                        vec_res.push_back(copie_solution);
                        vec_parents.push_back(copie_parents2);
                    }
                    else // Il est possible qu'il y ai une autre voiture juste deriere qui gene, dans se cas on lui demande de degager, on recupere le resultat, et on restock ca dans la file pour la traiter plus tard
                    {
                        Voiture* nouveau_geneur2 = nullptr;
                        int offset2 = copie_jeu.getVoitureParId(geneur)->getPosY() - 1;

                        while(offset2 >= 0 && nouveau_geneur2 == nullptr)
                        {
                            nouveau_geneur2 = copie_jeu.grille.etatGrille[copie_jeu.getVoitureParId(geneur)->getPosX()][offset2].getOccupe();
                            if(nouveau_geneur2 == nullptr)
                                offset2--;
                        }

                        if(nouveau_geneur2 != nullptr)
                        {
                            //auto copie_parents2 = parents;
                            copie_parents2.push_back(std::pair<char,char>(geneur, nouveau_geneur2->getId()));
                            
                            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1bis, res2bis;
                            std::vector<std::queue<Mouvement>> mouv1bis, mouv2bis;

                            std::vector< std::vector<std::pair<char,char>> > res_p1bis, res_p2bis;

                            if(nouveau_geneur2->getOrientation() == VERTICAL)
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), HAUT, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), BAS, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                            }
                            else
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), GAUCHE, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), DROITE, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                            }

                            mouv1bis = res1bis.first;
                            mouv2bis = res2bis.first;
                            res_p1bis = res1bis.second;
                            res_p2bis = res2bis.second;

                            mouv1bis.insert(mouv1bis.end(), mouv2bis.begin(), mouv2bis.end());
                            res_p1bis.insert(res_p1bis.end(), res_p2bis.begin(), res_p2bis.end());

                            for(int i = 0; i < mouv1bis.size() ; i++)
                            {
                                copie_solution = file_mouvement.front();
                                std::queue<Mouvement> copie_a_concatener = mouv1bis[i];
                                while(copie_a_concatener.size() > 0)
                                {
                                    copie_solution.push(copie_a_concatener.front());
                                    copie_a_concatener.pop();
                                }

                                file_mouvement.push(copie_solution);
                                file_parents.push(res_p1bis[i]);
                            }
                        }
                    }  
                }
				// Une fois la simulation traité, on la défile et on passe à la suivante
                file_mouvement.pop();
                file_parents.pop();
            }
        }
    }
    
    return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
}

std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > Grabator::degager_bas_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase)
{
    std::vector<std::queue<Mouvement>> vec_res;
    std::vector<std::vector<std::pair<char,char>>> vec_parents;
    std::vector<std::pair<char,char>> copie_parent = parents;

    if(parents.size() > 50)
        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);

    int offset = -j.getVoitureParId(geneur)->getPosY() + yCase + 1; // La case que le joueur essaye d'atteindre
    //std::cout << geneur << " OFFSET : " << offset << std::endl;
    
    if(j.peutDeplacerVoiture(*j.getVoitureParId(geneur), BAS, offset))
    {
        std::queue<Mouvement> res;

        for(int i = 0; i < offset; i++)
            res.push(Mouvement(geneur,BAS));
        
        vec_parents.push_back(parents);
        vec_res.push_back(res);
    }
    else
    {
        Voiture* nouveau_geneur = nullptr;
        offset = j.getVoitureParId(geneur)->getPosY() + j.getVoitureParId(geneur)->getTaille();

        while(offset < 6 && nouveau_geneur == nullptr)
        {
            nouveau_geneur = j.grille.etatGrille[j.getVoitureParId(geneur)->getPosX()][offset].getOccupe();
            if(nouveau_geneur == nullptr)
                offset++;
        }

        if(nouveau_geneur != nullptr)
        {
            std::pair<char,char> test(geneur,nouveau_geneur->getId());
            std::pair<char,char> test2(nouveau_geneur->getId(),geneur);
            
            if(parents.size() > 0)
                if(test == parents[parents.size() - 1] || test2 == parents[parents.size() - 1])
                    if((nouveau_geneur)->getOrientation() == j.getVoitureParId(geneur)->getOrientation())
                        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);

            bool accu = true;

            for(int i = 0; i < (int)(parents.size() - 1); i++)
                accu = accu & (test != parents[i]);
                //accu = accu && test != parents[i] && test2 != parents[i];

            if(!accu)
                return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
            
            std::queue<std::queue<Mouvement>> file_mouvement;
            std::queue<std::vector<std::pair<char,char>>> file_parents;
            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1, res2;
            std::vector<std::queue<Mouvement>> mouv1, mouv2;
            std::vector< std::vector<std::pair<char,char>> > res_p1, res_p2;

            auto copie_parents = parents;
            copie_parents.push_back(std::pair<char,char>(geneur, nouveau_geneur->getId()));

            if(nouveau_geneur->getOrientation() == VERTICAL)
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), HAUT, j.getVoitureParId(geneur)->getPosX() , offset);
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), BAS, j.getVoitureParId(geneur)->getPosX(), offset);
            }
            else
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), GAUCHE, j.getVoitureParId(geneur)->getPosX() , offset);
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), DROITE, j.getVoitureParId(geneur)->getPosX(), offset);
            }

            mouv1 = res1.first;
            mouv2 = res2.first;
            res_p1 = res1.second;
            res_p2 = res2.second;

            mouv1.insert(mouv1.end(), mouv2.begin(), mouv2.end());
            res_p1.insert(res_p1.end(), res_p2.begin(), res_p2.end());

            for(int i = 0; i < mouv1.size(); i++)
            {
                file_mouvement.push(mouv1[i]);
                file_parents.push(res_p1[i]);
            }

            while(file_mouvement.size() > 0)
            {
                Jeu copie_jeu = j;
                std::queue<Mouvement> copie_solution = file_mouvement.front();
                auto copie_parents2 = file_parents.front();

                if(copie_solution.size() > 0)
                {
                    while(copie_solution.size() > 0)
                    {
                        Mouvement m = copie_solution.front();
                        copie_jeu.deplacerVoiture(*copie_jeu.getVoitureParId(m.getId()), m.getDirection());
                        copie_solution.pop();
                    }

                    if(copie_jeu.peutDeplacerVoiture(*copie_jeu.getVoitureParId(geneur), BAS, -copie_jeu.getVoitureParId(geneur)->getPosY() + yCase + 1))
                    {
                        copie_solution = file_mouvement.front();
                        int offset2 = -copie_jeu.getVoitureParId(geneur)->getPosY() + yCase + 1;
                        for(int i = 0; i < offset2; i++)
                            copie_solution.push(Mouvement(geneur,BAS));
                        
                        vec_res.push_back(copie_solution);
                        vec_parents.push_back(copie_parents2);
                    }
                    else
                    {
                        Voiture* nouveau_geneur2 = nullptr;
                        int offset2 = copie_jeu.getVoitureParId(geneur)->getPosY() + copie_jeu.getVoitureParId(geneur)->getTaille();

                        while(offset2 < 6 && nouveau_geneur2 == nullptr)
                        {
                            nouveau_geneur2 = copie_jeu.grille.etatGrille[copie_jeu.getVoitureParId(geneur)->getPosX()][offset2].getOccupe();
                            if(nouveau_geneur2 == nullptr)
                                offset2++;
                        }

                        if(nouveau_geneur2 != nullptr)
                        {
                            //auto copie_parents2 = parents;
                            copie_parents2.push_back(std::pair<char,char>(geneur, nouveau_geneur2->getId()));
                            
                            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1bis, res2bis;
                            std::vector<std::queue<Mouvement>> mouv1bis, mouv2bis;

                            std::vector< std::vector<std::pair<char,char>> > res_p1bis, res_p2bis;

                            if(nouveau_geneur2->getOrientation() == VERTICAL)
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), HAUT, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), BAS, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                            }
                            else
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), GAUCHE, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), DROITE, copie_jeu.getVoitureParId(geneur)->getPosX(), offset2);
                            }

                            mouv1bis = res1bis.first;
                            mouv2bis = res2bis.first;
                            res_p1bis = res1bis.second;
                            res_p2bis = res2bis.second;

                            mouv1bis.insert(mouv1bis.end(), mouv2bis.begin(), mouv2bis.end());
                            res_p1bis.insert(res_p1bis.end(), res_p2bis.begin(), res_p2bis.end());

                            for(int i = 0; i < mouv1bis.size() ; i++)
                            {
                                copie_solution = file_mouvement.front();
                                std::queue<Mouvement> copie_a_concatener = mouv1bis[i];
                                while(copie_a_concatener.size() > 0)
                                {
                                    copie_solution.push(copie_a_concatener.front());
                                    copie_a_concatener.pop();
                                }

                                file_mouvement.push(copie_solution);
                                file_parents.push(res_p1bis[i]);
                            }
                        }
                    }  
                }

                file_mouvement.pop();
                file_parents.pop();
            }
        }
    }
    
    return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
}
std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > Grabator::degager_gauche_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase)
{
    std::vector<std::queue<Mouvement>> vec_res;
    std::vector<std::vector<std::pair<char,char>>> vec_parents;
    std::vector<std::pair<char,char>> copie_parents = parents;

    if(parents.size() > 50)
        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);

    int offset = j.getVoitureParId(geneur)->getPosX() - xCase + j.getVoitureParId(geneur)->getTaille();
    //std::cout << geneur << " OFFSET : " << offset << std::endl;
    
    if(j.peutDeplacerVoiture(*j.getVoitureParId(geneur), GAUCHE, offset))
    {
        std::queue<Mouvement> res;

        for(int i = 0; i < offset; i++)
            res.push(Mouvement(geneur,GAUCHE));
        
        vec_parents.push_back(parents);
        vec_res.push_back(res);
    }
    else
    {
        Voiture* nouveau_geneur = nullptr;
        offset = j.getVoitureParId(geneur)->getPosX() - 1;

        while(offset >= 0 && nouveau_geneur == nullptr)
        {
            nouveau_geneur = j.grille.etatGrille[offset][j.getVoitureParId(geneur)->getPosY()].getOccupe();
            if(nouveau_geneur == nullptr)
                offset--;
        }

        if(nouveau_geneur != nullptr)
        {
            std::pair<char,char> test(geneur,nouveau_geneur->getId());
            std::pair<char,char> test2(nouveau_geneur->getId(),geneur);
            
            if(parents.size() > 0)
                if(test == parents[parents.size() - 1] || test2 == parents[parents.size() - 1])
                    if((nouveau_geneur)->getOrientation() == j.getVoitureParId(geneur)->getOrientation())
                        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
            
            bool accu = true;
            
            for(int i = 0; i < (int)(parents.size() - 1); i++)
                accu = accu & (test != parents[i]);
                //accu = accu && test != parents[i] && test2 != parents[i];

            if(!accu)
                return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
            
            std::queue<std::queue<Mouvement>> file_mouvement;
            std::queue<std::vector<std::pair<char,char>>> file_parents;
            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1, res2;
            std::vector<std::queue<Mouvement>> mouv1, mouv2;
            std::vector< std::vector<std::pair<char,char>> > res_p1, res_p2;

            auto copie_parents = parents;
            copie_parents.push_back(std::pair<char,char>(geneur, nouveau_geneur->getId()));

            if(nouveau_geneur->getOrientation() == VERTICAL)
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), HAUT, offset, j.getVoitureParId(geneur)->getPosY() );
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), BAS, offset, j.getVoitureParId(geneur)->getPosY());
            }
            else
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), GAUCHE, offset, j.getVoitureParId(geneur)->getPosY());
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), DROITE, offset, j.getVoitureParId(geneur)->getPosY());
            }

            mouv1 = res1.first;
            mouv2 = res2.first;
            res_p1 = res1.second;
            res_p2 = res2.second;

            mouv1.insert(mouv1.end(), mouv2.begin(), mouv2.end());
            res_p1.insert(res_p1.end(), res_p2.begin(), res_p2.end());

            for(int i = 0; i < mouv1.size(); i++)
            {
                file_mouvement.push(mouv1[i]);
                file_parents.push(res_p1[i]);
            }

            while(file_mouvement.size() > 0)
            {
                Jeu copie_jeu = j;
                std::queue<Mouvement> copie_solution = file_mouvement.front();
                auto copie_parents2 = file_parents.front();
                if(copie_solution.size() > 0)
                {
                    while(copie_solution.size() > 0)
                    {
                        Mouvement m = copie_solution.front();
                        copie_jeu.deplacerVoiture(*copie_jeu.getVoitureParId(m.getId()), m.getDirection());
                        copie_solution.pop();
                    }

                    if(copie_jeu.peutDeplacerVoiture(*copie_jeu.getVoitureParId(geneur), GAUCHE, copie_jeu.getVoitureParId(geneur)->getPosX() - xCase + copie_jeu.getVoitureParId(geneur)->getTaille()))
                    {
                        copie_solution = file_mouvement.front();
                        int offset2 = copie_jeu.getVoitureParId(geneur)->getPosX() - xCase + copie_jeu.getVoitureParId(geneur)->getTaille();
                        for(int i = 0; i < offset2; i++)
                            copie_solution.push(Mouvement(geneur,GAUCHE));
                        
                        vec_res.push_back(copie_solution);
                        vec_parents.push_back(copie_parents2);
                    }
                    else
                    {
                        Voiture* nouveau_geneur2 = nullptr;
                        int offset2 = copie_jeu.getVoitureParId(geneur)->getPosX() - 1;

                        while(offset2 >= 0 && nouveau_geneur2 == nullptr)
                        {
                            nouveau_geneur2 = copie_jeu.grille.etatGrille[offset2][copie_jeu.getVoitureParId(geneur)->getPosY()].getOccupe();
                            if(nouveau_geneur2 == nullptr)
                                offset2--;
                        }

                        if(nouveau_geneur2 != nullptr)
                        {
                            //auto copie_parents2 = parents;
                            copie_parents2.push_back(std::pair<char,char>(geneur, nouveau_geneur2->getId()));
                            
                            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1bis, res2bis;
                            std::vector<std::queue<Mouvement>> mouv1bis, mouv2bis;

                            std::vector< std::vector<std::pair<char,char>> > res_p1bis, res_p2bis;


                            if(nouveau_geneur2->getOrientation() == VERTICAL)
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), HAUT, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), BAS, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                            }
                            else
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), GAUCHE, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), DROITE, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                            }

                            mouv1bis = res1bis.first;
                            mouv2bis = res2bis.first;
                            res_p1bis = res1bis.second;
                            res_p2bis = res2bis.second;

                            mouv1bis.insert(mouv1bis.end(), mouv2bis.begin(), mouv2bis.end());
                            res_p1bis.insert(res_p1bis.end(), res_p2bis.begin(), res_p2bis.end());

                            for(int i = 0; i < mouv1bis.size() ; i++)
                            {
                                copie_solution = file_mouvement.front();
                                std::queue<Mouvement> copie_a_concatener = mouv1bis[i];
                                while(copie_a_concatener.size() > 0)
                                {
                                    copie_solution.push(copie_a_concatener.front());
                                    copie_a_concatener.pop();
                                }

                                file_mouvement.push(copie_solution);
                                file_parents.push(res_p1bis[i]);
                            }
                        }
                    }  
                }

                file_mouvement.pop();
                file_parents.pop();
            }
        }
    }
    
    return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
}
std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > Grabator::degager_droite_recursive(std::vector<std::pair<char,char>> parents, Jeu& j, char gener, char geneur, int xCase, int yCase)
{
    std::vector<std::queue<Mouvement>> vec_res;
    std::vector<std::vector<std::pair<char,char>>> vec_parents;
    std::vector<std::pair<char,char>> copie_parents = parents;
    
    if(parents.size() > 50)
        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);

    int offset = -j.getVoitureParId(geneur)->getPosX() + xCase + 1;
    //std::cout << geneur << " OFFSET : " << offset << std::endl;
    
    if(j.peutDeplacerVoiture(*j.getVoitureParId(geneur), DROITE, offset))
    {
        std::queue<Mouvement> res;

        for(int i = 0; i < offset; i++)
            res.push(Mouvement(geneur,DROITE));
        
        vec_parents.push_back(parents);
        vec_res.push_back(res);
    }
    else
    {
        Voiture* nouveau_geneur = nullptr;
        offset = j.getVoitureParId(geneur)->getPosX() + j.getVoitureParId(geneur)->getTaille();

        while(offset < 6 && nouveau_geneur == nullptr)
        {
            nouveau_geneur = j.grille.etatGrille[offset][j.getVoitureParId(geneur)->getPosY()].getOccupe();
            if(nouveau_geneur == nullptr)
                offset++;
        }

        if(nouveau_geneur != nullptr)
        {
            std::pair<char,char> test(geneur,nouveau_geneur->getId());
            std::pair<char,char> test2(nouveau_geneur->getId(),geneur);
            
            if(parents.size() > 0)
                if(test == parents[parents.size() - 1] || test2 == parents[parents.size() - 1])
                    if((nouveau_geneur)->getOrientation() == j.getVoitureParId(geneur)->getOrientation())
                        return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
                        
            bool accu = true;

            for(int i = 0; i < (int)(parents.size() - 1); i++)
                accu = accu & (test != parents[i]);
                //accu = accu && test != parents[i] && test2 != parents[i];

            if(!accu)
                return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
            
            std::queue<std::queue<Mouvement>> file_mouvement;
            std::queue<std::vector<std::pair<char,char>>> file_parents;
            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1, res2;
            std::vector<std::queue<Mouvement>> mouv1, mouv2;
            std::vector< std::vector<std::pair<char,char>> > res_p1, res_p2;

            auto copie_parents = parents;
            copie_parents.push_back(std::pair<char,char>(geneur, nouveau_geneur->getId()));

            if(nouveau_geneur->getOrientation() == VERTICAL)
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), HAUT, offset, j.getVoitureParId(geneur)->getPosY());
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), BAS, offset, j.getVoitureParId(geneur)->getPosY());
            }
            else
            {
                res1 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), GAUCHE, offset, j.getVoitureParId(geneur)->getPosY());
                res2 = degager_recursive(copie_parents, j, geneur, nouveau_geneur->getId(), DROITE, offset, j.getVoitureParId(geneur)->getPosY());
            }

            mouv1 = res1.first;
            mouv2 = res2.first;
            res_p1 = res1.second;
            res_p2 = res2.second;

            mouv1.insert(mouv1.end(), mouv2.begin(), mouv2.end());
            res_p1.insert(res_p1.end(), res_p2.begin(), res_p2.end());

            for(int i = 0; i < mouv1.size(); i++)
            {
                file_mouvement.push(mouv1[i]);
                file_parents.push(res_p1[i]);
            }

            while(file_mouvement.size() > 0)
            {
                Jeu copie_jeu = j;
                std::queue<Mouvement> copie_solution = file_mouvement.front();
                auto copie_parents2 = file_parents.front();

                if(copie_solution.size() > 0)
                {
                    while(copie_solution.size() > 0)
                    {
                        Mouvement m = copie_solution.front();
                        copie_jeu.deplacerVoiture(*copie_jeu.getVoitureParId(m.getId()), m.getDirection());
                        copie_solution.pop();
                    }

                    if(copie_jeu.peutDeplacerVoiture(*copie_jeu.getVoitureParId(geneur), DROITE, -copie_jeu.getVoitureParId(geneur)->getPosX() + xCase + 1))
                    {
                        copie_solution = file_mouvement.front();
                        int offset2 = -copie_jeu.getVoitureParId(geneur)->getPosX() + xCase + 1;
                        for(int i = 0; i < offset2; i++)
                            copie_solution.push(Mouvement(geneur,DROITE));
                        
                        vec_res.push_back(copie_solution);
                        vec_parents.push_back(copie_parents2);
                    }
                    else
                    {
                        Voiture* nouveau_geneur2 = nullptr;
                        int offset2 = copie_jeu.getVoitureParId(geneur)->getPosX() + copie_jeu.getVoitureParId(geneur)->getTaille();

                        while(offset2 < 6 && nouveau_geneur2 == nullptr)
                        {
                            nouveau_geneur2 = copie_jeu.grille.etatGrille[offset2][copie_jeu.getVoitureParId(geneur)->getPosY()].getOccupe();
                            if(nouveau_geneur2 == nullptr)
                                offset2++;
                        }

                        if(nouveau_geneur2 != nullptr)
                        {
                            //auto copie_parents2 = parents;
                            copie_parents2.push_back(std::pair<char,char>(geneur, nouveau_geneur2->getId()));
                            
                            std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> > res1bis, res2bis;
                            std::vector<std::queue<Mouvement>> mouv1bis, mouv2bis;

                            std::vector< std::vector<std::pair<char,char>> > res_p1bis, res_p2bis;

                            if(nouveau_geneur2->getOrientation() == VERTICAL)
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), HAUT, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), BAS, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                            }
                            else
                            {
                                res1bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), GAUCHE, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                                res2bis = degager_recursive(copie_parents2, copie_jeu, geneur, nouveau_geneur2->getId(), DROITE, offset2, copie_jeu.getVoitureParId(geneur)->getPosY());
                            }

                            mouv1bis = res1bis.first;
                            mouv2bis = res2bis.first;
                            res_p1bis = res1bis.second;
                            res_p2bis = res2bis.second;

                            mouv1bis.insert(mouv1bis.end(), mouv2bis.begin(), mouv2bis.end());
                            res_p1bis.insert(res_p1bis.end(), res_p2bis.begin(), res_p2bis.end());

                            for(int i = 0; i < mouv1bis.size() ; i++)
                            {
                                copie_solution = file_mouvement.front();
                                std::queue<Mouvement> copie_a_concatener = mouv1bis[i];
                                while(copie_a_concatener.size() > 0)
                                {
                                    copie_solution.push(copie_a_concatener.front());
                                    copie_a_concatener.pop();
                                }

                                file_mouvement.push(copie_solution);
                                file_parents.push(res_p1bis[i]);
                            }
                        }
                    }  
                }

                file_mouvement.pop();
                file_parents.pop();
            }
        }
    }
    
    return std::pair< std::vector<std::queue<Mouvement>> , std::vector<std::vector<std::pair<char,char>>> >(vec_res,vec_parents);
}

void Grabator::resoudre_largeur(Jeu j)
{
    std::multimap< std::string, std::pair< Jeu, std::queue<Mouvement>> > essai; // Une map qui pour chaque etat du jeu, enregistre les mouvement necessaire pour l'atteindre
    std::queue<Mouvement> m; // La liste de mouvement en question

    essai.insert( std::pair< std::string, std::pair< Jeu, std::queue<Mouvement>>>( j.grille.encode(), std::pair<Jeu,std::queue<Mouvement>>(j,m) )); // A debut, on met la situation inition, avec une liste de mouvement vide
    std::multimap< std::string, std::pair< Jeu, std::queue<Mouvement>> >::iterator debut; // Cette iterateur va parcourir la map pour tester de nouvelles possibibltés

    int iterateur = 0;
	bool resolu = false;

    while(iterateur < 100 && !resolu) // On s'arretera si on trouve rien en moins de 100 mouvements
    {
		std::multimap< std::string, std::pair< Jeu, std::queue<Mouvement>> >::iterator fin = essai.end();
        debut = essai.begin();

        while(debut != fin)
        {
            if(debut->second.second.size() == iterateur) // On compare les situation en 0 mouvements, puis en 1, en 2, en 3, etc...
            {
                Jeu copie = debut->second.first;

                if(copie.joueur.getPosX() == 4) // Si le joueur est au bout, on a trouver la solution avec le moins de mouvements (du moins le moins d'appel a une fonction de deplacement)
                {
                    solutions.push_back(debut->second.second);
                    resolu = true;
					//break;
                }
                else
                {
                    int sub_iterateur = (int)(copie.voitures.size()) - 1 ; // Un iterateur qui va parcourir toutes les voitures de la situation de jeu

                    while(sub_iterateur >= - 1)
                    {                      
                        Jeu copie2 = copie;
                        
                        char id_a_tester; // L'identifiant de la voiture qu'on va essayer de deplacer
                        if(sub_iterateur < 0) // Si on est a -1, on prends le joueur, sinon on prend la voiture au bon indice
                            id_a_tester = copie2.joueur.getId();
                        else
                            id_a_tester = copie2.voitures[sub_iterateur].getId();
                        
                        Direction direction_a_tester[2];
                        if(copie2.getVoitureParId(id_a_tester)->getOrientation() == VERTICAL)
                        {
                            direction_a_tester[0] = HAUT;
                            direction_a_tester[1] = BAS;
                        }
                        else
                        {
                            direction_a_tester[0] = GAUCHE;
                            direction_a_tester[1] = DROITE;
                        }

                        for(int i = 0; i < 2; i++)
                        {
                            if( copie2.peutDeplacerVoiture( *copie2.getVoitureParId(id_a_tester) , direction_a_tester[i] )) // Si on peut deplacer la voiture dans la direction donnée
                            {
								Jeu copie3 = copie2; // On cree une nouvelle situation du jeu (copiée de la precedent) et on effectue le mouvement
                                copie3.deplacerVoiture(*copie3.getVoitureParId(id_a_tester), direction_a_tester[i]);

								std::queue<Mouvement> file_mouvement = debut->second.second;
								file_mouvement.push(Mouvement(id_a_tester, direction_a_tester[i]));

                                std::pair< std::multimap< std::string, std::pair< Jeu, std::queue<Mouvement>> >::iterator, std::multimap< std::string, std::pair< Jeu, std::queue<Mouvement>> >::iterator > recherche; // On cherche si ont peut trouver des situations avec la meme cle dans la map
                                recherche = essai.equal_range(copie3.grille.encode());

                                if(recherche.first == recherche.second) // Si il n'y a pas de situations similaires, on ajout juste notre situation
                                {
                                    std::pair< Jeu, std::queue<Mouvement> > a_inserer(copie3,file_mouvement);
                                    essai.insert(std::pair<std::string,std::pair< Jeu, std::queue<Mouvement> >> (copie3.grille.encode(), a_inserer));
                                }
                                else
                                {
                                    std::multimap< std::string, std::pair< Jeu, std::queue<Mouvement>> >::iterator recherche_iterateur;
                                    recherche_iterateur = recherche.first;
                                    bool insertion = false;

                                    while(recherche_iterateur != recherche.second)
                                        if ( nombreMouvement(file_mouvement) <= nombreMouvement(recherche_iterateur->second.second) ) // Si une des situations similaire se fait en plus de mouvement que la situations actuelle, on supprime et situations siilaire et on ajoute la situation actuelle
                                        {
                                            insertion = true;
                                            auto tmp = recherche_iterateur;
                                            recherche_iterateur++;
                                            essai.erase(tmp);
                                        }
                                        else
                                            recherche_iterateur++;

                                    if(insertion)
                                    {
                                        std::pair< Jeu, std::queue<Mouvement> > a_inserer(copie3,file_mouvement);
                                        essai.insert(std::pair<std::string,std::pair< Jeu, std::queue<Mouvement> >> (copie3.grille.encode(), a_inserer));
                                    }
                                }
                            }
                        }
                        sub_iterateur--;
                    }
                }
            }

            debut++;
        }

        iterateur++;
    }
}

Jeu sub_genererGrille(Jeu j, int nb_mouvement)
{
    std::map<std::string,Jeu> map_jeu;
    
    if(nb_mouvement == 0)
    {
        j.deplacerVoiture(j.joueur,GAUCHE);
    }
    else
    {
        for(int i = -1; i < (int)j.voitures.size(); i++)
        {
            char id_a_bouger;

            if(i == -1)
                id_a_bouger = j.joueur.getId();
            else
                id_a_bouger = j.voitures[i].getId();
            
            Direction direction_a_faire[2];

            if( j.getVoitureParId(id_a_bouger)->getOrientation() == VERTICAL )
            {
                direction_a_faire[0] = HAUT;
                direction_a_faire[1] = BAS;
            }
            else
            {
                direction_a_faire[0] = GAUCHE;
                direction_a_faire[1] = DROITE;
            }

            for(int k = 0; k < 2; k++)
            {
                Jeu copie_jeu = j;
                
                if( copie_jeu.peutDeplacerVoiture(*copie_jeu.getVoitureParId(id_a_bouger),direction_a_faire[k]))
                {
                    copie_jeu.deplacerVoiture(*copie_jeu.getVoitureParId(id_a_bouger),direction_a_faire[k]);
                    
                    std::map<std::string,Jeu>::iterator test_presence = map_jeu.find(copie_jeu.grille.encode());
                    if(test_presence == map_jeu.end())
                        map_jeu.insert(std::make_pair( copie_jeu.grille.encode() ,copie_jeu ));
                }
            }
        }

        std::map<std::string,Jeu>::iterator iterateur = map_jeu.begin();

        while(iterateur != map_jeu.end())
        {
			bool insertion = false;
			Jeu jeu_a_tester = iterateur->second;

            Grabator g;
            g.resoudre_largeur(jeu_a_tester);

            for(int l = 0; l < g.solutions.size(); l++)
                if( Grabator::nombreMouvement(g.solutions[l]) == nb_mouvement + 1)
                    return jeu_a_tester;
                    
            for(int i2 = -1; i2 < (int)jeu_a_tester.voitures.size(); i2++)
            {
                char id_a_bouger2;

                if(i2 == -1)
                    id_a_bouger2 = jeu_a_tester.joueur.getId();
                else
                    id_a_bouger2 = jeu_a_tester.voitures[i2].getId();
                
                Direction direction_a_faire2[2];

                if( jeu_a_tester.getVoitureParId(id_a_bouger2)->getOrientation() == VERTICAL )
                {
                    direction_a_faire2[0] = HAUT;
                    direction_a_faire2[1] = BAS;
                }
                else
                {
                    direction_a_faire2[0] = GAUCHE;
                    direction_a_faire2[1] = DROITE;
                }

                for(int k2 = 0; k2 < 2; k2++)
                {
                    Jeu copie_jeu2 = jeu_a_tester;
                    
                    if( copie_jeu2.peutDeplacerVoiture(*copie_jeu2.getVoitureParId(id_a_bouger2),direction_a_faire2[k2]))
                    {
                        copie_jeu2.deplacerVoiture(*copie_jeu2.getVoitureParId(id_a_bouger2),direction_a_faire2[k2]);
                        
                        std::map<std::string,Jeu>::iterator test_presence = map_jeu.find(copie_jeu2.grille.encode());
						if (test_presence == map_jeu.end())
						{
							map_jeu.insert(std::make_pair(copie_jeu2.grille.encode(), copie_jeu2));
							insertion = true;
						}
                    }
                }
            }

			if (insertion)
				iterateur = map_jeu.begin();
			else
				iterateur++;
        }
    }

    return j;
};

Jeu Grabator::genererGrille(int n)
{
    Jeu j("puzzle.txt");
    std::queue<Mouvement> file_mouvement;

    if(n < 0 || n > 15)
        return j;

    resoudre_largeur(j);

    if(solutions.size() > 0)
        file_mouvement = solutions[0];
    else
        return j;

    while(file_mouvement.size() > 0)
    {
        Mouvement m = file_mouvement.front();
        j.deplacerVoiture(*j.getVoitureParId(m.id),m.direction);

        file_mouvement.pop();
    }

    j.afficher();

    int nb_mouvement = 0;

    while(nb_mouvement < n)
    {
        j = sub_genererGrille(j,nb_mouvement);

        Grabator g2;
        g2.resoudre_largeur(j);

		for (int i = 0; i < g2.solutions.size(); i++)
			if (Grabator::nombreMouvement(g2.solutions[i]) == nb_mouvement + 1)
				nb_mouvement++;
    }

    return j;
}

void Grabator::afficherSolutions()
{
    std::cout << solutions.size() << " SOLUTIONS TROUVEES" << std::endl;
    
    for(int i = 0; i < solutions.size(); i++)
    {
        std::cout << "CHEMIN N°" << (i+1) << " : " << std::endl;
        std::queue<Mouvement> copie = solutions[i];
        while(copie.size() > 0)
        {
            Mouvement m = copie.front();
            std::cout << m.getId() << " : " << m.getDirection() << std::endl;
            copie.pop();
        }
    }
}
