#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#define INF     21e8
using namespace std;

struct Product {
	int profit;
	int id, revenue, dest;

	bool operator<(const Product& a) const {
		if (this->profit == a.profit) {
			return this->id > a.id;
		}

		return this->profit < a.profit;
	}
};

int Q, num, n, m, v, u, w, id, revenue, dest, s, news;
bool changed;
vector<pair<int, int>> graph[2000];
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
int dist[2000];
bool ids[30001];
priority_queue<Product> products1, products2;

void dijkstra(int x) {
	fill(dist, dist + 2000, INF);
	dist[x] = 0;
	pq.emplace(0, x);

	while (!pq.empty()) {
		int cur = pq.top().second;
		int cost = pq.top().first;
		pq.pop();

		if (dist[cur] < cost) continue;

		for (auto p : graph[cur]) {
			int next = p.first;
			int ncost = p.second + cost;

			if (dist[next] > ncost) {
				dist[next] = ncost;
				pq.emplace(ncost, next);
			}
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> Q >> num >> n >> m;
	while (m--) {
		cin >> v >> u >> w;
		graph[v].emplace_back(u, w);
		graph[u].emplace_back(v, w);
	}

	dijkstra(s);

	while (--Q) {
		cin >> num;
		priority_queue<Product>& products = changed ? products2 : products1;
		if (num == 200) {
			cin >> id >> revenue >> dest;
			products.push({ revenue - dist[dest], id, revenue, dest });
			ids[id] = true;
		}
		else if (num == 300) {
			cin >> id;
			ids[id] = false;
		}
		else if (num == 400) {
            while(!products.empty() && !ids[products.top().id]) products.pop();
            if(products.empty() || products.top().profit < 0) {
                cout << -1 << '\n';
            }
            else {
                cout << products.top().id << '\n';
                ids[products.top().id] = false;
            }
		}
		else {
			cin >> news;
			if (s != news) {
				dijkstra(news);
				priority_queue<Product>& nproducts = changed ? products1 : products2;

                while(!products.empty()) {
                    Product tmp = products.top();
                    products.pop();
                    tmp.profit = tmp.revenue - dist[tmp.dest];
                    nproducts.push(tmp);
                }

				s = news;
				changed ^= true;
			}
		}
	}

	return 0;
}
