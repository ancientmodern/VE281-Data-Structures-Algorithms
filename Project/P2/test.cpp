#include <exception>
#include <stdexcept>
#include <functional>
#include <vector>
#include <forward_list>
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> a = {1, 2, 3, 4, 5};
    auto it1 = a.begin() + 1, it2 = a.end() - 1;
    cout << *(min(it1, it2)) << endl;
}