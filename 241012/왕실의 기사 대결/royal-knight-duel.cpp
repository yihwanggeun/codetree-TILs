#include<iostream>
#include<vector>

using namespace std;

int L, N, Q;
struct robot{
    int row;
    int col;
    int h;
    int w;
    int dur;
    int die;
    int attack;
    int move_turn;
};
struct sign{
    int id;
    int d;
};
int map[40][40];
vector<robot> robots;
vector<sign> signs;
int robot_map[40][40];
int finish = 0;
int drow[4] = {-1, 0, 1, 0};
int dcol[4] = {0, 1, 0 ,-1};
int visited[30] = {0};
vector<int> possible;
void makeRobotMap(){
    for(int i=0; i<robots.size(); i++){
        if(robots[i].die == 1) continue;
        for(int j=0; j<robots[i].h; j++){
            for(int k=0; k<robots[i].w; k++){
                robot_map[robots[i].row+j][robots[i].col+k] = i;
            }
        }
    }
}
void move(int id, int d){
    vector<int> next_idx; 
    if(robots[id].die == 1) {
        finish = 1;
        return;
    }
    for(int i=0; i<robots[id].h; i++){
        for(int j=0; j<robots[id].w; j++){
            int jump =0;
            int nrow = robots[id].row + i + drow[d];
            int ncol = robots[id].col + j + dcol[d];
            if(nrow<0 || ncol <0 || nrow + 1 > L || ncol + 1 > L){
                finish = 1;
                return;
            }
            if(map[nrow][ncol] == 2){
                finish = 1;
                return;
            }
            if(robot_map[nrow][ncol] != -1 && robot_map[nrow][ncol] != id ){
                for(int v=0; v< next_idx.size(); v++){
                    if(next_idx[v] == robot_map[nrow][ncol]) jump = 1;
                }
                if(jump == 1) continue;
                next_idx.push_back(robot_map[nrow][ncol]);
            }
        }
    }
    
    for(int i=0; i<next_idx.size(); i++){
        move(next_idx[i],d);
        if(finish == 1) return;
    }
    if(finish == 1) return;
    possible.push_back(id);
}
void realMove(int q, int d){
    for(int i = 0; i<possible.size();i++){
        robots[possible[i]].row = robots[possible[i]].row + drow[d];
        robots[possible[i]].col = robots[possible[i]].col + dcol[d];
        robots[possible[i]].move_turn = q;
    }
}
void Damage(int id, int q){
    for(int i=0; i<L; i++){
        for(int j=0; j<L; j++){
            if(map[i][j] == 1 && robot_map[i][j] != -1 && robot_map[i][j] != id 
                && robots[robot_map[i][j]].die != 1&& robots[robot_map[i][j]].move_turn == q){
                robots[robot_map[i][j]].attack ++;
                if(robots[robot_map[i][j]].attack>= robots[robot_map[i][j]].dur) robots[robot_map[i][j]].die = 1;
            }
        }
    }
    
}
void macro(int q){
    possible = vector<int>();
    for(int i=0; i<L; i++) for(int j=0; j<L; j++) robot_map[i][j] = -1;
    for(int i=0; i<N; i++) visited[i] = 0;
    makeRobotMap();
    //cout << "잘 그려지는지 확인\n";
    // for(int i=0; i<L; i++) {
    //     for(int j=0; j<L; j++) {
    //         cout << robot_map[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    int id = signs[q].id;
    int d = signs[q].d;
    move(id, d);
    if(finish == 1){
        return;
    }
    else{
        realMove(q, d);
    }
    for(int i=0; i<L; i++) for(int j=0; j<L; j++) robot_map[i][j] = -1;
    makeRobotMap();
    //cout << "다시 그릴 떄 잘 그려지는지 확인\n";
    // for(int i=0; i<L; i++) {
    //     for(int j=0; j<L; j++) {
    //         cout << robot_map[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    Damage(id, q);
}
int main(){
    cin >> L >> N >> Q;
    for(int i=0; i<L; i++)  for(int j=0; j<L; j++) cin >> map[i][j];
    for(int i=0; i<N; i++){
        int row,col,h,w,dur;
        cin >> row >> col >> h >> w >> dur;
        robots.push_back({row-1,col-1,h,w,dur,0,0,-1});
    }
    
    for(int i=0; i<Q; i++){
        int id, d;
        cin >> id >> d;
        signs.push_back({id-1,d});
    }
    
    for(int i=0; i<Q; i++){
        finish = 0;
        macro(i);
    }
    int total = 0;
    for(int i=0; i<N; i++){
        if(robots[i].die == 1) continue;
        total += robots[i].attack;
    }
    cout << total << endl;
}