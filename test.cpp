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
using namespace std;

void deleter(int *ptr) {
    cout << "delete " << *ptr << endl;
    delete[] ptr;
};
struct sdeleter {
    void operator()(int *ptr) {
        cout << "delete " << *ptr << endl;
        delete[] ptr;
    }
};
int main() {
    array<int, 5> ai{1, 2, 3, 4, 5};
    int sumvalue1 = accumulate(ai.begin(), ai.end(), 0);
    int sumValue2 = reduce(ai.begin(), ai.end(), 0,
                           [](int val01, int val02) { return val01 + val02; });
    int sumValue3 = reduce(ai.begin(), ai.end(), 1,
                           [](int val01, int val02) { return val01 * val02; });
    cout << "value01: " << sumvalue1 << " value02: " << sumValue2
         << " value03: " << sumValue3 << endl;
}
