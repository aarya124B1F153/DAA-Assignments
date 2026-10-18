#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N, W;

    cout << "Enter number of items: ";
    cin >> N;

    vector<int> weight(N), value(N);

    cout << "Enter weight and utility value of each item:\n";
    for (int i = 0; i < N; i++) {
        cin >> weight[i] >> value[i];
    }

    cout << "Enter truck capacity: ";
    cin >> W;

    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= N; i++) {
        for (int w = 1; w <= W; w++) {
            if (weight[i - 1] <= w) {
                dp[i][w] = max(
                    value[i - 1] + dp[i - 1][w - weight[i - 1]],
                    dp[i - 1][w]
                );
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    cout << "\nMaximum utility: " << dp[N][W] << endl;

    cout << "Selected items: ";
    int w = W;

    for (int i = N; i > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            cout << i << " ";
            w -= weight[i - 1];
        }
    }

    cout << endl;

    return 0;
}
