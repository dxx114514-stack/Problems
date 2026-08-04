#include <bits/stdc++.h>
using namespace std;

// 生成 string_merge 的测试数据
int main(int argc, char* argv[]) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int t = atoi(argv[1]);
    int n;
    if (t <= 3) n = rng() % 3 + 2;
    else if (t <= 6) n = rng() % 5 + 3;
    else n = rng() % 8 + 5;
    string s;
    for (int i = 0; i < n; i++)
        s += (char)('a' + rng() % 3);
    cout << s << endl;
    return 0;
}
