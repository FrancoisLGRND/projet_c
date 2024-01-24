##Commande pour compiler:
gcc -o flappy bird main.c -std=c11 -Wall -Wextra $(pkg-config --cflags --libs sdl2 SDL2_ttf glib-2.0) -Wl,-subsystem,windows -DSDL_MAIN_HANDLED

##Commande pour télécharger les libs:
pacman -S mingw-w64-x86_64-SDL2 
pacman -S mingw-w64-x86_64-SDL2_ttf
pacman -S mingw-w64-x86_64-glib2

##Utilisation de 2 structures Bouton et Tuyau pour simplifier la création de forme avec SDL
##Utilisation de fonction de "page" afin de navigué dans le jeu (menu, pseudo, jouer, diff)
##Lecture du fichier de configuration ini afin de choisir la vitesse du jeu pour chaque difficuté
##Chargement des images en BMP
##Sauvegarde du meilleur score dans un fichier txt

##Touches du programme:
Echap pour quitter le programme ou quitter la partie si en jeu
Bar espace pour monter pendant une partie
Tout le clavier pour écrire son pseudo dans la page pseudo