#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_set>
#include <utility>
#include <vector>

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

class Dijkstra {
public:
    using weight_t = int;

private:
    // saves the cost of a certain node (left) to reach
    // the target via an other node (right) at a certain cost.
    std::map<int, std::pair<int, weight_t>> paths;

    UndirectedGraph& g;
    weight_t current_weight = 0;
    int target;

    // source : node
    // s :
    // weight: weight of shortest path to target.
    void push_back_path(int source, int target, int weight_till_final)
    {
        auto p = std::make_pair(target, weight_till_final);
        paths.insert(std::make_pair(source, p));
    }

    // Use visitor pattern ... (makes sence no?)
    void Visit(int vertex)
    {
        auto adj = AdjacentEdges(g, vertex); 
        // Normally we would take the edge with smallest weight, 
        // but here we assume all the weights are equal(and 1), so not 
        // required.
        for (auto v : adj) {
            auto current = paths.find(v);
            if (current != paths.end()) // we already went into that vertex
            {
                const auto weight = current->second.second;
                const auto potential_new_weight = current_weight + 1;
                if (weight > potential_new_weight) {
                    // tell the path's, this guy is cheaper.
                    current->second = std::make_pair(vertex, potential_new_weight);

                    const auto old_weight = current_weight;
                    current_weight = potential_new_weight;
                    Visit(v);
                    current_weight = old_weight;
                }
            } else {
                push_back_path(v, vertex, current_weight+1);
                current_weight = current_weight + 1;
                Visit(v);
                current_weight = current_weight - 1;
            }
        }
    }

public:
    // Assume we alway's have a valid source
    Dijkstra(UndirectedGraph& g, int target)
        : g(g), target(target)
    {
        push_back_path(target, target, 0);
        Visit(target);
    }

    void PrintPaths() const
    {
        for (auto [source, p] : paths) {
            std::cout << "source:[" 
                << source 
                << "] path=>" 
                << p.first 
                << " with cost=" 
                << p.second;
            std::cout << std::endl;
        }
    }

    std::vector<int> FindPath(int source) const
    {
        // run through this backwards to get the path
        std::vector<int> p ={source};
        while(source != target)
        {
            auto b = paths.find(source);
            source = b->second.first;
            p.push_back(source);
        }

        return p;
    }

        
};

void PrintPath(std::vector<int>& p)
{
    for(auto n : p)
    {
        std::cout << n << " ";
    }

    std::cout << std::endl;
}

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

    std::cout << "dijkstra at 0: " << std::endl;
    Dijkstra d = {g, 0};
    // d.PrintPaths();

    std::cout << "routing from node 3:" << std::endl;
    auto p = d.FindPath(3);
    PrintPath(p);
    std::cout << std::endl;
}
