#include <iostream>
#include <vector>
#include <algorithm>
//#include "sort.hpp"
using namespace std;

struct Point
{
    int x;
    int y;
};

int ccw(const Point &p1, const Point &p2, const Point &p3)
{
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

int Dist(const Point &p1, const Point &p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

struct compare
{
    Point p0;
    bool operator()(Point &p1, Point &p2)
    {
        return ccw(p0, p1, p2) > 0 || (ccw(p0, p1, p2) == 0 && Dist(p0, p2) >= Dist(p0, p1));
    }
};

vector<Point> grahamScan(vector<Point> &points)
{
    vector<Point> s;
    Point minp = points[0];
    int mini = 0;
    for (int i = 1; i < (int)points.size(); ++i)
    {
        if (points[i].y < minp.y)
        {
            minp.y = points[i].y;
            minp.x = points[i].x;
            mini = i;
        }
        else if (points[i].y == minp.y)
        {
            if (points[i].x < minp.x)
            {
                minp.y = points[i].y;
                minp.x = points[i].x;
                mini = i;
            }
        }
    }
    compare comp;
    comp.p0 = minp;
    swap(points[0], points[mini]);
    sort(points.begin() + 1, points.end(), comp);
    for (auto it = points.begin() + 1; it != points.end() - 1 && it != points.end(); ++it)
    {
        if (ccw(minp, *it, *(it + 1)) == 0)
        {
            points.erase(it--);
        }
    }
    for (auto it = points.begin() + 1; it != points.end(); ++it)
    {
        if ((*it).x == minp.x && (*it).y == minp.y)
        {
            points.erase(it--);
        }
    }
    for (auto &x : points)
    {
        while (s.size() > 1 && ccw(s[s.size() - 2], s.back(), x) <= 0)
        {
            s.pop_back();
        }
        s.push_back(x);
    }
    return s;
}

int main()
{
    int n;
    cin >> n;
    if (n == 0)
        return 0;
    vector<Point> points(n);
    int i, j;
    for (int cnt = 0; cnt < n; ++cnt)
    {
        cin >> i;
        cin >> j;
        points[cnt] = {i, j};
    }
    auto ans = grahamScan(points);
    for (auto &p : ans)
    {
        cout << p.x << ' ' << p.y << "\n";
    }
    return 0;
}