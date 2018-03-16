/**************************************************************
 * Problem: 普通平衡树
 * Author: _rqy
 * Date: 2018 Mar 16
**************************************************************/
#include <algorithm>
#include <cstdio>

const int N = 100050;

int fa[N], ch[N][2], num[N], siz[N], val[N], cnt, root;

inline int NewNode(int v) {
  val[++cnt] = v;
  ch[cnt][0] = ch[cnt][1] = 0;
  num[cnt] = siz[cnt] = 1;
  return cnt;
}

inline int dir(int x) { return ch[fa[x]][1] == x; }
inline void upd(int x) { siz[x] = num[x] + siz[ch[x][0]] + siz[ch[x][1]]; }

inline void Rotate(int x) {
  int d = dir(x), f = fa[x];
  if ((ch[f][d] = ch[x][d ^ 1]) != 0) fa[ch[f][d]] = f;
  if ((fa[x] = fa[f]) != 0) ch[fa[x]][dir(f)] = x;
  else root = x;
  upd(ch[x][d ^ 1] = f);
  upd(fa[f] = x);
}

void Splay(int x) {
  for (int f; (f = fa[x]) != 0; Rotate(x))
    if (fa[f]) Rotate(dir(f) == dir(x) ? f : x);
}

void Insert(int x) {
  if (root == 0) {
    root = NewNode(x);
    return;
  }
  int o = root;
  while (1) {
    int d = (val[o] < x);
    if (val[o] == x) {
      ++num[o];
      ++siz[o];
      break;
    } if (ch[o][d] == 0) {
      ch[o][d] = NewNode(x);
      fa[ch[o][d]] = o;
      o = ch[o][d];
      break;
    } else {
      o = ch[o][d];
    }
  }
  Splay(o);
  upd(o);
}

void Find(int x) {
  // find the max v in tree that v <= x
  int o = root, res = 0;
  while (o != 0) {
    if (val[o] == x) res = o, o = 0;
    else if (val[o] < x) res = o, o = ch[o][1];
    else if (ch[o][0] == 0 && res == 0) res = o, o = 0;
    else o = ch[o][0];
  }
  Splay(res);
}

void Delete(int x) {
  Find(x);
  if (num[root] > 1) {
    --num[root];
    upd(root);
  } else if (ch[root][0] == 0) {
    fa[root = ch[root][1]] = 0;
  } else if (ch[root][1] == 0) {
    fa[root = ch[root][0]] = 0;
  } else {
    int _tmp = root, o = ch[root][0];
    while (ch[o][1] != 0) o = ch[o][1];
    Splay(o);
    ch[o][1] = ch[_tmp][1];
    upd(fa[ch[_tmp][1]] = o);
  }
}

int Kth(int x) {
  int o = root;
  while (1) {
    if (x <= siz[ch[o][0]]) o = ch[o][0];
    else if (x <= siz[ch[o][0]] + num[o]) return val[o];
    else x -= siz[ch[o][0]] + num[o], o = ch[o][1];
  }
}

int main() {
  int n;
  scanf("%d", &n);
  for (int i = 0, opt, x; i < n; ++i) {
    scanf("%d%d", &opt, &x);
    if (opt == 1) {
      Insert(x);
    } else if (opt == 2) {
      Delete(x);
    } else if (opt == 3) {
      Find(x);
      printf("%d\n", siz[ch[root][0]] + 1);
    } else if (opt == 4) {
      printf("%d\n", Kth(x));
    } else if (opt == 5) {
      Find(x - 1);
      printf("%d\n", val[root]);
    } else {
      Find(x);
      if (val[root] > x) {
        printf("%d\n", val[root]);
      } else {
        int o = ch[root][1];
        while (ch[o][0] != 0) o = ch[o][0];
        printf("%d\n", val[o]);
      }
    }
  }
  return 0;
}
