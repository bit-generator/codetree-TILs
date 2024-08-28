#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <map>
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
int ids[2][30001];
map<Product, bool> maps1, maps2;

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
		map<Product, bool>& maps = changed % 2 ? maps2 : maps1;
		if (num == 200) {
			cin >> id >> revenue >> dest;
			ids[0][id] = revenue;
			ids[1][id] = dest;
			maps[{revenue - dist[dest], id, revenue, dest}] = true;
		}
		else if (num == 300) {
			cin >> id;
			maps[{ids[0][id] - dist[ids[1][id]], id, ids[0][id], ids[1][id]}] = false;
		}
		else if (num == 400) {
			bool found = false;
			for (auto it = maps.begin(); it != maps.end(); it++) {
				auto product = it->first;
				if (product.profit < 0) break;
				if (!(it->second)) continue;

				cout << product.id << '\n';
				it->second = false;
				found = true;
				break;
			}

			if (!found) cout << -1 << '\n';
		}
		else {
			cin >> news;
			if (s != news) {
				dijkstra(news);
				map<Product, bool>& nmaps = changed % 2 ? maps1 : maps2;

				for (auto it = maps.begin(); it != maps.end(); it++) {
					if (it->second) {
						auto tmp = it->first;
						tmp.profit = tmp.revenue - dist[tmp.dest];
						nmaps[tmp] = true;
					}
				}

				maps.clear();
				s = news;
				changed++;
			}
		}
	}

	return 0;
}