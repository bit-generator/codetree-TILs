#include <iostream>
#include <unordered_map>
using namespace std;

constexpr size_t MAX_SIZE = 100020;
constexpr size_t MAX_LIST = 11;

struct Node {
    int id;
    int weight;
    int belt;
    Node* prev;
    Node* next;
} nodepool[MAX_SIZE];
int node_cnt = 0;

Node* make_node(int id, int weight, int belt) {
    nodepool[node_cnt].id = id;
    nodepool[node_cnt].weight = weight;
    nodepool[node_cnt].belt = belt;
    return &nodepool[node_cnt++];
}

struct LinkedList {
    Node* head;
    Node* tail;

    void init() {
        this->head = make_node(-1, -1, -1);
        this->tail = this->head;
    }

    void insert(Node* newnode) {
        newnode->next = this->head->next;
        if (newnode->next != nullptr) {
            newnode->next->prev = newnode;
        }

        this->head->next = newnode;
        newnode->prev = this->head;
        if (this->tail == this->head) this->tail = newnode;
    }

    void remove() {
        if (this->head == this->tail) return;
        this->tail = this->tail->prev;
        this->tail->next = nullptr;
    }
} lList[MAX_LIST];

int q, n, m, cmd, id, w;
int ids[MAX_SIZE], weights[MAX_SIZE];
unordered_map<int, Node*> boxes;
int parent[MAX_LIST];
int beltprev[MAX_LIST];
int beltnext[MAX_LIST];

int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> q;
    while (q--) {
        cin >> cmd;
        if (cmd == 100) {
            cin >> n >> m;

            for (int i = 0; i < n; ++i) cin >> ids[i];
            for (int i = 0; i < n; ++i) cin >> weights[i];

            for (int i = 0; i < m; ++i) {
                lList[i].init();
                parent[i] = i;
                beltprev[i] = (i + m - 1) % m;
                beltnext[i] = (i + 1) % m;

                for (int j = 0; j < n / m; ++j) {
                    Node* tmp = make_node(ids[i * n / m + j], weights[i * n / m + j], i);
                    lList[i].insert(tmp);
                    boxes.emplace(ids[i * n / m + j], tmp);
                }
            }
        }
        else if (cmd == 200) {
            int w_max;
            cin >> w_max;

            long long ret = 0;
            int idx = 0;
            while (parent[idx] != idx) idx = beltnext[idx];

            for (int i = 0; i < m; ++i) {
                Node* tmp = lList[idx].tail;
                lList[idx].remove();

                if (tmp->weight <= w_max) {
                    ret += tmp->weight;
                    boxes.erase(tmp->id);
                }
                else lList[idx].insert(tmp);

                idx = beltnext[idx];
            }

            cout << ret << '\n';
        }
        else if (cmd == 300) {
            int r_id;
            cin >> r_id;

            if (boxes.find(r_id) == boxes.end()) {
                cout << -1 << '\n';
                continue;
            }

            Node* tmp = boxes[r_id];
            boxes.erase(r_id);

            tmp->prev->next = tmp->next;
            if (tmp->next == nullptr) {
                lList[find(tmp->belt)].tail = tmp->prev;
            }
            else {
                tmp->next->prev = tmp->prev;
            }

            cout << r_id << '\n';
        }
        else if (cmd == 400) {
            int f_id;
            cin >> f_id;

            if (boxes.find(f_id) == boxes.end()) {
                cout << -1 << '\n';
                continue;
            }

            Node* last = boxes[f_id];
            int beltNum = find(last->belt);
            Node* start = lList[beltNum].head->next;

            if(last != lList[beltNum].tail) {
                lList[beltNum].head->next = last->next;
                last->next->prev = lList[beltNum].head;

                lList[beltNum].tail->next = start;
                start->prev = lList[beltNum].tail;
                lList[beltNum].tail = last;
                last->next = nullptr;
            }
            

            cout << beltNum + 1 << '\n';
        }
        else {
            int b_num;
            cin >> b_num;

            if (parent[b_num - 1] != b_num - 1) {
                cout << -1 << '\n';
                continue;
            }

            int target = beltnext[b_num - 1];
            Node* start = lList[b_num - 1].head->next;
            Node* last = lList[b_num - 1].tail;

            last->next = lList[target].head->next;
            lList[target].head->next->prev = last;

            lList[target].head->next = start;
            start->prev = lList[target].head;

            beltprev[beltnext[b_num - 1]] = beltprev[b_num - 1];
            beltnext[beltprev[b_num - 1]] = beltnext[b_num - 1];
            parent[b_num - 1] = target;
            --m;

            cout << b_num << '\n';
        }
    }

    return 0;
}