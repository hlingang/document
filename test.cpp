#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <set>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

int main() {
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
}