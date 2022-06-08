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
class A {
  public:
    A(int index) { mIndex = index; }
    int mIndex;
};
void test(A &a) { cout << "index: " << a.mIndex << endl; }
void test(A &&a) { cout << "index: " << a.mIndex << endl; }

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
    shared_ptr<int> intPtr(new int[10], deleter);
    shared_ptr<int> intPtr(new int[10], sdeleter());
    return 0;
}
