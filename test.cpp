#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <pthread.h>
#include <regex>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include <array>
using namespace std;

int main() {
    std::array<int, 5> a;
    istringstream strins("abcde\nfghij");
    istream ins(strins.rdbuf());
    char rdbuf[128] = {0};
    while (!ins.eof()) {
        // cout << "get: " << ins.get() << endl;
        // ins.read(rdbuf, 1);

        ins.getline(rdbuf, sizeof(rdbuf));
        cout << "rdbuf: " << rdbuf << endl;
        int pos = ins.tellg();
        cout << "pos: " << pos << endl;
        // ins.seekg(1, ios::cur /*begin/cur/end*/);
        ins.sync();
    }
    stringstream stros("");
    stros.put('1');
    stros.put('2');
    stros.put('3');
    cout << "os " << stros.eof() << endl;
    while (!stros.eof()) {
        cout << "get os: " << stros.get() << " " << stros.eof() << endl;
    }
    istringstream iistr("abc");
    while (true) {
        char c = iistr.get();
        if (iistr.eof()) {
            break;
        }
        cout << c << endl;
    }
    ifstream inf("001.txt");
    char c;
    while (inf.get(c)) {
        cout << c << endl;
    }
}
