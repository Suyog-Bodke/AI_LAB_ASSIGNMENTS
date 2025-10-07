#include <iostream>
#include <vector>
using namespace std;

bool is_safe(vector<vector<char>>& board, int row, int col) {
    int n = board.size();

    // Check vertically upward
    for (int i = row - 1; i >= 0; i--) {
        if (board[i][col] == 'Q')
            return false;
    }

    // Check upper left diagonal
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 'Q')
            return false;
    }

    // Check upper right diagonal
    for (int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
        if (board[i][j] == 'Q')
            return false;
    }

    return true;
}

bool n_queens(vector<vector<char>>& board, int row) {
    int n = board.size();

    // Base condition: all queens placed
    if (row == n) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << board[i][j] << " ";
            cout << endl;
        }
        cout << endl;
        return true;  // Stop after first valid solution
    }

    for (int col = 0; col < n; col++) {
        if (is_safe(board, row, col)) {
            board[row][col] = 'Q';  // Place queen

            if (n_queens(board, row + 1))
                return true;  // Stop after first solution

            board[row][col] = '.';  // Backtrack
        }
    }

    return false;
}

int main() {
    int n = 8;
    vector<vector<char>> board(n, vector<char>(n, '.'));

    if (!n_queens(board, 0))
        cout << "No solution found." << endl;

    return 0;
}
