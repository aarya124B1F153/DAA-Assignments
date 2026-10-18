#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    int N, S, R;

    cout << "Enter number of courses: ";
    cin >> N;

    vector<string> course(N);

    cout << "Enter course names:\n";
    for (int i = 0; i < N; i++)
        cin >> course[i];

    cout << "Enter number of students: ";
    cin >> S;

    vector<vector<int>> graph(N);

    cout << "For each student, enter number of courses followed by course numbers:\n";

    for (int i = 0; i < S; i++) {
        int k;
        cin >> k;

        vector<int> list(k);

        for (int j = 0; j < k; j++) {
            cin >> list[j];
            list[j]--;
        }

        for (int a = 0; a < k; a++) {
            for (int b = a + 1; b < k; b++) {
                int u = list[a], v = list[b];

                graph[u].push_back(v);
                graph[v].push_back(u);
            }
        }
    }

    for (int i = 0; i < N; i++) {
        sort(graph[i].begin(), graph[i].end());
        graph[i].erase(
            unique(graph[i].begin(), graph[i].end()),
            graph[i].end()
        );
    }

    vector<int> order(N);
    for (int i = 0; i < N; i++)
        order[i] = i;

    sort(order.begin(), order.end(), [&](int a, int b) {
        if (graph[a].size() != graph[b].size())
            return graph[a].size() > graph[b].size();
        return a < b;
    });

    vector<int> color(N, -1);
    int maxColor = -1;

    for (int u : order) {
        vector<bool> used(N, false);

        for (int v : graph[u]) {
            if (color[v] != -1)
                used[color[v]] = true;
        }

        int c = 0;
        while (c < N && used[c])
            c++;

        color[u] = c;
        maxColor = max(maxColor, c);
    }

    cout << "Enter number of rooms: ";
    cin >> R;

    vector<string> room(R);
    vector<int> capacity(R);

    cout << "Enter room name and capacity:\n";
    for (int i = 0; i < R; i++)
        cin >> room[i] >> capacity[i];

    vector<int> students(N);

    cout << "Enter number of students in each course:\n";
    for (int i = 0; i < N; i++) {
        cout << course[i] << ": ";
        cin >> students[i];
    }

    vector<int> assigned(N, -1);
    vector<vector<bool>> occupied(
        maxColor + 1, vector<bool>(R, false)
    );

    for (int c = 0; c <= maxColor; c++) {
        vector<int> exams;

        for (int i = 0; i < N; i++) {
            if (color[i] == c)
                exams.push_back(i);
        }

        sort(exams.begin(), exams.end(), [&](int a, int b) {
            return students[a] > students[b];
        });

        for (int exam : exams) {
            int best = -1;

            for (int j = 0; j < R; j++) {
                if (!occupied[c][j] &&
                    capacity[j] >= students[exam] &&
                    (best == -1 || capacity[j] < capacity[best])) {
                    best = j;
                }
            }

            if (best != -1) {
                assigned[exam] = best;
                occupied[c][best] = true;
            }
        }
    }

    cout << "\nFinal Exam Timetable:\n";
    cout << "Course\tTime Slot\tRoom\n";

    for (int i = 0; i < N; i++) {
        cout << course[i] << "\tSlot " << color[i] + 1 << "\t\t";

        if (assigned[i] == -1)
            cout << "No suitable room";
        else
            cout << room[assigned[i]];

        cout << "\n";
    }

    cout << "\nTotal time slots required: " << maxColor + 1 << "\n";

    return 0;
}
