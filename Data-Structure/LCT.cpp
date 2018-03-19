/**************************************************************
 * Problem: 弹飞绵羊
 * Author: _rqy
 * Date: 2018 Mar 19
**************************************************************/
#include <algorithm>
#include <cstdio>

const int N = 200050;

int fa[N], ch[N][2], siz[N];
bool rev[N];

inline void pushd(int x) {
  if (rev[x]) {
    std::swap(ch[x][0], ch[x][1]);
    rev[ch[x][0]] ^= 1;
    rev[ch[x][1]] ^= 1;
    rev[x] = 0;
  }
}

inline void upd(int x) { 
  siz[x] = 1 + siz[ch[x][0]] + siz[ch[x][1]];
}

inline int dir(int x) {
  if (ch[fa[x]][0] == x) return 0;
  else if (ch[fa[x]][1] == x) return 1;
  else return -1;
}

void Rotate(int x) {
  int d = dir(x), f = fa[x];
  fa[x] = fa[f];
  if (dir(f) != -1) ch[fa[f]][dir(f)] = x;
  if ((ch[f][d] = ch[x][d ^ 1]) != 0) fa[ch[f][d]] = f;
  upd(ch[x][d ^ 1] = f);
  upd(fa[f] = x);
}

void Splay(int x) {
  static int stack[N];
  int top = 0;
  for (int y = x; dir(y) != -1; y = fa[y]) stack[top++] = fa[y];
  while (top--) pushd(stack[top]);
  pushd(x);
  for (; dir(x) != -1; Rotate(x))
    if (dir(fa[x]) != -1) Rotate(dir(fa[x]) == dir(x) ? fa[x] : x);
}

void Access(int x) {
  int y = 0;
  while (x != 0) { Splay(x); ch[x][1] = y; upd(y = x); x = fa[y]; }
}

inline void MkRoot(int x) { Access(x); Splay(x); rev[x] ^= 1; }
inline void Link(int x, int y) { MkRoot(x); fa[x] = y; }
inline void Cut(int x, int y) {
  MkRoot(x); Access(y); Splay(y); fa[x] = ch[y][0] = 0;
}
inline int Query(int x, int y) {
  MkRoot(x); Access(y); Splay(y); return siz[y];
}

int n, nxt[N];

int main() {
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i) {
    scanf("%d", &nxt[i]);
    Link(i, nxt[i] = std::min(nxt[i] + i, n + 1));
    siz[i] = 1;
  }
  siz[n + 1] = 1;

  int m;
  scanf("%d", &m);
  while (m--) {
    int opt, x, y;
    scanf("%d%d", &opt, &x);
    ++x;
    if (opt == 1) {
      printf("%d\n", Query(x, n + 1) - 1);
    } else {
      scanf("%d", &y);
      Cut(x, nxt[x]);
      Link(x, nxt[x] = std::min(x + y, n + 1));
    }
  }
  return 0;
}
