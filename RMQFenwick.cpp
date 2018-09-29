//Team Myoung
//RMQ and Fenwick tree
#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;

constexpr auto INF = numeric_limits<int>::max();

struct rmq{
    int n; vi rm;
    rmq(const vi&arr){
        n = arr.size();
        rm.resize(n<<2);
        init(arr,0,n-1,1);
    }
    int init(const vi&arr, int l, int r, int node){
        if(l == r) return rm[node] = arr[l];
        int m = (l+r)/2;
        int lmin = init(arr,l,m,node*2);
        int rmin = init(arr,m+1,r,node*2+1);
        return rm[node] = min(lmin,rmin);
    }
    int query(int l, int r, int node, int nl, int nr){
        if(r<nl || nr<l) return INF;
        if(l <= nl && nr <= r) return rm[node];
        int m = (nl+nr)/2;
        return min(query(l,r,node*2,nl,m),
                query(l,r,node*2+1,m+1,nr));
    }
    int query(int l, int r){
        return query(l,r,1,0,n-1);
    }
    int update(int idx, int val, int node, int nl, int nr){
        if(idx < nl || nr < idx) return rm[node];
        if(nl == nr) return rm[node]=val;
        int m=(nl+nr)/2;
        return rm[node]=min(update(idx,val,node*2,nl,m),
                update(idx,val,node*2+1,m+1,nr));
    }
    int update(int idx, int val){
        return update(idx, val, 1, 0, n-1);
    }
};

struct fenwick{
    vi tree;
    fenwick(int n):tree(n+1){}
    int sum(int idx){
        ++idx; int ret = 0;
        while(idx>0){
            ret += tree[idx];
            idx &= (idx-1);
        }
        return ret;
    }
    void add(int idx, int val){
        ++idx;
        while(idx < tree.size()){
            tree[idx] += val;
            idx += (idx & -idx);
        }
    }
};
