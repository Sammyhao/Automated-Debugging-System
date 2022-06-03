#include <bits/stdc++.h>
using namespace std;

string s;

bool have(char c) {
	return s.find(c) != s.size();
}

int main() {
	ios::sync_with_stdio(false), cin.tie(0);
	cin >> s;
	cout << ((have('N') ^ have('S')) | (have('E') ^ have('W'))? "No": "Yes");
}
