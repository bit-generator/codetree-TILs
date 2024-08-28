#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#define INF     21e8
using namespace std;
using Pair = pair<int, int>;

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

int Q, num, n, m, v, u, w, id, revenue, dest, s, news, changed;
vector<Pair> graph[2000];
priority_queue<Pair, vector<Pair>, greater<>> pq;
int dist[2000];
bool enable[30001];
priority_queue<Product> trip1, trip2;

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
		priority_queue<Product>& trip = changed % 2 ? trip2 : trip1;
		if (num == 200) {
			cin >> id >> revenue >> dest;
			trip.push({ revenue - dist[dest], id, revenue, dest });
			enable[id] = true;
		}
		else if (num == 300) {
			cin >> id;
			enable[id] = false;
		}
		else if (num == 400) {
			bool found = false;

			while (!trip.empty() && !enable[trip.top().id]) {
				trip.pop();
			}

			if (trip.empty() || trip.top().profit < 0) {
				cout << -1 << '\n';
				continue;
			}

			cout << trip.top().id << '\n';
			trip.pop();
		}
		else {
			cin >> news;
			if (s != news) {
				dijkstra(news);
				priority_queue<Product>& ntrip = changed % 2 ? trip1 : trip2;

				while (!trip.empty()) {
					auto tmp = trip.top();
					trip.pop();
					tmp.profit = tmp.revenue - dist[tmp.dest];
					ntrip.push(tmp);
					
				}

				s = news;
				changed++;
			}
		}
	}

	return 0;
}