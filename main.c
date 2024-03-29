#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <glib-2.0/glib.h>

typedef struct
{
    SDL_Rect rect;
    SDL_Color rectColor;
    SDL_Color textColor;
    char texte[50];
    TTF_Font *font;
} Bouton;

typedef struct
{
    SDL_Rect rect;
    SDL_Color rectColor;
} Tuyau;

enum Page
{
    MENU,
    PSEUDO,
    JOUER,
    DIFF
};

Bouton initButton(int x, int y, int width, int height, SDL_Color rectColor, SDL_Color textColor, const char *texte, TTF_Font *font)
{
    Bouton bouton;
    bouton.rect.x = x;
    bouton.rect.y = y;
    bouton.rect.w = width;
    bouton.rect.h = height;
    bouton.rectColor = rectColor;
    bouton.textColor = textColor;
    strncpy(bouton.texte, texte, sizeof(bouton.texte));
    bouton.font = font;

    return bouton;
}

Tuyau initTuyau(int x, int y, int width, int height, SDL_Color rectColor)
{
    Tuyau tuyau;
    tuyau.rect.x = x;
    tuyau.rect.y = y;
    tuyau.rect.w = width;
    tuyau.rect.h = height;
    tuyau.rectColor = rectColor;

    return tuyau;
}

int nombreRandom(int min, int max)
{
    return min + rand() % (max - min + 1);
}

int isMouseInsideButton(SDL_Event event, Bouton boutton)
{
    int mouseX = event.button.x;
    int mouseY = event.button.y;

    return (mouseX >= boutton.rect.x && mouseX <= boutton.rect.x + boutton.rect.w && mouseY >= boutton.rect.y && mouseY <= boutton.rect.y + boutton.rect.h);
}

void drawTextOnButton(SDL_Renderer *renderer, Bouton *boutton)
{
    // Taille max du texte
    int maxTextWidth = boutton->rect.w;
    int maxTextHeight = boutton->rect.h;
    int margin = 10;

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(boutton->font, boutton->texte, boutton->textColor, maxTextWidth);

    int centerX = boutton->rect.x + margin + (maxTextWidth - 2 * margin - textSurface->w) / 2;
    int centerY = boutton->rect.y + margin + (maxTextHeight - 2 * margin - textSurface->h) / 2;

    // Conversion surface du texte en texture
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect destRect = {centerX, centerY, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textureTexte);
}

