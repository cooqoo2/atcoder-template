#include "mytemplate.h"

// =====================================================================
//  セグメント木
//   - SegTree<M>      : 一点更新・区間取得 (非再帰)
//   - LazySegTree<M>  : 区間作用・区間取得 (遅延評価)
//   - 下の方にプリセットのモノイド多数
//  区間はすべて半開区間 [l, r)、0-indexed
// =====================================================================

// ---------------------------------------------------------------------
//  SegTree<M>
//   M に必要なもの:
//     using S;  static S op(S a, S b);  static S e();
//
//   SegTree<M> seg(n);            // 全要素 e() で初期化
//   SegTree<M> seg(vector<S> v);  // v で初期化 O(n)
//   seg.set(p, x)       a[p] = x                         O(log n)
//   seg.chop(p, x)      a[p] = op(a[p], x)  (加算/chmin等) O(log n)
//   seg.get(p)          a[p]                             O(1)
//   seg.prod(l, r)      op(a[l], ..., a[r-1])            O(log n)
//   seg.all_prod()      op(a[0], ..., a[n-1])            O(1)
//   seg.max_right(l, f) f(prod(l, r)) が true の最大 r  O(log n)
//   seg.min_left(r, f)  f(prod(l, r)) が true の最小 l  O(log n)
//     ※ f は単調 (true...true false...false)、f(e()) == true が必要
// ---------------------------------------------------------------------
template <class M>
struct SegTree
{
    using S = typename M::S;
    ll n, sz, lg;
    vector<S> d;

    SegTree() : SegTree(0) {}
    explicit SegTree(ll n_) : SegTree(vector<S>(n_, M::e())) {}
    explicit SegTree(const vector<S> &v) : n((ll)v.size())
    {
        lg = 0;
        while ((1LL << lg) < n)
            lg++;
        sz = 1LL << lg;
        d.assign(2 * sz, M::e());
        rep(i, n) d[sz + i] = v[i];
        for (ll i = sz - 1; i >= 1; i--)
            update(i);
    }

    void set(ll p, S x)
    {
        assert(0 <= p && p < n);
        p += sz;
        d[p] = x;
        for (ll i = 1; i <= lg; i++)
            update(p >> i);
    }
    void chop(ll p, S x) { set(p, M::op(get(p), x)); }
    S get(ll p) const
    {
        assert(0 <= p && p < n);
        return d[p + sz];
    }
    S prod(ll l, ll r) const
    {
        assert(0 <= l && l <= r && r <= n);
        S sml = M::e(), smr = M::e();
        l += sz, r += sz;
        while (l < r)
        {
            if (l & 1)
                sml = M::op(sml, d[l++]);
            if (r & 1)
                smr = M::op(d[--r], smr);
            l >>= 1, r >>= 1;
        }
        return M::op(sml, smr);
    }
    S all_prod() const { return d[1]; }

