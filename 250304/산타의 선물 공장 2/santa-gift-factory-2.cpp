#include <iostream>
using namespace std;

constexpr size_t MAX_NODE = 200000;
constexpr size_t MAX_SIZE = 100001;
int q, cmd, n, m, m_src, m_dst;

struct Node {
    int id;
    Node* next;
    Node* prev;
} nodepool[MAX_NODE];
int nodeCnt = 0;

Node* makeNode(int data) {
    nodepool[nodeCnt].id = data;
    return &nodepool[nodeCnt++];
}

Node* boxes[MAX_SIZE];

struct LinkedList {
    Node* head;
    Node* tail;
    int size;

    LinkedList() {
        tail = makeNode(-1);
        head = tail;
        size = 0;
    }

    void push_back(int data) {
        Node* newNode = makeNode(data);
        boxes[data] = newNode;

        if(head != tail) {
            tail->prev->next = newNode;
        }
        else {
            head = newNode;
        }

        newNode->prev = tail->prev;
        tail->prev = newNode;
        newNode->next = tail;
        ++size;
    }
} llist[MAX_SIZE];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> q;
    while(q--) {
        cin >> cmd;
        if(cmd == 100) {
            cin >> n >> m;

            int b_num;
            for(int i = 1; i <= m; ++i) {
                cin >> b_num;
                llist[b_num].push_back(i);
            }
        }
        else if(cmd == 200) {
            cin >> m_src >> m_dst;

            if(llist[m_src].size) {
                Node* start = llist[m_src].head;
                Node* last = llist[m_src].tail->prev;
                
                llist[m_dst].head->prev = last;
                last->next = llist[m_dst].head;
                llist[m_dst].head = start;
                llist[m_src].head = llist[m_src].tail;
                llist[m_src].tail->prev = nullptr;

                llist[m_dst].size += llist[m_src].size;
                llist[m_src].size = 0;
            }
            
            cout << llist[m_dst].size << '\n';
        }
        else if(cmd == 300) {
            cin >> m_src >> m_dst;
            Node* front_src = llist[m_src].head;
            Node* front_dst = llist[m_dst].head;
            
            if(llist[m_src].size && llist[m_dst].size) {
                llist[m_src].head = front_dst;
                llist[m_dst].head = front_src;

                Node* tmp = front_src->next;
                front_src->next = front_dst->next;
                front_dst->next->prev = front_src;

                front_dst->next = tmp;
                tmp->prev = front_dst;
            }
            else if(llist[m_src].size && !llist[m_dst].size) {
                llist[m_src].head = front_src->next;
                llist[m_src].head->prev = nullptr;
                llist[m_dst].head = front_src;

                front_src->next = llist[m_dst].tail;
                llist[m_dst].tail->prev = front_src;

                --llist[m_src].size;
                ++llist[m_dst].size;
            }
            else if(!llist[m_src].size && llist[m_dst].size) {
                llist[m_dst].head = front_dst->next;
                llist[m_dst].head->prev = nullptr;
                llist[m_src].head = front_dst;

                front_dst->next = llist[m_src].tail;
                llist[m_src].tail->prev = front_dst;

                ++llist[m_src].size;
                --llist[m_dst].size;
            }

            cout << llist[m_dst].size << '\n';
        }
        else if(cmd == 400) {
            cin >> m_src >> m_dst;
            int sz = llist[m_src].size >> 1;

            Node* start = llist[m_src].head;
            Node* last = llist[m_src].head;

            if(sz) {
                for(int i = 1; i < sz; ++i) last = last->next;

                llist[m_src].head = last->next;
                llist[m_src].head->prev = nullptr;
                last->next = llist[m_dst].head;
                llist[m_dst].head->prev = last;
                llist[m_dst].head = start;

                llist[m_src].size -= sz;
                llist[m_dst].size += sz;
            }
            
            cout << llist[m_dst].size << '\n';
        }
        else if(cmd == 500) {
            int p_num;
            cin >> p_num;
            int a = boxes[p_num]->prev != nullptr ? boxes[p_num]->prev->id : -1;
            int b = boxes[p_num]->next->id;
            cout << a + 2 * b << '\n';
        }
        else {
            int b_num;
            cin >> b_num;
            int a = llist[b_num].size ? llist[b_num].head->id : -1;
            int b = llist[b_num].size ? llist[b_num].tail->prev->id : -1;
            int c = llist[b_num].size;
            cout << a + 2 * b + 3 * c << '\n';
        }
    }

    return 0;
}