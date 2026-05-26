#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#define MAX_SEEDS 15
#define GRID_MAX_SIZE 15
#define GRID_LOWEST_BOUND 0
#define GRID_HIGHEST_BOUND 14
#define HOME_LOC_R 1
#define HOME_LOC_C 0

using namespace std;

class Loc {
   public:
    int r;
    int c;
    int dir;
    int distance;

    Loc() {
        this->r = 0;
        this->c = 0;
        this->dir = -1;
        this->distance = INT_MAX;
    }

    Loc(int r, int c) {
        this->r = r;
        this->c = c;
        this->dir = -1;
        this->distance = INT_MAX;
    }
    
    Loc(int r, int c, int dir, int distance) {
        this->r = r;
        this->c = c;
        this->dir = dir;
        this->distance = distance;
    }
};

class State {
public:
    int seedIdx;
    Loc mouseLoc;
    Loc catLoc;

    State(int seedIdx, Loc mouseLoc, Loc catLoc) {
        this->seedIdx = seedIdx;
        this->mouseLoc = mouseLoc;
        this->catLoc = catLoc;
    }
};

int solve(const vector<Loc> &locations);
void printLocations(const vector<Loc> &locations);
int calculateDistance(const Loc &a, const Loc &b);

/*
2
3
2 3
2 2
1 1
4 4
3
2 3
2 2
1 1
5 3
*/
int main() {
    int T;

    cin >> T;

    for (int t = 1; t <= T; ++t) {
        int M;
        cin >> M;
        M += 1;

        int a, b;
        vector<Loc> locations;
        for (int m = 0; m < M; ++m) {
            // add the location of mouse home
            if (m == M - 1) locations.push_back(Loc(1, 0));

            cin >> a >> b;
            locations.push_back(Loc(b, a));
        }

        int output = solve(locations);

        cout << "#" << t << " " << output << endl;
    }

    return 0;
}

int calculateDistance(const Loc &a, const Loc &b) { return abs(b.r - a.r) + abs(b.c - a.c); }

void printLocations(const vector<Loc> &locations) {
    for (auto loc : locations) {
        cout << "Loc("<<loc.r << "," << loc.c << "), Dir ("<< loc.dir << "), Dist("<< loc.distance << ");" << endl;
    }
    cout << endl;
}

int solve(const vector<Loc> &locations) {
    // printLocations(locations);

    queue<State> q;
    bool isVisited [MAX_SEEDS][GRID_MAX_SIZE][GRID_MAX_SIZE][GRID_MAX_SIZE][GRID_MAX_SIZE] {};
    
    int seedInitialIdx = 1;
    Loc mouseInitialLoc = locations.at(0);
    Loc catInitialLoc = locations.at(locations.size() - 1);
    q.push(State(seedInitialIdx, mouseInitialLoc, catInitialLoc));
    
    isVisited[seedInitialIdx][mouseInitialLoc.r][mouseInitialLoc.c][catInitialLoc.r][catInitialLoc.c] = true;

    while (!q.empty()) {
        State state = q.front();
        q.pop();

        Loc seedLoc = locations.at(state.seedIdx);
        Loc mouseLoc = state.mouseLoc;
        Loc catLoc = state.catLoc;

        cout << "Seed: " << seedLoc.r << " " << seedLoc.c << endl;
        cout << "Mouse: " << mouseLoc.r << " " << mouseLoc.c << endl;
        cout << "Cat: " << catLoc.r << " " << catLoc.c << endl << endl;

        // MOUSE MOVEMENT
        vector<Loc> mouseLocations;
        
        int mDirections[4][2] {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1}
        };
        
        for (int d = 0; d < 4; ++d) {
            int nextR = mouseLoc.r + mDirections[d][0];
            int nextC = mouseLoc.c + mDirections[d][1];
            if (nextR <= GRID_LOWEST_BOUND || GRID_HIGHEST_BOUND <= nextR || 
                nextC <= GRID_LOWEST_BOUND || GRID_HIGHEST_BOUND <= nextC) {
                if (nextR != 1 && nextC != 0) {
                    continue;
                }
            }
            
            Loc nextLoc(nextR, nextC, d, calculateDistance(seedLoc, Loc(nextR, nextC)));
            mouseLocations.push_back(nextLoc);
        }

        sort(mouseLocations.begin(), mouseLocations.end(), [](Loc a, Loc b) { return a.distance < b.distance; });
        
        cout << "Mouse: " << endl;
        printLocations(mouseLocations);

        int mLowestDist = mouseLocations.at(0).distance;
        int countMLowest = 0;
        for (auto nextMouseLoc : mouseLocations) {
            if (mLowestDist == nextMouseLoc.distance) {
                countMLowest += 1;
            }
        }

        // CAT MOVEMENT
        for (auto nextMouseLoc : mouseLocations) {
            if (nextMouseLoc.distance != mLowestDist) break;
            if (countMLowest > 1 && (nextMouseLoc.dir == 0 || nextMouseLoc.dir == 1)) continue;

            // BASE CASE
            // mouse has reached the seed
            int nextSeedIdx = state.seedIdx;
            if (nextMouseLoc.r == seedLoc.r && nextMouseLoc.c == seedLoc.c) {
                nextSeedIdx += 1;
            }

            // mouse has reached home
            if (nextMouseLoc.r == HOME_LOC_R && nextMouseLoc.c == HOME_LOC_C) {
                return 1;
            }
            
            int cDirections[8][2] {
                {-1, 0},
                {1, 0},
                {0, -1},
                {0, 1},
                {-2, 0},
                {2, 0},
                {0, -2},
                {0, 2}
            };
            
            vector<Loc> catLocations;
            for (int d = 0; d < 8; ++d) {
                int nextR = catLoc.r + cDirections[d][0];
                int nextC = catLoc.c + cDirections[d][1];
                
                if (nextR <= GRID_LOWEST_BOUND || GRID_HIGHEST_BOUND <= nextR || 
                    nextC <= GRID_LOWEST_BOUND || GRID_HIGHEST_BOUND <= nextC) continue;
                
                Loc nextLoc(nextR, nextC, d, calculateDistance(nextMouseLoc, Loc(nextR, nextC)));
                catLocations.push_back(nextLoc);
            }

            sort(catLocations.begin(), catLocations.end(), [](Loc a, Loc b) { 
                return a.distance < b.distance; }
            );
            
            cout << "Cat:" << endl;
            printLocations(catLocations);

            int cLowestDist = catLocations.at(0).distance;
            int countCLowest = 0;
            for (auto nextCatLocation : catLocations) {
                if (cLowestDist == nextCatLocation.distance) {
                    countCLowest += 1;
                }
            }

            for (auto nextCatLocation : catLocations) {
                if (cLowestDist != nextCatLocation.distance) break;
                if (countCLowest > 1 && (nextCatLocation.dir == 2 || nextCatLocation.dir == 3 || 
                    nextCatLocation.dir == 6 || nextCatLocation.dir == 7)) continue;
                    
                // add the state to qs
                if (nextMouseLoc.r != nextCatLocation.r || nextMouseLoc.c != nextCatLocation.c) {
                    if (!isVisited[nextSeedIdx][nextMouseLoc.r][nextMouseLoc.c][nextCatLocation.r][nextCatLocation.c]) {
                        isVisited[nextSeedIdx][nextMouseLoc.r][nextMouseLoc.c][nextCatLocation.r][nextCatLocation.c] = true;
                        q.push(State(nextSeedIdx, nextMouseLoc, nextCatLocation));
                    }
                }
            }
        }
    }

    return 0;
}
