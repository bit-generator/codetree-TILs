#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <set>
#define INF     21e8
using namespace std;

struct Product {
	int profit;
	int id, revenue, dest;

	bool operator<(const Product& a) const {
		if (this->profit == a.profit) {
			return this->id < a.id;
		}

		return this->profit > a.profit;
	}
};

int Q, num, n, m, v, u, w, id, revenue, dest, s, news, changed;
vector<pair<int, int>> graph[2000];
priority_queue<pair<int, int>> pq;
int dist[2000];
bool ids[30001];
set<Product> products1, products2;

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
		set<Product>& products = changed % 2 ? products2 : products1;
		if (num == 200) {
			cin >> id >> revenue >> dest;
			products.insert({ revenue - dist[dest], id, revenue, dest });
			ids[id] = true;
		}
		else if (num == 300) {
			cin >> id;
			ids[id] = false;
		}
		else if (num == 400) {
			bool found = false;
			for (auto it = products.begin(); it != products.end(); ) {
				auto product = *it;

				if (product.profit < 0) break;
				if (!ids[product.id]) {
					it = products.erase(it);
					continue;
				}

				cout << product.id << '\n';
				products.erase(it);
				found = true;
				break;
			}

			if (!found) cout << -1 << '\n';
		}
		else {
			cin >> news;
			if (s != news) {
				dijkstra(news);
				set<Product>& nproducts = changed % 2 ? products1 : products2;

				for (auto it = products.begin(); it != products.end(); ) {
					nproducts.insert({ it->revenue - dist[it->dest], it->id, it->revenue, it->dest });
					it = products.erase(it);
				}

				s = news;
				changed++;
			}
		}
	}

	return 0;
}