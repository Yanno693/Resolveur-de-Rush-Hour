Yannick KWANSA  
Vincent LEVEQUE

# Rush Hour

Ce projet a pour but de résoudre le jeu Rush Hour (Ou Unblock Me), un jeu de plateau ou le but est de sortir une voiture d'une grille en déplacant d'autre voitures.

# Le projet

- Le projet a été testé et compilé sur Linux (Ubuntu et Fedora)
- Le projet a aussi été testé et compilé sur Windows (Code::Blocks et Visual Studio)
- Il y a deux version du Rush Hour dans se projet : Une version normal en ligne de code, et une version graphique
- Il y a deux méthodes de resolutions : Une methode de résolution en largeur et une méthode de résolution recursive
- Pour compiler la version graphique, il faut certaines librairies particulières qui ne sont pas toujours installées, pour les installer, lancez la commande suivant :

```
sudo apt install libx11-dev libpng-dev libgl1-mesa-dev
```

### Composition de l'archive

- Enum.hpp : Un ensemble d'enum représentant les directions que peuvent prendre les voitures, et l'orientation des voitures
- Voiture : Une voiture pouvant etre placé dans un Jeu, et pouvant se déplacer
- Case : Une case dune Grille (utilise pour savoir si une case est occupée par un voiture ou pas)
- Grille : Une réprésentation dans une tableau 2D des voitures
- Mouvement : Une classe repésentant un mouvement, c'est a dire une identifiant de voiture et une direction de déplacement (On peut les enfiler pour avoir une suite de mouvements)
- Jeu : Une situation de jeu composé d'une Grille, et d'un ensemble de Voitures disposées dans cette Grille
- Grabator : Le résolveur qui va chercher à resoudre un Jeu
- rushhour.cpp : Le point d'entré du programme
- Rushhour_graphique.cpp : Le point d'entreé dans le version graphique du programme
- olcPixelGameEngine.h : Un simple fichier d'en-tete permettant de facilement créer une fênetre et de dessiner dedans

Vous pouvez compilez les deux projet simultanément sur Linux avec la commande suivante :  

```
make
```

Vous pouvez aussi générer une projet sur une autre plateforme à l'aide de CMake

## Résolution en largeur

Le pricipe est de bouger toutes les voitures de la grille une fois pour pour recuperer de nouvelles situations de jeu, et de ses nouvelles sutiations, re-bouger les voitures une fois, etc...  
Ainsi, on teste toutes le grille en 1 mouvement, puis 2, puis 3, ... jusqu'à arriver a une situation ou le joueur se trouve à la ligne d'arrivée.

- Pour eviter les recherches infini dans les grilles impossible a résoudre, on limite le nombre de mouvement a maximum 100

## Résolution récursive

L'idée est de partie du postulat qu'on veut bouger le joueur  
- Si la voie est libre, la joueur va jusqu'a la ligne d'arrivée
- Sinon, il demande de degager la voiture qui le gène
- La voiture qui gène le joueur va soit dégager la case s'il peut, soit demander a une autre voiture qui le gène de dégager pour que lui même puisse degager la place pour le joueur, et ainsi de suite.
  
A l'heure actuelle, cette méthode de résolution ne marche pas sur toutes les grilles (notamment la grille donnée au début du projet, malheureusement)

# Rush Hour Console

Cette version du Rush Hour est la version basique du résolveur, et peut prendre deux arguments au lancement:  

- Une premier, le nom du fichier texte a ouvrir pour generer la grille
- En deuxième, une drapeau "-r" ou "--recursive" pour utiliser la methode recursive au lieu de la methode par largeur.

Exemple de commande :  

```
./rushhour puzzle.txt -r
```

# Rush Hour Graphique

Cette version du Rush Hour offre une fênetre permettant de créer sa propre grille en de chercher à la résoudre.  
Vous pouvez passez une fichier texte en paremètre de programme pour démarrer directement avec une grille personnalisée.

## Commandes

- Clique Gauche/Droit : Placer/Supprimer une voiture de la grille
- R : Reinitialiser la fenetre avec la grille donnée en debut d'éxécution
- T : Changer la taille de la voiture à placer
- O : Changer l'orientation de la voiture à placer
- Espace : Changer des solution a la grille (Méthode Largeur)
- Maj/Shift : Changer des solution a la grille (Méthode Récursive)
  
En bas sont indiqués le nombre de solutions trouvées, ainsi que le nombre de mouvement pour la solution séléctionnée

- Flèche Droite/Gauche : Sélectionner une autre solution

## A propos

Même si nous n'avons pour l'instant pas quelque chose qui marche parfaitement (notemment la recherche recursive), nous avons beaucoup appris au cours de la conception de le projet.  
Nous esperons que vous apprécirez ces lignes de C++ écrites à la sueur de nos fronts.  

OneLoneCoder (aussi connu sous le pseudo Javidx9) est un Youtubeur faisant différents tutoriaux en programmation, et à créer pour ses tutoriaux olcPixelGameEngine.h, un simple fichier d'en-tête permettant de rapidement créer des fenêtre et d'y afficher des choses.  
Ce fichier est sous licences GNU GPLv3 et peut être utilisé dans tout projet si son créateur s'y trouve acrédité (pseudonyme en haut de la fenêtre de la version graphique).

## Lien Utiles

- Le jeu de société [Rush Hour](https://fr.wikipedia.org/wiki/Rush_hour_(casse-t%C3%AAte))
- Pour tous connaître du C++ et de la librairie standard : [cplusplus.com](http://www.cplusplus.com/) ou encore [cppreference.com](https://fr.cppreference.com/w/)
- Chaîne Youtube de [OneLoneCoder](https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA)
- Site web de [OneLoneCoder](https://onelonecoder.com/)
- Le compilateur C/C++ [GCC](https://gcc.gnu.org/)
- Un editeur de texte sympa sur Windows/Max/Linux : [Visual Studio Code](https://code.visualstudio.com/)
- Pour travailler sur avec un IDE complet sur Windows : [Visual Studio](https://visualstudio.microsoft.com/fr/vs/) et [Code::Blocks](http://www.codeblocks.org/)