#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::min;

vector<int> z_function(const string& s) {
	int n = (int)s.length();
	vector<int> z(n);
	int l = 0;
	int r = 0;
	for (int i = 1; i < n; ++i) {
		if (i <= r) {
			z[i] = min(r - i + 1, z[i - l]);
		}
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			++z[i];
		}
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
	return z;
}

int main(){
	string s;
	cin >> s;
	vector<int> z = z_function(s);
	for (int i = 0; i < s.length(); ++i) {
		cout << z[i] << " ";
	}
	return 0;
}
