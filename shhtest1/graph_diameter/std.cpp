#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, int> pli;
const int MAXN = 100005;
const ll INF = 1e18;

int n, m;
vector<pair<int, ll>> adj[MAXN];
ll dist[MAXN];
int parent[MAXN];

void dijkstra(int src) {
    for (int i = 1; i <= n; i++) { dist[i] = INF; parent[i] = -1; }
    dist[src] = 0;
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v; ll w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    dijkstra(1);
    int a = 1;
    for (int i = 2; i <= n; i++)
        if (dist[i] > dist[a]) a = i;
    dijkstra(a);
    int b = 1;
    for (int i = 2; i <= n; i++)
        if (dist[i] > dist[b]) b = i;
    ll diameter = dist[b];
    vector<int> path;
    for (int cur = b; cur != -1; cur = parent[cur])
        path.push_back(cur);
    reverse(path.begin(), path.end());
    ll minEdge = INF;
    for (int i = 1; i < (int)path.size(); i++) {
        int u = path[i-1], v = path[i];
        for (auto [to, w] : adj[u])
            if (to == v) { minEdge = min(minEdge, w); break; }
    }
    ll ans = diameter;
    if (minEdge < INF)
        ans = min(ans, diameter + minEdge);
    cout << ans << endl;
    return 0;
}
