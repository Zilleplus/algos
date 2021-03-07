#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <stack>

struct UndirectedGraph {
    using vertex_t = int;

    std::unordered_set<int> vertices;

    // The first vertex is alway's the lowest vertex...
    std::map<vertex_t, std::unordered_set<vertex_t>> edges;
};

void Print(UndirectedGraph& g)
{
    std::cout << "vertices: [ ";
    for (auto v : g.vertices) {
        std::cout << v << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "edges: [";
    for (auto e : g.edges) {
        std::cout << std::endl;
        std::cout << e.first << " ";
        std::cout << "{ ";
        for (auto se : e.second) {
            std::cout << se << " ";
        }
        std::cout << "}";
    }
    std::cout << std::endl
              << "]";
}

void AddVertex(UndirectedGraph& g, UndirectedGraph::vertex_t vertex)
{
    g.vertices.insert(vertex);
}

std::optional<std::pair<int, int>> FindEdge(
    UndirectedGraph& g,
    UndirectedGraph::vertex_t v1,
    UndirectedGraph::vertex_t v2)
{
    if (v2 < v1) {
        std::swap(v1, v2);
    }

    auto node = g.edges.find(v1);

    if (node == g.edges.end()) {
        return {};
    }
    auto e = node->second.find(v2);
    if (e == node->second.end()) {
        return {};
    }

    return { std::make_pair(node->first, *e) };
}

// return edge, with bool indicating if the edge was newly added.
std::pair<std::pair<int, int>, bool> AddEdge(
    UndirectedGraph& g,
    UndirectedGraph::vertex_t v1,
    UndirectedGraph::vertex_t v2)
{
    if (v2 < v1) {
        std::swap(v1, v2);
    }

    auto current_edge = FindEdge(g, v1, v2);
    if (current_edge.has_value()) {
        return std::make_pair(*current_edge, false);
    }

    auto node = g.edges.lower_bound(v1);
    if (node != g.edges.end()
        && node->first == v1) {
        node->second.insert(v2);
    } else {
        // put hint on node
        g.edges.insert(
            node,
            std::make_pair(v1, std::unordered_set<int> { v2 }));
    }

    return std::make_pair(std::make_pair(v1, v2), true);
}

std::vector<int> AdjacentEdges(
    const UndirectedGraph& g,
    int v)
{
    // Find all vertices that have a connection to this vertice.
    std::vector<int> vs;
    for (const auto& e : g.edges) {
        if (e.second.find(v) != e.second.end()) {
            vs.push_back(e.first);
        }
    }

    // If the vertex itself has a list of connections, we
    // obviously need to pass them along...
    auto node = g.edges.find(v);
    if (node != g.edges.end()) {
        for (const auto& e : node->second) {
            vs.push_back(e);
        }
    }

    return vs;
}

template <typename TBFVisitor>
void BreathFirst(
    const UndirectedGraph& g,
    const int start_node, TBFVisitor&& vis)
{
    std::queue<int> to_visit;
    std::set<int> visited_nodes;

    for(auto n : AdjacentEdges(g, start_node)) {to_visit.push(n);}

    while(!to_visit.empty())
    {
        auto node = to_visit.front();
        bool already_visited  = visited_nodes.find(node) != visited_nodes.end();
        if(!already_visited)
        {
            vis.Visit(g, node);
            for(auto n : AdjacentEdges(g, node)) {to_visit.push(n);}

            // we could use upperbound on visited_nodes, and use the hint here.
            visited_nodes.insert(node);
        }

        to_visit.pop();
    }
}

template <typename TBFVisitor>
void DepthFirst(
    const UndirectedGraph& g,
    const int start_node, TBFVisitor&& vis)
{
    std::stack<int> to_visit;
    std::set<int> visited_nodes;

    for(auto n : AdjacentEdges(g, start_node)) {to_visit.push(n);}

    while(!to_visit.empty())
    {
        auto node = to_visit.top();
        to_visit.pop();
        bool already_visited  = visited_nodes.find(node) != visited_nodes.end();
        if(!already_visited)
        {
            vis.Visit(g, node);
            for(auto n : AdjacentEdges(g, node)) {to_visit.push(n);}

            // we could use upperbound on visited_nodes, and use the hint here.
            visited_nodes.insert(node);
        }
    }
}

class VisitorLargetThen {
    int n;

public:
    VisitorLargetThen(int n)
        : n(n)
    {
    }

    void Visit(const UndirectedGraph& g, int node)
    {
        if (node > n) { // bingo we got one
            std::cout
                << "node: "
                << node
                << std::endl;
        }
    }
};

int main()
{
    UndirectedGraph g;
    /* **** graph **** 
        0-- 1 -- 2 -- 3
        |   |    |
        \-- 4 -- 5
            |    |
            \--- 6
    */

    for (int i = 0; i < 7; ++i) {
        AddVertex(g, i);
    }

    AddEdge(g, 0, 1);
    AddEdge(g, 0, 4);

    AddEdge(g, 1, 2);
    AddEdge(g, 1, 4);

    AddEdge(g, 2, 3);
    AddEdge(g, 2, 5);

    AddEdge(g, 4, 5);
    AddEdge(g, 4, 6);

    Print(g);

    std::cout << std::endl;
    int n = 3;
    std::cout << "searching for nodes larger then 3 with breath first" << std::endl;
    BreathFirst(g, 0, VisitorLargetThen(n));

    std::cout << "searching for nodes larger then 3 with breath first" << std::endl;
    DepthFirst(g, 0, VisitorLargetThen(n));

    // Find out if there is a node with id larger then n.
    //
}
