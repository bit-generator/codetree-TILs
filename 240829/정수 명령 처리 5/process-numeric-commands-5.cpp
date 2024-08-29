#include <iostream>
#include <vector>
#include <string>
using namespace std;

int N, M;
string str;

int main() {
    cin >> N;
    vector<int> v;
    while(N--) {
        cin >> str;
        if(str == "push_back") {
            cin >> M;
            v.push_back(M);
        }
        else if(str == "pop_back") {
            v.pop_back();
        }
        else if(str == "size") {
            cout << v.size() << '\n';
        }
        else {
            cin >> M;
            cout << v[M - 1] << '\n';
        }
    }

    return 0;
}