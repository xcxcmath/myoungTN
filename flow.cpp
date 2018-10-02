//Team Myoung
//Flows
#include <bits/stdc++.h>
using namespace std;

// O(min(V^2E, Ef))
struct dinic{
    const int inf = 1e9;
    int v,s,t;
    struct edge{
        int x,cap,rev;
    };
    vector<vector<edge>> adj;
    vector<int> lv, work;
    dinic(int vv, int ss, int tt):v(vv),s(ss),t(tt),adj(v),lv(v),work(v){}
    void add_edge(int x, int y,int cap){
        int sz = adj[x].size(), rsz = adj[y].size();
        adj[x].push_back({y,cap,rsz});
        adj[y].push_back({x,0,sz});
    }
    bool bfs(){
        for(int i = 0; i < v; ++i)lv[i]=work[i]=0;
        queue<int> q;
        q.push(s);
        lv[s] =1;
        while(!q.empty()){
            int h = q.front();
            q.pop();
            for(auto it:adj[h]) if(it.cap && !lv[it.x]){
                lv[it.x] =lv[h]+1;
                q.push(it.x);
            }
        }
        return lv[t];
    }
    int dfs(int h, int flow){
        if(h==t)return flow;
        for(int &i=work[h];i<adj[h].size();++i)if(lv[h] < lv[adj[h][i].x] && adj[h][i].cap){
            int ret = dfs(adj[h][i].x, min(flow, adj[h][i].cap));
            if(ret){
                adj[h][i].cap -= ret;
                adj[adj[h][i].x][adj[h][i].rev].cap += ret;
                return ret;
            }
        }
        return 0;
    }
    int solve(){
        int ret = 0;
        while(bfs())for(int flow; flow=dfs(s,inf);) ret += flow;
        return res;
    }
};

//LR flow
//l : lower, r : upper
//s, t not in [0, v-1]
struct lr{
    const int inf = 1e9;
    int v, s, t, ns, nt, goal=0;
    struct infor{
        int x, y, low, forw, back;
    };
    vector<infor> ad;
    dinic *dn;
    lr(int vv, int ss, int tt):v(vv),s(ss),t(tt),ns(v),nt(v+1){
        dn = new dinic(v+2, s, t);
        dn->add_edge(t, s, inf);
    }
    void add_edge(int x, int y, int l, int r){
        dn->add_edge(x,nt,l);
        dn->add_edge(ns, y,l);
        ad.push_back({x,y,l,int(dn->adj[x].size()),int(dn->adj[y].size())});
        dn->add_edge(x,y,r-l);
        goal += l;
    }
    int solve(vector<vector<int>> & res){
        dn->s = ns; dn->t = nt;
        if(dn->solve() ^ goal) return -1;
        dn->s = s; dn->t = t;
        int ret = dn->solve();
        for(auto it:ad){
            int f = it.low + dn->adj[it.y][it.back].cap, c=f+dn->adj[it.x][it.forw].cap;
            res.push_back({it.x,it.y,f,c});
        }
        return ret;
    }
};

struct mcmf{
    const int inf = 1e9;
    int v, s, t;
    vector<int> ck, fr, mini;
    struct edge{
        int x, cap, cost, rev;
    };
    vector<vector<edge>> adj;
    mcmf(int vv, int ss, int tt):v(vv),s(ss),t(tt),ch(v),fr(v),mini(v),adj(v){}
    void add_edge(int x, int y, int cap, int cost){
        int sz = adj[x].size(), rsz = adj[y].size();
        adj[x].push_back({y,cap,cost,rsz});
        adj[y].push_back({x,0,-cost,sz});
    }
    pair<int, int> solve(){
        int totfl = 0, totcst = 0;
        for(;;){
            for(int i = 0; i < v; ++i) mini[i]=inf;
            queue<int>q; q.push(s);
            mini[s]=0;
            while(!q.empty()){
                int h = q.front(); q.pop();
                ck[h]=0;
                for(auto&it:adj[h])if(it.cap && mini[it.x]>mini[h]+it.cost){
                    mini[it.x] = mini[h] + it.cost;
                    fr[it.x] = it.rev;
                    if(!ck[it.x]) ck[it.x]=1, q.push(it.x);
                }
            }
            if(mini[t] == inf)break;
            int flow = inf;
            for(int i=t; i!=s;i = adj[i][fr[i]].x)
                flow = min(flow,adj[adj[i][fr[i]].x][adj[i][fr[i]].rev].cap);
            for(int i=t; i!=s; i=adj[i][fr[i]].x){
                adj[adj[i][fr[i]].x][adj[i][fr[i]].rev].cap -= flow;
                adj[i][fr[i]].cap+= flow;
            }
            totfl += flow;
            totcst += mini[t]*flow;
        }
        return {totfl, totcst};
    }
};

