#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    SDL_Rect rect;
    SDL_Color rectColor;
    SDL_Color textColor;
    char texte[50];
    TTF_Font *font;
} Boutton;

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

char *menu(SDL_Event event, Boutton menuBoutton1, Boutton menuBoutton2, int *running) {
    switch (event.type) {
    case SDL_QUIT:
        *running = 0;
        return "menu";
        break;
    case SDL_MOUSEBUTTONDOWN:
        // Vérifier si le clic de souris est à l'intérieur d'un bouton
        if (isMouseInsideButton(event, menuBoutton1)) {
            printf("Clic sur le bouton!\n");
            return "menu";
            // Vous pouvez ajouter du code pour réagir au clic ici
        }
        if (isMouseInsideButton(event, menuBoutton2)) {
            printf("azdijiazdazd!\n");
            return "menu";
            // Vous pouvez ajouter du code pour réagir au clic ici
        }
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            *running = 0;
            return "menu";
            break;
        default:
            return "menu";
            break;
        }
        break;
    default:
        return "menu";
        break;
    }
    return "menu";
}


int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }
    TTF_Init();

    SDL_Window *fenetre = SDL_CreateWindow("Ma fenêtre", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (fenetre == NULL) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("Butler_Regular.ttf", 24);

    Boutton menuBoutton1 = initButton(100, 100, 200, 50, (SDL_Color){0, 0, 0, 255}, (SDL_Color){255, 255, 255, 255}, "Couille :)", font);
    Boutton menuBoutton2 = initButton(500, 100, 200, 50, (SDL_Color){0, 0, 0, 255}, (SDL_Color){255, 255, 255, 255}, "celestin", font);

    // Chargement de l'image BMP
    // ...

    SDL_Surface *surfaceImage = SDL_LoadBMP("menu.bmp");
    if (surfaceImage == NULL) {
        printf("Erreur lors du chargement de l'image : %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return 1;
    }

    // Créer une texture à partir de la surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceImage);
    SDL_FreeSurface(surfaceImage);

    // Boucle d'événements SDL
    SDL_Event event;
    int running = 1;
    char *page = "menu"; 

    while (running) {
        while (SDL_PollEvent(&event)) {
            page = menu(event, menuBoutton1, menuBoutton2, &running);
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

        // Mettre à jour l'affichage
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
