#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "dijkstra.hpp"
using namespace std;





int main(int argc, char *args[])
{
    if (argc >1){
        if (argc >=2){
            int x  = stoi(args[1]);
            if (x%2==0){
                cout <<"Height and width must be of odd length.\n";
                return 0;
            }
            height = width = x;
        }
        if (argc >=3){
            int x = stoi(args[2]);
            cellWidth = cellHeight = x;
        }
        if (argc >3){
            cout <<"Incorrect number of arguments, pass ./simulation <height> <cellWidth>\n";
            return 0;
        }
        SCREEN_WIDTH = width*cellWidth;
        SCREEN_HEIGHT = height*cellHeight;
    }
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
                if(dijkstra() < 0){
                    simulationRunning = false;
                    quit = true;
                }
                flag = true;
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}