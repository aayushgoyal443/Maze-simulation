#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "maze.hpp"
using namespace std;





int main(int argc, char *args[])
{
    
    cout << "No. of Delivery Points: " << endl;
    cin >> no_of_delivery_points; 
    const int FPS = 40;
    const int delay = 1000/FPS;
    
    Uint32 frameStart;
    int frameTime;

    if (!init())
    {
        printf("Failed to initialize!\n");
        close();
        return 0;
    }
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        close();
        return 0;
    }

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    bool simulationRunning = false;

    
    
    
    formMaze();
    //While application is running
    while (!quit)
    {
        //Going to the Main Menu
        while(!simulationRunning){
            while (SDL_PollEvent(&e) != 0)
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                    break;
                }

                // User presses mouse
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    pair<int, int> pos = check_position(x, y);
                    if(maze[pos.first][pos.second] != 1){
                        delivery_points.insert(pos);
                        cout << pos.first<< " " << pos.second << endl;
                    }
                }
            }
            if(quit) break;
            //Clear screen
            SDL_RenderClear(simulationRenderer);
            //Render texture to screen
            updateScreen();
            //Update screen
            SDL_RenderPresent(simulationRenderer);
            if (delivery_points.size() == no_of_delivery_points) simulationRunning = true;
        }
        bool flag = false;
        while(simulationRunning){
            
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    simulationRunning = false;
                    quit = true;
                }
            }
            if (flag == false) {
                dijakstra();
                flag = true;
            }

            // SDL_SetRenderDrawColor( simulationRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            // SDL_RenderClear( simulationRenderer );
            // updateScreen();  
            // SDL_RenderPresent( simulationRenderer );	
            // frameTime = SDL_GetTicks()-frameStart;
            // if(delay>frameTime){
            //     SDL_Delay(delay - frameTime);
            // }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}