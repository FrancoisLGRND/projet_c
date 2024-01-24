#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color rectColor;
    SDL_Color textColor;
    char texte[50];
    TTF_Font *font;
} Boutton;

typedef struct {
    SDL_Rect rect;
    SDL_Color rectColor;
} Tuyau;

enum Page {
    MENU,
    PSEUDO,
    JOUER,
};

Boutton initButton(int x, int y, int width, int height, SDL_Color rectColor, SDL_Color textColor, const char *texte, TTF_Font *font) {
    Boutton boutton;
    boutton.rect.x = x;
    boutton.rect.y = y;
    boutton.rect.w = width;
    boutton.rect.h = height;
    boutton.rectColor = rectColor;
    boutton.textColor = textColor;
    strncpy(boutton.texte, texte, sizeof(boutton.texte));
    boutton.font = font;

    return boutton;
}

Tuyau initTuyau(int x, int y, int width, int height, SDL_Color rectColor) {
    Tuyau tuyau;
    tuyau.rect.x = x;
    tuyau.rect.y = y;
    tuyau.rect.w = width;
    tuyau.rect.h = height;
    tuyau.rectColor = rectColor;

    return tuyau;
}

int nombreRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

int isMouseInsideButton(SDL_Event event, Boutton boutton) {
    int mouseX = event.button.x;
    int mouseY = event.button.y;

    return (mouseX >= boutton.rect.x && mouseX <= boutton.rect.x + boutton.rect.w && mouseY >= boutton.rect.y && mouseY <= boutton.rect.y + boutton.rect.h);
}

void drawTextOnButton(SDL_Renderer *renderer, Boutton *boutton) {
    int maxTextWidth = boutton->rect.w;  // Largeur maximale du texte (largeur du bouton)
    int maxTextHeight = boutton->rect.h; // Hauteur maximale du texte (hauteur du bouton)
    int margin = 10;

    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(boutton->font, boutton->texte, boutton->textColor, maxTextWidth);

    int centerX = boutton->rect.x + margin + (maxTextWidth - 2 * margin - textSurface->w) / 2;
    int centerY = boutton->rect.y + margin + (maxTextHeight - 2 * margin - textSurface->h) / 2;

    // Dessiner le texte sur le bouton
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect destRect = {centerX, centerY, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);
    SDL_FreeSurface(textSurface);

    // Libérer la texture du texte
    SDL_DestroyTexture(textureTexte);
}

enum Page menu(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Event event, Boutton menuBoutton1, Boutton menuBoutton2, Boutton menuBoutton3, int *running) {
    enum Page page = MENU;
    switch (event.type) {
    case SDL_QUIT:
        *running = 0;
        page = MENU;
        break;
    case SDL_MOUSEBUTTONDOWN:
        // Vérifier si le clic de souris est à l'intérieur d'un bouton
        if (isMouseInsideButton(event, menuBoutton1)) {
            page = JOUER;
            // Vous pouvez ajouter du code pour réagir au clic ici
        }
        if (isMouseInsideButton(event, menuBoutton2)) {
            page = PSEUDO;
            // Vous pouvez ajouter du code pour réagir au clic ici
        }
        if (isMouseInsideButton(event, menuBoutton3)) {
            page = MENU;
            // Vous pouvez ajouter du code pour réagir au clic ici
        }
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            *running = 0;
            page = MENU;
            break;
        default:
            page = MENU;
            break;
        }
        break;
    default:
        page = MENU;
        break;
    }
    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Afficher l'image
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Dessiner le bouton
    SDL_SetRenderDrawColor(renderer, menuBoutton1.rectColor.r, menuBoutton1.rectColor.g, menuBoutton1.rectColor.b, menuBoutton1.rectColor.a);
    SDL_RenderFillRect(renderer, &menuBoutton1.rect);
    drawTextOnButton(renderer, &menuBoutton1);

    SDL_SetRenderDrawColor(renderer, menuBoutton2.rectColor.r, menuBoutton2.rectColor.g, menuBoutton2.rectColor.b, menuBoutton2.rectColor.a);
    SDL_RenderFillRect(renderer, &menuBoutton2.rect);
    drawTextOnButton(renderer, &menuBoutton2);

    SDL_SetRenderDrawColor(renderer, menuBoutton3.rectColor.r, menuBoutton3.rectColor.g, menuBoutton3.rectColor.b, menuBoutton3.rectColor.a);
    SDL_RenderFillRect(renderer, &menuBoutton3.rect);
    drawTextOnButton(renderer, &menuBoutton3);
    return page;
}

