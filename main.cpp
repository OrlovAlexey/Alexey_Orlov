#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::map;
using std::set;

struct node{
    std::map<char, int> to;
    bool terminal;
};

void add(const std::string& s, std::vector<node>& trie) {
    int v = 0;
    for (char i : s) {
        if (!trie[v].to.count(i)) {
            trie.emplace_back();
            trie[v].to[i] = (int)trie.size() - 1;
        }
        v = trie[v].to[i];
    }
    trie[v].terminal = true;
}

void trie_construnting(const string& s, string& without_words, vector<node>& trie) {
    string temp;
    int j;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] != '.') {
            temp = "";
            for (j = i; j < s.size() && s[j] != '.'; ++j) {
                temp += s[j];
            }
            i = j - 1;
            add(temp, trie);
            without_words += '#';
        }
        else {
            without_words += '.';
        }
    }
}

void dfs(int v, bool* used, const string& current_string, const std::vector<node>& trie, vector<string>& answer_array) {
    used[v] = true;
    const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";
    if (trie[v].terminal) {
        answer_array.push_back(current_string);
    }
    for (char i : ALPHABET) {
        if (trie[v].to.count(i)) {
            int u = trie[v].to.at(i);
            if (!used[u]) {
                dfs(u, used, current_string + i, trie, answer_array);
            }
        }
    }
}

void algo_dfs(const vector<node>& trie, vector<string>& ans) {
    int n = (int)trie.size();
    bool used[n];
    for (int i = 0 ; i < n; ++i)
        used[i] = false;

    string curr;

    for (int i = 0 ; i < n; ++i) {
        if (!used[i])
            dfs(i, used, curr, trie, ans);
    }
}

void printing_answer(const vector<string>& ans, const string& without_words) {
    auto it = ans.begin();
    string result;
    for (char without_word : without_words) {
        if (without_word == '#') {
            result += *it;
            ++it;
        }
        else {
            result += '.';
        }
    }
    cout << result;
}

int main() {
    node root;
    vector<node> trie(1);
    trie[0] = root;
    string s;
    cin >> s;
    string without_words;
    trie_construnting(s, without_words, trie);

    vector<string> ans;
    algo_dfs(trie, ans);

    printing_answer(ans, without_words);
    return 0;
}
