//Team Myoung
//KMP
#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;

vi get_par(const string& s){
    const auto sz = s.size();
    vi pi(sz);
    int b=1,m=0;
    while(b+m<sz){
        if(s[b+m] == s[m]){
            ++m; pi[b+m-1]=m;
        }else{
            if(m ==0) ++b;
            else{
                b+=m-pi[m-1];
                m=pi[m-1];
            }
        }
    }
    return pi;
}

vi kmp(const string& origin, const string& target){
    const auto os = origin.size(), ts = target.size();
    vi ret, pi = get_par(target);
    int b=0,m=0;
    while(b<=os-ts){
        if(m < ts && origin[b+m] == target[m]){
            ++m; if(m == ts) ret.push_back(b);
        }else{
            if(m==0)++b;
            else{
                b += m - pi[m-1];
                m = pi[m-1];
            }
        }
    }
    return ret;
}