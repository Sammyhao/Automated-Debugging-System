#include<iostream>
#include<string>
using namespace std;
int main() {
	string s;
	int ans[2] = {0,0};
	cin >> s;
	for (int i = 0; s[i] != '\0'; i++) {
		if (s[i] == 'N') {
			ans[0]++;
		}
		else if (s[i] == 'S') {
			ans[0]--;
		}
		else if (s[i] == 'W') {
            ans[1]++;
		}
		else if (s[i] == 'E') {
			ans[1]--;
		}

	}
	if (ans[0] == 0 && ans[1] == 0) {
		cout << "Yes" << endl;
	}
	else {
		cout << "No" << endl;
	}

	return 0;

}