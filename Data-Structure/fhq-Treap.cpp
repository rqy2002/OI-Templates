/**************************************************************
 * Problem: 普通平衡树
 * Author: _rqy
 * Date: 2018 Mar 16
**************************************************************/
#include <algorithm>
#include <cstdio>
#include <cstdlib>

const int N = 100050;

int ch[N][2], val[N], key[N], num[N], siz[N], cnt, root;

inline int NewNode(int v) {
  val[++cnt] = v;
  key[cnt] = rand();
  num[cnt] = siz[cnt] = 1;
  ch[cnt][0] = ch[cnt][1] = 0;
  return cnt;
}

inline void upd(int o) { siz[o] = num[o] + siz[ch[o][0]] + siz[ch[o][1]]; }

void Split(int o, int x, int &l, int &r) {
  if (o == 0) {
    l = r = 0;
  } else if (val[o] < x) {
    l = o;
    Split(ch[o][1], x, ch[l][1], r);
    upd(l);
  } else {
    r = o;
    Split(ch[o][0], x, l, ch[r][0]);
    upd(r);
  }
}

int Merge(int l, int r) {
  if (l == 0) return r;
  else if (r == 0) return l;
  else if (key[l] > key[r]) {
    ch[l][1] = Merge(ch[l][1], r), upd(l);
    return l;
  } else {
    ch[r][0] = Merge(l, ch[r][0]), upd(r);
    return r;
  }
}

void Modify(int x, int k) {
  // k = 1: Insert; k = -1: Delete.
  int _tl, _tm, _tr;
  Split(root, x, _tl, _tr);
  Split(_tr, x + 1, _tm, _tr);
  if (_tm == 0) _tm = NewNode(x); // k = 1
  else num[_tm] += k, upd(_tm);
  root = Merge(_tl, Merge(_tm, _tr));
}

int Rank(int x) {
  int _tl, _tr;
  Split(root, x, _tl, _tr);
  int ans = siz[_tl];
  root = Merge(_tl, _tr);
  return ans;
}

int Kth(int o, int x) {
  if (x <= siz[ch[o][0]]) return Kth(ch[o][0], x);
  else if (x <= siz[ch[o][0]] + num[o]) return val[o];
  else return Kth(ch[o][1], x - (siz[ch[o][0]] + num[o]));
}

int main() {
  int n;
  scanf("%d", &n);
  for (int i = 0, opt, x; i < n; ++i) {
    scanf("%d%d", &opt, &x);
    if (opt == 1) Modify(x, 1);
    else if (opt == 2) Modify(x, -1);
    else if (opt == 3) printf("%d\n", Rank(x) + 1);
    else if (opt == 4) printf("%d\n", Kth(root, x));
    else if (opt == 5) printf("%d\n", Kth(root, Rank(x)));
    else if (opt == 6) printf("%d\n", Kth(root, Rank(x + 1) + 1));
  }
  return 0;
}
