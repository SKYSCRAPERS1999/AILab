#pragma GCC optimize(3,"Ofast","inline")
#include<bits/stdc++.h>
using namespace std;
#define lowbit(x) ((x)&(-(x)))
#define MP make_pair
#define fi first
#define se second
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count()); //mt19937_64 for 64-bits

struct Node{
    char id[10];
    double profit[7];
};
vector<Node> input;
void readin(){
    Node x;
    while (scanf("%s", x.id) != EOF) {
        for (int i = 0; i < 7; i++) scanf("%lf", &x.profit[i]);
        input.push_back(x);
    }
}

vector<vector<double> > Vec;
vector<double> vec;
const int S = 25;
void generate_arr(int n = 0, int tot = 0){
    if (n == 9) {
        vec.push_back((S - tot) / double(S));
        Vec.push_back(vec);
        vec.pop_back();
        return;
    }
    for (int i = 0; i <= S - tot; i++) {
        vec.push_back(i / double(S));
        generate_arr(n + 1, tot + i);
        vec.pop_back();
    }
}

int main(){
    freopen("profit", "r", stdin);
    readin();
    generate_arr();
    for (auto arr_k: Vec) {

    }
}

