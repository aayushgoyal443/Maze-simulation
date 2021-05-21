#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "maze.hpp"

using namespace std;



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
            else if(maze[i][j] == 2){
                SDL_RenderCopy(simulationRenderer, gGrassTexture, NULL, &fillRect);
            }
            else if (maze[i][j]>10 && maze[i][j]<16){
                SDL_RenderCopy(simulationRenderer, white[maze[i][j]-10], NULL, &fillRect);
            }
            else{
                SDL_RenderCopy(simulationRenderer, gGrassTexture, NULL, &fillRect);
            }
		}
	}
    for (auto u: delivery_points){
        SDL_Rect fillRect = { u.second*cellWidth, u.first*cellHeight, cellWidth, cellHeight };
        SDL_RenderCopy(simulationRenderer, red[maze[u.first][u.second]-10] , NULL, &fillRect);
    }
    SDL_Rect fillRect = { home.second*cellWidth, home.first*cellHeight, cellWidth, cellHeight };
    SDL_RenderCopy(simulationRenderer, gPizzamanTexture, NULL, &fillRect);
}

void createPath(int i, int j){
	SDL_Rect fillRect = { j*cellWidth, i*cellHeight, cellWidth, cellHeight };
    if(maze[i][j]==1){
        SDL_RenderCopy(simulationRenderer, gWallTexture, NULL, &fillRect);
    }
    else if(maze[i][j] == 2){
        SDL_RenderCopy(simulationRenderer, gGrassTexture, NULL, &fillRect);
    }
    else if (maze[i][j]>10 && maze[i][j]<16){
        SDL_RenderCopy(simulationRenderer, green[maze[i][j]-10], NULL, &fillRect);
    }
    else{
        SDL_RenderCopy(simulationRenderer, gGrassTexture, NULL, &fillRect);
    }
    fillRect = { home.second*cellWidth, home.first*cellHeight, cellWidth, cellHeight };
    SDL_RenderCopy(simulationRenderer, gPizzamanTexture, NULL, &fillRect);
    SDL_RenderPresent( simulationRenderer );	
}

int movePizza(vector<pair<int,int>> path){
    SDL_Event e;
    reverse(path.begin(), path.end());
    int n = path.size();
    pair<int,int> curr;
    pair<int,int> next;
    for (int i=0; i<n;i++ ){
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                return -1;
            }
        }
        curr = path[i];
        if (i+1<n) next = path[i+1];
        else next = curr;
        int angle = 0;
        int dx=0, dy=0;
        dx = next.first - curr.first;
        dy = next.second - curr.second;
        if (dx==0 && dy<0) angle = 90;
        else if (dx ==0 && dy >0) angle = 270;
        else if (dx>0 && dy==0) angle = 0;
        else if (dx<0 && dy==0) angle = 180;
        else angle =0;
        SDL_Rect fillRect = { curr.second*cellWidth, curr.first*cellHeight, cellWidth, cellHeight };
        SDL_RenderCopyEx(simulationRenderer, gPizzamanTexture, NULL, &fillRect, angle, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent( simulationRenderer );	
        SDL_Delay(100);
        SDL_RenderCopy(simulationRenderer, green[maze[curr.first][curr.second]-10], NULL, &fillRect);
    }
    SDL_Delay(2000);
    return 1;
}


int dijkstra(){
    SDL_Event e;
    // n is num rows, m is num cols
    int n = height, m = width;

    // Make adjacency list from the maze
    vector <pair<int, pair<int,int>>> adj[n][m];

    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            if (i>0 && (maze[i-1][j]!=1)) adj[i][j].push_back({maze[i-1][j]-10,{i-1,j}});
            if (j>0 && maze[i][j-1]!=1) adj[i][j].push_back({maze[i][j-1]-10,{i,j-1}});
            if (i<n-1 && maze[i+1][j]!=1) adj[i][j].push_back({maze[i+1][j]-10,{i+1,j}});
            if (j<m-1 && maze[i][j+1]!=1) adj[i][j].push_back({maze[i][j+1]-10,{i,j+1}});

        }
    }

    while(!delivery_points.empty()){
        SDL_RenderClear( simulationRenderer );
        updateScreen();  
        // SDL_RenderPresent( simulationRenderer );
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

        // Run the dijkstra's untill we find a find a location to visit
        while(!q.empty()){
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    return -1;
                }
            }
            pair<int, int> a = q.top().second;
            q.pop();
            if (processed[a.first][a.second] == 1) continue;
            processed[a.first][a.second] = 1;
			SDL_Rect fillRect = { a.second*cellWidth, a.first*cellHeight, cellWidth, cellHeight };
            if (delivery_points.count(a)<=0) SDL_RenderCopy(simulationRenderer, orange[maze[a.first][a.second]-10] , NULL, &fillRect);
            fillRect = { home.second*cellWidth, home.first*cellHeight, cellWidth, cellHeight };
            SDL_RenderCopy(simulationRenderer, gPizzamanTexture , NULL, &fillRect);
            SDL_RenderPresent( simulationRenderer );	
            SDL_Delay(100);
            if (delivery_points.count(a)>0){
                target  = a;
                delivery_points.erase(target);
                break;   // because we found atleast one point to visit, so we firstly run the simulation till there
            }
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
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    return -1;
                }
            }
            path.push_back(parent[curr.first][curr.second]);
            curr = parent[curr.first][curr.second];
            createPath(curr.first, curr.second);
            SDL_Delay(100);
        }
        SDL_Delay(1000);
        if(movePizza(path) < 0){ return -1; }
        home = target;	
        target = pair<int,int>();

    }
    return 1;
}