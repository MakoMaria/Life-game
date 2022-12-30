#include <iostream>
#include <vector>

using namespace std;
//
#define ALIVE_SIGN 1
#define DEAD_SIGN 0

auto count_neighbors(int N, int M, int i, int j, int **universe) -> int;
auto is_anyone_alive(int **universe, int N, int M) -> bool;
auto is_the_same(const vector<vector<vector<int>>>& history) -> bool;
auto coords_to_vec(int i, int j) -> vector<int>;

auto main() -> int {
    int N, M;

    cout << "Enter size of universe matrix (div by space pls)" << endl;
    cout << "> ";
    cin >> N >> M;
    vector<vector<vector<int>>> gen_history; /* vector that contain vectors with coordinates (i, j)
                                               * working with vectors in this case
                                               * is much easier than using arrays.
                                               * The reason in that we don't know exactly
                                               * duration of universe live (how many arrays we gonna have)
                                               * The question is that why we don't use int[2]
                                               * like vector<vector<int[2]>> ????
                                               * We use vectors instead because we have less problems with
                                               * realisation.
                                               * */
    vector<vector<int>> curr_alive; /* vector that contains coordinates (i, j) of alive cells */

    int **universe = new int*[N];
    int **after_ms = new int*[N];

    for (int i = 0; i < N; i++) {
        universe[i] = new int[M];
        after_ms[i] = new int[M];
    }
    cout << "Enter the Universe" << endl;
    cout << "(where 0 -> dead cell, 1 -> alive cell)" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> universe[i][j];
            if (universe[i][j] == ALIVE_SIGN) {
                curr_alive.push_back(coords_to_vec(i, j)); // add coordinates (i, j) into curr_alive vector
            }
        }
    }
    gen_history.push_back(curr_alive); // put curr_alive vector into the history
    curr_alive.clear(); // clear this vector for future using

    while (true) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                auto neighbors_num = count_neighbors(N, M, i, j, universe); // count alive neighbours around cell
                if (neighbors_num == 3 || neighbors_num == 2 && universe[i][j] == ALIVE_SIGN) {
                    // from the task. If cell has 3 neighbours then cell birth
                    // OR if cell is alive and has 2 (or 3) neighbours then keep cell alive
                    after_ms[i][j] = ALIVE_SIGN;
                } else
                    // else kill this cell :(
                    after_ms[i][j] = DEAD_SIGN;
            }
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                universe[i][j] = after_ms[i][j] == ALIVE_SIGN ? ALIVE_SIGN : DEAD_SIGN;
                if (universe[i][j] == ALIVE_SIGN) {
                    curr_alive.push_back(coords_to_vec(i, j));
                }
                cout << universe[i][j];
            }
             cout << endl;
        }
        cout << endl;
        gen_history.push_back(curr_alive); // put our vector of coordinates into history
        curr_alive.clear(); // clear vector of coordinates (i, j)

        if (!is_anyone_alive(after_ms, N, M) || is_the_same(gen_history)) {
            /* if no cells alive OR
             * we have cycle repeats
             * then our universe died (but why?)
             * */
            cout << "*** UNIVERSE IS DEAD ***" << endl;
            break;
        }
    }

    for (int i = 0; i < N; i++) { // cleaning our shit
        delete universe[i];
        delete after_ms[i];
    }

    return 0;
}

auto count_neighbors(int N, int M, int i, int j, int **universe) -> int { // count alive cells around our cell

    int count = 0;
    int left_row, mid_col, right_row, up_col, mid_row, down_col;

    left_row = i == 0 ? N-1 : i - 1; // if we located near border -> go around
    right_row = i == N - 1 ? 0 : i + 1;
    mid_row = i;
    mid_col = j;
    up_col = j == 0 ? M - 1 : j - 1;
    down_col = j == M - 1 ? 0 : j + 1;

    // don't read if you are not drunk right now
    count += universe[left_row][up_col] == 1 ? 1 : 0; // check if left up cell == 1
    count += universe[left_row][mid_col] == 1 ? 1 : 0;
    count += universe[left_row][down_col] == 1 ? 1 : 0;
    count += universe[mid_row][up_col] == 1 ? 1 : 0;
    count += universe[mid_row][down_col] == 1 ? 1 : 0;
    count += universe[right_row][up_col] == 1 ? 1 : 0;
    count += universe[right_row][mid_col] == 1 ? 1 : 0;
    count += universe[right_row][down_col] == 1 ? 1 : 0;

    return count;
}

auto is_anyone_alive(int **universe, int N, int M) -> bool {  // Does anyone exist in our universe?
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (universe[i][j] == ALIVE_SIGN) {
                return true;
            }
        }
    }
    return false;
}

auto is_the_same(const vector<vector<vector<int>>>& history) -> bool { //
    auto universe_num = history.size(); // get amount of generations from history
    const vector<vector<int>>& last_universe = history.back(); // get our current universe state
    for (int i = 0; i < universe_num - 1; i++) {
        // here we check if we ever had the same state
        if (history.at(i) == last_universe) return true;
    }
    return false;
}

auto coords_to_vec(int i, int j) -> vector<int> {
    vector<int> coord;
    coord.push_back(i);
    coord.push_back(j);
    return coord;
}
