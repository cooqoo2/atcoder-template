#include "mytemplate.h"

// ===== 統合dump =====
template <class T>
struct _isp : false_type
{
};
template <class A, class B>
struct _isp<pair<A, B>> : true_type
{
};
template <class T>
constexpr bool _isp_v = _isp<remove_cvref_t<T>>::value;
template <class T>
constexpr bool _leaf_v = is_arithmetic_v<remove_cvref_t<T>> || is_same_v<remove_cvref_t<T>, string>;
template <class T>
void _inl(ostream &, const T &);
template <class T>
void _wrp(ostream &os, const T &x)
{ // 中身が容器なら()で括る
    if constexpr (_leaf_v<T>)
        os << x;
    else
    {
        os << '(';
        _inl(os, x);
        os << ')';
    }
}
template <class T>
void _inl(ostream &os, const T &x)
{ // 1行に空白区切りで描画
    if constexpr (_leaf_v<T>)
        os << x;
    else if constexpr (_isp_v<T>)
    {
        _wrp(os, x.fi);
        os << ' ';
        _wrp(os, x.se);
    }
    else
    {
        bool f = true;
        for (auto &e : x)
        {
            if (!f)
                os << ' ';
            f = false;
            _wrp(os, e);
        }
    }
}
template <class T>
void _dmp(const char *nm, const T &x)
{
    if constexpr (_leaf_v<T>)
        cerr << nm << " = " << x << '\n'; // 1変数のみ名前付き
    else if constexpr (_isp_v<T>)
    {
        _inl(cerr, x);
        cerr << '\n';
    } // pll
    else
    {
        using E = remove_cvref_t<ranges::range_value_t<T>>;
        if constexpr (_leaf_v<E> && !is_same_v<E, string>) // vll等: 1行
        {
            _inl(cerr, x);
            cerr << '\n';
        }
        else // それ以上: 行ごと+空行
        {
            for (auto &e : x)
            {
                _inl(cerr, e);
                cerr << '\n';
            }
            cerr << '\n';
        }
    }
}
#define dump(x) _dmp(#x, (x))