//Team Myoung
//Trie
#include <bits/stdc++.h>
using namespace std;

constexpr int alphabets = 26;
constexpr char alpha = 'A';
struct trie{
    trie* child[alphabets];
    bool terminal;
    trie(): terminal(false){
        fill(begin(child),end(child),0);
    }
    ~trie(){
        for(auto&i:child)if(i)delete i;
    }
    void insert(const char* key){
        if(*key == 0) terminal = true;
        else{
            int next = (*key)-alpha;
            if(child[next]==0)child[next]=new trie();
            child[next]->insert(key+1);
        }
    }
    trie* find(const char* key){
        if(*key == 0) return this;
        int next = (*key)-alpha;
        if(child[next]==0)return 0;
        return child[next]->find(key+1);
    }
};