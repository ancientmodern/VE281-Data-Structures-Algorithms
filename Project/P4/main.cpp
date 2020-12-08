#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>

constexpr size_t INF = 1000000000;

class Graph
{
public:
    typedef std::pair<size_t, size_t> valNode;

private:
    struct Edge
    {
        size_t start = 0, end = 0, weight = 0;

        Edge(size_t _start, size_t _end, size_t _weight) : start(_start), end(_end), weight(_weight)
        {
        }
    };

    class Node
    {
    private:
        size_t index = 0;
        std::vector<Edge *> pre;
        std::vector<Edge *> next;

    public:
        friend class Graph;

        Node(size_t _index) : index(_index)
        {
        }

        ~Node()
        {
            // delete pre will cause double free
            for (auto &e : next)
            {
                delete e;
            }
        }
    };

    size_t numNode = 0, numEdge = 0, sourceIndex = 0, destIndex = 0;
    std::vector<Node *> nodes;
    std::vector<Edge *> edges;
    std::vector<size_t> parent;

    size_t dijkstra()
    {
        std::priority_queue<valNode, std::vector<valNode>, std::greater<valNode>> pq;
        std::vector<size_t> dist(numNode, INF);

        pq.emplace(std::make_pair(0, sourceIndex));
        dist[sourceIndex] = 0;

        while (!pq.empty())
        {
            size_t u = pq.top().second;
            pq.pop();

            for (auto &e : nodes[u]->next)
            {
                size_t weight = e->weight;
                size_t v = e->end;

                if (dist[v] > dist[u] + weight)
                {
                    dist[v] = dist[u] + weight;
                    pq.emplace(std::make_pair(dist[v], v));
                }
            }
        }

        return dist[destIndex];
    }

    bool topoSort()
    {
        std::vector<size_t> inDegree(numNode), order(numNode);
        std::vector<bool> visited(numNode, false);
        std::queue<Node *> q;
        size_t count = 0;

        for (size_t i = 0; i < numNode; ++i)
        {
            inDegree[i] = nodes[i]->pre.size();
            if (inDegree[i] == 0)
            {
                q.emplace(nodes[i]);
                visited[i] = true;
            }
        }

        while (!q.empty())
        {
            Node *temp = q.front();
            q.pop();
            order[temp->index] = count++;
            for (auto &e : temp->next)
            {
                size_t i = e->end;
                if (--inDegree[i] == 0 && !visited[i])
                {
                    q.emplace(nodes[i]);
                    visited[i] = true;
                }
            }
        }

        for (auto &e : edges)
        {
            if (order[e->start] >= order[e->end])
            {
                return false;
            }
        }

        return true;
    }

    size_t prim()
    {
        std::priority_queue<valNode, std::vector<valNode>, std::greater<valNode>> pq;
        std::vector<bool> inMST(numNode, false);
        std::vector<size_t> cost(numNode, INF);
        size_t sum = 0, cnt = 0;

        pq.emplace(std::make_pair(0, sourceIndex));
        cost[sourceIndex] = 0;

        while (!pq.empty())
        {
            auto [num, u] = pq.top();
            pq.pop();

            if (!inMST[u])
            {
                sum += num;
                ++cnt;
                inMST[u] = true;

                for (auto &e : nodes[u]->pre)
                {
                    size_t weight = e->weight;
                    size_t v = e->start;

                    if (!inMST[v] && cost[v] > weight)
                    {
                        cost[v] = weight;
                        pq.emplace(std::make_pair(cost[v], v));
                    }
                }

                for (auto &e : nodes[u]->next)
                {
                    size_t weight = e->weight;
                    size_t v = e->end;

                    if (!inMST[v] && cost[v] > weight)
                    {
                        cost[v] = weight;
                        pq.emplace(std::make_pair(cost[v], v));
                    }
                }
            }
        }
        // std::cout << sum << ' ' << cnt << std::endl;
        return (cnt == numNode) ? sum : INF;
    }

    size_t kruskal()
    {
        size_t sum = 0, cnt = 0;

        std::sort(edges.begin(), edges.end(), [](const Edge *lhs, const Edge *rhs) {
            return lhs->weight < rhs->weight;
        });

        parent.resize(numNode);
        for (size_t i = 0; i < numNode; ++i)
        {
            parent[i] = i;
        }

        for (auto &e : edges)
        {
            size_t uRep = findSet(e->start);
            size_t vRep = findSet(e->end);

            if (uRep != vRep)
            {
                sum += e->weight;
                ++cnt;
                parent[uRep] = parent[vRep];
            }
        }
        // std::cout << sum << ' ' << cnt << std::endl;
        return (cnt == numNode - 1) ? sum : INF;
    }

    inline size_t findSet(size_t i)
    {
        return (i == parent[i]) ? i : findSet(parent[i]);
    }

public:
    Graph()
    {
        std::cin >> numNode >> sourceIndex >> destIndex;

        nodes.resize(numNode);
        for (size_t i = 0; i < numNode; ++i)
        {
            nodes[i] = new Node{i};
        }

        size_t startNode, endNode, weight;
        for (; std::cin >> startNode >> endNode >> weight; ++numEdge)
        {
            Edge *e = new Edge{startNode, endNode, weight};
            nodes[startNode]->next.emplace_back(e);
            nodes[endNode]->pre.emplace_back(e);
            edges.emplace_back(e);
        }
    }

    ~Graph()
    {
        for (auto &node : nodes)
        {
            delete node;
        }
    }

    void P4()
    {
        shortestPath();
        isDAG();
        MST();
    }

    void shortestPath()
    {
        size_t pathLen = dijkstra();
        if (pathLen == INF)
        {
            std::cout << "No path exists!" << std::endl;
        }
        else
        {
            std::cout << "Shortest path length is " << pathLen << std::endl;
        }
    }

    void isDAG()
    {
        if (topoSort())
        {
            std::cout << "The graph is a DAG" << std::endl;
        }
        else
        {
            std::cout << "The graph is not a DAG" << std::endl;
        }
    }

    void MST()
    {
        size_t totWeight;
        double _numNode = numNode, _numEdge = numEdge;
        totWeight = (_numEdge > _numNode * std::log(_numNode)) ? prim() : kruskal();

        if (totWeight == INF)
        {
            std::cout << "No MST exists!" << std::endl;
        }
        else
        {
            std::cout << "The total weight of MST is " << totWeight << std::endl;
        }
    }
};

int main()
{
    Graph graph{};
    graph.P4();
}