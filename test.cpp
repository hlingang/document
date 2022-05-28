#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <pthread.h>
#include <regex>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

class MM {
  public:
    MM() { cout << "no" << endl; }
    MM(initializer_list<int> li) {
        for (auto val : li) {
            cout << val << endl;
        }
    }
};

void test(char *p) { printf("%s:\n", p); }
using namespace std;

int retry() {
    int a = 10;
    return a;
}

int main() {
    ios::in;
    plus<int> mplus;
    int sum = mplus(3, 4);
    printf("sum = %d\n", sum);
    array<int, 3> a1 = {1, 2, 3};
    array<int, 3> a2 = a1;
    int b[3] = {4, 5, 6};
    copy(begin(b), end(b), a2.begin());
    for (auto val : a2) {
        printf("val:%d\n", val);
    }
    transform(a2.begin(), a2.end(), a2.begin(),
              [](int val) { return val + 100; });
    for_each(a2.begin(), a2.end(), [](int &val) { val++; });
    for_each(a2.begin(), a2.end(), [](int val) { printf("val:%d\n", val); });
    cout << hex << 0xffffffffe1 << ", " << dec << 0xffffffffe1 << "  end "
         << endl;
    std::fstream out;
    out.open("001.txt", ios::out);
    printf("isopen:%d\n", out.is_open());
    istringstream in("abc, def, ghk");
    string word;
    while (!in.eof()) {
        in >> word;
        printf("word:%s\n", word.c_str());
    }
    // printf("str:%s\n", in.str());
    vector<map<int, vector<int>>> vecm;
    map<int, vector<int>> m1;
    map<int, vector<int>> m2;
    vecm.push_back(m1);
    vecm.push_back(m2);
    vecm[0][100].push_back(101);
    vecm[0][100].push_back(102);
    vecm[0][200].push_back(201);
    vecm[0][200].push_back(202);
    map<int, vector<int>>::iterator iter;
    map<int, vector<int>> &imap = vecm[0];
    iter = imap.begin();
    while (iter != imap.end()) {
        vector<int> &ivec = iter->second;
        printf("key:%d\n", iter->first);
        for (size_t i = 0; i < ivec.size(); i++) {
            printf("val:%d\n", ivec[i]);
        }
        ++iter;
    }

    // 简单正则表达式匹配
    std::string fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};
    std::regex txt_regex("(\\w+)");

    for (const auto &fname : fnames) {
        smatch match;
        int ret = regex_search(fname, match, txt_regex);
        if (ret) {
            ssub_match submatch;
            cout << "prefix :" << match.prefix() << endl;
            cout << "suffix :" << match.suffix() << endl;
            for (int i = 0; i < match.size(); i++) {
                submatch = match[i];
                cout << i << ":  " << submatch.str() << endl;
            }
        }
    }
    vector<string> vs;
    vs = vector<string>({"csv"});
    for (auto val : vs) {
        cout << val << endl;
    }
    MM mm({1, 2, 3});
}