    template <class F>
    ll max_right(ll l, F f) const
    {
        assert(0 <= l && l <= n);
        assert(f(M::e()));
        if (l == n)
            return n;
        l += sz;
        S sm = M::e();
        do
        {
            while (l % 2 == 0)
                l >>= 1;
            if (!f(M::op(sm, d[l])))
            {
                while (l < sz)
                {
                    l = 2 * l;
                    if (f(M::op(sm, d[l])))
                        sm = M::op(sm, d[l++]);
                }
                return l - sz;
            }
            sm = M::op(sm, d[l++]);
        } while ((l & -l) != l);
        return n;
    }
    template <class F>
    ll min_left(ll r, F f) const
    {
        assert(0 <= r && r <= n);
        assert(f(M::e()));
        if (r == 0)
            return 0;
        r += sz;
        S sm = M::e();
        do
        {
            r--;
            while (r > 1 && (r % 2))
                r >>= 1;
            if (!f(M::op(d[r], sm)))
            {
                while (r < sz)
                {
                    r = 2 * r + 1;
                    if (f(M::op(d[r], sm)))
                        sm = M::op(d[r--], sm);
                }
                return r + 1 - sz;
            }
            sm = M::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
    vector<S> to_vec() const
    {
        vector<S> res(n);
        rep(i, n) res[i] = get(i);
        return res;
    }

private:
    void update(ll k) { d[k] = M::op(d[2 * k], d[2 * k + 1]); }
};

// ---------------------------------------------------------------------
//  LazySegTree<M>
//   M に必要なもの:
//     using S;  static S op(S a, S b);  static S e();
//     using F;  static S mapping(F f, S x);        // f(x)
//               static F composition(F f, F g);    // f∘g (g を先に適用)
//               static F id();                     // 恒等写像
//
//   LazySegTree<M> seg(n) / seg(vector<S> v)
//   seg.set(p, x) / seg.get(p) / seg.prod(l, r) / seg.all_prod()
//   seg.apply(p, f)        a[p] = f(a[p])
//   seg.apply(l, r, f)     i∈[l,r) について a[i] = f(a[i])
//   seg.max_right(l, g) / seg.min_left(r, g)   (SegTree と同じ)
// ---------------------------------------------------------------------
template <class M>
struct LazySegTree
{
    using S = typename M::S;
    using F = typename M::F;
    ll n, sz, lg;
    vector<S> d;
    vector<F> lz;

    LazySegTree() : LazySegTree(0) {}
    explicit LazySegTree(ll n_) : LazySegTree(vector<S>(n_, M::e())) {}
    explicit LazySegTree(const vector<S> &v) : n((ll)v.size())
    {
        lg = 0;
        while ((1LL << lg) < n)
            lg++;
        sz = 1LL << lg;
        d.assign(2 * sz, M::e());
        lz.assign(sz, M::id());
        rep(i, n) d[sz + i] = v[i];
        for (ll i = sz - 1; i >= 1; i--)
            update(i);
    }

    void set(ll p, S x)
    {
        assert(0 <= p && p < n);
        p += sz;
        for (ll i = lg; i >= 1; i--)
            push(p >> i);
        d[p] = x;
        for (ll i = 1; i <= lg; i++)
            update(p >> i);
    }
    S get(ll p)
    {
        assert(0 <= p && p < n);
        p += sz;
        for (ll i = lg; i >= 1; i--)
            push(p >> i);
        return d[p];
    }
    S prod(ll l, ll r)
    {
        assert(0 <= l && l <= r && r <= n);
        if (l == r)
            return M::e();
        l += sz, r += sz;
        for (ll i = lg; i >= 1; i--)
        {
            if (((l >> i) << i) != l)
                push(l >> i);
            if (((r >> i) << i) != r)
                push((r - 1) >> i);
        }
        S sml = M::e(), smr = M::e();
        while (l < r)
        {
            if (l & 1)
                sml = M::op(sml, d[l++]);
            if (r & 1)
                smr = M::op(d[--r], smr);
            l >>= 1, r >>= 1;
        }
        return M::op(sml, smr);
    }
    S all_prod() const { return d[1]; }

    void apply(ll p, F f)
    {
        assert(0 <= p && p < n);
        p += sz;
        for (ll i = lg; i >= 1; i--)
            push(p >> i);
        d[p] = M::mapping(f, d[p]);
        for (ll i = 1; i <= lg; i++)
            update(p >> i);
    }
    void apply(ll l, ll r, F f)
    {
        assert(0 <= l && l <= r && r <= n);
        if (l == r)
            return;
        l += sz, r += sz;
        for (ll i = lg; i >= 1; i--)
        {
            if (((l >> i) << i) != l)
                push(l >> i);
            if (((r >> i) << i) != r)
                push((r - 1) >> i);
        }
        {
            ll l2 = l, r2 = r;
            while (l < r)
            {
                if (l & 1)
                    all_apply(l++, f);
                if (r & 1)
                    all_apply(--r, f);
                l >>= 1, r >>= 1;
            }
            l = l2, r = r2;
        }
        for (ll i = 1; i <= lg; i++)
        {
            if (((l >> i) << i) != l)
                update(l >> i);
            if (((r >> i) << i) != r)
                update((r - 1) >> i);
        }
    }

    template <class G>
    ll max_right(ll l, G g)
    {
        assert(0 <= l && l <= n);
        assert(g(M::e()));
        if (l == n)
            return n;
        l += sz;
        for (ll i = lg; i >= 1; i--)
            push(l >> i);
        S sm = M::e();
        do
        {
            while (l % 2 == 0)
                l >>= 1;
            if (!g(M::op(sm, d[l])))
            {
                while (l < sz)
                {
                    push(l);
                    l = 2 * l;
                    if (g(M::op(sm, d[l])))
                        sm = M::op(sm, d[l++]);
                }
                return l - sz;
            }
            sm = M::op(sm, d[l++]);
        } while ((l & -l) != l);
        return n;
    }
    template <class G>
    ll min_left(ll r, G g)
    {
        assert(0 <= r && r <= n);
        assert(g(M::e()));
        if (r == 0)
            return 0;
        r += sz;
        for (ll i = lg; i >= 1; i--)
            push((r - 1) >> i);
        S sm = M::e();
        do
        {
            r--;
            while (r > 1 && (r % 2))
                r >>= 1;
            if (!g(M::op(d[r], sm)))
            {
                while (r < sz)
                {
                    push(r);
                    r = 2 * r + 1;
                    if (g(M::op(d[r], sm)))
                        sm = M::op(d[r--], sm);
                }
                return r + 1 - sz;
            }
            sm = M::op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
    vector<S> to_vec()
    {
        vector<S> res(n);
        rep(i, n) res[i] = get(i);
        return res;
    }

private:
    void update(ll k) { d[k] = M::op(d[2 * k], d[2 * k + 1]); }
    void all_apply(ll k, F f)
    {
        d[k] = M::mapping(f, d[k]);
        if (k < sz)
            lz[k] = M::composition(f, lz[k]);
    }
    void push(ll k)
    {
        all_apply(2 * k, lz[k]);
        all_apply(2 * k + 1, lz[k]);
        lz[k] = M::id();
    }
};

// =====================================================================
//  プリセット: SegTree 用モノイド
// =====================================================================
struct M_Sum
{ // 区間和
    using S = ll;
    static S op(S a, S b) { return a + b; }
    static S e() { return 0; }
};
struct M_Min
{ // 区間最小
    using S = ll;
    static S op(S a, S b) { return min(a, b); }
    static S e() { return INF; }
};
struct M_Max
{ // 区間最大
    using S = ll;
    static S op(S a, S b) { return max(a, b); }
    static S e() { return -INF; }
};
struct M_Xor
{ // 区間XOR
    using S = ll;
    static S op(S a, S b) { return a ^ b; }
    static S e() { return 0; }
};
struct M_Gcd
{ // 区間GCD
    using S = ll;
    static S op(S a, S b) { return gcd(a, b); }
    static S e() { return 0; }
};
struct M_MinIdx
{                  // 区間最小とその位置 {値, index}  (同値なら左)
    using S = pll; // 初期化: v[i] = {a[i], i}
    static S op(S a, S b) { return min(a, b); }
    static S e() { return {INF, INF}; }
};
struct M_MaxIdx
{                  // 区間最大とその位置 {値, -index}  (同値なら左)
    using S = pll; // 初期化: v[i] = {a[i], -i}  → 位置は -res.second
    static S op(S a, S b) { return max(a, b); }
    static S e() { return {-INF, -INF}; }
};
template <ll MOD>
struct M_Affine
{                  // 一次関数 x→ax+b の合成 (左から順に適用)
    using S = pll; // {a, b}
    static S op(S f, S g)
    { // f を先に、g を後に
        return {f.first * g.first % MOD, (f.second * g.first + g.second) % MOD};
    }
    static S e() { return {1, 0}; }
    static ll eval(S f, ll x) { return (f.first * x + f.second) % MOD; }
};
struct M_MaxSubarray
{ // 最大部分配列和 (空配列不可)
    struct S
    {
        ll tot, pre, suf, best;
    }; // 初期化: v[i] = {a, a, a, a}
    static S op(S a, S b)
    {
        if (a.best == -INF)
            return b;
        if (b.best == -INF)
            return a;
        return {a.tot + b.tot, max(a.pre, a.tot + b.pre), max(b.suf, b.tot + a.suf),
                max({a.best, b.best, a.suf + b.pre})};
    }
    static S e() { return {0, -INF, -INF, -INF}; }
};

// =====================================================================
//  プリセット: LazySegTree 用
//   ※ 和を扱うものは S = {値, 区間長}。初期化は v[i] = {a[i], 1}
// =====================================================================
struct SL
{ // 区間長つきの和
    ll s, len;
};

struct L_AddMin
{ // 区間加算・区間最小
    using S = ll;
    using F = ll;
    static S op(S a, S b) { return min(a, b); }
    static S e() { return INF; }
    static S mapping(F f, S x) { return x == INF ? x : x + f; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
struct L_AddMax
{ // 区間加算・区間最大
    using S = ll;
    using F = ll;
    static S op(S a, S b) { return max(a, b); }
    static S e() { return -INF; }
    static S mapping(F f, S x) { return x == -INF ? x : x + f; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
struct L_AddSum
{ // 区間加算・区間和   (v[i] = {a[i], 1})
    using S = SL;
    using F = ll;
    static S op(S a, S b) { return {a.s + b.s, a.len + b.len}; }
    static S e() { return {0, 0}; }
    static S mapping(F f, S x) { return {x.s + f * x.len, x.len}; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};

constexpr ll LAZY_NONE = LLONG_MIN; // 「更新なし」を表す値 (代入値として使えない)
struct L_UpdMin
{ // 区間代入・区間最小
    using S = ll;
    using F = ll;
    static S op(S a, S b) { return min(a, b); }
    static S e() { return INF; }
    static S mapping(F f, S x) { return f == LAZY_NONE ? x : f; }
    static F composition(F f, F g) { return f == LAZY_NONE ? g : f; }
    static F id() { return LAZY_NONE; }
};
struct L_UpdMax
{ // 区間代入・区間最大
    using S = ll;
    using F = ll;
    static S op(S a, S b) { return max(a, b); }
    static S e() { return -INF; }
    static S mapping(F f, S x) { return f == LAZY_NONE ? x : f; }
    static F composition(F f, F g) { return f == LAZY_NONE ? g : f; }
    static F id() { return LAZY_NONE; }
};
struct L_UpdSum
{ // 区間代入・区間和   (v[i] = {a[i], 1})
    using S = SL;
    using F = ll;
    static S op(S a, S b) { return {a.s + b.s, a.len + b.len}; }
    static S e() { return {0, 0}; }
    static S mapping(F f, S x) { return f == LAZY_NONE ? x : S{f * x.len, x.len}; }
    static F composition(F f, F g) { return f == LAZY_NONE ? g : f; }
    static F id() { return LAZY_NONE; }
};
struct L_ChminMin
{ // 区間chmin・区間最小
    using S = ll;
    using F = ll;
    static S op(S a, S b) { return min(a, b); }
    static S e() { return INF; }
    static S mapping(F f, S x) { return min(f, x); }
    static F composition(F f, F g) { return min(f, g); }
    static F id() { return INF; }
};
struct L_ChmaxMax
{ // 区間chmax・区間最大
    using S = ll;
    using F = ll;
    static S op(S a, S b) { return max(a, b); }
    static S e() { return -INF; }
    static S mapping(F f, S x) { return max(f, x); }
    static F composition(F f, F g) { return max(f, g); }
    static F id() { return -INF; }
};
template <ll MOD>
struct L_AffineSum
{ // 区間 a[i]=b*a[i]+c・区間和 mod  (v[i] = {a[i], 1})
    using S = SL;
    using F = pll; // F = {b, c}
    static S op(S a, S b) { return {(a.s + b.s) % MOD, a.len + b.len}; }
    static S e() { return {0, 0}; }
    static S mapping(F f, S x) { return {(f.first * x.s + f.second * x.len) % MOD, x.len}; }
    static F composition(F f, F g)
    {
        return {f.first * g.first % MOD, (f.first * g.second + f.second) % MOD};
    }
    static F id() { return {1, 0}; }
};
struct L_FlipSum
{ // 01列: 区間反転・区間の1の個数  (v[i] = {a[i], 1})
    using S = SL;
    using F = bool;
    static S op(S a, S b) { return {a.s + b.s, a.len + b.len}; }
    static S e() { return {0, 0}; }
    static S mapping(F f, S x) { return f ? S{x.len - x.s, x.len} : x; }
    static F composition(F f, F g) { return f ^ g; }
    static F id() { return false; }
};
struct L_AddMinCnt
{ // 区間加算・{区間最小, その個数}  (長方形の和集合面積など)
    using S = pll;
    using F = ll; // v[i] = {a[i], 1}  (重み付きなら {a[i], 幅})
    static S op(S a, S b)
    {
        if (a.first != b.first)
            return a.first < b.first ? a : b;
        return {a.first, a.second + b.second};
    }
    static S e() { return {INF, 0}; }
    static S mapping(F f, S x) { return x.first == INF ? x : S{x.first + f, x.second}; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};

/*
===== 使用例 =====
SegTree<M_Sum> seg(n);                 seg.chop(i, x); seg.prod(l, r);
SegTree<M_Min> seg(a);                 // a は vll
ll r = seg.max_right(l, [&](ll v) { return v >= k; });  // a[l..r) が全部 k 以上の最大 r

vector<SL> v(n, {0, 1});
LazySegTree<L_AddSum> lseg(v);         lseg.apply(l, r, x); lseg.prod(l, r).s;
LazySegTree<L_AffineSum<998244353>> aseg(v);  aseg.apply(l, r, {b, c});

// 自作モノイドは M_xxx / L_xxx の形をコピーして書き換える
*/