// #include <bits/stdc++.h>
// #include <stdlib.h>
// #include <time.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>


#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "initialise.hpp"

#define INF 1000000000


using namespace std;

int height = 51;
int width = 51;


vector <vector<int>> maze;  //1 for wall
vector <vector<int>> visit;

vector<int> traffic = {11,12,13,14,15};
int nr=0,nc=0;

bool rand_unvisited_neighbour(int r,int c){
    vector<pair<int,int>> unvisited;
    if(r+2<height && maze[r+2][c]==0 && visit[r+2][c]==0)unvisited.push_back({r+2,c});
    if(r-2>0 && maze[r-2][c]==0 && visit[r-2][c]==0)unvisited.push_back({r-2,c});
    if(c+2<width && maze[r][c+2]==0 && visit[r][c+2]==0)unvisited.push_back({r,c+2});
    if(c-2>0 && maze[r][c-2]==0 && visit[r][c-2]==0)unvisited.push_back({r,c-2});
    if(unvisited.size()==0){
        return false;
    }
    int random = rand()%(unvisited.size());
    nr = unvisited[random].first;
    nc = unvisited[random].second;
    return true;
}



void initMaze(){
    for(int i = 0 ; i < height ; i += 2){
        for(int j=0;j<width ; j++){
            maze[i][j]=1;
            visit[i][j]=1;
        }
    }

    for(int i=1;i<height;i+=2){
        for(int j=0;j<width;j+=2){
            maze[i][j]=1;
            visit[i][j]=1;
        }
    }
}



void break_rand_wall(int r,int c){
    vector<pair<int,int>> visited;
    visited.push_back({-1, -1});
    if(r + 2 < height && maze[r + 1][c] == 1)visited.push_back({r + 2, c});
    if(r - 2 > 0 && maze[r - 1][c] == 1)visited.push_back({r - 2, c});
    if(c + 2 < width && maze[r][c+1] == 1)visited.push_back({r, c + 2});
    if(c - 2 > 0 && maze[r][c - 1] == 1)visited.push_back({r, c - 2});

    int random = rand()%(visited.size());
    nr = visited[random].first;
    nc = visited[random].second;
    if(nr == -1 && nc == -1){
        return;
    }
    else{
        maze[(r + nr)/2][(c + nc)/2] = 0;
    }
}



void dfsMaze(int r, int c){
    visit[r][c]=1;
    int flag = 0;

    while(rand_unvisited_neighbour(r,c)){
        flag = 1;
        maze[(r+nr)/2][(c+nc)/2]=0;
        dfsMaze(nr,nc);
    }

    if(flag==0){
        break_rand_wall(r,c);
    }

}



bool junction(int i,int j){
    // cout<<i<<" "<<j<<endl;
    if(maze[i][j-1]==1 && maze[i][j+1]==1 && maze[i-1][j]==0 && maze[i+1][j]==0)return false;
    if(maze[i][j-1]==0 && maze[i][j+1]==0 && maze[i-1][j]==1 && maze[i+1][j]==1)return false;
    return true;
}


void breakSomeWalls(){
    // cout<<"yes";
    for(int i=1;i<height-1;i++){
        int count = 0;
        int j = 0;
        while(j<width-2){
            j++;
            if(maze[i][j]==0){
                count = 0;
            }
            else{
                count++;
            }
            if(count>=4 && !(junction(i,j))){
                maze[i][j]=0;
                count = 0;
            }     
        }
        
    }
    for(int i=1;i<width-1;i++){
        int count = 0;
        int j = 0;
        while(j<height-2){
            j++;
            if(maze[j][i]==0){
                count = 0;
            }
            else{
                count++;
            } 
            if(count>=4 && !(junction(j,i))){
                maze[j][i]=0;
                count = 0;
            }    
        }
        
    }
    maze[height-2][width-2] = 0;
}


void clearMaze(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            maze[i][j]=0;
            visit[i][j]=0;
        }
    }
}

void getTraffic(){
    for(int i = 0;i<height; i++){
        for(int j = 0; j<width; j++){
            if(maze[i][j] == 0){
                int random = rand()%(traffic.size()); 
                maze[i][j] = traffic[random];
            }
        }
    }
}

void formMaze(){
    maze = vector <vector<int>>(height, vector<int>(width, 0));
    visit = vector <vector<int>>(height, vector<int>(width, 0));
    srand(time(0));
    clearMaze();
    initMaze();
    dfsMaze(1,1);
    breakSomeWalls();
    getTraffic();

}


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
    SDL_Delay(3000);
    return 1;
}


int dijakstra(){
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

        // Run the dijakstra's untill we find a find a location to visit
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
        if(movePizza(path) < 0){ return -1; }
        home = target;	
        target = pair<int,int>();

    }
    return 1;
}