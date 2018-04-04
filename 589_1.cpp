#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

void init(vector<vector<int>> &mapa, int row, int col, array<int, 2> &start, array<int, 2> &box, array<int, 2> &target){
    for (int i = 0; i < row; i++){
        vector<int> r;
        for (int j = 0; j < col; j++){
            r.push_back(1);
            char c;
            cin >> c;
            switch (c) {
                case '#': r.back() = 0; break;
                case 'S': start[0] = i; start[1] = j; break;
                case 'B': box[0] = i; box[1] = j; break;
                case 'T': target[0] = i; target[1] = j; break;
                default: break;
            }
        }
        mapa.push_back(r);
    }

}

bool check(vector<vector<int>> &mapa, array<int, 2> next){
    int i = next[0], j = next[1];
    if (i >= 0 && i < mapa.size() && j >= 0 && j < mapa[0].size())
        if (mapa[i][j] == 1)
            return true;

    return false;
}

vector<array<int,2>> Astar(vector<vector<int>> &mapa, array<int, 2> &start, array<int, 2> &box, array<int, 2> &target){
    vector<array<int, 2>> route;

    queue<array<int, 2>> frontier;
    frontier.push(start);
    map <array<int,2>, array<int,2>> came_from;
    came_from[start] = start;

    mapa[box[0]][box[1]] = 0;

    while (!frontier.empty()){
        array<int, 2> current = frontier.front();
        frontier.pop();

        vector<array<int, 2>> neighbors;
        array<int, 2> up{}, left{}, right{}, down{};
        up[0] = current[0]-1;       up[1] = current[1];   neighbors.push_back(up);
        left[0] = current[0];   left[1] = current[1]-1;   neighbors.push_back(left);
        down[0] = current[0]+1;     down[1] = current[1]; neighbors.push_back(down);
        right[0] = current[0];  right[1] = current[1]+1;  neighbors.push_back(right);

        for (auto &next : neighbors){
            if (check(mapa, next) && came_from.count(next) == 0){
                frontier.push(next);
                came_from[next] = current;
            }
        }
    }

    mapa[box[0]][box[1]] = 1;

    if (!came_from.count(target)) {
        return route;
    }

    array<int, 2> current = target;
    while (current != start){
        route.push_back(current);
        current = came_from[current];
    }
    route.push_back(start);
    reverse(route.begin(), route.end());

    return route;
};

void printPath(vector<array<int, 2>> path, string &s){

    for (int i = 1; i < path.size(); i++){
        int ix, iy;
        ix = path[i][1] - path[i-1][1];
        iy = path[i][0] - path[i-1][0];
        if (ix == -1)
            s += 'w';
        if (ix == 1)
            s += 'e';
        if (iy == 1)
            s += 's';
        if (iy == -1)
            s += 'n';
    }
}

string findPath(vector<vector<int>> &mapa, array<int, 2> &start, array<int, 2> &box, array<int, 2> &target, int stpt){
    array<array<int,2>, 2> begining = {box, start};
    queue<array<array<int,2>, 2>> frontier;
    frontier.push(begining);
    map <array<array<int,2>, 2>, array<array<int,2>, 2>> came_from;
    came_from[begining] = begining;

    map <array<array<int,2>, 2>, vector<array<int,2>>> walks;

    while (!frontier.empty()){
        array<array<int,2>, 2> current = frontier.front();
        frontier.pop();

        vector<array<int, 2>> neighbors;
        array<int, 2> up{}, left{}, right{}, down{};
        up[0] = current[0][0]-1;       up[1] = current[0][1];   neighbors.push_back(up);
        left[0] = current[0][0];   left[1] = current[0][1]-1;   neighbors.push_back(left);
        down[0] = current[0][0]+1;     down[1] = current[0][1]; neighbors.push_back(down);
        right[0] = current[0][0];  right[1] = current[0][1]+1;  neighbors.push_back(right);

        int j = 0;
        for (int i = stpt++; j++ < neighbors.size(); i++){
            i = i % 4;
            array<array<int,2>, 2> newn = {neighbors[i], current[0]};
            if (check(mapa, neighbors[i]) && check(mapa, neighbors[(i+2)%4]) && came_from.count(newn) == 0){
                if (current[1] == neighbors[(i+2)%4]){
                    frontier.push(newn);
                    came_from[newn] = current;
                }
                else{
                    vector<array<int,2>> route = Astar(mapa, current[1], current[0], neighbors[(i+2)%4]);
                    if (!route.empty()){
                        walks[newn] = route;
                        frontier.push(newn);
                        came_from[newn] = current;
                    }
                }
            }
        }
    }

    vector <array<array<int,2>, 2>> ends;
    array<int, 2> up{}, left{}, right{}, down{};
    up[0] = target[0]-1;       up[1] = target[1];   array<array<int,2>, 2> ups = {target, up};      ends.push_back(ups);
    left[0] = target[0];   left[1] = target[1]-1;   array<array<int,2>, 2> lefts = {target, left};  ends.push_back(lefts);
    down[0] = target[0]+1;     down[1] = target[1]; array<array<int,2>, 2> downs = {target, down};  ends.push_back(downs);
    right[0] = target[0];  right[1] = target[1]+1;  array<array<int,2>, 2> rights = {target, right};ends.push_back(rights);

    string output;
    for (auto &current : ends){
        if (came_from.count(current) == 1){
            vector<array<array<int,2>, 2>> path;
            string s;
            while (current != begining){
                path.push_back(current);
                current = came_from[current];
            }
            path.push_back(begining);
            reverse(path.begin(), path.end());

            for (int i = 1; i < path.size(); i++){
                if(walks.count(path[i])){
                    vector<array<int,2>> walk = walks[path[i]];
                    printPath(walk, s);
                }
                int ix, iy;
                ix = path[i][0][1] - path[i-1][0][1];
                iy = path[i][0][0] - path[i-1][0][0];
                if (ix == -1)
                    s += 'W';
                if (ix == 1)
                    s += 'E';
                if (iy == 1)
                    s += 'S';
                if (iy == -1)
                    s += 'N';
            }

            if (output.empty() || s.size() < output.size())
                output = s;
        }
    }

    return output;
}

int main() {
    int row, col;
    int casenum = 0;

    while (cin >> row >> col){          //attention 0 0 will be read
        if (row == 0 && col == 0)
            break;
        casenum++;
        vector<vector<int>> mapa;
        array<int, 2> start{}, box{}, target{};

        init(mapa, row, col, start, box, target);

        cout << "Maze #" << casenum << endl;

        string output;
        for (int i = 0; i < 4; i++){
            string s = findPath(mapa, start, box, target, i);
            if (output.empty() || s.size() < output.size())
                output = s;
        }
        if (output.empty())
            cout << "Impossible." << endl;
        else
            cout << output << endl;

        cout << endl;
    }
    return 0;
}
