#include <iostream>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/subprimos/#problems

const int MaxN = 1e5+10;

bool primo[MaxN];
int fst_primo[MaxN];
int cnt_decomposicion[MaxN];
void precalculo_criba(int Lim) {
    for (int i = 2; i <= Lim; ++i) {
        if (fst_primo[i] > 0) {
            cnt_decomposicion[i] = cnt_decomposicion[i / fst_primo[i]] + 1;
            continue;
        }
        primo[i] = true;
        cnt_decomposicion[i] = 1;
        for (int j = i; j <= Lim; j += i) {
            fst_primo[j] = i;
        }
    }
}

int main(){
    int N, M;
    cin >> N >> M;
    precalculo_criba(M);
    int ans = 0;
    for (int i = N; i <= M; ++i) {
        if (primo[cnt_decomposicion[i]]) {
            ++ans;
        }
    }
    cout << ans << "\n";
}
