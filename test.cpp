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

int main() {
    chrono::time_point ts_point = chrono::system_clock::now();
    chrono::nanoseconds ts_duration_ns = ts_point.time_since_epoch();
    chrono::milliseconds ts_duration_ms =
        chrono::duration_cast<chrono::milliseconds>(ts_duration_ns);
    uint64_t ms_count = ts_duration_ms.count();
    cout << "ms_count: " << ms_count << endl;
}
