#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <box2d/box2d.h>
#include <vector>
#include <random>
#include <cmath>

#include "RenderWindow.h"
#include "LoadRessource.h"
#include "Box2D_Init.h"
#include "Tray.h"

class BoxCollider : public b2ContactListener
{
public:
    std::vector<Mix_Chunk*> Sound;
    b2Body* Tray;
    //b2Body* FallingBody;



    void BeginContact(b2Contact* contact) override
    {
        std::random_device MusicRd;
        std::mt19937 genMusicRd(MusicRd());
        std::uniform_int_distribution<> disMusicRd(0, Sound.size() - 1);

        if (contact->GetFixtureA()->GetBody() == Tray && contact->GetFixtureB()->GetBody()->GetType() == b2_dynamicBody ||
            contact->GetFixtureB()->GetBody() == Tray && contact->GetFixtureA()->GetBody()->GetType() == b2_dynamicBody)
        {
            if (!Sound.empty()) {
                //Mix_PlayChannel(-1,Sound[disMusicRd(genMusicRd)],0);
            }
        }
    }
};

void SpawnFallingBody(std::vector <b2Body*>& BodiesList, SDL_Window* window, b2World& world, int WindowHeight, int WindowWidth)
{
    // int WindowWidth, WindowHeight;
    // SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);
    const float SCALE = 30.0f;
    float FallingBodyAngle = 0.0f;

    // FallingBodies
    b2PolygonShape FallingBodyShape;
    FallingBodyShape.SetAsBox((40.0f / 2.0f) / SCALE, (40.0f / 2.0f) / SCALE);


    std::random_device LinearDampingRd;
    std::mt19937 LinearDampingGenRd(LinearDampingRd());
    std::uniform_real_distribution<float> DisDampingGenRd(0.1f, 1.5f);


    /*
    std::random_device WindowHeightRd;
    std::mt19937 genRdHeigh(WindowHeightRd());
    std::uniform_int_distribution<> disHeightRd(0, WindowHeight);
    */

    b2BodyDef FallingBodyDef;
    FallingBodyDef.type = b2_dynamicBody; // (b2_staticBody, b2_dynamicBody, ou b2_kinematicBody)
    FallingBodyDef.linearDamping = DisDampingGenRd(LinearDampingGenRd) / SCALE;
    FallingBodyDef.allowSleep = false;

    b2FixtureDef FallingBodyFixtureDef;
    FallingBodyFixtureDef.shape = &FallingBodyShape;
    FallingBodyFixtureDef.density = 15.0f;
    //FallingBodyFixtureDef.friction = 0.5f; // (0 = glass, 1 = sandpaper)

    // FallingBody Spawn Random
    std::random_device WindowWidthRd;
    std::mt19937 genRdWidth(WindowWidthRd());
    std::uniform_int_distribution<> disWeightRd(0, WindowWidth);

    // Velocity Random
    std::random_device VelocityRd;
    std::mt19937 VelocityGenRd(VelocityRd());
    std::uniform_real_distribution<> disVelocityRd(-2.0f, 2.0f);

        b2Body* newBody = world.CreateBody(&FallingBodyDef);
        newBody->CreateFixture(&FallingBodyFixtureDef);
        // Vitesse de départ vers le bas + légère dérive horizontale
        newBody->SetLinearVelocity(b2Vec2(disVelocityRd(VelocityGenRd), 2.0f));
        // Petite rotation au vol
        newBody->SetAngularVelocity(3.0f); // Rad/s
        BodiesList.push_back(newBody);
        BodiesList[BodiesList.size() - 1]->SetTransform(b2Vec2(disWeightRd(genRdWidth) / SCALE, -5), FallingBodyAngle);


}


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;

    if (!IMG_Init(IMG_INIT_PNG))
        std::cout << "IMG_init has failed ! SDL_image Error: " << IMG_GetError() << std::endl;

    if (TTF_Init() < 0)
    {
        std::cout << "TTF_Init has failed ! TTF_Init Error: " << TTF_GetError() << std::endl;
    }

    RenderWindowClass* InitWindow = new RenderWindowClass("CrazyBar",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,600,SDL_WINDOW_SHOWN);

    SDL_Window* window = InitWindow->GetWindow();
    SDL_Renderer* renderer = InitWindow->GetRenderer();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    LoadRessource* Ressource = new LoadRessource(renderer);

    // Box2D settings
    Box2D_Init Box2D;
    b2World& world = Box2D.GetWorld();
    world.SetGravity(Box2D.GetGravity());

    // Clean

    std::vector <b2Body*> FallingBodies;

    // int WindowWidth, WindowHeight;
    // SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);

    std::random_device TimerRd;
    std::mt19937 TimerGen(TimerRd());
    std::uniform_int_distribution<> TimerDis(60, 120);
    int Timer = TimerDis(TimerGen);

    int maxbodiesInAir = 3;
    int BodiesInAir = 0;
    int BodiesOnTray = 0;



    BoxCollider ColliderListener;
    world.SetContactListener(&ColliderListener);

    const float SCALE = 30.0f;

    // TrayBody
    Tray* TrayBody = new Tray(&world);

    b2BodyDef WallDef;
    WallDef.type = b2_kinematicBody; // Les murs ne bougent jamais

    b2PolygonShape WallShape;
    // Un mur fait 1 mètre de large et prend toute la hauteur de l'écran
    WallShape.SetAsBox(1.0f, (InitWindow->GetWindowHeight() / SCALE) + 10.0f / 2.0f);

    b2FixtureDef WallFixture;
    WallFixture.shape = &WallShape;
    WallFixture.friction = 0.3f;
    WallFixture.restitution = 0.4f; // Ajoute un petit effet de rebond amusant !

    // Mur de Gauche (Placé sur x = 0)
    b2Body* LeftWall = world.CreateBody(&WallDef);
    LeftWall->CreateFixture(&WallFixture);
    LeftWall->SetTransform(b2Vec2(0, (InitWindow->GetWindowHeight() / SCALE) / 2.0f), 0.0f);

    // Mur de Droite (Placé sur x = Largeur de l'écran)
    b2Body* RightWall = world.CreateBody(&WallDef);
    RightWall->CreateFixture(&WallFixture);
    RightWall->SetTransform(b2Vec2(InitWindow->GetWindowWidth() / SCALE, (InitWindow->GetWindowHeight() / SCALE) / 2.0f), 0.0f);

    std::vector<Mix_Chunk*> Box_Sound;
    Box_Sound.push_back(Ressource->GetSound("BoxSound1"));
    Box_Sound.push_back(Ressource->GetSound("BoxSound2"));
    Box_Sound.push_back(Ressource->GetSound("BoxSound3"));
    Box_Sound.push_back(Ressource->GetSound("BoxSound4"));
    Box_Sound.push_back(Ressource->GetSound("BoxSound5"));

    ColliderListener.Sound = Box_Sound;


    std::random_device BoxSoundRd;
    std::mt19937 genRdBoxSound(BoxSoundRd());
    std::uniform_int_distribution<> disBoxSoundRd(0, Box_Sound.size() - 1);



    bool AlreadyScored = false;
    bool SoundAlreadyPlayed = false;
    int Score = 0;
    int popupTimer = 0;
    float popupX = 0.0f;
    float popupY = 0.0f;

    bool isRunning = true;
    SDL_Event event;

    //Game Loop
    Uint32 frameStart;
    int frameTime;

    //Game Loop
    while (isRunning)
    {
        frameStart = SDL_GetTicks(); // On chronomètre le début de la frame

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        TrayBody->TrayMovement();

        // // 1. INPUTS ET VITESSES (Avant la physique !)
        // SDL_GetMouseState(&mouseX, &mouseY);
        //
        // // Nouvelle technique fluide : On calcule la vitesse pour atteindre la souris
        // b2Vec2 targetPosition(mouseX / SCALE, mouseY / SCALE);
        // b2Vec2 currentPosition = TrayBody->GetBody()->GetPosition();
        // b2Vec2 velocity = targetPosition - currentPosition;
        // velocity *= 60.0f; // On multiplie par les FPS pour qu'il y aille en 1 frame
        //
        // TrayBody->GetBody()->SetLinearVelocity(velocity);
        // // SUPPRESSION des TrayBody->SetTransform ! La vitesse fera tout le travail.

        // 2. SIMULATION PHYSIQUE
        world.Step(Box2D.GetTimeStep(), Box2D.GetVelocityIterations(), Box2D.GetPositionIterations());

        // 3. GESTION DU SPAWN
        if (Timer == 0 && BodiesInAir < maxbodiesInAir)
        {
            SpawnFallingBody(FallingBodies, window, world, InitWindow->GetWindowHeight(), InitWindow->GetWindowWidth());
            Timer = TimerDis(TimerGen);
        }
        else if (Timer > 0)
        {
            Timer--;
        }

        // 4. RENDU GRAPHIQUE
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int y = 0; y <= InitWindow->GetWindowWidth(); y += 128) {
            for (int x = 0; x <= InitWindow->GetWindowWidth(); x += 128)
            {
                SDL_Rect BGReact;
                BGReact.x = x;   // Position X
                BGReact.y = y;   // Position Y
                BGReact.w = 128; // Largeur (Width)
                BGReact.h = 128; // Hauteur (Height)
                SDL_RenderCopyEx(renderer, Ressource->GetSprite("BG"), NULL, &BGReact, 0.0F, NULL, SDL_FLIP_HORIZONTAL);
            }
        }

        //SDL_RenderClear(renderer);

        // RENDU DU SCORE (Propre et sécurisé)
        SDL_Color color = {255, 255, 255, 255}; // Couleur blanche
        std::string scoreText = std::to_string(Score);

        SDL_Surface* surface = TTF_RenderText_Solid(Ressource->GetFont("ScoreGUIFont"), scoreText.c_str(), color);
        if (surface != nullptr)
        {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect ScoreRect;
            // On conserve la VRAIE taille générée par la police
            ScoreRect.w = surface->w * 2;
            ScoreRect.h = surface->h * 2;

            // Formule pour centrer parfaitement le texte à l'écran
            ScoreRect.x = (InitWindow->GetWindowWidth() - ScoreRect.w) / 2;
            ScoreRect.y = (InitWindow->GetWindowHeight() - ScoreRect.h) / 2;

            // On dessine le score
            SDL_RenderCopy(renderer, texture, NULL, &ScoreRect);

            // Libération IMMÉDIATE de la mémoire de la frame
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        BodiesInAir = 0;
        BodiesOnTray = 0;

        for (b2Body * i : FallingBodies)
        {
            SDL_Rect SpawningObject;
            SpawningObject.w = 40;
            SpawningObject.h = 40;
            SpawningObject.x = (i->GetPosition().x * SCALE) - (SpawningObject.w / 2);
            SpawningObject.y = (i->GetPosition().y * SCALE) - (SpawningObject.h / 2);


            if (Ressource->GetSprite("Box") == nullptr)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &SpawningObject);
            }
            else
            {
                SDL_RenderCopyEx(renderer, Ressource->GetSprite("Box"), NULL, &SpawningObject, i->GetAngle() * 180.0f / M_PI, NULL, SDL_FLIP_HORIZONTAL);
            }

            if (std::abs(i->GetLinearVelocity().y) > 0.5f)
            {
                BodiesInAir++;
            }
            else
            {
                BodiesOnTray++;

                //Mix_PlayChannel(-1,Box_Sound[disBoxSoundRd(genRdBoxSound)],0);

                b2Vec2 trayPos = TrayBody->GetBody()->GetPosition();
                b2Vec2 boxPos = i->GetPosition();
                float distance = (trayPos - boxPos).Length(); // Calcule la vraie distance (hypoténuse)

                if (distance < 5.0f)
                {
                    b2Vec2 attractionDir = trayPos - boxPos;
                    attractionDir *= 100.0f;
                    i->ApplyForceToCenter(attractionDir, true);
                }

                // CHAMP D'ATTRACTION MAGNÉTIQUE
                /*
                if (distanceToTray < 5.0f) // Si la caisse est à moins de 5 mètres
                {
                    // 1. On calcule la direction vers le centre du plateau
                    b2Vec2 trayPos = TrayBody->GetPosition();
                    b2Vec2 boxPos = i->GetPosition();
                    b2Vec2 attractionDir = trayPos - boxPos;

                    // 2. On augmente la force d'attraction
                    attractionDir *= 100.0f; // Modifie ce chiffre pour un aimant plus ou moins fort

                    // 3. On applique la force sur la caisse !
                    i->ApplyForceToCenter(attractionDir, true);
                }
                */
            }
        }

        // 5. NETTOYAGE
        for (int i = 0; i < FallingBodies.size();)
        {
            if (FallingBodies[i]->GetPosition().y > (InitWindow->GetWindowHeight() / SCALE) + 2.0f)
            {
                world.DestroyBody(FallingBodies[i]);
                FallingBodies.erase(FallingBodies.begin() + i);
                BodiesInAir--; // Optionnel, mais propre
            }
            else
            {
                i++;
            }
        }

        // 6. DESSIN DU PLATEAU
        SDL_Rect TrayObject;
        TrayObject.w = 200;
        TrayObject.h = 10;
        TrayObject.x = (TrayBody->GetBody()->GetPosition().x * SCALE) - (TrayObject.w / 2);
        TrayObject.y = (TrayBody->GetBody()->GetPosition().y * SCALE) - (TrayObject.h / 2);

        if (Ressource->GetSprite("Tray") == nullptr)
        {
            SDL_SetRenderDrawColor(renderer, 104, 39, 13, 255); // Couleur marron
            SDL_RenderFillRect(renderer, &TrayObject);
        }
        else
        {
            SDL_RenderCopyEx(renderer, Ressource->GetSprite("Tray"), NULL, &TrayObject, 0.0f, NULL, SDL_FLIP_NONE);
        }


