#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>


using namespace std;


const int SCREEN_WIDTH = 1000;  //696
const int SCREEN_HEIGHT = 1200;
const int cellHeight =24;
const int cellWidth = 24;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window *simulationWindow = NULL;

//The window renderer
SDL_Renderer *simulationRenderer = NULL;



SDL_Texture* gWallTexture = NULL;
SDL_Texture* gGrassTexture = NULL;
SDL_Texture* gDocmanTexture = NULL;


pair<int,int> check_position(int x, int y)
{
    return {x/cellWidth , y/cellHeight};
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        simulationWindow = SDL_CreateWindow("COVID SMASH", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (simulationWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            simulationRenderer = SDL_CreateRenderer(simulationWindow, -1, SDL_RENDERER_ACCELERATED);
            if (simulationRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(simulationRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    gDocmanTexture = loadTexture("Resources/docman.png");
    if (gDocmanTexture == NULL)
    {
        printf("Failed to load Docman image!\n");
        success = false;
    }

    gWallTexture = loadTexture("Resources/wall.png");
    if (gDocmanTexture == NULL)
    {
        printf("Failed to load Docman image!\n");
        success = false;
    }

    gGrassTexture = loadTexture("Resources/grass.png");
    if (gDocmanTexture == NULL)
    {
        printf("Failed to load Docman image!\n");
        success = false;
    }
    return success;
}


void close()
{
    //Free loaded image
    SDL_DestroyTexture( gWallTexture );
	gWallTexture = NULL;
	SDL_DestroyTexture( gDocmanTexture );
	gDocmanTexture = NULL;
	SDL_DestroyTexture( gGrassTexture );
	gGrassTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer(simulationRenderer);
    SDL_DestroyWindow(simulationWindow);
    simulationWindow = NULL;
    simulationRenderer = NULL;
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(std::string path)
{
    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(simulationRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}


