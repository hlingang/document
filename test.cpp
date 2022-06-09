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
    char a[]{"123"};
    char b[]{"abc"};
    // strcpy(a, b);
    // strncpy(a, b, sizeof(b));
    strcat(a, b);
    strcmp(a, b);
    cout << a << endl;
    string ss;
    ss.assign(5, 'c');
    ss.assign("dd");
    list<int> li;
    li.assign({1, 2, 3, 4});
    li.assign(2, 100);
    for_each(li.begin(), li.end(), [](int &val) { cout << val << endl; });
    cout << ss << " capacity " << ss.capacity() << endl;
}
