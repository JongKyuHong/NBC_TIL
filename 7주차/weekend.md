# 고전 컴퓨터 알고리즘 인 파이썬

알고리즘 실력 향상을 위해서 예전에 보던 알고리즘 책을 다시 꺼내왔다.

![](https://velog.velcdn.com/images/kyu_/post/4ec6a74e-14e5-410f-ba68-f293ef1b5479/image.jpg)

이 책은 파이썬을 기준으로 쓰여있지만 C++로 한번 공부해볼 생각이다.

## Chapter 1. 작은 문제

### 비보나치 직접 구현해보기

```c++
#include <iostream>
#include <vector>

long long fibonachi(int N) {
    std::vector<long long> fibonachi(N+1);
    fibonachi[0] = 0;
    fibonachi[1] = 1;

    for (int i = 2; i <= N; i++) {
        fibonachi[i] = fibonachi[i - 1] + fibonachi[i - 2];
    }

    return fibonachi[N];
}

int main() {
    int N;
    std::cin >> N;

    std::cout << fibonachi(N);

    return 0;
}
```

## Chapter 2. 검색 문제

### dfs/bfs 구현해보기

잊지않게 C++로 구현해보았다.

```c++
// 스택
void dfs_stack(int start) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (visited[node]) {
            continue;
        }

        visited[node] = true;
        cout << node << " ";

        for (int i = graph[node].size() - 1; i >= 0; i--) {
            int next = graph[node][i];
            if (!visited[next]) {
                s.push(next);
            }
        }
    }
}

// 재귀버전
void dfs(int node) {
    visited[node] = true;
    cout << node << " ";

    for (int next : graph[node]) {
        if (!visited[next]) {
            dfs(next);
        }
    }
}

void bfs(int start) {
    queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        for (int next : graph[node]) {
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
}
```

### A\* 알고리즘

- 우선순위큐와 휴리스틱함수를 사용하는 기법
- 가중치가 있다면 BFS보다 훨씬 빠르다.

f(n) = g(n) + h(n)

g(n) : 시작점부터 현재 노드까지의 실제 비용(확정된 값)
h(n) : 현재 노드부터 목표까지의 추정 비용(휴리스틱)
f(n) : 예상되는 총 비용 (시작->현재->목표)

1. 초기설정

- OPEN 리스트 : 탐색할 후보 노드들(우선순위 큐)
- CLOSED 리스트 : 이미 탐색 완료된 노드들(집합)

초기상태 : Open에 시작점 추가, CLOSED는 비어있음

2. 반복과정

```
   while OPEN이 비어있지 않을때까지:
    1. OPEN에서 f(n)이 가장 작은 노드 선택
    2. 해당 노드가 목표라면 종료
    3. 해당 노드를 CLOSED에 추가
    4. 인접 노드들을 확인:
      - 이미 CLOSED에 있으면 무시
      - 더 좋은 경로를 발견하면 g(n), f(n) 갱신
      - 새로운 노드면 OPEN에 추가
```

- 거리측정에는 유클리드 거리와 맨해튼 거리가 있는데
  - 유클리드거리는 피타고라스에서 착안하여 대각 거리를 계산하는거고
  - 맨해튼거리는 맨해튼의 거리를 착안하여 대각말고 직각으로만 움직일 수 있음

```c++
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

struct Node {
    int x, y;
    int g;
    int h;
    int f;

    bool operator>(const Node& o) const {
        return f > o.f; // f가 작을수록 우선순위
    }
};

int N, M;
int maze[101][101];
bool visited[101][101];
int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };

int heuristic(int x, int y, int goalX, int goalY) {
    return abs(x - goalX) + abs(y - goalY);
}

int astar(int startX, int startY, int goalX, int goalY) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    memset(visited, false, sizeof(visited));

    int h = heuristic(startX, startY, goalX, goalY);
    pq.push({ startX, startY, 0, h, 0 + h });
    visited[startX][startY] = true;

    cout << "\n=== A* 탐색 시작 ===\n";
    cout << "시작: (" << startX << ", " << startY << ")\n";
    cout << "목표: (" << goalX << ", " << goalY << ")\n\n";

    int step = 0;

    while (!pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        step++;
        cout << "Step " << step << ": (" << cur.x << ", " << cur.y << ") ";
        cout << "g=" << cur.g << ", h=" << cur.h << ", f=" << cur.f << "\n";

        // 목표 도착
        if (cur.x == goalX && cur.y == goalY) {
            cout << "\n목표 도착! 최단 거리: " << cur.g << "\n";
            cout << "탐색한 노드 수: " << step << "\n";
            return cur.g;
        }

        // 4방향 탐색
        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
            if (maze[nx][ny] == 1) continue;  // 벽
            if (visited[nx][ny]) continue;

            visited[nx][ny] = true;

            int g = cur.g + 1;
            int h = heuristic(nx, ny, goalX, goalY);
            int f = g + h;

            pq.push({ nx, ny, g, h, f });
        }
    }

    return -1;
}


int main() {
    cout << "=== A* 알고리즘 테스트 ===\n\n";

    // 예제 1: 간단한 미로
    cout << "[ 예제 1: 5x5 간단한 미로 ]\n";
    N = 5;
    M = 5;

    // 미로 설정 (0: 길, 1: 벽)
    int maze1[5][5] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            maze[i][j] = maze1[i][j];
        }
    }

    // 미로 출력
    cout << "\n미로 구조 (0: 길, 1: 벽, S: 시작, G: 목표):\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0 && j == 0) cout << "S ";
            else if (i == N - 1 && j == M - 1) cout << "G ";
            else cout << maze[i][j] << " ";
        }
        cout << "\n";
    }

    int result = astar(0, 0, N - 1, M - 1);

    cout << "\n" << string(50, '=') << "\n\n";

    return 0;
}
```

- memset을 사용해 visited를 초기화해줌
- 우선순위큐의 마지막 인자에 greate<Node>를 넣어주고 Node안에 > 연산자를 재정의 함으로써 f가 작은순으로 앞으로 오게끔 정렬됨
  - 우선순위 큐는 비교함수가 true를 반환하는 쪽을 뒤로 보낸다 다시말해 큰값이 앞으로오는 내림차순
  - greater를 써주었기 때문에 오름차순으로 정렬, greater는 내부적으로 operator>를 호출함

### 상황에따라 다르다

| 상황                             | 최적 알고리즘 | 이유                                             |
| -------------------------------- | ------------- | ------------------------------------------------ |
| 가중치 없음 (모든 간선 = 1)      | BFS           | 단순하고 빠름my-twinkle-tech-tales.tistory+1​    |
| 가중치 있음 + 명확한 목표        | A\*           | 휴리스틱으로 탐색 범위 축소int8.tistory​         |
| 가중치 있음 + 모든 노드 최단거리 | 다익스트라    | 모든 노드 대상int8.tistory​                      |
| 음수 가중치 있음                 | 벨만-포드     | A\*, BFS 모두 불가my-twinkle-tech-tales.tistory​ |
| 목표가 불명확                    | BFS           | 균등 탐색이 유리                                 |

### 문제 푼거

- [초코바](https://www.acmicpc.net/problem/27959)
  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/weekend7/%EC%B4%88%EC%BD%94%EB%B0%94.cpp)
- [더하기](https://www.acmicpc.net/problem/11024)
  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/weekend7/%EB%8D%94%ED%95%98%EA%B8%B04.cpp)

```c++
int T;
cin >> T;
cin.ignore();
for (int i = 0; i < T; i++) {
    int sumV = 0;
    string inp;
    getline(cin, inp);
    stringstream ss(inp);
    int num = 0;

    while (ss >> num) {
        sumV += num;
    }

    cout << sumV << "\n";
}
```

- T를 입력하자마자 0이 출력되는 버그가 있었는데 T 뒤에 \n개행 문자가 남아서 cin.ignore()를 달아주어야 했다.

  - 이런부분은 참 불편한듯..

- [크리스마스 파티](https://www.acmicpc.net/problem/10708)

  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/weekend7/%ED%81%AC%EB%A6%AC%EC%8A%A4%EB%A7%88%EC%8A%A4%ED%8C%8C%ED%8B%B0.cpp)

- [단어 퍼즐](https://www.acmicpc.net/problem/9946)
  - [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/weekend7/단어퍼즐.cpp)

-[끝말잇기](https://www.acmicpc.net/problem/28432)

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/weekend7/끝말잇기.cpp)

-[아~파트 아파트](https://www.acmicpc.net/problem/31797)

- [내 풀이](https://github.com/JongKyuHong/NBC_TIL/blob/main/%EC%BD%94%EB%93%9C%EC%B9%B4%ED%83%80/weekend7/아파트아파트.cpp)
