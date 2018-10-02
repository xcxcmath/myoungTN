//Team Myoung
//Geometry
#include <bits/stdc++.h>
using namespace std;

const auto PI = 2.0 * acos(0.0);
const auto EPS = numeric_limits<double>::min();
struct vec{
    double x, y;
    explicit vec(double _x=0, double _y=0):x(_x),y(_y){}
    bool operator==(const vec&rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator<(const vec&rhs) const{
        return x != rhs.x ? x < rhs.x : y < rhs.y;
    }
    vec operator+(const vec&rhs) const{
        return vec(x+rhs.x,y+rhs.y);
    }
    vec operator-(const vec&rhs) const{
        return vec(x-rhs.x,y-rhs.y);
    }
    vec operator*(double rhs) const{
        return vec(x*rhs, y*rhs);
    }
    double norm() const {return hypot(x,y);}
    vec normalize() const { return vec(x/norm(), y/norm());}
    double polar() const {return fmod(atan2(y,x) + 2*PI, 2*PI);}
    double dot(const vec&rhs) const{
        return x * rhs.x + y * rhs.y;
    }
    double cross(const vec&rhs) const{
        return x * rhs.y - rhs.x * y;
    }
    vec project(const vec&rhs) const {
        vec r = rhs.normalize();
        return r * r.dot(*this);
    }
};
double ccw(const vec& a, const vec&b){
    return a.cross(b);
}
double ccw(const vec& p, const vec& a, const vec& b){
    return ccw(a-p,b-p);
}
bool lineIntersection(const vec& a, const vec& b, const vec& c, const vec& d, vec& x){
    double det = (b-a).cross(d-c);
    if(fabs(det) < EPS) return false;
    x = a + (b-a) * ((c-a).cross(d-c) / det);
    return true;
}
bool segmentIntersection(const vec& a, const vec& b, const vec& c, const vec& d){
    double ab = ccw(a, b, c) * ccw(a, b, d);
    double cd = ccw(c, d, a) * ccw(c, d, b);
    if(ab == 0 && cd == 0){
        if(b<a)swap(a, b);
        if(d<c)swap(c, d);
        return !(b<c||d<a);
    }
    return ab <= 0 && cd <= 0;
}
double area(const vector<vec>&p){
    double ret = 0;
    for(int i= 0 ; i < p.size(); ++i){
        int j = (i +1)%p.size();
        ret += p[i].cross(p[j]);
    }
    return fabs(ret) / 2.0;
}
bool isInside(const vec& q, const vector<vec>& p){
    int crosses = 0;
    for(int i = 0; i < p.size(); ++i){
        int j = (i+1)%p.size();
        if((p[i].y > q.y) != (p[j].y > q.y)){
            double atX = (p[j].x - p[i].x) * (q.y - p[i].y)
                / (p[j].y - p[i].y) + p[i].x;
            if(q.x < atX) ++crosses;
        }
    }
    return crosses % 2 > 0;
}

// numerically correct convex hull
struct convexHull {
    using pii = pair<int, int>;
    using vpii = vector<pii>;
    vpii p;
    void add_point(int x, int y){
        p.emplace_back(x, y);
    }
    long long ch_ccw(pii i, pii j, pii k){
        return 1LL * (j.first - i.first) * (k.second - i.second)
            - 1LL * (k.first - i.first) * (j.second - i.second);
    }
    vpii solve(){
        swap(p[0], *min_element(begin(p), end(p)));
        sort(begin(p)+1,end(p),[&](pii u, pii v){
                auto t = ch_ccw(p[0], u, v);
                return t > 0 || !t && u<v;
                });
        vpii stk; for(auto it:p){
            while(stk.size()>1 && ch_ccw(stk[stk.size()-2],stk[stk.size()-1],it)<=0)
                stk.pop_back();
            stk.push_back(it);
        }
        return stk;
    }
};
