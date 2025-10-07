// a_star.cpp
#include <bits/stdc++.h>
using namespace std;

const int ROW = 9;
const int COL = 10;

struct Cell {
    int parent_i;
    int parent_j;
    double f;
    double g;
    double h;
    Cell() {
        parent_i = -1;
        parent_j = -1;
        f = numeric_limits<double>::infinity();
        g = numeric_limits<double>::infinity();
        h = 0.0;
    }
};

bool is_valid(int row, int col) {
    return row >= 0 && row < ROW && col >= 0 && col < COL;
}

bool is_unblocked(const vector<vector<int>>& grid, int row, int col) {
    return grid[row][col] == 1;
}

bool is_destination(int row, int col, const pair<int,int>& dest) {
    return row == dest.first && col == dest.second;
}

double calculate_h_value(int row, int col, const pair<int,int>& dest) {
    double dr = static_cast<double>(row - dest.first);
    double dc = static_cast<double>(col - dest.second);
    return sqrt(dr*dr + dc*dc); // Euclidean distance
}

void trace_path(const vector<vector<Cell>>& cell_details, const pair<int,int>& dest) {
    cout << "The Path is\n";
    vector<pair<int,int>> path;
    int row = dest.first;
    int col = dest.second;

    // trace back to the source (parent of source is itself)
    while (!(cell_details[row][col].parent_i == row && cell_details[row][col].parent_j == col)) {
        path.emplace_back(row, col);
        int temp_row = cell_details[row][col].parent_i;
        int temp_col = cell_details[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
    // add the source
    path.emplace_back(row, col);
    reverse(path.begin(), path.end());

    for (const auto &p : path) {
        cout << "-> (" << p.first << ", " << p.second << ") ";
    }
    cout << "\n";
}

void a_star_search(const vector<vector<int>>& grid, const pair<int,int>& src, const pair<int,int>& dest) {
    if (!is_valid(src.first, src.second) || !is_valid(dest.first, dest.second)) {
        cout << "Source or destination is invalid\n";
        return;
    }
    if (!is_unblocked(grid, src.first, src.second) || !is_unblocked(grid, dest.first, dest.second)) {
        cout << "Source or the destination is blocked\n";
        return;
    }
    if (is_destination(src.first, src.second, dest)) {
        cout << "We are already at the destination\n";
        return;
    }

    vector<vector<bool>> closed_list(ROW, vector<bool>(COL, false));
    vector<vector<Cell>> cell_details(ROW, vector<Cell>(COL));

    // initialize start cell
    int i = src.first;
    int j = src.second;
    cell_details[i][j].f = 0.0;
    cell_details[i][j].g = 0.0;
    cell_details[i][j].h = 0.0;
    cell_details[i][j].parent_i = i;
    cell_details[i][j].parent_j = j;

    // min-heap priority queue by f value: tuple<f, row, col>
    using Node = tuple<double,int,int>;
    priority_queue<Node, vector<Node>, greater<Node>> open_list;
    open_list.emplace(0.0, i, j);

    bool found_dest = false;

    // 8 possible movements
    const vector<pair<int,int>> directions = {
        {0,1}, {0,-1}, {1,0}, {-1,0},
        {1,1}, {1,-1}, {-1,1}, {-1,-1}
    };

    while (!open_list.empty()) {
        double f_curr;
        int r, c;
        std::tie(f_curr, r, c) = open_list.top();
        open_list.pop();

        // if already processed (closed), skip
        if (closed_list[r][c]) continue;

        closed_list[r][c] = true;

        // check all successors
        for (const auto &d : directions) {
            int new_i = r + d.first;
            int new_j = c + d.second;

            if (is_valid(new_i, new_j) && is_unblocked(grid, new_i, new_j) && !closed_list[new_i][new_j]) {
                if (is_destination(new_i, new_j, dest)) {
                    cell_details[new_i][new_j].parent_i = r;
                    cell_details[new_i][new_j].parent_j = c;
                    cout << "The destination cell is found\n";
                    trace_path(cell_details, dest);
                    found_dest = true;
                    return;
                } else {
                    double g_new = cell_details[r][c].g + 1.0;
                    double h_new = calculate_h_value(new_i, new_j, dest);
                    double f_new = g_new + h_new;

                    if (cell_details[new_i][new_j].f == numeric_limits<double>::infinity() ||
                        cell_details[new_i][new_j].f > f_new) {
                        open_list.emplace(f_new, new_i, new_j);
                        cell_details[new_i][new_j].f = f_new;
                        cell_details[new_i][new_j].g = g_new;
                        cell_details[new_i][new_j].h = h_new;
                        cell_details[new_i][new_j].parent_i = r;
                        cell_details[new_i][new_j].parent_j = c;
                    }
                }
            }
        }
    }

    if (!found_dest) {
        cout << "Failed to find the destination cell\n";
    }
}

int main() {
    // grid: 1 = unblocked, 0 = blocked (copied from your python example)
    vector<vector<int>> grid = {
        {1,0,1,1,1,1,0,1,1,1},
        {1,1,1,0,1,1,1,0,1,1},
        {1,1,1,0,1,1,0,1,0,1},
        {0,0,1,0,1,0,0,0,0,1},
        {1,1,1,0,1,1,1,0,1,0},
        {1,0,1,1,1,1,0,1,0,0},
        {1,0,0,0,0,1,0,0,0,1},
        {1,0,1,1,1,1,0,1,1,1},
        {1,1,1,0,0,0,1,0,0,1}
    };

    pair<int,int> src = {8, 0};
    pair<int,int> dest = {0, 0};

    a_star_search(grid, src, dest);

    return 0;
}
