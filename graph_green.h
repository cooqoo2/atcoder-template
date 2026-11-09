#include "mytemplate.h"

// mytemplate.h に以下の追加が必要（現状定義）
// using vvpll = vector<vpll>;
//
// visitedは vector<bool> ではなく vll(0/1) で持つ（ビットパッキング回避）
// 　グリッド系
const vector<pll> offsets = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
inline bool ingrid(ll y, ll x, ll H, ll W)
{
    return 0 <= y && y < H && 0 <= x && x < W;
}


// ##################### 1. Union-Find #####################

// ================= Union-Find =================
// 経路圧縮 + union by size。ほぼO(1)
// unite(x,y):併合できたらtrue / same:同じ木か / size:属する木の要素数
struct UF
{
    vll par, sz;
    UF(ll n) : par(n, -1), sz(n, 1) {}
    ll root(ll x)
    {
        if (par[x] == -1)
            return x;
        return par[x] = root(par[x]);
    }
    bool same(ll x, ll y)
    {
        return root(x) == root(y);
    }
    // 併合できたらtrue、既に同じ木ならfalse
    bool unite(ll x, ll y)
    {
        x = root(x);
        y = root(y);
        if (x == y)
            return false;
        if (sz[x] < sz[y])
            swap(x, y);
        par[y] = x;
        sz[x] += sz[y];
        return true;
    }
    ll size(ll x)
    {
        return sz[root(x)];
    }
};

// ##################### 2. 探索・最短路 #####################

// ================= DFS(順列辞書順) =================
void dfs(ll pos){

    vll aa;
    ll m,n;
    //↑ここは消す
    if(pos==n){
        // calc();
        return;
    }

    ll sta=1;
    if(pos!=0){
        sta=aa[pos-1];
    }

    rep(x,sta,m+1){
        aa.pub(x);
        dfs(pos+1);
        aa.pob();
    }

}

// ================= DFS(再帰) =================
// graph: 隣接リスト。curから到達可能な頂点を visited[v]=1 にする
// 呼び出し: vll visited(n, 0); dfs(graph, start, visited);
// 深さがN=2e5級になるとローカルでスタックオーバーフローの危険
void dfs(const vvll &graph, ll cur, vll &visited)
{
    visited[cur] = 1;
    // ここに行きがけ処理
    for (ll next : graph[cur])
    {
        if (visited[next])
            continue;
        dfs(graph, next, visited);
    }
    // ここに帰りがけ処理
}

// ================= グリッドDFS（再帰） =================
// grid[y][x] : 0=通行不可、1=通行可能
// visited[y][x] : 0=未訪問、1=訪問済み
// 4方向・8方向を選択可能
// 呼び出し: dfs_grid(grid, y, x, visited, offsets4);
//           dfs_grid(grid, y, x, visited, offsets8);
// 計算量: O(H*W)（全マスを探索する場合）
// 注意: gridは空でないこと。深い再帰ではスタックオーバーフローに注意
inline bool ingrid(ll y, ll x, ll H, ll W)
{
    return 0 <= y && y < H && 0 <= x && x < W;
}
// (y,x)から到達可能なマスをすべて訪問済みにする
void dfs_grid(const vvll &grid, ll y, ll x,vvll &visited)
{
    ll H = grid.size(), W = grid[0].size();
    visited[y][x] = 1;

    for (auto [dy, dx] : offsets)
    {
        ll ny = y + dy;
        ll nx = x + dx;

        if (!ingrid(ny, nx, H, W)) continue;
        if (grid[ny][nx] == 0) continue;
        if (visited[ny][nx]) continue;

        dfs_grid(grid, ny, nx, visited);
    }

    // ここに帰りがけ処理
}

// ================= BFS(重みなし) =================
// graphは隣接リスト。始点集合からの最短距離を返す。到達不可は-1
// 単一始点の場合は bfs(graph, {start});
vll bfs(const vvll &graph, const vll &starts)
{
    vll dist(graph.size(), -1);
    qll q;
    for (ll v : starts)
    {
        dist[v] = 0;
        q.push(v);
    }
    while (!q.empty())
    {
        ll cur = q.front();
        q.pop();
        for (ll next : graph[cur])
        {
            if (dist[next] != -1)
                continue;
            dist[next] = dist[cur] + 1;
            q.push(next);
        }
    }
    return dist;
}

