/*
    2022년 상반기 오전 1번 - 술래잡기
    문제 이해
    n*n 격자에 진행되고 술래는 n/2, n/2에서 시작한다. 

    m명의 도망자가 있고 도망자는 처음 지정된 곳에 있다.
    1. 좌우로만 움직이는 유형 (오른쪽 보고 시작)
    2. 상하로만 움직이는 유형 (아래쪽 보고 시작)

    h개의 나무가 있다. 

    m명의 도망자가 동시에 움직이고 -> 술래가 움직이고 -> 반복

    도망자 이동 Logic
    1. 술래와의 거리가 3이하인 도망자만 움직인다. 
    2-1. 움직이는 데 격자를 벗어나지 않은 경우
        술래가 거기 있으면 움직이지 않는다.
        술래만 없으면 그냥 움직여도 된다.
    2-2. 움직이는데 격자를 벗어나는 경우
        방향을 틀어서 술래가 있으면~ 술래가 없으면
    
    술래 이동 Logic 
    처음 위 방향으로 시작해서 달팽이 모양으로 움직인다. 
    그리고 끝에 도달하면 반대 방향으로 
    만약 이동방향이 틀어지는 지점이라면 바로 틀어준다.

    술래잡기 Logic
    시야 내에 있는 도망자를 잡는다. 바라보고 있는 방향 기준 3칸
    단, 나무가 있으면 Pass

    t턴이라고 했을 때 현재 턴에 잡힌 도망자의 수 
    도망자는 사라지게 된다. 

    자료 구조
    나무를 저장할 Map
    사람의 위치를 저장할 Map
    사람의 정보를 저장할 Vector
    술래의 정보를 저장할 변수 


*/
#include<iostream>
#include<vector>
#include<cmath>
using namespace std;

struct Player{
    int row;
    int col;
    int d;
    int die;
};

// 상우하좌
// 초기에는 1 -> 1, 2 -> 2

int drow[4] = {-1, 0, 1, 0};
int dcol[4] = {0, 1, 0, -1};
int max_dist = 1;
int now_dist = 0;
int flag = 0;
bool reverse_flag = false;
int n, m, h, k;
int tree_map[100][100] = {0};
vector<int> player_map[100][100];
vector<Player> players;
Player sulae;

int answer = 0;
int turn;

void syncWithVector(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            player_map[i][j] = vector<int>();
        }
    }

    for(int i = 0; i < m; i++){
        if(players[i].die == 1) continue;
        player_map[players[i].row][players[i].col].push_back(i);
    }
}
void init(){
    cin >> n >> m >> h >> k;
    for(int i = 0; i < m; i++){
        int row, col, d;
        cin >> row >> col >> d;
        players.push_back({row-1,col-1,d,0});
    }
    for(int i = 0; i < h; i++){
        int row, col;
        cin >> row >> col;
        tree_map[row-1][col-1] = 1;
    }
    syncWithVector();
    sulae = {n/2, n/2, 0, 0};

    // cout << "초기 상태의 애들 위치 \n";
    // for(int i = 0; i < m; i++){
    //     cout << players[i].row << " " << players[i].col << " " << players[i].d << " " << players[i].die << "\n";
    // }
}
void move(int idx){
    int dist = abs(sulae.row - players[idx].row) + abs(sulae.col - players[idx].col);
    if(dist > 3) return;

    int nrow = players[idx].row + drow[players[idx].d];
    int ncol = players[idx].col + dcol[players[idx].d];

    if(nrow < 0 || ncol < 0 || nrow + 1 > n || ncol + 1 > n){
        players[idx].d = (players[idx].d + 2) % 4;
        nrow = players[idx].row + drow[players[idx].d];
        ncol = players[idx].col + dcol[players[idx].d];
    }

    if(nrow == sulae.row && ncol == sulae.col) return;

    players[idx].row = nrow; 
    players[idx].col = ncol;

}
void movePlayer(){
    for(int i = 0; i < m; i++){
        if(players[i].die == 1) continue;
        move(i);
    }
    // cout << "이동 후 위치 \n";
    // for(int i = 0; i < m; i++){
    //     cout << players[i].row << " " << players[i].col << " " << players[i].d << " " << players[i].die << "\n";
    // }
    syncWithVector();

}

void moveSulae(){
    now_dist++;
    sulae.row = sulae.row + drow[sulae.d];
    sulae.col = sulae.col + dcol[sulae.d];
    
    //cout << sulae.row << " " << sulae.col << "\n";
    if(sulae.row == 0 && sulae.col == 0){
        flag = 1;
        max_dist = n;
        now_dist = 1;
        sulae.d = (sulae.d+2) % 4;
        reverse_flag = !reverse_flag;
        return;
    }
    if(sulae.row == n/2 && sulae.col == n/2){
        flag = 0;
        max_dist = 1;
        now_dist = 0;
        sulae.d = (sulae.d+2) % 4;
        reverse_flag = !reverse_flag;
        return;
    }
    if(reverse_flag){
        if(now_dist == max_dist){
            sulae.d = (sulae.d + 3) % 4;
            now_dist = 0; 
            if(flag == 0){
                flag = 1;
            }
            else{
                max_dist--; 
                flag = 0;
            }
        }
    }
    else{
        if(now_dist == max_dist){
            sulae.d = (sulae.d + 1) % 4;
            now_dist = 0; 
            if(flag == 0){
                flag = 1;
            }
            else{
                max_dist++; 
                flag = 0;
            }
        }
    }

    
    //cout << reverse_flag << " " << flag << " " << now_dist << " " << max_dist << "\n";
    

}

void getPoint(){
    //술래 위에서 방향대로 for(3칸)
    // 만약 나무가 있는 위치면 Conitnue;
    // 만약 Size가 0명이 아니면 죽인다. 

    for(int i = 0; i < 3; i++){
        int nrow = sulae.row + drow[sulae.d] * i;
        int ncol = sulae.col + dcol[sulae.d] * i;

        if(nrow < 0 || ncol < 0 || nrow + 1 > n || ncol + 1 > n) continue;
        if(tree_map[nrow][ncol] != 0) continue;
        if(player_map[nrow][ncol].size()!=0){
            for(int j = 0; j < player_map[nrow][ncol].size(); j++){
                players[player_map[nrow][ncol][j]].die = 1;
                answer += turn;
            }
        }
        else{
            continue;
        }
    }
}
void macro(){
        //cout << "술래 이동 후 위치\n";

    for(turn = 1; turn <= k; turn++){
        movePlayer();
        moveSulae();
        getPoint();
    }
    
}
int main(){
    init();
    macro();
    cout << answer << "\n";
}