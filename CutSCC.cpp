//Team Myoung
//Graph algorithms (cut, scc, path..)
#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using vvi = vector<vi>;
using pii = pair<int, int>;
using vpii = vector<pii>;

// 1-based numbering
// O(V+E)
// 2-SAT : "(x or ~y) and ..." into (~x to ~y) and (y to x)
// if x and ~x are in same scc, unsatisfiable
// if not, later one (smaller num of sccID) is true
struct scc{
    vvi adj[2];
    vi vis, lst;
    int v;
    scc(int vv):v(vv){
        adj[0].resize(v+1);
        adj[1].resize(v+1);
        vis.resize(v+1);
    }
    void add_edge(int x, int y){
        adj[1][x].push_back(y);
        adj[0][y].push_back(x);
    }
    void dfs(int h, int t){
        if(vis[h] == t) return;
        vis[h] = t;
        for(int it:adj[t][h]) dfs(it, t);
        lst.push_back(h);
    }
    vvi solve(){
        vvi ret;
        for(int i = 1; i<=v;++i)if(!vis[i])dfs(i,1);
        vi tlst = lst;
        reverse(begin(tlst), end(tlst));
        for(auto it:tlst) if(vis[it]){
            lst.clear();
            dfs(it,0);
            ret.push_back(lst);
        }
        return ret;
    }
};

// Cut Edges
// no loops or duplicate edges
// 1-based..
struct cut_edges{
    int v;
    vi t; vvi adj; vpii res;
    cut_edges(int vv):v(vv),t(v+1),adj(v+1){}
    void add_edge(int x, int y){
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    void dfs(int h, int p, int d){
        t[h]=d;
        for(auto it:adj[h]){
            if(it == p) continue;
            if(!t[it]){
                dfs(it,h,d+1);
                if(t[it]==d+1)
                    res.emplace_back(h,it);
            }
            t[h] = min(t[h],t[it]);
        }
    }
    vpii solve(){
        for(int i = 1; i <= v; ++i)if(!t[i])dfs(i,-1,1);
        return res;
    }
};

// Cut Vertices
// No loops or multiple edges, 1-based
struct cut_vertices{
    int v, cnt = 0;
    vi ck, cutv;
    vvi adj;
    cut_vertices(int vv):v(vv),ck(v+1),cutv(v+1),adj(v+1){}
    void add_edge(int x, int y){
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    void dfs(int h, bool rt){
        ck[h] = ++cnt;
        int vcnt = 0, mini = v;
        for(auto t: adj[h]){
            if(!ck[t]){
                dfs(t,false);
                if(ck[t]==ck[h])cutv[h] = true;
                vcnt++;
            }
            mini = min(mini,ck[t]);
        }
        ck[h] = mini;
        if(rt) cutv[h] = vcnt >= 2;
    }
    vi solve(){
        vi ret;
        for(int i = 1; i <= v; ++i)if(!ck[i]) dfs(i, true);
        for(int i = 1; i <= v; ++i)if(cutv[i]) ret.push_back(i);
        return ret;
    }
};

// Eulerian path
struct edge;
using iter = list<edge>::iterator;
struct edge{
    int next;
    iter rev;
    edge(int nv):next(nv){}
};
const int max_v = ; // fill!
int num_v;
list<edge> adj[max_v];
vi path;
void find_path(int v){
    while(adj[v].size() > 0){
        int vn = adj[v].front().next;
        adj[vn].erase(adj[v].front().rev);
        adj[v].pop_front();
        find_path(vn);
    }
    path.push_back(v);
}
void add_edge_eulerian(int a, int b){
    adj[a].emplace_front(b);
    iter ita = begin(adj[a]);
    adj[b].emplace_front(a);
    iter itb = begin(adj[b]);
    ita->rev = itb;
    itb->rev = ita;
}