enum Page menu(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Event event, Bouton menuBoutton1, Bouton menuBoutton2, Bouton menuBoutton3, int *running, Bouton highestScoreRect, char highestScoreTexte[16])
{
    enum Page page = MENU;
    switch (event.type)
    {
    case SDL_QUIT:
        *running = 0;
        page = MENU;
        break;
    case SDL_MOUSEBUTTONDOWN:
        // Vérifier si le clic de souris est à l'intérieur d'un bouton
        if (isMouseInsideButton(event, menuBoutton1))
        {
            page = JOUER;
        }
        if (isMouseInsideButton(event, menuBoutton2))
        {
            page = PSEUDO;
        }
        if (isMouseInsideButton(event, menuBoutton3))
        {
            page = DIFF;
        }
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
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

    strncpy(highestScoreRect.texte, highestScoreTexte, 16);
    SDL_SetRenderDrawColor(renderer, highestScoreRect.rectColor.r, highestScoreRect.rectColor.g, highestScoreRect.rectColor.b, highestScoreRect.rectColor.a);
    SDL_RenderFillRect(renderer, &highestScoreRect.rect);
    drawTextOnButton(renderer, &highestScoreRect);
    return page;
}

enum Page pseudo(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Event event, Bouton valider, Bouton *inputPseudo, int *running, char pseudoJoueur[50], Bouton highestScoreRect, char highestScoreTexte[16])
{
    enum Page page = PSEUDO;
    switch (event.type)
    {
    case SDL_QUIT:
        *running = 0;
        page = MENU;
        break;
    case SDL_MOUSEBUTTONDOWN:
        // Vérifier si le clic de souris est à l'intérieur d'un bouton
        if (isMouseInsideButton(event, valider))
        {
            if (strcmp(pseudoJoueur, " ") == 0)
            {
                strcpy(pseudoJoueur, "Guest");
                strncpy(inputPseudo->texte, pseudoJoueur, 50);
            }
            page = MENU;
        }
        break;
    case SDL_TEXTINPUT:
        strncat(pseudoJoueur, event.text.text, 50 - strlen(pseudoJoueur) - 1);
        strncpy(inputPseudo->texte, pseudoJoueur, 50);
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            *running = 0;
            page = MENU;
            break;
        case SDLK_BACKSPACE:
            if (strlen(pseudoJoueur) > 0)
            {
                pseudoJoueur[strlen(pseudoJoueur) - 1] = '\0';
            }
            if (strlen(pseudoJoueur) == 0)
            {
                strcpy(pseudoJoueur, " ");
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

    strncpy(highestScoreRect.texte, highestScoreTexte, 16);
    SDL_SetRenderDrawColor(renderer, highestScoreRect.rectColor.r, highestScoreRect.rectColor.g, highestScoreRect.rectColor.b, highestScoreRect.rectColor.a);
    SDL_RenderFillRect(renderer, &highestScoreRect.rect);
    drawTextOnButton(renderer, &highestScoreRect);
    return page;
}

enum Page jouer(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Event event, int width, int height, int *running, char pseudoJoueur[50], int *highestScore, float speed)
{
    enum Page page = JOUER;
    int alive = 1;
    int score = 0;
    char scoreStr[16] = "Score : ";
    sprintf(scoreStr, "Score : %d", score);
    srand((unsigned int)time(NULL));
    speed;

    int espace_y = height / 6;
    int espace_x = width / 4;

    int joueurX = width / 8;
    int joueurY = height / 2;
    int joueurWidth = width / 20;
    int joueurHeight = height / 10;
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
    Bouton pseudoTexte = initButton(joueurX, joueurY - joueurHeight / 2, joueurWidth, joueurHeight / 2, (SDL_Color){0, 0, 0, 80}, (SDL_Color){255, 255, 255, 255}, pseudoJoueur, font);
    Bouton scoreTexte = initButton(0, 0, width / 8, height / 12, (SDL_Color){0, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, scoreStr, font);

    SDL_Surface *imageJoueur = SDL_LoadBMP("bird.bmp");
    SDL_Texture *textureJoueur = SDL_CreateTextureFromSurface(renderer, imageJoueur);
    SDL_Rect PositionJoueur = {joueurX, joueurY, joueurWidth, joueurHeight};
    SDL_FreeSurface(imageJoueur);

    // Initialisation des coordonnées des tuyaux
    for (int i = 0; i < nombreTuyaux; ++i)
    {
        tuyauX[i] = width + i * espacementEntreTuyaux;
        tuyauY[i] = nombreRandom(height / 4, height / 2 + height / 4);
    }

    while (alive)
    {
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                *running = 0;
                alive = 0;
                page = MENU;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
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
                if (event.key.keysym.sym == SDLK_SPACE)
                {
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

        // Affichage image
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(renderer, textureJoueur, NULL, &PositionJoueur);

        // Affichage pseudo
        SDL_SetRenderDrawColor(renderer, pseudoTexte.rectColor.r, pseudoTexte.rectColor.g, pseudoTexte.rectColor.b, pseudoTexte.rectColor.a);
        SDL_RenderFillRect(renderer, &pseudoTexte.rect);
        drawTextOnButton(renderer, &pseudoTexte);

        // Affichage score
        SDL_SetRenderDrawColor(renderer, scoreTexte.rectColor.r, scoreTexte.rectColor.g, scoreTexte.rectColor.b, scoreTexte.rectColor.a);
        SDL_RenderFillRect(renderer, &scoreTexte.rect);
        drawTextOnButton(renderer, &scoreTexte);

        // update position joueur
        if (spaceKey == 1)
        {
            PositionJoueur.y -= 5;
            pseudoTexte.rect.y -= 5;
        }
        else
        {
            PositionJoueur.y += 5;
            pseudoTexte.rect.y += 5;
        }

        for (int i = 0; i < nombreTuyaux; ++i)
        {
            tuyauX[i] -= 5;

            // Suppression des tuyaux à gauche
            if (tuyauX[i] + width / 20 < 0)
            {
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

            // On vérifie si le joueur réalise une collision avec les tuyaux ou avec le sol
            if (((joueurX + joueurWidth > tuyauX[i] && joueurX + joueurWidth < tuyauX[i] + tuyauWidth) && (PositionJoueur.y + joueurHeight < tuyauY[i] || PositionJoueur.y + joueurHeight > tuyauY[i] + espace_y)) || PositionJoueur.y > height)
            {
                alive = 0;
            }

            // Changement vitesse

            if (joueurX > tuyauX[i] && joueurX < tuyauX[i]+5)
            {
                speed -= 1;
                score += 1; 
                sprintf(scoreStr, "Score : %d", score);
                strncpy(scoreTexte.texte, scoreStr, 16);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(speed);
    }
    if (score > *highestScore)
    {
        *highestScore = score;
    }
    return MENU;
}

enum Page diff(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Event event, Bouton diff1, Bouton diff2, Bouton diff3, int *running, Bouton highestScoreRect, char highestScoreTexte[16], int *diffChoice)
{
    enum Page page = MENU;
    switch (event.type)
    {
    case SDL_QUIT:
        *running = 0;
        page = MENU;
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (isMouseInsideButton(event, diff1))
        {
            *diffChoice = 1;
            page = MENU;
        }
        if (isMouseInsideButton(event, diff2))
        {
            *diffChoice = 2;
            page = MENU;
        }
        if (isMouseInsideButton(event, diff3))
        {
            *diffChoice = 3;
            page = MENU;
        }
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            *running = 0;
            page = MENU;
            break;
        default:
            page = DIFF;
            break;
        }
        break;
    default:
        page = DIFF;
        break;
    }
    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Afficher l'image
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Dessiner le bouton
    SDL_SetRenderDrawColor(renderer, diff1.rectColor.r, diff1.rectColor.g, diff1.rectColor.b, diff1.rectColor.a);
    SDL_RenderFillRect(renderer, &diff1.rect);
    drawTextOnButton(renderer, &diff1);

    SDL_SetRenderDrawColor(renderer, diff2.rectColor.r, diff2.rectColor.g, diff2.rectColor.b, diff2.rectColor.a);
    SDL_RenderFillRect(renderer, &diff2.rect);
    drawTextOnButton(renderer, &diff2);

    SDL_SetRenderDrawColor(renderer, diff3.rectColor.r, diff3.rectColor.g, diff3.rectColor.b, diff3.rectColor.a);
    SDL_RenderFillRect(renderer, &diff3.rect);
    drawTextOnButton(renderer, &diff3);

    strncpy(highestScoreRect.texte, highestScoreTexte, 16);
    SDL_SetRenderDrawColor(renderer, highestScoreRect.rectColor.r, highestScoreRect.rectColor.g, highestScoreRect.rectColor.b, highestScoreRect.rectColor.a);
    SDL_RenderFillRect(renderer, &highestScoreRect.rect);
    drawTextOnButton(renderer, &highestScoreRect);
    return page;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
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

    Bouton menuBoutton1 = initButton(width / 2 - boutonWidth / 2, boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, "Jouer", font);
    Bouton menuBoutton2 = initButton(width / 2 - boutonWidth / 2, 3 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, "Pseudo", font);
    Bouton menuBoutton3 = initButton(width / 2 - boutonWidth / 2, 5 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, "Difficulté", font);

    int diffChoice = 1;
    float speed;
    GKeyFile *keyfile = g_key_file_new();
    GError *error = NULL;
    if (!g_key_file_load_from_file(keyfile, "config.ini", G_KEY_FILE_NONE, &error))
    {
        g_printerr("Erreur fichier non trouvée : %s\n", error->message);
        g_error_free(error);
        g_key_file_free(keyfile);
        return 1;
    }
    char *facile = g_key_file_get_string(keyfile, "niveau", "facile", NULL);
    char *moyen = g_key_file_get_string(keyfile, "niveau", "moyen", NULL);
    char *difficile = g_key_file_get_string(keyfile, "niveau", "difficile", NULL);

    Bouton diff1 = initButton(width / 2 - boutonWidth / 2, boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 255, 0, 100}, (SDL_Color){255, 255, 255, 255}, "Facile", font);
    Bouton diff2 = initButton(width / 2 - boutonWidth / 2, 3 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){255, 255, 0, 100}, (SDL_Color){255, 255, 255, 255}, "Moyen", font);
    Bouton diff3 = initButton(width / 2 - boutonWidth / 2, 5 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){255, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, "Difficile", font);

    Bouton valider = initButton(width / 2 - boutonWidth / 2, boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, "Valider", font);
    char pseudoJoueur[50] = "Guest";
    Bouton inputPseudo = initButton(width / 2 - boutonWidth / 2, 3 * boutonHeight, boutonWidth, boutonHeight, (SDL_Color){0, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, pseudoJoueur, font);

    // récupération du meilleur score
    int highestScore;
    char highestScoreStr[16];
    FILE *fichierRead = fopen("highestScore.txt", "r");
    fgets(highestScoreStr, sizeof(highestScoreStr), fichierRead);
    highestScore = atoi(highestScoreStr);
    fclose(fichierRead);
    char highestScoreTexte[16];
    sprintf(highestScoreTexte, "Highest Score : %d", highestScore);
    Bouton highestScoreRect = initButton(0, 0, width / 8, height / 12, (SDL_Color){0, 0, 0, 100}, (SDL_Color){255, 255, 255, 255}, highestScoreTexte, font);

    SDL_Event event;

    int running = 1;
    enum Page page = MENU;

    while (running)
    {
        FILE *fichierRead = fopen("highestScore.txt", "r");
        fgets(highestScoreStr, sizeof(highestScoreStr), fichierRead);
        highestScore = atoi(highestScoreStr);
        fclose(fichierRead);
        sprintf(highestScoreTexte, "Highest Score : %d", highestScore);
        switch (diffChoice)
        {
        case 1:
            speed = atoi(facile);
            break;
        case 2:
            speed = atoi(moyen);
            break;
        case 3:
            speed = atoi(difficile);
            break;
        }
        while (SDL_PollEvent(&event))
        {
            switch (page)
            {
            case MENU:
                page = menu(renderer, texture, event, menuBoutton1, menuBoutton2, menuBoutton3, &running, highestScoreRect, highestScoreTexte);
                break;
            case PSEUDO:
                page = pseudo(renderer, texture, event, valider, &inputPseudo, &running, pseudoJoueur, highestScoreRect, highestScoreTexte);
                break;
            case JOUER:
                page = jouer(renderer, texture, event, width, height, &running, pseudoJoueur, &highestScore, speed);
                break;
            case DIFF:
                page = diff(renderer, texture, event, diff1, diff2, diff3, &running, highestScoreRect, highestScoreTexte, &diffChoice);
                break;
            }
        }
        FILE *fichierWrite = fopen("highestScore.txt", "w");
        fprintf(fichierWrite, "%d", highestScore);
        fclose(fichierWrite);
        SDL_RenderPresent(renderer);
    }

    // Libération des ressources

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    free(facile);
    free(moyen);
    free(difficile);

    return 0;
}
