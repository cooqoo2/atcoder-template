#ifndef ONLINE_JUDGE
#define _GLIBCXX_DEBUG //[]で配列外参照をするとエラーにしてくれる
#endif

#include <bits/stdc++.h>
// #include <atcoder/all>
// using namespace atcoder;
using namespace std;

// clang-format off
#define lf "\n"
#define mp make_pair
#define fi first
#define se second
#define pub push_back
#define pob pop_back
#define pof pop_front

using ll = long long; using ld = long double; using ull = unsigned long long;
using vll = vector<ll>; using vvll = vector<vll>; using vvvll = vector<vvll>;
using vld = vector<ld>; using vvld = vector<vld>; using vvvld = vector<vvld>;
using vs = vector<string>; using vvs = vector<vs>; using vvvs = vector<vvs>;
using vb = vector<bool>; using vvb = vector<vb>;
using qll = queue<ll>; using dqll = deque<ll>;
using pll = pair<ll,ll>; using vpll = vector<pll>; using vvpll = vector<vpll>;

// using mint1 = modint998244353;
// using mint2 = modint1000000007;

template <typename T> vector<T> makev(size_t n, T init) { return vector<T>(n, init); }
template <typename T, typename... Args> auto makev(size_t n, Args... args) { return vector<decltype(makev<T>(args...))>(n, makev<T>(args...)); }
// auto v = makev<ll>(H, W, K, 0LL);
// で vvvll dp(H, vvll(W, vll(K, 0LL))) と同じ

#define _REP3_(i, a, b) for (ll i = (a); i < (ll)(b); i++)
#define _REP2_(i, n)    for (ll i = 0; i < (ll)(n); i++)
#define _REP_SEL_(_1, _2, _3, NAME, ...) NAME
#define rep(...) _REP_SEL_(__VA_ARGS__, _REP3_, _REP2_, ~)(__VA_ARGS__)

#define vin(v, n) rep(_i_, n) cin >> v.at(_i_)
#define v2in(v1, v2, n) rep(_i_, n) cin >> v1.at(_i_) >> v2.at(_i_)
#define vvin(v, n, m) rep(_i_, n) rep(_j_, m) cin >> v.at(_i_).at(_j_)

#define all(x) (x).begin(), (x).end()
#define rall(x) x.rbegin(), x.rend()
#define sum(v) accumulate(all(v), 0LL)// vector要素の総和（ll型で返る）
#define maxel(v) max_element(all(v))// 最大要素のイテレータを返す（値は*maxel(v)）
#define minel(v) min_element(all(v))// 最小要素のイテレータを返す（値は*minel(v)）
#define nexper(v) next_permutation(all(v))// do{}while(nexper(v))で順列全探索　vはソート済み
#define popcnt(x) (ll)popcount((ull)x)//整数xの二進数表記で1が何個あるかを数えるO(1)

inline void clf() { cout << "\n"; }
inline void csp() { cout << " "; }
template<typename T> inline void clf(T ans) { cout << ans << "\n"; }
template<typename T> inline void csp(T ans) { cout << ans << " "; }
inline void cYes(bool x) { cout << (x ? "Yes" : "No") << "\n"; }
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return true; } return false; } // a = max(a,b)
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return true; } return false; } // a = min(a,b)

const ll MOD = 1000000007;
const ll INF = 2e18;
const ld PI = acos(-1);//π
// clang-format on

// clang-format off
#define dump(x) cerr << #x << " = " << (x) << endl                 // 変数の値をデバッグ出力
#define dumpv(v) do { rep(_i_, (v).size()) cerr << (v)[_i_] << ' '; cerr << endl; } while(0)     // 1次元vector全要素をデバッグ出力
#define dumpvs(v) do { rep(_i_, (v).size()) cerr << (v)[_i_] << lf; cerr << endl; } while(0)     // 1次元vector<string>全要素をデバッグ出力
#define dumpvv(vv) do { rep(_i_, (vv).size()) { rep(_j_, (vv)[_i_].size()) cerr << (vv)[_i_][_j_] << ' '; cerr << endl; } } while(0)  // ２次元vector全要素をデバッグ出力// clang-format on
// clang-format on

#ifdef ONLINE_JUDGE
#define dump(x)
#define dumpv(v)
#define dumpvs(v)
#define dumpvv(vv)
#endif

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    return 0;
}