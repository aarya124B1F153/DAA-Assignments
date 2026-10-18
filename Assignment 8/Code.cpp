
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

const int INF = INT_MAX / 4;

struct Node {
    vector<vector<int>> matrix;
    vector<int> path;
    int level;
    int city;
    int cost;
};

struct Compare {
    bool operator()(const Node& a, const Node& b) {
        return a.cost > b.cost;
    }
};

int reduceMatrix(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int reduction = 0;

    for (int i = 0; i < n; i++) {
        int minVal = INF;

        for (int j = 0; j < n; j++)
            minVal = min(minVal, matrix[i][j]);

        if (minVal != INF && minVal > 0) {
            reduction += minVal;

            for (int j = 0; j < n; j++)
                if (matrix[i][j] != INF)
                    matrix[i][j] -= minVal;
        }
    }

    for (int j = 0; j < n; j++) {
        int minVal = INF;

        for (int i = 0; i < n; i++)
            minVal = min(minVal, matrix[i][j]);

        if (minVal != INF && minVal > 0) {
            reduction += minVal;

            for (int i = 0; i < n; i++)
                if (matrix[i][j] != INF)
                    matrix[i][j] -= minVal;
        }
    }

    return reduction;
}

int main() {
    int n;

    cout << "Enter number of cities: ";
    cin >> n;

    vector<vector<int>> cost(n, vector<int>(n));

    cout << "Enter cost matrix (enter 0 for diagonal):\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> cost[i][j];

            if (i == j)
                cost[i][j] = INF;
        }
    }

    int start;
    cout << "Enter starting city (0 to " << n - 1 << "): ";
    cin >> start;

    vector<vector<int>> rootMatrix = cost;
    int initialBound = reduceMatrix(rootMatrix);

    Node root;
    root.matrix = rootMatrix;
    root.path = {start};
    root.level = 0;
    root.city = start;
    root.cost = initialBound;

    priority_queue<Node, vector<Node>, Compare> pq;
    pq.push(root);

    int bestCost = INF;
    vector<int> bestPath;

    while (!pq.empty()) {
        Node node = pq.top();
        pq.pop();

        if (node.cost >= bestCost)
            continue;

        if (node.level == n - 1) {
            int last = node.city;

            if (cost[last][start] != INF) {
                int total = 0;

                for (int i = 0; i < n - 1; i++)
                    total += cost[node.path[i]][node.path[i + 1]];

                total += cost[last][start];

                if (total < bestCost) {
                    bestCost = total;
                    bestPath = node.path;
                    bestPath.push_back(start);
                }
            }

            continue;
        }

        int u = node.city;

        for (int v = 0; v < n; v++) {
            if (find(node.path.begin(), node.path.end(), v)
                != node.path.end())
                continue;

            if (node.matrix[u][v] == INF ||
                cost[u][v] == INF)
                continue;

            Node child;
            child.matrix = node.matrix;
            child.path = node.path;
            child.path.push_back(v);
            child.level = node.level + 1;
            child.city = v;

            for (int j = 0; j < n; j++)
                child.matrix[u][j] = INF;

            for (int i = 0; i < n; i++)
                child.matrix[i][v] = INF;

            child.matrix[v][start] = INF;

            int reduction = reduceMatrix(child.matrix);
            child.cost = node.cost + node.matrix[u][v]
                         + reduction;

            if (child.cost < bestCost)
                pq.push(child);
        }
    }

    if (bestPath.empty()) {
        cout << "\nNo valid tour found.\n";
    } else {
        cout << "\nMinimum tour cost: " << bestCost << endl;
        cout << "Optimal path: ";

        for (int city : bestPath)
            cout << city << " ";

        cout << endl;
    }

    return 0;
}
