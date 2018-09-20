#pragma GCC optimize(3,"Ofast","inline")
#include<bits/stdc++.h>

using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count()); //mt19937_64 for 64-bits

// S: size of searching space
// N: number of selected gupiaos
const int S = 20, N = 10;

struct Node{
    char id[10];
    double profit[7];
    bool operator < (const Node& rhs) const {
        return profit[6] > rhs.profit[6];
    }
};
vector<Node> input;
void readin(){
    Node x;
    while (scanf("%s", x.id) != EOF) {
        for (int i = 0; i < 7; i++) scanf("%lf", &x.profit[i]);
        input.push_back(x);
    }
    sort(input.begin(), input.end());
    input.erase(input.begin() + N, input.end());
}

vector<vector<double> > Vec;
vector<double> vec;
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

// VaR = -mu(T) - C * Sigma(T)
//k1x1+k2x2+...k10x10 >= lower_bound_profit, mini VaR
//VaR <= upper_bound_VaR, maxi: k1x1+k2x2+...k10x10

const double C = -1.6448536269514729;
const double lower_bound_profit = 0.018;
const double upper_bound_VaR = 0.00000172;

double get_VaR(vector<double>& r) {
    double VaR = 0.0, bar = 0.0;
    int n = 6;
    for (int i = 0; i < n; i++) bar += r[i];
    bar /= double(n);
    for (int i = 0; i < n; i++) VaR += (r[i] - bar) * (r[i] - bar);
    return VaR / double(n);
}

void output_mini_VaR(){
    // getting min_VaR, s.t profit >= lower_bound_profit
    int best_idx = 0; double min_VaR = DBL_MAX;
    vector<double> r(7);
    for (int i = 0; i < Vec.size(); i++) {
        vector<double> &arr_k = Vec[i];
        for (int idx = 0; idx < 7; idx++){
            r[idx] = 0.0;
            for (int i = 0; i < N; i++) {
                r[idx] += input[i].profit[idx] * arr_k[i];
            }
        }
        if (r[6] < lower_bound_profit) continue;
        else {
            double cur_VaR = get_VaR(r);
            if (cur_VaR < min_VaR) min_VaR = cur_VaR, best_idx = i;
        }
        if (i % 100000 == 0) printf("cnt = %d\n", i);
    }
    printf("best_idx = %d, min_VaR = %.15f\n", best_idx, min_VaR);
    for (int i = 0; i < N; i++) printf("%s: %f\n", input[i].id, Vec[best_idx][i]);
}

void output_maxi_profit(){
    // getting max_profit, s.t VaR <= upper_bound_VaR
    int best_idx = 0; double max_profit = DBL_MIN;
    vector<double> r(7);
    for (int i = 0; i < Vec.size(); i++) {
        vector<double> &arr_k = Vec[i];
        for (int idx = 0; idx < 7; idx++){
            r[idx] = 0.0;
            for (int i = 0; i < N; i++) {
                r[idx] += input[i].profit[idx] * arr_k[i];
            }
        }
        double cur_VaR = get_VaR(r);
        if (cur_VaR > upper_bound_VaR) continue;
        else {
            if (r[6] > max_profit) max_profit = r[6], best_idx = i;
        }
        if (i % 100000 == 0) printf("cnt = %d\n", i);
    }
    printf("best_idx = %d, max_profit = %.15f\n", best_idx, max_profit);
    for (int i = 0; i < N; i++) printf("%s: %f\n", input[i].id, Vec[best_idx][i]);
}

int main(){
    freopen("profit", "r", stdin);
    readin();
    generate_arr();
    printf("Searching size is %d\n", Vec.size());

    puts("getting min_VaR, s.t profit >= lower_bound_profit");
    output_mini_VaR();

    puts("\ngetting max_profit, s.t VaR <= upper_bound_VaR");
    output_maxi_profit();

    return 0;
}


