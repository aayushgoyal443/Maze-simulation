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

// function to find an unvisited random neighbour
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


// function to initialise the maze
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


// function to break a random wall
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


// run dfs to build the maze
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


// check for junctions in the maze
bool junction(int i,int j){
    // cout<<i<<" "<<j<<endl;
    if(maze[i][j-1]==1 && maze[i][j+1]==1 && maze[i-1][j]==0 && maze[i+1][j]==0)return false;
    if(maze[i][j-1]==0 && maze[i][j+1]==0 && maze[i-1][j]==1 && maze[i+1][j]==1)return false;
    return true;
}

// function to break some more walls
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


// give random traffic value to different cells
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


// main function to build a maze
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