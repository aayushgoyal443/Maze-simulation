#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "maze.hpp"
#include "initialise.hpp"
using namespace std;

#define INF 1000000000


int no_of_delivery_points;
set<pair<int, int>> delivery_points;
pair <int, int> home = {1, 1};
pair<int,int> target;

void updateScreen(){
	SDL_SetRenderDrawColor( simulationRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			SDL_Rect fillRect = { j*cellWidth, i*cellHeight, cellWidth, cellHeight };
			if(maze[i][j]==1){
				SDL_RenderCopy(simulationRenderer, gWallTexture, NULL, &fillRect);
			}
            else if (maze[i][j]>10 && maze[i][j]<16){
                SDL_RenderCopy(simulationRenderer, white[maze[i][j]-10], NULL, &fillRect);
            }
            else{
                SDL_RenderCopy(simulationRenderer, gGrassTexture, NULL, &fillRect);
            }
		}
	}
}


int dijakstra(){

    // n is num rows, m is num cols
    int n = height, m = width;

    // Make adjacency list from the maze
    vector <pair<int, pair<int,int>>> adj[n][m];

    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            if (i>0 && !maze[i-1][j]==1) adj[i][j].push_back({maze[i-1][j]-10,{i-1,j}});
            if (j>0 && !maze[i][j-1]==1) adj[i][j].push_back({maze[i][j-1]-10,{i,j-1}});
            if (i<n-1 && !maze[i+1][j]==1) adj[i][j].push_back({maze[i+1][j]-10,{i+1,j}});
            if (j<m-1 && !maze[i][j+1]==1) adj[i][j].push_back({maze[i][j+1]-10,{i,j+1}});
        }
    }

    while(!delivery_points.empty()){
        priority_queue <pair<int, pair<int, int>>> q;
        int distance[n][m];
        pair<int,int> parent[n][m];
        int processed[n][m]={0};

        // initialising the data 
        for (int i=0;i<n;i++){
            for (int j=0;j<m;j++){
                distance[i][j] = INF;
            }
        }
        distance[home.first][home.second] =0;
        q.push({0,{home.first, home.second}});
        parent[home.first][home.second] ={-1,-1};

        // Run the dijakstra's untill we find a find a location to visit
        while(!q.empty()){
            pair<int, int> a = q.top().second;
			SDL_Rect fillRect = { a.second*cellWidth, a.first*cellHeight, cellWidth, cellHeight };
			SDL_RenderCopy(simulationRenderer, gGrassTexture, NULL, &fillRect);
            SDL_RenderPresent(simulationRenderer);
            SDL_wait(500);
            if (delivery_points.count(a)>0){
                target  = a;
                delivery_points.erase(a);
                break;   // because we found atleast one point to visit, so we firstly run the simulation till there
            }
            q.pop();
            if (processed[a.first][a.second]==1) continue;
            processed[a.first][a.second] =1;
            for (auto p: adj[a.first][a.second]){
                pair<int,int> point = p.second;
                int dist = p.first;
                if ( distance[a.first][a.second] + dist < distance[point.first][point.second]){
                    distance[point.first][point.second] = distance[a.first][a.second] + dist;
                    q.push({-1*distance[point.first][point.second],point});
                    parent[point.first][point.second] = a;
                }
            }
        }

        vector<pair<int,int>> path; // from target to home
        path.push_back(target);
        pair <int,int> curr = target;
        while(curr!= home){
            path.push_back(parent[curr.first][curr.second]);
            curr = parent[curr.first][curr.second];
        }
        SDL_RenderClear( simulationRenderer );
        updateScreen();  
        SDL_RenderPresent( simulationRenderer );	
        home = target;
        target = pair<int,int>();

    }
}