// ----------------------------------------------------
        // GESTION DU SCORE ET DÉCLENCHEMENT DU POPUP
        // ----------------------------------------------------
        if (BodiesOnTray >= 6)
        {
            Score++;
            popupTimer = 60; // 1 seconde à 60 FPS

            b2Body* highestBox = nullptr;
            float minY = 99999.0f; // On cherche le Y le plus bas (le plus haut à l'écran)

            // On cherche la boîte immobile la plus haute
            for (b2Body* b : FallingBodies)
            {
                if (std::abs(b->GetLinearVelocity().y) <= 0.5f) // Boîte posée
                {
                    if (b->GetPosition().y < minY)
                    {
                        minY = b->GetPosition().y;
                        highestBox = b;
                    }
                }
            }

            // On positionne le popup sur la boîte la plus haute
            if (highestBox != nullptr)
            {
                popupX = highestBox->GetPosition().x * SCALE;
                popupY = (highestBox->GetPosition().y * SCALE) - 40.0f;
            }
            else
            {
                popupX = TrayBody->GetBody()->GetPosition().x * SCALE;
                popupY = (TrayBody->GetBody()->GetPosition().y * SCALE) - 40.0f;
            }
        }

        // ----------------------------------------------------
        // RENDU DU POPUP ANIMÉ
        // ----------------------------------------------------
        if (popupTimer > 0)
        {
            Uint8 alpha = (Uint8)((popupTimer * 255) / 60);
            SDL_Color popupColor = {255, 255, 0, alpha};
            std::string popupText = "+1 Box Stacking";
            if (!SoundAlreadyPlayed)
            {
                Mix_PlayChannel(-1,Ressource->GetSound("BonusSound"),0);
                SoundAlreadyPlayed = true;
            }

            SDL_Surface* ScoreSurface = TTF_RenderText_Blended(Ressource->GetFont("ScoreAnimation"), popupText.c_str(), popupColor);
            if (ScoreSurface != nullptr)
            {
                SDL_Texture* ScoreTexture = SDL_CreateTextureFromSurface(renderer, ScoreSurface);
                SDL_SetTextureBlendMode(ScoreTexture, SDL_BLENDMODE_BLEND);

                SDL_Rect ScoreRect;
                ScoreRect.w = ScoreSurface->w;
                ScoreRect.h = ScoreSurface->h;
                ScoreRect.x = popupX - (ScoreRect.w / 2);
                ScoreRect.y = popupY;

                SDL_RenderCopy(renderer, ScoreTexture, NULL, &ScoreRect);

                SDL_FreeSurface(ScoreSurface);
                SDL_DestroyTexture(ScoreTexture);
            }

            for (auto it = FallingBodies.begin(); it != FallingBodies.end(); )
            {
                b2Body* body = *it;
                // Si la boîte est immobile (sur le plateau)
                if (std::abs(body->GetLinearVelocity().y) <= 0.5f)
                {
                    world.DestroyBody(body); // Destruction dans Box2D
                    it = FallingBodies.erase(it); // Suppression du tableau C++ (avance l'itérateur)
                }
                else
                {
                    ++it;
                }
            }

            // NE PAS METTRE FallingBodies.clear() ICI !
            popupY -= 0.5f; // Monte doucement
            popupTimer--;
            if (popupTimer == 0)
            {
                SoundAlreadyPlayed = false;
            }
        }


        // Rendu final à l'écran (TOUJOURS en dernier !)
        SDL_RenderPresent(renderer);

        if (Score == 5 && !AlreadyScored)
        {
            InitWindow->SetWindowHeight(600);
            InitWindow->SetWindowWidth(1080);

            SDL_SetWindowSize(window, InitWindow->GetWindowWidth(), InitWindow->GetWindowHeight());
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            RightWall->SetTransform(b2Vec2(InitWindow->GetWindowWidth() / SCALE, (InitWindow->GetWindowHeight() / SCALE) / 2.0f), 0.0f);
            Mix_PlayChannel(-1,Ressource->GetSound("LevelUpSound"),0);
            AlreadyScored = true;
        }
        else if (Score > 5 && Score < 10)
        {
            AlreadyScored = false;
        }
        else if (Score == 10 && !AlreadyScored)
        {
            InitWindow->SetWindowHeight(600);
            InitWindow->SetWindowWidth(1680);

            SDL_SetWindowSize(window, InitWindow->GetWindowWidth(), InitWindow->GetWindowHeight());
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            RightWall->SetTransform(b2Vec2(InitWindow->GetWindowWidth() / SCALE, (InitWindow->GetWindowHeight() / SCALE) / 2.0f), 0.0f);
            Mix_PlayChannel(-1,Ressource->GetSound("LevelUpSound"),0);
            AlreadyScored = true;
        }
        else if (Score > 10 && Score < 15)
        {
            AlreadyScored = false;
        }
        else if (Score == 15 && !AlreadyScored)
        {
            InitWindow->SetWindowHeight(600);
            InitWindow->SetWindowWidth(2280);

            SDL_SetWindowSize(window, InitWindow->GetWindowWidth(), InitWindow->GetWindowHeight());
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            RightWall->SetTransform(b2Vec2(InitWindow->GetWindowWidth() / SCALE, (InitWindow->GetWindowHeight() / SCALE) / 2.0f), 0.0f);
            Mix_PlayChannel(-1,Ressource->GetSound("LevelUpSound"),0);
            AlreadyScored = true;
        }


        // 7. GESTION STABLE DES 60 FPS
        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < 16)
        {
            SDL_Delay(16 - frameTime);
        }
    }

    TTF_CloseFont(Ressource->GetFont("ScoreGUIFont"));
    TTF_CloseFont(Ressource->GetFont("ScoreAnimation"));

    Mix_FreeChunk(Ressource->GetSound("BonusSound"));
    Mix_FreeChunk(Ressource->GetSound("BoxSound1"));
    Mix_FreeChunk(Ressource->GetSound("BoxSound2"));
    Mix_FreeChunk(Ressource->GetSound("BoxSound3"));
    Mix_FreeChunk(Ressource->GetSound("BoxSound4"));
    Mix_FreeChunk(Ressource->GetSound("BoxSound5"));
    Mix_FreeChunk(Ressource->GetSound("LevelUpSound"));

    Mix_CloseAudio();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}