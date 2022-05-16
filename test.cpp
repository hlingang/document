#include <algorithm>
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
    vector<int> vec{1, 2, 3};
}