//Hopcroft-Karp
//opt for bipartiting
//O(Esqrt(V))
struct hk{
    int vl, vr;
    vector<int> lv, mc, rev;
    vector<vector<int>> adj;
    hk(int l, int r):vl(l),vr(r),lv(vl+1),mc(vl+1),rev(vr+1),adj(vl+1){}
    void add_edge(int x, int y){
        adj[x].push_back(y);
    }
    int bfs(){
        queue<int> q;
        for(int i = 0; i <= vl; ++i){
            if(!mc[i]) lv[i] = 1, q.push(i);
            else lv[i] = 0;
        }
        int ret= 0;
        while(!q.empty()){
            int h = q.front();
            q.pop();
            for(int it:adj[h]){
                if(!rev[it]) ret = 1;
                else if(!lv[rev[it]]) lv[rev[it]]=lv[h]+1,q.push(rev[it]);
            }
        }
        return ret;
    }
    int dfs(int h){
        for(int it:adj[h]) if(!rev[it] || lv[rev[it]]>lv[h] && dfs(rev[it])){
            mc[h] = it;
            rev[it]=h;
            return 1;
        }
        lv[h] = 0;
        return 0;
    }
    vector<pair<int,int>> solve(){
        vector<pair<int,int>> ret;
        while(bfs()) for(int i = 1; i <= vl; ++i) if(!mc[i])dfs(i);
        for(int i = 1; i <= vl; ++i) if(mc[i])ret.emplace_back(i, mc[i]);
        return ret;
    }
};

//Global min-cut
//O(V^3)
constexpr int INF = 1e9;
using vi = vector<int>;
using vvi = vector<vi>;
pair<int, vi> getMinCut(vvi& mat){
    const auto sz = mat.size();
    vi used(sz), cut, best_cut;
    int best_w = -1;
    for(int iter = sz-1; iter >= 0; --iter){
        vi w = mat[0];
        vi added = used;
        int prev, last = 0;
        for(int i = 0; i < iter; ++i){
            prev= last;
            last = -1;
            for(int j = 1; j < sz; ++j)
                if(!added[j]&&(last==-1||w[j]>w[last]))
                    last = j;
            if(i == iter-1){
                for(int j = 0; j < sz; ++j)
                    mat[prev][j] += mat[last][j];
                for(int j = 0; j < sz; ++j)
                    mat[j][prev] = mat[prev][j];
                used[last] = true;
                cut.push_back(last);
                if(best_w == -1 || w[last] < best_w){
                    best_cut = cut;
                    best_w = w[last];
                }
            } else {
                for(int j = 0; j < sz; ++j)
                    w[j] += mat[last][j];
                added[last] = true;
            }
        }
    }
    return make_pair(best_w, best_cut);
}
void mincut_example(){
    int v, e; cin >> v >> e;
    vvi mat(v, vi(v));
    while(e--){
        int a, b, c; cin >> a >> b >> c; --a; --b;
        mat[a][b] = mat[b][a] = c;
    }
    pair<int, vi> ret = getMinCut(mat);
    //...
}
