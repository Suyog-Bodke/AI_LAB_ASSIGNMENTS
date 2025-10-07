#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <stack>
#include <algorithm>

using namespace std;

struct PuzzleState {
    vector<vector<int>> board;
    int blankRow, blankCol;
    int gScore, hScore;
    PuzzleState* parent;

    PuzzleState(vector<vector<int>> b, int br, int bc, int g, PuzzleState* p)
        : board(b), blankRow(br), blankCol(bc), gScore(g), parent(p) {
        hScore = calculateHeuristic();
    }

    // Manhattan distance heuristic
    int calculateHeuristic() {
        int dist = 0;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                int val = board[i][j];
                if (val != 0) {
                    int goalRow = (val - 1) / 3;
                    int goalCol = (val - 1) % 3;
                    dist += abs(i - goalRow) + abs(j - goalCol);
                }
            }
        return dist;
    }

    int fScore() const {
        return gScore + hScore;
    }

    // Compare two board configurations
    bool operator==(const PuzzleState& other) const {
        return board == other.board;
    }

    // For priority queue (min-heap)
    struct Comparator {
        bool operator()(const PuzzleState* a, const PuzzleState* b) const {
            return a->fScore() > b->fScore();
        }
    };
};

// Hash function for unordered_set
struct PuzzleHash {
    size_t operator()(const PuzzleState* state) const {
        size_t hash = 0;
        for (auto& row : state->board)
            for (int val : row)
                hash = hash * 31 + val;
        return hash;
    }
};

// Equality check for unordered_set
struct PuzzleEqual {
    bool operator()(const PuzzleState* a, const PuzzleState* b) const {
        return a->board == b->board;
    }
};

vector<vector<int>> goal = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int val : row)
            cout << val << " ";
        cout << "\n";
    }
}

void printPath(PuzzleState* state) {
    stack<PuzzleState*> path;
    while (state) {
        path.push(state);
        state = state->parent;
    }

    cout << "Solution steps:\n";
    while (!path.empty()) {
        printBoard(path.top()->board);
        cout << "\n";
        path.pop();
    }
}

void solve(vector<vector<int>> start) {
    int br = -1, bc = -1;
    for (int i = 0; i < 3 && br == -1; ++i)
        for (int j = 0; j < 3; ++j)
            if (start[i][j] == 0) {
                br = i;
                bc = j;
                break;
            }

    auto cmp = PuzzleState::Comparator();
    priority_queue<PuzzleState*, vector<PuzzleState*>, PuzzleState::Comparator> open(cmp);
    unordered_set<PuzzleState*, PuzzleHash, PuzzleEqual> closed;

    PuzzleState* initial = new PuzzleState(start, br, bc, 0, nullptr);
    open.push(initial);

    while (!open.empty()) {
        PuzzleState* current = open.top();
        open.pop();

        if (current->board == goal) {
            printPath(current);
            return;
        }

        closed.insert(current);

        int dr[4] = {-1, 1, 0, 0};
        int dc[4] = {0, 0, -1, 1};

        for (int d = 0; d < 4; ++d) {
            int nr = current->blankRow + dr[d];
            int nc = current->blankCol + dc[d];

            if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
                vector<vector<int>> newBoard = current->board;
                swap(newBoard[current->blankRow][current->blankCol], newBoard[nr][nc]);

                PuzzleState* neighbor = new PuzzleState(newBoard, nr, nc, current->gScore + 1, current);

                if (closed.find(neighbor) == closed.end()) {
                    open.push(neighbor);
                }
            }
        }
    }

    cout << "No solution found.\n";
}

int main() {
    vector<vector<int>> initial = {
        {8, 2, 7},
        {0, 4, 6},
        {3, 5, 1}
    };

    solve(initial);
    return 0;
}
