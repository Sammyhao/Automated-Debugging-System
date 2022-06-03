#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

int main() {
    string t;
    cin >> t;
    int n, s, w, e;
    for (int i = 0; i < (int) t.size(); ++i) {
        if (t[i] == 'N') {
            ++n;
        }else if (t[i] == 'W') {
            ++w;
        }else if (t[i] == 'S') {
            ++s;
        }else if (t[i] == 'E') {
            ++e;
        }
    }

    if (n > 0 && s > 0 || w > 0 && e > 0) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }
}