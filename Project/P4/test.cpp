#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

constexpr size_t INF = 1000000000;

struct Node
{
    int val = INF;
    int label = 0;

    Node(int _label, int _val) : label(_label), val(_val)
    {
    }
};

int main()
{
    // Node *n1 = new Node(0, 10);
    // Node *n2 = new Node(1, 8);
    // auto cmp = [](const Node *lhs, const Node *rhs) {
    //     return lhs->val > rhs->val;
    // };
    // priority_queue<Node *, vector<Node *>, decltype(cmp)> pq(cmp);
    // pq.push(n1);
    // pq.push(n2);
    // n1->val = 1;
    // cout << pq.top()->label << ' ' << pq.top()->val << endl;
    pair<int, int> a = {1,2};
    auto [i, j] = a;
    cout << i << j << endl;
}