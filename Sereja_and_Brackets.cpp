#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<int,int> pii;
// Problem source: https://codeforces.com/contest/380/problem/C

struct Segment {
    int completes;
    int unused_closed;
    int unused_open;
};

Segment unir_segmentos(Segment A, Segment B) {
    Segment C;
    int new_pairs = min(A.unused_open, B.unused_closed);
    C.completes = A.completes + B.completes + new_pairs;
    C.unused_closed = A.unused_closed + B.unused_closed - new_pairs;
    C.unused_open = A.unused_open + B.unused_open - new_pairs;
    return C;
}

struct SegmentTree {
    vector<Segment> Tree;
    string brackets;
    
    void build(int x, int y, int n) {
        if (x + 1 == y) {
            Tree[n] = {0, brackets[x] == ')', brackets[x] == '('};
            return;
        }
        build(x, (x+y)/2, 2*n);
        build((x+y)/2, y, 2*n+1);
        Tree[n] = unir_segmentos(Tree[2*n], Tree[2*n+1]);
    }
    
    SegmentTree(string& _brackets) {
        brackets = _brackets;
        Tree.resize(4 * brackets.size() + 10);
        build(0, (int) brackets.size(), 1);
    }
    
    Segment _query(int x, int y, int n, int I, int D) {
        if (I <= x && y <= D) {
            return Tree[n];
        }
        if (D <= (x+y)/2) {
            return _query(x, (x+y)/2, 2*n, I, D);
        }
        if ((x+y)/2 <= I) {
            return _query((x+y)/2, y, 2*n+1, I, D);
        }
        Segment izq = _query(x, (x+y)/2, 2*n, I, D);
        Segment der = _query((x+y)/2, y, 2*n+1, I, D);
        return unir_segmentos(izq, der);
    }
    
    int query(int x, int y) {
        return 2 * _query(0, (int) brackets.size(), 1, x, y).completes;
    }
};

int main(){
    string brackets;
    cin >> brackets;
    SegmentTree ST(brackets);
    
    int Q;
    cin >> Q;
    for (int i = 0; i < Q; i++) {
        int I, D;
        cin >> I >> D;
        cout << ST.query(I-1, D) << "\n";
    }
}
