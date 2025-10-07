// tic_tac_toe_minimax.cpp
#include <bits/stdc++.h>
using namespace std;

const char PLAYER_X = 'X';   // human
const char PLAYER_O = 'O';   // computer
const char EMPTY    = '.';

// Print board
void print_board(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (size_t j = 0; j < row.size(); ++j) {
            cout << row[j];
            if (j + 1 < row.size()) cout << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

// Evaluate board: +10 if X wins, -10 if O wins, 0 otherwise
int evaluate_board(const vector<vector<char>>& b) {
    // rows & columns
    for (int i = 0; i < 3; ++i) {
        if (b[i][0] == b[i][1] && b[i][1] == b[i][2] && b[i][0] != EMPTY)
            return (b[i][0] == PLAYER_X) ? 10 : -10;
        if (b[0][i] == b[1][i] && b[1][i] == b[2][i] && b[0][i] != EMPTY)
            return (b[0][i] == PLAYER_X) ? 10 : -10;
    }
    // diagonals
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2] && b[0][0] != EMPTY)
        return (b[0][0] == PLAYER_X) ? 10 : -10;
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0] && b[0][2] != EMPTY)
        return (b[0][2] == PLAYER_X) ? 10 : -10;

    return 0;
}

// Check if any moves remain
bool moves_left(const vector<vector<char>>& board) {
    for (const auto& row : board)
        for (char c : row)
            if (c == EMPTY) return true;
    return false;
}

// Check if game over (win or draw)
bool is_game_over(const vector<vector<char>>& board) {
    int score = evaluate_board(board);
    if (score == 10 || score == -10) return true;
    return !moves_left(board);
}

// Minimax implementation
int minimax(vector<vector<char>>& board, int depth, bool is_maximizing) {
    int score = evaluate_board(board);

    // Terminal states
    if (score == 10 || score == -10) return score;
    if (!moves_left(board)) return 0; // draw

    if (is_maximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    best = max(best, minimax(board, depth + 1, false));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    best = min(best, minimax(board, depth + 1, true));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    }
}

// Find best move for the computer (PLAYER_O)
pair<int,int> find_best_move(vector<vector<char>>& board) {
    int best_value = numeric_limits<int>::max(); // computer minimizes
    pair<int,int> best_move = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_O;
                int move_value = minimax(board, 0, true); // after O move, it's X's turn (maximizing)
                board[i][j] = EMPTY;

                if (move_value < best_value) {
                    best_value = move_value;
                    best_move = {i, j};
                }
            }
        }
    }
    return best_move;
}

// Main game loop
void play_game() {
    vector<vector<char>> board(3, vector<char>(3, EMPTY));
    cout << "Tic-Tac-Toe: Player X (you) vs. Player O (computer)\n\n";
    print_board(board);

    while (true) {
        // Player X (Human)
        int row, col;
        cout << "Enter your move row (0, 1, or 2): ";
        if (!(cin >> row)) { cout << "Invalid input\n"; return; }
        cout << "Enter your move column (0, 1, or 2): ";
        if (!(cin >> col)) { cout << "Invalid input\n"; return; }

        if (row < 0 || row > 2 || col < 0 || col > 2) {
            cout << "Invalid input! Please enter 0, 1, or 2 for both row and column.\n";
            continue;
        }
        if (board[row][col] != EMPTY) {
            cout << "Invalid move! Cell already occupied. Try again.\n";
            continue;
        }

        board[row][col] = PLAYER_X;

        if (is_game_over(board)) {
            print_board(board);
            int final_score = evaluate_board(board);
            if (final_score == 10) cout << "You win!\n";
            else cout << "It's a draw!\n";
            break;
        }

        // Computer's turn
        cout << "Computer's turn...\n";
        auto best_move = find_best_move(board);

        if (best_move.first == -1) {
            // No move found (should be draw)
            print_board(board);
            cout << "It's a draw!\n";
            break;
        }

        board[best_move.first][best_move.second] = PLAYER_O;
        print_board(board);

        if (is_game_over(board)) {
            int final_score = evaluate_board(board);
            if (final_score == -10) cout << "Computer wins!\n";
            else cout << "It's a draw!\n";
            break;
        }
    }
}

int main() {
    play_game();
    return 0;
}
