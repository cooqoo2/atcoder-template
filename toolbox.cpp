#include "mytemplate.h"

// 2点(x1,y1),(x2,y2)間のユークリッド距離を返す
ld dist(ll x1, ll y1, ll x2, ll y2)
{
    return sqrtl((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// nの階乗を返す　※n>=21でllオーバーフロー
ll fact(ll n)
{
    ll res = 1;
    rep(i, 2, n + 1) res *= i;
    return res;
}

// 10の整数乗を返す（pow(10,n)のキャスト精度誤差を避けるため）
ll ipow10(ll n)
{
    ll res = 1;
    rep(i, n) res *= 10;
    return res;
}

// 　グリッド系
const vector<pll> offsets = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
inline bool ingrid(ll y, ll x, ll H, ll W)
{
    return 0 <= y && y < H && 0 <= x && x < W;
}

// スタートからの距離のグリッドを返す
//vs grid;
// vin(grid, H);
// vvll vvd =  bfs(grid, 0, 0);
//0,0からの距離が入ったvvllが返る
//#は壁
vvll bfs_grid(const vs &grid, ll sy, ll sx)
{
    ll H = grid.size(), W = grid[0].size();
    vvll dist(H, vll(W, -1));
    queue<pll> q;
    dist[sy][sx] = 0;
    q.push({sy, sx});
    while (!q.empty())
    {
        auto [y, x] = q.front();
        q.pop();
        for (auto [dy, dx] : offsets)
        {
            ll ny = y + dy, nx = x + dx;
            if (!ingrid(ny, nx, H, W))
                continue;
            if (grid[ny][nx] == '#')
                continue;
            if (dist[ny][nx] != -1)
                continue;
            dist[ny][nx] = dist[y][x] + 1;
            q.push({ny, nx});
        }
    }
    return dist;
}

// 　小数の二分探索
ld f(ld x)
{
    return x * x;
}

void bynary_search()
{
    ld lo = 0, hi = INF;
    rep(i, 100)
    {
        ld mid = (lo + hi) / 2;
        if (f(mid) < f(mid + 1e-9))
            hi = mid;
        else
            lo = mid;
    }
}

// 二分探索
bool check(ll x)
{
    return true;
}

void bynary_search()
{
    ll lo = 0, hi = INF;
    while (hi - lo > 1)
    {
        ll mid = (lo + hi) / 2;
        if (check(mid))
            lo = mid;
        else
            hi = mid;
    }
    clf(lo);//lo = 条件を満たす最大の値,hi= 条件を満たさない最小の値
}

// 小数を小数点以下10桁で固定小数点表示にする（cout全体に適用）
int main()
{
    cout << fixed << setprecision(10);
}

//尺取り
void syakutori(int n){
    int right = 0;     
    for (int left = 0; left < n; ++left) {
        // while (right < n && (right を 1 個進めたときに条件を満たす)) {
            /* 実際に right を 1 進める */
            // ex: sum += a[right];
            ++right;
        }

        /* break した状態で right は条件を満たす最大なので、何かする */
        // ex: res += (right - left);

        /* left をインクリメントする準備 */
        // ex: if (right == left) ++right;
        // ex: else sum -= a[left];
}


// 0からｎ回連続した値を入れる
void io(ll n){
    vector<int> v(n);
    iota(all(v), 0);
}


//n進数探索 3進数で表したときの各桁を、配列 B に保存
void solve(ll x) {
    vll B(18);
    ll power3 = 1;
    rep(i,18) {
        B[i] = (x / power3) % 3;
        power3 *= 3;
    }
}

//三分探索
ld calc(ld x){
    return (ld)pow(2,x)-x;//任意の単峰性を持つ(二次)関数
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ld lo=0.0;
    ld hi=1e2;

    ll cnt=200;
    while(cnt--){
        ld c1=(lo*2+hi)/3;
        ld c2=(lo+hi*2)/3;

        if(calc(c1)>calc(c2)) lo=c1;
        else hi=c2;
    }

    clf(calc(lo));


    return 0;
}


//座標圧縮
void solve(){
    vs v;
    ll n;
    vs b=v;
    sort(all(b));
    b.erase(unique(all(b)),b.end());
    clf(b.size());

    vll c(n);
    rep(i,n){
        c[i]=lower_bound(all(b),v[i])-b.begin();
    }
}

