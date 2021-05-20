#include <bits/stdc++.h>
using namespace std;

#define INF 1000000000



int main(){

    // n is num rows, m is num cols
    int n,m;
    set <pair<int ,int>> delivery_points;
    int maze[n][m];
    pair <int, int> home;
    pair<int,int> target;

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
        }
        home = target;
        target = pair<int,int>();

    }
}