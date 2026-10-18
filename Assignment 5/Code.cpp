
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct Edge {
    int to;
    double cost;
    double base;
};

int main() {
    int S;
    cout << "Enter number of stages: ";
    cin >> S;

    if (S <= 0) return 0;

    vector<int> count(S), start(S);
    int N = 0;

    cout << "Enter number of nodes in each stage (" << S << " values): ";
    for (int i = 0; i < S; i++) {
        cin >> count[i];
        start[i] = N;
        N += count[i];
    }

    int M;
    cout << "Enter number of edges: ";
    cin >> M;

    vector<vector<Edge>> adj(N);

    cout << "Enter each edge as: u v cost\n";
    for (int i = 0; i < M; i++) {
        int u, v;
        double c;
        cin >> u >> v >> c;

        if (u < 0 || u >= N || v < 0 || v >= N) {
            cout << "Invalid edge\n";
            return 0;
        }

        adj[u].push_back({v, c, c});
    }

    const double INF = 1e18;
    vector<double> dp(N, INF);
    vector<int> next(N, -1);

    auto calculate = [&]() {
        fill(dp.begin(), dp.end(), INF);
        fill(next.begin(), next.end(), -1);

        for (int i = start[S - 1]; i < N; i++)
            dp[i] = 0;

        for (int st = S - 2; st >= 0; st--) {
            for (int u = start[st]; u < start[st] + count[st]; u++) {
                for (auto e : adj[u]) {
                    if (dp[e.to] != INF && e.cost + dp[e.to] < dp[u]) {
                        dp[u] = e.cost + dp[e.to];
                        next[u] = e.to;
                    }
                }
            }
        }
    };

    calculate();
    cout << fixed << setprecision(6);

    cout << "\nBest costs from Stage-0 nodes:\n";
    for (int i = start[0]; i < start[0] + count[0]; i++) {
        if (dp[i] == INF)
            cout << "Node " << i << ": unreachable\n";
        else
            cout << "Node " << i << ": cost = " << dp[i] << "\n";
    }

    int src;
    cout << "\nEnter a source node id (in stage 0) to print path, or -1 to skip: ";
    cin >> src;

    if (src >= start[0] && src < start[0] + count[0]) {
        if (dp[src] == INF) {
            cout << "No route from " << src << "\n";
        } else {
            cout << "Path from " << src << " : ";
            int cur = src;
            double total = 0;

            cout << cur;
            while (next[cur] != -1) {
                int nxt = next[cur];
                double edgeCost = 0;

                for (auto e : adj[cur]) {
                    if (e.to == nxt) {
                        edgeCost = e.cost;
                        break;
                    }
                }

                total += edgeCost;
                cout << " -> " << nxt;
                cur = nxt;
            }

            cout << "\nTotal route cost (sum edges): " << total << "\n";
        }
    }

    int Q;
    cout << "\nEnter number of live updates to edge costs (0 to finish): ";
    cin >> Q;

    for (int i = 0; i < Q; i++) {
        int u, v;
        double multiplier;

        cout << "Enter edge update (u v multiplier): ";
        cin >> u >> v >> multiplier;

        if (u < 0 || u >= N || v < 0 || v >= N || multiplier < 0) {
            cout << "Invalid update\n";
            continue;
        }

        bool found = false;
        for (auto &e : adj[u]) {
            if (e.to == v) {
                e.cost = e.base * multiplier;
                found = true;
            }
        }

        if (!found) {
            cout << "Edge not found\n";
            continue;
        }

        calculate();
    }

    cout << "\nAfter updates, best costs from Stage-0 nodes:\n";
    for (int i = start[0]; i < start[0] + count[0]; i++) {
        if (dp[i] == INF)
            cout << "Node " << i << ": unreachable\n";
        else
            cout << "Node " << i << ": cost = " << dp[i] << "\n";
    }

    return 0;
}