enum Page pseudo(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Event event, Boutton valider, Boutton *inputPseudo, int *running, char pseudoJoueur[50]) {
    enum Page page = PSEUDO;
    switch (event.type) {
    case SDL_QUIT:
        *running = 0;
        page = MENU;
        break;
    case SDL_MOUSEBUTTONDOWN:
        // Vérifier si le clic de souris est à l'intérieur d'un bouton
        if (isMouseInsideButton(event, valider)) {
            page = MENU;
            // Vous pouvez ajouter du code pour réagir au clic ici
        }
        break;
    case SDL_TEXTINPUT:
        // Si un texte est entré, ajoutez le texte au buffer
        strncat(pseudoJoueur, event.text.text, 50 - strlen(pseudoJoueur) - 1);
        strncpy(inputPseudo->texte, pseudoJoueur, 50);
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                *running = 0;
                page = MENU;
                break;
            case SDLK_BACKSPACE:
                if (strlen(pseudoJoueur) > 0) {           
                    pseudoJoueur[strlen(pseudoJoueur) - 1] = '\0';
                    }
                if (strlen(pseudoJoueur) == 0){
                    pseudoJoueur = " ";
                    }
                strncpy(inputPseudo->texte, pseudoJoueur, 50);
                
                break;
            default:
                page = PSEUDO;
                break;
        }
        break;
    default:
        page = PSEUDO;
        break;
    }
    
    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Afficher l'image
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Dessiner le bouton
    SDL_SetRenderDrawColor(renderer, valider.rectColor.r, valider.rectColor.g, valider.rectColor.b, valider.rectColor.a);
    SDL_RenderFillRect(renderer, &valider.rect);
    drawTextOnButton(renderer, &valider);

    SDL_SetRenderDrawColor(renderer, inputPseudo->rectColor.r, inputPseudo->rectColor.g, inputPseudo->rectColor.b, inputPseudo->rectColor.a);
    SDL_RenderFillRect(renderer, &inputPseudo->rect);
    drawTextOnButton(renderer, inputPseudo);
    return page;


}

enum Page jouer(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Event event, int width, int height, int *running, char pseudoJoueur[50]) {
    enum Page page = JOUER;
    int alive = 1;
    int score = 0;
    srand((unsigned int)time(NULL));
    float delayMS = 10;
   
    int espace_y = height / 6;
    int espace_x = width / 4;

    int joueurX = width / 8 ;
    int joueurY = height / 2;
    int joueurWidth = width/20;
    int joueurHeight = height/10;
    int tuyauWidth = width / 20;
    int spaceKey = 0;
    

    // Calculer le nombre de tuyaux en fonction de la largeur de la fenêtre
    int nombreTuyaux = width / (espace_x + width / 20);

    // Calculer l'espacement nécessaire entre chaque paire de tuyaux
    int espacementTotal = width - espace_x;
    int espacementEntreTuyaux = espacementTotal / (nombreTuyaux - 1);

    float tuyauX[nombreTuyaux];
    int tuyauY[nombreTuyaux];
    Tuyau arrayTuyauHaut[nombreTuyaux];
    Tuyau arrayTuyauBas[nombreTuyaux];
    TTF_Font *font = TTF_OpenFont("Butler_Regular.ttf", 24);
    Boutton pseudoText = initButton(joueurX , joueurY-joueurHeight/2, joueurWidth, joueurHeight/2, (SDL_Color){0, 0, 0, 55}, (SDL_Color){255, 255, 255, 255}, pseudoJoueur, font);

    SDL_Surface *imageJoueur = SDL_LoadBMP("bird.bmp");
    SDL_Texture *textureJoueur = SDL_CreateTextureFromSurface(renderer, imageJoueur);
    SDL_Rect PositionJoueur = {joueurX, joueurY, joueurWidth, joueurHeight};
    SDL_FreeSurface(imageJoueur);

    // Initialisation des coordonnées des tuyaux
    for (int i = 0; i < nombreTuyaux; ++i) {
        tuyauX[i] = width + i * espacementEntreTuyaux;
        tuyauY[i] = nombreRandom(height / 4, height / 2 + height / 4);

    }

    while (alive) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    *running = 0;
                    alive = 0;
                    page = MENU;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            alive = 0;
                            page = MENU;
                            break;
                        case SDLK_SPACE:
                            spaceKey = 1;
                            break;
                        default:
                            page = MENU;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    if(event.key.keysym.sym == SDLK_SPACE){
                        spaceKey = 0;
                    }        
                    break;
                default:
                    page = JOUER;
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Afficher l'image
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(renderer, textureJoueur, NULL, &PositionJoueur);

        SDL_SetRenderDrawColor(renderer, pseudoText.rectColor.r, pseudoText.rectColor.g, pseudoText.rectColor.b, pseudoText.rectColor.a);
        SDL_RenderFillRect(renderer, &pseudoText.rect);
        drawTextOnButton(renderer, &pseudoText);
        
