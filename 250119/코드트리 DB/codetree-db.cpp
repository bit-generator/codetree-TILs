#include <iostream>
#include <string>
#include <unordered_map>
#include <cstring>
using namespace std;

struct Node {
    int l, r;
    long long sum;
    int cnt;
};

constexpr size_t MAX_SIZE = 100'000 * 30;
constexpr size_t MAX_VAL = 1'000'000'000;
Node segtree[MAX_SIZE];
int node_cnt = 1;
unordered_map<string, int> menu;
unordered_map<int, string> menu_invert;
int Q, price;
string cmd, name;

void update(int node, int s, int e, int idx, int v) {
    if (s == e) {
        if(v == 1) segtree[node].sum = idx;
        else segtree[node].sum = 0;
        segtree[node].cnt += v;
        return;
    }

    int mid = (s + e) >> 1;
    if (idx <= mid) {
        if (segtree[node].l == -1) {
            segtree[node].l = node_cnt++;
            segtree[segtree[node].l] = { -1, -1, 0, 0 };
        }
        update(segtree[node].l, s, mid, idx, v);
    }
    else {
        if (segtree[node].r == -1) {
            segtree[node].r = node_cnt++;
            segtree[segtree[node].r] = { -1, -1, 0, 0 };
        }
        update(segtree[node].r, mid + 1, e, idx, v);
    }

    long long t1 = 0, t2 = 0;
    int cnt1 = 0, cnt2 = 0;
    if (segtree[node].l != -1) {
        t1 = segtree[segtree[node].l].sum;
        cnt1 = segtree[segtree[node].l].cnt;
    }

    if (segtree[node].r != -1) {
        t2 = segtree[segtree[node].r].sum;
        cnt2 = segtree[segtree[node].r].cnt;
    }

    segtree[node].sum = t1 + t2;
    segtree[node].cnt = cnt1 + cnt2;
}

long long sumQuery(int node, int s, int e, int l, int r) {
    if (node == -1) return 0;
    if (r < s || e < l) return 0;
    if (l <= s && e <= r) return segtree[node].sum;
    int mid = (s + e) >> 1;
    return sumQuery(segtree[node].l, s, mid, l, r) + sumQuery(segtree[node].r, mid + 1, e, l, r);
}

int cntQuery(int node, int s, int e, int k) {
    if (node == -1) return 0;
    if (s == e) return segtree[node].sum;
    int l = segtree[node].l != -1 ? segtree[segtree[node].l].cnt : 0;
    int mid = (s + e) >> 1;

    if (k <= l) {
        return cntQuery(segtree[node].l, s, mid, k);
    }
    else {
        return cntQuery(segtree[node].r, mid + 1, e, k - l);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> Q;
    while (Q--) {
        cin >> cmd;
        if (cmd == "init") {
            memset(segtree, 0, sizeof(Node) * MAX_SIZE);
            menu.clear();
            menu_invert.clear();
            node_cnt = 1;
            segtree[0] = { -1, -1, 0, 0 };
        }
        else if (cmd == "insert") {
            cin >> name >> price;
            if (menu.find(name) != menu.end() || menu_invert.find(price) != menu_invert.end()) {
                cout << 0 << '\n';
                continue;
            }

			update(0, 1, MAX_VAL, price, 1);
            menu[name] = price;
            menu_invert[price] = name;
            cout << 1 << '\n';
        }
        else if (cmd == "delete") {
            cin >> name;
            if (menu.find(name) == menu.end()) {
                cout << 0 << '\n';
                continue;
            }

			update(0, 1, MAX_VAL, menu[name], -1);
            int val = menu[name];
            menu_invert.erase(menu[name]);
            menu.erase(name);
            cout << val << '\n';
        }
        else if (cmd == "rank") {
            cin >> price;
            if (price > segtree[0].cnt) {
                cout << "None\n";
                continue;
            }

            cout << menu_invert[cntQuery(0, 1, MAX_VAL, price)] << '\n';
        }
        else {
            cin >> price;
            cout << sumQuery(0, 1, MAX_VAL, 1, price) << '\n';
        }
    }

    return 0;
}