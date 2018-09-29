//Team Myoung
//Minimum spanning tree algorithms
#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using pii = pair<int,int>;
using vpii = vector<pii>;
using vvpii = vector<vpii>;

struct disjoint{
    vi par, ran;
    disjoint(int n):par(n),ran(n,1){
        for(int i = 0; i < n; ++i)par[i]=i;
    }
    int find(int u){
        return par[u]==u ? u : (par[u]=find(par[u]));
    }
    void merge(int u, int v){
        u = find(u); v=find(v);
        if(u==v)return;
        if(ran[u]>ran[v]) swap(u,v);
        par[u]=v;
        if(ran[u]==ran[v])++ran[v];
    }
};

// adj[i][j] = pair(cost, destination)

int kruskal(const vvpii& adj){
    const auto sz = adj.size();
    int ret = 0;
    vector<pair<int,pii>> edges;
    for(int u = 0; u < sz;++u){
        for(auto p : adj[u]){
            int v = p.second, c = p.first;
            edges.emplace_back(c, make_pair(u,v));
        }
    }
    sort(begin(edges), end(edges));
    disjoint dis(sz);
    for(auto e : edges){
        int c = e.first;
        int u = e.second.first;
        int v = e.second.second;
        if(dis.find(u) == dis.find(v)) continue;
        dis.merge(u,v);
        ret += c;
    }
    return ret;
}

constexpr auto INF = numeric_limits<int>::max();
int prim(const vvpii& adj){
    const auto sz = adj.size();
    int ret = 0;
    vi dist(sz, INF);
    priority_queue<pii, vpii, greater<pii>> q;

    dist[0] = 0;
    q.emplace(0, 0);

    while(!q.empty()){
        int c = q.top().first;
        int v = q.top().second;
        q.pop();
        if(dist[v]) continue;
        dist[v] = true;
        ret += c;
        for(auto p : adj[v]){
            q.emplace(p.first, p.second);
        }
    }

    return ret;
}