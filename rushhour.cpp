#include "grabator.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    Jeu j;
    bool recursive = false;
    std::string flag = "";
    Grabator g;

    if(argc > 1)
        j = Jeu(argv[1]);

    if(argc > 2)
        flag = argv[2];

    if(flag == "-r" || flag == "--recursive")
        recursive = true;

    if(recursive)
        g.resoudre_recursive(j);
    else
        g.resoudre_largeur(j);

	g.afficherSolutions();

    return 0;
}
