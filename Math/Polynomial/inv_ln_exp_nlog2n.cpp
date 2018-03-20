/**************************************************************
 * Problem: 求贝尔数 (w_n) 的前 n 项 (w_0 .. w_{n-1}) mod 998244353. n <= 10^5
 * Author: _rqy
 * Date: 2018 Mar 20
 * Note: sum{n} w_n x^n / n! = e^(e^x - 1)
**************************************************************/
#include <algorithm>
#include <cctype>
#include <cstdio>

typedef long long LL;

const int mod = 998244353;
const int g = 3;
const int N = 100050;

inline LL pow_mod(LL x, int p) {
  LL ans = 1;
  for ((p += mod - 1) %= (mod - 1); p; p >>= 1, (x *= x) %= mod)
    if (p & 1) (ans *= x) %= mod;
  return ans;
}

int n;
LL inv[N], invf[N], fac[N];

void NTT(LL *A, int len, int opt) {
  for (int i = 1, j = 0; i < len; ++i) {
    int k = len;
    while (~j & k) j ^= (k >>= 1);
    if (i < j) std::swap(A[i], A[j]);
  }

  for (int h = 2; h <= len; h <<= 1) {
    LL wn = pow_mod(g, (mod - 1) / h * opt);
    for (int j = 0; j < len; j += h) {
      LL w = 1;
      for (int i = j; i < j + (h >> 1); ++i) {
        LL _tmp1 = A[i], _tmp2 = A[i + (h >> 1)] * w;
        A[i] = (_tmp1 + _tmp2) % mod;
        A[i + (h >> 1)] = (_tmp1 - _tmp2) % mod;
        (w *= wn) %= mod;
      }
    }
  }

  if (opt == -1)
    for (int i = 0; i < len; ++i)
      (A[i] *= -(mod - 1) / len) %= mod;
}

LL F[N], G[N];
LL T1[N * 4], T2[N * 4];

void Conv(LL *A, int n, LL *B, int m) {
  int len = 1;
  while (len <= n + m) len <<= 1;
  for (int i = 0; i < len; ++i)
    T1[i] = (i < n ? A[i] : 0);
  for (int i = 0; i < len; ++i)
    T2[i] = (i < m ? B[i] : 0);

  NTT(T1, len, 1);
  NTT(T2, len, 1); 
  for (int i = 0; i < len; ++i)
    (T1[i] *= T2[i]) %= mod;
  NTT(T1, len, -1);
}

void Solve(int l, int r) {
  if (l == r - 1) {
    F[l] = (l == 0 ? 1 : F[l] * inv[l] % mod);
    return;
  }

  int mid = (l + r) >> 1;
  Solve(l, mid);
  Conv(F + l, mid - l, G, r - l);
  for (int i = mid; i < r; ++i)
    (F[i] += T1[i - l]) %= mod;
  Solve(mid, r);
}

int main() {
  scanf("%d", &n);
  fac[0] = invf[0] = inv[1] = 1;
  for (int i = 2; i <= n; ++i) inv[i] = -(mod / i) * inv[mod % i] % mod;
  for (int i = 1; i <= n; ++i) invf[i] = inv[i] * invf[i - 1] % mod;
  for (int i = 1; i <= n; ++i) fac[i] = i * fac[i - 1] % mod;
  for (int i = 1; i <= n; ++i) G[i] = invf[i] * i % mod;
  Solve(0, n);
  for (int i = 0; i < n; ++i)
    printf("%lld ", (F[i] * fac[i] % mod + mod) % mod);
  return 0;
}
