#include <iostream>
#include <vector>
using namespace std;
typedef pair<int, int> pii;
typedef long long int lli;
// Problem source: https://dmoj.ca/problem/ioi16p1

int find_subset(int l, int u, int w[], int n, int result[]) {
    vector<pii> w_sorted;
    for (int i = 0; i < n; i++) {
        w_sorted.push_back({w[i], i });
    }
    sort(w_sorted.begin(), w_sorted.end());
    
    int in = 0, fn = 0;
    lli suma = 0;
    for (fn = 0; fn < n; fn++) {
        suma += w_sorted[fn].first;
        while (suma > u) {
            suma -= w_sorted[in].first;
            ++in;
        }
        if (l <= suma) {
            // l <= suma <= u
            for (int i = in; i <= fn; i++) {
                result[i-in] = w_sorted[i].second;
            }
            return fn-in+1;
        }
    }
    return 0;
}
