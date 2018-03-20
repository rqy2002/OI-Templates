/**************************************************************
 * Problem: 求贝尔数 (w_n) 的前 n 项 (w_0 .. w_{n-1}) mod 998244353. n <= 10^5
 * Author: _rqy
 * Date: 2018 Mar 20
 * Note: sum{n} w_n x^n / n! = e^(e^x - 1)
**************************************************************/
#include <algorithm>
#include <cstdio>

typedef long long LL;

const int N = 100050 * 4;
const int mod = 998244353;
const int g = 3;

int n;
LL inv[N], invf[N], fac[N];

LL pow_mod(LL x, LL b) {
  LL ans = 1;
  for ((b += mod - 1) %= (mod - 1); b != 0; b >>= 1, (x *= x) %= mod)
    if ((b & 1) == 1) (ans *= x) %= mod;
  return ans;
}

inline void Mod(LL &x) {
  if (x >= mod) x -= mod;
  else if (x <= -mod) x += mod;
}

void NTT(LL *A, int len, int opt) {
  for (int i = 1, j = 0; i < len; ++i) {
    int k = len;
    do j ^= (k >>= 1); while ((j & k) == 0);
    if (i < j) std::swap(A[i], A[j]);
  }

  for (int h = 2; h <= len; h <<= 1) {
    LL wn = pow_mod(g, (mod - 1) / h * opt);
    for (int j = 0; j < len; j += h) {
      LL w = 1;
      for (int i = j; i < j + (h >> 1); ++i) {
        LL _t1 = A[i], _t2 = A[i + (h >> 1)] * w % mod;
        Mod(A[i] = _t1 + _t2);
        Mod(A[i + (h >> 1)] = _t1 - _t2);
        (w *= wn) %= mod;
      }
    }
  }

  if (opt == -1)
    for (int i = 0; i < len; ++i)
      (A[i] *= -(mod - 1) / len) %= mod;
}

void PolyInv(const LL *A, int len, LL *B) {
  static LL tA[N], tB[N];

  if (len == 1) {
    B[0] = pow_mod(A[0], mod - 2);
    return;
  }

  PolyInv(A, len >> 1, B);
  std::copy(B, B + (len >> 1), tB);
  std::fill(tB + (len >> 1), tB + (len << 1), 0LL);
  std::copy(A, A + len, tA);
  std::fill(tA + len, tA + (len << 1), 0LL);
  NTT(tB, len << 1, 1);
  NTT(tA, len << 1, 1);

  for (int i = 0; i < (len << 1); ++i)
    tA[i] = (2 - tA[i] * tB[i]) % mod * tB[i] % mod;
  NTT(tA, len << 1, -1);
  std::copy(tA, tA + len, B);
}

void PolyLn(const LL *A, int len, LL *B) {
  static LL t1[N], t2[N];

  PolyInv(A, len, t1);
  std::fill(t1 + len, t1 + (len << 1), 0LL);
  for (int i = 0; i < len - 1; ++i)
    t2[i] = A[i + 1] * (i + 1) % mod;
  std::fill(t2 + len - 1, t2 + (len << 1), 0LL);

  NTT(t1, len << 1, 1);
  NTT(t2, len << 1, 1);
  for (int i = 0; i < (len << 1); ++i)
    (t1[i] *= t2[i]) %= mod;
  NTT(t1, len << 1, -1);
  B[0] = 0;
  for (int i = 1; i < len; ++i)
    B[i] = inv[i] * t1[i - 1] % mod;
}

void PolyExp(const LL *A, int len, LL *B) {
  static LL t1[N], t2[N];
  
  if (len == 1) {
    B[0] = 1;
    return;
  }

  PolyExp(A, len >> 1, B);
  std::fill(B + (len >> 1), B + len, 0LL);
  PolyLn(B, len, t1);
  std::copy(B, B + len, t2);
  for (int i = 0; i < (len >> 1); ++i)
    t1[i] = (A[i + (len >> 1)] - t1[i + (len >> 1)]) % mod;
  std::fill(t1 + (len >> 1), t1 + len, 0LL);
  NTT(t1, len, 1);
  NTT(t2, len, 1);
  for (int i = 0; i < len; ++i)
    (t1[i] *= t2[i]) %= mod;
  NTT(t1, len, -1);
  std::copy(t1, t1 + (len >> 1), B + (len >> 1));
}

LL A[N], B[N];

int main() {
  scanf("%d", &n);
  int m = 1;
  while (m < n) m <<= 1;
  fac[0] = invf[0] = inv[1] = 1;
  for (int i = 2; i < m; ++i) inv[i] = -(mod / i) * inv[mod % i] % mod;
  for (int i = 1; i < m; ++i) invf[i] = inv[i] * invf[i - 1] % mod;
  for (int i = 1; i < m; ++i) fac[i] = i * fac[i - 1] % mod;
  std::copy(invf + 1, invf + m, A + 1);
  PolyExp(A, m, B);
  for (int i = 0; i < n; ++i)
    printf("%lld ", (B[i] * fac[i] % mod + mod) % mod);
  return 0;
}
