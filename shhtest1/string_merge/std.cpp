#include <bits/stdc++.h>
using namespace std;

string s;
int n;
char dp[305][305][2];
bool vis[305][305][2];

char solve(int i, int j, int turn) {
    if (i == j) return s[i];
    if (vis[i][j][turn]) return dp[i][j][turn];
    vis[i][j][turn] = 1;
    char best;
    if (turn == 0) best = 'z' + 1;
    else best = 'a' - 1;
    for (int k = i; k < j; k++) {
        char lo = min(s[k], s[k + 1]);
        char hi = max(s[k], s[k + 1]);
        for (char c = lo; c <= hi; c++) {
            char leftRes = 0, rightRes = 0;
            if (k > i) leftRes = solve(i, k - 1, turn);
            if (k + 2 <= j) rightRes = solve(k + 2, j, 1 - turn);
            char cur;
            if (!leftRes && !rightRes) cur = c;
            else if (!leftRes) cur = rightRes;
            else if (!rightRes) cur = leftRes;
            else {
                if (turn == 0) cur = min(leftRes, rightRes);
                else cur = max(leftRes, rightRes);
            }
            if (turn == 0) best = min(best, cur);
            else best = max(best, cur);
        }
    }
    return dp[i][j][turn] = best;
}

int main() {
    cin >> s;
    n = s.size();
    int turn = (n % 2 == 0) ? 0 : 1;
    cout << solve(0, n - 1, turn) << endl;
    return 0;
}
