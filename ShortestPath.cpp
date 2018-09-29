//Team Myoung
//Shortest Path algorithms
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
using vpii = vector<pii>;
using vvpii = vector<vpii>;
using vi = vector<int>;
using vvi = vector<vi>;
using pqpii = priority_queue<pii, vpii, greater<pii>>;

constexpr auto INF = numeric_limits<int>::max() / 2;
// half because of floyd

//pii = (cost, index)
vi dijkstra(const vvpii & adj, int s){
    vi dist(adj.size(), INF);
    dist[s] = 0; pqpii q; q.emplace(0,s);
    while(!q.empty()){
        auto temp = q.top(); q.pop();
        auto here = temp.second, cost = temp.first;
        if(dist[here] < cost) continue;
        for(auto& p: adj[here]){
            auto there = p.second, d = p.first+cost;
            if(dist[there] > d){
                dist[there] = d;
                q.emplace(d, there);
            }
        }
    }
    return dist;
}

//pii = (cost, index)
vi bellman(const vvpii& adj, int s){
    const auto sz = adj.size();
    vi upper(sz, INF); upper[s] = 0;
    bool updated = false;
    for(int iter = 0; iter < sz; ++iter){
        updated = false;
        for(int here = 0; here < sz; ++here){
            for(auto&p:adj[here]){
                int there = p.second, cost=p.first+upper[here];
                if(upper[there] > cost){
                    upper[there] = cost;
                    updated = true;
                }
            }
        }
        if(!updated) break;
    }
    if(updated)upper.clear(); // negative-weighted cycle!
    return upper;
}

// T(E), O(VE)
//pii = (cost, index)
vi spfa(const vvpii& adj, int s){
    const auto sz = adj.size();
    vi dist(sz, INF), in_queue(sz);
    queue<int> q;

    dist[s] = 0;
    q.push(s);
    in_queue[s] = 1;
    while(!q.empty()){
        int here = q.front(); q.pop(); in_queue[here]=0;
        for(auto e:adj[here]){
            int there=e.second, cost=e.first+dist[here];
            if(dist[there] > cost){
                dist[there] = cost;
                if(!in_queue[there]){
                    q.push(there);
                    in_queue[there]=1;
                }
            }
        }
    }
    return dist;
}

// mat[i][j] == INF if not connected
void floyd(vvi& mat){
    const auto sz = mat.size();
    for(int i = 0; i < sz; ++i)mat[i][i] = 0;
    for(int k = 0; k < sz; ++k)
        for(int i = 0; i < sz; ++i){
            if(mat[i][k] == INF) continue;
            for(int j = 0; j < sz; ++j)
                mat[i][j] = min(mat[i][j],mat[i][k]+mat[k][j]);
        }
}
void floyd_path(vvi& mat, vvi& via){
    const auto sz = mat.size();
    for(int i = 0; i < sz; ++i) mat[i][i] = 0;
    via = vvi(sz, vi(sz, -1));
    for(int k = 0; k < sz; ++k){
        for(int i = 0; i < sz; ++i){
            for(int j = 0; j < sz; ++j){
                if(mat[i][j] > mat[i][k] + mat[k][j]){
                    via[i][j] = k;
                    mat[i][j] = mat[i][k] + mat[k][j];
                }
            }
        }
    }
}
void reconstruct(int u, int v, vi&path, const vvi& via){
    if(via[u][v] == -1){
        path.push_back(u);
        if(u!=v)path.push_back(v);
    }else{
        int w = via[u][v];
        reconstruct(u,w,path,via);
        path.pop_back();
        reconstruct(w,v,path,via);
    }
}