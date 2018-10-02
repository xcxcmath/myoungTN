//Team Myoung
//Suffix Array
#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;

constexpr auto MAX = 256;
pair<vi,vi> sfx_lcp(const string &s){
    const auto n = s.size();
    int lim = max(n+1, MAX+1);
    vi sfx(n), g(n+1), ng(n+1), order2idx(n+1);
    vi cnt;
    for(int i = 0; i < n; ++i)
        sfx[i] = i, g[i]=s[i];

    for(int t = 1; t<n; t<<=1){
        cnt.clear(); cnt.resize(lim, 0);
        for(int i = 0; i < n; ++i)++cnt[g[min(i+t,n)]];
        for(int i=1;i<lim;++i) cnt[i] += cnt[i-1];
        for(int i = n-1; i >= 0; --i) order2idx[--cnt[g[min(i+t,n)]]]=i;

        cnt.clear(); cnt.resize(lim,0);
        for(int i = 0; i < n; ++i) ++cnt[g[i]];
        for(int i = 1; i < lim; ++i) cnt[i] += cnt[i-1];
        for(int i = n-1; i >=0; --i)sfx[--cnt[g[order2idx[i]]]]=order2idx[i];

        auto cmp = [&](int i, int j)->bool{
            if(g[i]==g[j]) return g[i+t]<g[j+t];
            return g[i]<g[j];
        };

        ng[sfx[0]] = 1;
        for(int i = 1; i < n; ++i){
            if(cmp(sfx[i-1],sfx[i])) ng[sfx[i]]=ng[sfx[i-1]]+1;
            else ng[sfx[i]]=ng[sfx[i-1]];
        }
        g = ng;
    }

    vi lcp(n), prevsfx(n+1), plcp(n+1);

    prevsfx[sfx[0]] = -1;
    for(int i=1; i < n; ++i) prevsfx[sfx[i]] = sfx[i-1];
    for(int i = 0, common=0; i<n;++i){
        if(prevsfx[i]==-1) plcp[i] = 0;
        else{
            while(s[i+common] == s[prevsfx[i] + common]) ++common;
            plcp[i] = common;
            common = max(common-1, 0);
        }
    }
    for(int i=0;i<n;++i) lcp[i] = plcp[sfx[i]];
    return make_pair(sfx, lcp);
}