        //update position joueur
        if (spaceKey == 1) {
            PositionJoueur.y -= 5;
            pseudoText.rect.y -= 5;
        } else {
            PositionJoueur.y += 5;
            pseudoText.rect.y += 5;

        }
 
        
        for (int i = 0; i < nombreTuyaux; ++i) {
            tuyauX[i] -= 5;

            // Suppression des tuyaux à gauche
            if (tuyauX[i] + width / 20 < 0) {
                tuyauX[i] = width;
                tuyauY[i] = nombreRandom(height / 4, height / 2 + height / 4);
 
            }
            // Dessiner le tuyau
             arrayTuyauHaut[i] = initTuyau(tuyauX[i], 0, tuyauWidth, tuyauY[i] - espace_y, (SDL_Color){255, 0, 0, 255});
             arrayTuyauBas[i] = initTuyau(tuyauX[i], tuyauY[i] + espace_y, tuyauWidth, height - espace_y, (SDL_Color){255, 0, 0, 255});
             
             SDL_SetRenderDrawColor(renderer, arrayTuyauHaut[i].rectColor.r, arrayTuyauHaut[i].rectColor.g, arrayTuyauHaut[i].rectColor.b, arrayTuyauHaut[i].rectColor.a);
             SDL_SetRenderDrawColor(renderer, arrayTuyauBas[i].rectColor.r, arrayTuyauBas[i].rectColor.g, arrayTuyauBas[i].rectColor.b, arrayTuyauBas[i].rectColor.a);
             
            SDL_RenderFillRect(renderer, &arrayTuyauHaut[i].rect);
            SDL_RenderFillRect(renderer, &arrayTuyauBas[i].rect);

            //On vérifie si le joueur réalise une collision avec les tuyaux ou avec le sol
            if (((joueurX+joueurWidth > tuyauX[i] && joueurX+joueurWidth < tuyauX[i] + tuyauWidth ) && (PositionJoueur.y+joueurHeight  < tuyauY[i] || PositionJoueur.y+joueurHeight > tuyauY[i] + espace_y )) || PositionJoueur.y > height) {
            alive = 0;
            }





            // Changement vitesse
            
            if (joueurX == tuyauX[i]) {
            delayMS -= 1;
            score += 1;
            printf("score : %d\n", score);
            }
          
            
            
        }
          SDL_RenderPresent(renderer);
          SDL_Delay(delayMS);
    }

    return MENU;
}


int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }
    TTF_Init();
    SDL_DisplayMode resolution;
    SDL_GetDesktopDisplayMode(0, &resolution);
    int width = resolution.w;
    int height = resolution.h;
    int boutonWidth = width / 4;
    int boutonHeight = height / 8;
    SDL_Window *fenetre = SDL_CreateWindow("Ma fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    TTF_Font *font = TTF_OpenFont("Butler_Regular.ttf", 24);
    // Création de l'image de fond 
    SDL_Surface *surfaceImage = SDL_LoadBMP("menu.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceImage);
    SDL_FreeSurface(surfaceImage);


    Boutton menuBoutton1 = initButton(width/2 - boutonWidth/2 , boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 255}, (SDL_Color){255, 255, 255, 255}, "Jouer", font);
    Boutton menuBoutton2 = initButton(width/2 - boutonWidth/2,  3 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 255}, (SDL_Color){255, 255, 255, 255}, "Pseudo", font);
    Boutton menuBoutton3 = initButton(width/2 - boutonWidth/2,  5 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 255}, (SDL_Color){255, 255, 255, 255}, "Difficultée", font);

    Boutton valider = initButton(width/2 - boutonWidth/2 , boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 255}, (SDL_Color){255, 255, 255, 255}, "Valider", font);
    char pseudoJoueur[50] = "Guest";
    Boutton inputPseudo = initButton(width/2 - boutonWidth/2,  3 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 255}, (SDL_Color){255, 255, 255, 255}, pseudoJoueur, font);
    
    




    
    // Boucle d'événements SDL
    SDL_Event event;
    
    int running = 1;
    enum Page page = MENU;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (page) {
                case MENU:
                    page = menu(renderer, texture, event, menuBoutton1, menuBoutton2, menuBoutton3, &running);
                    break;
                case PSEUDO:
                    page = pseudo(renderer, texture, event, valider, &inputPseudo, &running, pseudoJoueur);
                    printf("%s\n", pseudoJoueur);
                    break;
                case JOUER:
                    page = jouer(renderer, texture, event, width, height, &running, pseudoJoueur);
                    printf("%s\n", pseudoJoueur);
                    break;
            }
        }
        // Mettre à jour l'affichage et le texte du inputPseudo
        SDL_RenderPresent(renderer);
    }

    // Libération des ressources
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}
