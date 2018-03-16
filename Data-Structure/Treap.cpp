/**************************************************************
 * Problem: 普通平衡树
 * Author: _rqy
 * Date: 2018 Mar 16
**************************************************************/
#include <algorithm>
#include <cstdio>
#include <cstdlib>

const int N = 100050;

// key[0] = -1
int ch[N][2], num[N], siz[N], val[N], key[N], cnt, root;

inline int NewNode(int v) {
  val[++cnt] = v;
  key[cnt] = rand();
  ch[cnt][0] = ch[cnt][1] = 0;
  num[cnt] = siz[cnt] = 1;
  return cnt;
}

inline void upd(int x) { siz[x] = num[x] + siz[ch[x][0]] + siz[ch[x][1]]; }

inline void Rotate(int &o, int d) {
  int c = ch[o][d];
  ch[o][d] = ch[c][d ^ 1];
  upd(ch[c][d ^ 1] = o);
  upd(o = c);
}

void Insert(int &o, int x) {
  if (!o) {
    o = NewNode(x);
  } else if (val[o] == x) {
    ++num[o];
  } else {
    int d = (val[o] < x);
    Insert(ch[o][d], x);
    if (key[ch[o][d]] > key[o])
      Rotate(o, d);
  }
  upd(o);
}

void Delete(int &o, int x) {
  if (val[o] == x) {
    if (num[o] > 1) {
      --num[o];
    } else if (ch[o][0] == 0 && ch[o][1] == 0) {
      o = 0;
    } else {
      int d = key[ch[o][1]] > key[ch[o][0]];
      Rotate(o, d);
      Delete(ch[o][d ^ 1], x);
    }
  } else {
    int d = val[o] < x;
    Delete(ch[o][d], x);
  }
  upd(o);
}

int Query(int o, int x) {
  if (o == 0) return 0;
  else if (val[o] < x) return siz[ch[o][0]] + num[o] + Query(ch[o][1], x);
  else return Query(ch[o][0], x);
}

int Kth(int o, int x) {
  if (x <= siz[ch[o][0]]) return Kth(ch[o][0], x);
  else if (x <= siz[ch[o][0]] + num[o]) return val[o];
  else return Kth(ch[o][1], x - (siz[ch[o][0]] + num[o]));
}

int main() {
  int n;
  scanf("%d", &n);
  key[0] = -1;
  for (int i = 0, opt, x; i < n; ++i) {
    scanf("%d%d", &opt, &x);
    if (opt == 1) Insert(root, x);
    else if (opt == 2) Delete(root, x);
    else if (opt == 3) printf("%d\n", Query(root, x) + 1);
    else if (opt == 4) printf("%d\n", Kth(root, x));
    else if (opt == 5) printf("%d\n", Kth(root, Query(root, x)));
    else if (opt == 6) printf("%d\n", Kth(root, Query(root, x + 1) + 1));
  }
  return 0;
}