// ================= Dijkstra =================
// graph: vvpll (next, weight)、負辺なし前提。O(ElogV)
// 到達不可はINFのまま
vll dijkstra(const vvpll &graph, ll start)
{
    vll dist(graph.size(), INF);
    priority_queue<pll, vpll, greater<pll>> que; // (距離, 頂点)

    dist[start] = 0;
    que.push({0, start});

    while (!que.empty())
    {
        auto [cost, cur] = que.top();
        que.pop();
        if (cost > dist[cur])
            continue; // 古いエントリはスキップ

        for (auto [next, weight] : graph[cur])
        {
            if (dist[cur] + weight < dist[next])
            {
                dist[next] = dist[cur] + weight;
                que.push({dist[next], next});
            }
        }
    }
    return dist;
}

// ================= Warshall-Floyd(全点対最短路) =================
// dist[i][j]: iからjへの最短距離。O(N^3)。負閉路がある場合の挙動は未定義
// 初期化: dist[i][i]=0、辺(u,v,w)についてdist[u][v]=w、それ以外はINF
vvll warshall_floyd(vvll dist)
{
    ll n = dist.size();
    rep(k, n) rep(i, n) rep(j, n)
    {
        if (dist[i][k] == INF || dist[k][j] == INF)
            continue;
        chmin(dist[i][j], dist[i][k] + dist[k][j]);
    }
    return dist;
}

// ##################### 3. 最小全域木 #####################

// ================= Kruskal法(最小全域木) =================
// edges: (weight, u, v) のリスト。MSTの総コストを返す。nは頂点数
// 非連結なら最小全域森のコストになる（連結判定は別途UFで）
ll kruskal(vector<tuple<ll, ll, ll>> edges, ll n)
{
    sort(edges.begin(), edges.end());
    UF uf(n);
    ll cost = 0;
    for (auto [weight, u, v] : edges)
        if (uf.unite(u, v))
            cost += weight;
    return cost;
}

// ##################### 4. 有向グラフ #####################

// ================= トポロジカルソート(Kahn) =================
// graph: 有向グラフ。閉路があれば空のvllを返す
// この順でDPを回せばDAG上の最長路・経路数が求まる
vll topological_sort(const vvll &graph)
{
    ll n = graph.size();
    vll indeg(n, 0);
    rep(cur, n) for (ll next : graph[cur]) indeg[next]++;

    qll q;
    rep(i, n) if (indeg[i] == 0) q.push(i);

    vll order;
    while (!q.empty())
    {
        ll cur = q.front();
        q.pop();
        order.push_back(cur);
        for (ll next : graph[cur])
            if (--indeg[next] == 0)
                q.push(next);
    }
    if ((ll)order.size() != n)
        return vll(); // 閉路あり
    return order;
}

// ##################### 5. 無向グラフの構造 #####################

// ================= 二部グラフ判定 =================
// graph: 無向グラフ。色配列(0/1)をcolorに書き込み、判定結果を返す
// 非連結でも各成分ごとに彩色する
bool is_bipartite(const vvll &graph, vll &color)
{
    ll n = graph.size();
    color.assign(n, -1);
    rep(i, n)
    {
        if (color[i] != -1)
            continue;
        color[i] = 0;
        qll q;
        q.push(i);
        while (!q.empty())
        {
            ll cur = q.front();
            q.pop();
            for (ll next : graph[cur])
            {
                if (color[next] == -1)
                {
                    color[next] = color[cur] ^ 1;
                    q.push(next);
                }
                else if (color[next] == color[cur])
                    return false;
            }
        }
    }
    return true;
}

// ##################### 6. 木 #####################

// ================= 木のDFS(部分木サイズ) =================
// 木では visited を持たず「親と等しいか」だけ見れば十分
// 呼び出し: vll sz(n); dfs_tree(tree, root, -1, sz);
ll dfs_tree(const vvll &tree, ll cur, ll parent, vll &subtree_size)
{
    subtree_size[cur] = 1;
    for (ll next : tree[cur])
    {
        if (next == parent)
            continue;
        subtree_size[cur] += dfs_tree(tree, next, cur, subtree_size);
    }
    return subtree_size[cur];
}

// ================= 木の直径 =================
// 戻り値: (直径, 端点u, 端点v)。BFSを2回するだけ
tuple<ll, ll, ll> tree_diameter(const vvll &tree)
{
    vll d = bfs(tree, {0});
    ll u = maxel(d) - d.begin();
    d = bfs(tree, {u});
    ll v = maxel(d) - d.begin();
    return {d[v], u, v};
}