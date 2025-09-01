#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <iostream>          // <-- add this
#include <iomanip>           // <-- for setprecision
using namespace std;
using json = nlohmann::json;


long long convertToDecimal(const string& s, int base) {
    long long num = 0;
    for (char c : s) {
        int d = isdigit(c) ? c - '0' : c - 'A' + 10;
        num = num * base + d;
    }
    return num;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string raw((istreambuf_iterator<char>(cin)), {});
    json input = json::parse(raw);

    int totalRoots = input["keys"]["n"];
    int required = input["keys"]["k"];
    int degree = required - 1;

    vector<pair<long double,long double>> roots;
    for (auto& item : input.items()) {
        if (item.key() == "keys") continue;
        int x = stoi(item.key());
        int base = stoi(item.value()["base"].get<string>());
        string value = item.value()["value"].get<string>();
        roots.push_back({x, convertToDecimal(value, base)});
    }

    vector<long double> coeff(degree + 1, 0);
    for (int i = 0; i < required; i++) {
        long double xi = roots[i].first, yi = roots[i].second;
        vector<long double> poly = {1};
        long double denom = 1;
        for (int j = 0; j < required; j++) {
            if (i == j) continue;
            long double xj = roots[j].first;
            denom *= (xi - xj);
            vector<long double> nxt(poly.size() + 1);
            for (int p = 0; p < poly.size(); p++) {
                nxt[p]   -= poly[p] * xj;
                nxt[p+1] += poly[p];
            }
            poly.swap(nxt);
        }
        for (int p = 0; p <= degree; p++) coeff[p] += poly[p] * yi / denom;
    }

    cout << "P(x) = ";
    for (int i = degree; i >= 0; i--) {
        cout << fixed << setprecision(2) << coeff[i];
        if (i > 0) cout << "x^" << i << " + ";
    }
    cout << "\n";
}
