//Team Myoung
//Treap
#include <bits/stdc++.h>
using namespace std;
using vi = vector<int>;

struct treap {
    int key, pri, sz; treap *left, *right;
    treap(int k):key(k),pri(rand()),sz(1),left(0),right(0){}
    void set(treap* le, treap* ri){left=le;right=ri;calc_sz();}
    void calc_sz(){sz=1;sz+=(left?left->sz:0);sz+=(right?right->sz:0);}
};
using ptreap = pair<treap*,treap*>;
ptreap split(treap* root, int key){
    if(!root) return ptreap(0,0);
    if(root->key<key){
        auto rs = split(root->right, key);
        root->set(root->left, rs.first);
        return ptreap(root, rs.second);
    }
    ptreap ls = split(root->left, key);
    root->set(ls.second, root->right);
    return ptreap(ls.first, root);
}
treap* insert(treap* root, treap* node){
    if(!root) return node;
    if(root->pri < node->pri){
        auto sp = split(root, node->key);
        node->set(sp.first, sp.second); return node;
    } else if(node->key < root->key)
        root->set(insert(root->left, node),root->right);
    else root->set(root->left, insert(root->right, node));
    return root;
}
treap* merge(treap* a, treap* b){
    if(!a)return b;
    if(!b)return a;
    if(a->pri < b->pri){
        b->set(merge(a, b->left),b->right); return b;
    }
    a->set(a->left, merge(a->right, b)); return a;
}
treap* erase(treap* root, int key){
    if(!root) return root;
    if(root->key == key){
        auto ret = merge(root->left, root->right);
        delete root; return ret;
    }
    if(key < root->key) root->set(erase(root->left, key),root->right);
    else root->set(root->left, erase(root->right, key));
    return root;
}

treap* kth_number(treap* root, int k){
    int ls = 0; if(root->left)ls=root->left->sz;
    if(k<=ls) return kth_number(root->left, k);
    if(k == ls+1) return root;
    return kth_number(root->right, k-ls-1);
}
int countLessThan(treap* root, int key){
    if(!root) return 0;
    if(root->key >= key) return countLessThan(root->left, key);
    int ls = (root->left? root->left->sz : 0);
    return ls + 1 + countLessThan(root->right, key);
