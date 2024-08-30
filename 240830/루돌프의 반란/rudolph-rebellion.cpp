#include <iostream>
#include <queue>
#include <utility>
#include <tuple>
#include <algorithm>
#define X	first
#define Y	second
#define DEER	-1
#define SQUR(X)			((X) * (X))
#define DIST(X1, X2, Y1, Y2)	(SQUR((X1) - (X2)) + SQUR((Y1) - (Y2)))
using namespace std;
using Tuple = tuple<int, int, int, int>;

int N, M, P, Pn, C, D, T, Pcnt;
int Rr, Rc;

int board[51][51];
pair<int, int> santa[31];
bool died[31];
int score[31], stunned[31];
int dx[] = { 0, 1, 0, -1, 1, 1, -1, -1 };
int dy[] = { -1, 0, 1, 0, 1, -1, 1, -1 };

bool validRange(int x, int y) {
	return x >= 1 && x <= N && y >= 1 && y <= N;
}

void slide(int idx, int dir, int value) {
	int x = santa[idx].X + dx[dir] * value;
	int y = santa[idx].Y + dy[dir] * value;

	if (!validRange(x, y)) {
		died[idx] = true;
		Pcnt--;
		return;
	}

	int curSanta = idx;
	int nSanta = board[x][y];
	board[x][y] = curSanta;
	santa[curSanta].X = x;
	santa[curSanta].Y = y;

	int nx = x + dx[dir];
	int ny = y + dy[dir];

	while (validRange(nx, ny) && nSanta > 0) {
		curSanta = nSanta;
		nSanta = board[nx][ny];
		board[nx][ny] = curSanta;
		santa[curSanta].X = nx;
		santa[curSanta].Y = ny;

		nx += dx[dir];
		ny += dy[dir];
	}

	if (nSanta != 0) {
		died[nSanta] = true;
		Pcnt--;
	}
}

void collision(int target, int dir) {
	if (target == DEER) {
		int santaIdx = board[Rr][Rc];
		score[santaIdx] += C;
		stunned[santaIdx] = T + 1;
		slide(santaIdx, dir, C);
	}
	else {
		score[target] += D;
		stunned[target] = T + 1;
		slide(target, (dir + 2) % 4, D);
	}
}

void moveDeer() {
	priority_queue<Tuple> pq;
	for (int i = 1; i <= P; i++) {
		if (died[i]) continue;

		int dist = DIST(Rr, santa[i].X, Rc, santa[i].Y);
		pq.emplace(dist * -1, santa[i].X, santa[i].Y, i);
	}

	int destD, destX, destY, destN;
	tie(destD, destX, destY, destN) = pq.top();

	int dir = -1;
	int minDist = 2500;
	for (int i = 0; i < 8; i++) {
		int nx = Rr + dx[i];
		int ny = Rc + dy[i];
		if (!validRange(nx, ny)) continue;

		int dist = DIST(nx, destX, ny, destY);
		if (dist < minDist) {
			minDist = dist;
			dir = i;
		}
	}

	board[Rr][Rc] = 0;
	Rr += dx[dir];
	Rc += dy[dir];
	if (board[Rr][Rc] > 0) collision(DEER, dir);
	board[Rr][Rc] = DEER;
}

void moveSanta() {
	for (int i = 1; i <= P; i++) {
		if (died[i] || stunned[i] >= T) continue;

		int minDist = DIST(Rr, santa[i].X, Rc, santa[i].Y);
		int dir = -1;
		for (int j = 0; j < 4; j++) {
			int nx = santa[i].X + dx[j];
			int ny = santa[i].Y + dy[j];
			if (!validRange(nx, ny) || board[nx][ny] > 0) continue;

			int dist = DIST(nx, Rr, ny, Rc);
			if (dist < minDist) {
				minDist = dist;
				dir = j;
			}
			else if (dist == minDist) {
				dir = j;
			}
		}

		if (dir == -1) continue;

		board[santa[i].X][santa[i].Y] = 0;
		santa[i].X += dx[dir];
		santa[i].Y += dy[dir];
		if (board[santa[i].X][santa[i].Y] == DEER) collision(i, dir);
		else board[santa[i].X][santa[i].Y] = i;
	}
}

void getScore() {
	for (int i = 1; i <= P; i++) {
		if (!died[i]) score[i]++;
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> N >> M >> P >> C >> D >> Rr >> Rc;
	for (int i = 0; i < P; i++) {
		cin >> Pn;
		cin >> santa[Pn].X >> santa[Pn].Y;
		board[santa[Pn].X][santa[Pn].Y] = Pn;
	}

	board[Rr][Rc] = DEER;
	Pcnt = P;

	for (T = 1; T <= M && Pcnt != 0; T++) {
		moveDeer();
		moveSanta();
		getScore();
	}

	for (int i = 1; i <= P; i++) {
		cout << score[i] << ' ';
	}
	return 0;
}