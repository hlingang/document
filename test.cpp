#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <pthread.h>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
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
    char dir[] = "/dev/shm/dirXXXXXX";
    char file[] = "/dev/shm/fileXXXXXX";
    mkdtemp(dir);
    mkstemp(file);
    printf("%s:\n", dir);
    printf("%s:\n", file);
}