#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <stack>

struct UG {
    using vertex_t = int;
    using edge_t = int;
    using weight_t = int;

    std::map<edge_t, weight_t> edge_weights;
    std::set<vertex_t> vertices;

    // Assume an edge alway's has it's lowest edge_id first.
    std::map<vertex_t, std::map<vertex_t, edge_t>> edges;

    edge_t next_edge_id = 0;
};

UG::weight_t Weight(const UG& g, UG::edge_t e)
{
    auto w = g.edge_weights.find(e);
    return w == g.edge_weights.end() ? UG::weight_t { 0 } : w->second;
}

void Print(const UG& g)
{
    std::cout << "edges:" << std::endl;
    for (const auto& e : g.edges) {
        std::cout << e.first << ":[ ";
        for (const auto& v : e.second) {
            std::cout << v.first << "{" << Weight(g, v.first) << "} ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << "vertices:" << std::endl;
    std::cout << "[ ";
    for (const auto& v : g.vertices) {
        std::cout << v << " ";
    }
    std::cout << "]" << std::endl;
}

void SetWeight(UG& g, UG::edge_t e, UG::weight_t w)
{
    auto edi = g.edge_weights.find(e);
    if (edi == g.edge_weights.end()) {
        g.edge_weights.insert({ e, w });
    } else {
        g.edge_weights[e] = w;
    }
}

void AddVertex(UG& g, UG::vertex_t v)
{
    g.vertices.insert(v);
}

std::optional<std::pair<UG::vertex_t, UG::edge_t>> FindEdge(
    const UG& g,
    UG::vertex_t v1,
    UG::vertex_t v2)
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

    return { *e };
}

// Creates edge, the bool indicates if the edge was actually created.
// If true: edge was created
// If false: edge already existed
std::pair<UG::edge_t, bool> AddEdge(UG& g, UG::vertex_t v1, UG::vertex_t v2)
{
    auto existing_edge = FindEdge(g, v1, v2);

    if (existing_edge.has_value()) {
        return std::make_pair(existing_edge->second, false);
    }

    if (v2 < v1) // v1 must be smaller then v2
    {
        std::swap(v1, v2);
    }

    auto id = g.next_edge_id;
    g.next_edge_id++;

    auto c = g.edges.find(v1);
    if (c == g.edges.end()) {
        g.edges.insert({ v1, {} });
        c = g.edges.find(v1);
    }

    c->second.insert({ v2, id });
    return { v2, true };
}

std::vector<std::pair<UG::vertex_t, UG::edge_t>>
AdjacentEdges(const UG& g, UG::vertex_t v)
{
    std::vector<std::pair<UG::vertex_t, UG::edge_t>> out;

    // Check if any node with a lower vertex id has an
    // edge with this vertex, if so add it too out.
    for (const auto& e : g.edges) {
        auto v_connection = e.second.find(v); // Is e.first connected to v?
        if (v_connection != e.second.end()) { // If so, push it on out.
            out.push_back(std::make_pair(e.first, v_connection->second));
        }
        if (e.first >= v) {
            break;
        }
    }

    // If the vertex itself has a list of connections, we
    // obviously need to pass them along...
    auto node = g.edges.find(v);
    if (node != g.edges.end()) {
        for (const auto& e : node->second) {
            out.push_back(e);
        }
    }

    return out;
}

template <typename TBFVisitor>
void BreathFirst(
    UG& g,
    const UG::vertex_t start_vertex,
    TBFVisitor&& vis)
{
    std::queue<UG::vertex_t> to_visit;
    std::set<UG::vertex_t> visited_nodes;

    to_visit.push(start_vertex);
    for(auto [v, e] : AdjacentEdges(g, start_vertex)) {to_visit.push(v);}

    while(!to_visit.empty())
    {
        auto node = to_visit.front();
        bool already_visited  = visited_nodes.find(node) != visited_nodes.end();
        if(!already_visited)
        {
            vis.Visit(g, node);
            for(auto [v, e] : AdjacentEdges(g, node)) {to_visit.push(v);}

            // we could use upperbound on visited_nodes, and use the hint here.
            visited_nodes.insert(node);
        }

        to_visit.pop();
    }
}

template <typename TBFVisitor>
void DepthFirst(
    UG& g,
    const UG::vertex_t start_vertex, TBFVisitor&& vis)
{
    std::stack<UG::vertex_t> to_visit;
    std::set<UG::vertex_t> visited_nodes;

    for(auto [v, e] : AdjacentEdges(g, start_vertex)) {to_visit.push(v);}
    to_visit.push(start_vertex);

    while(!to_visit.empty())
    {
        auto node = to_visit.top();
        to_visit.pop();
        bool already_visited  = visited_nodes.find(node) != visited_nodes.end();
        if(!already_visited)
        {
            vis.Visit(g, node);
            for(auto [v, e] : AdjacentEdges(g, node)) {to_visit.push(v);}

            // we could use upperbound on visited_nodes, and use the hint here.
            visited_nodes.insert(node);
        }
    }
}

struct PrintVisitor{
    void Visit(UG& g, UG::vertex_t v)
    {
        std::cout << v << " ";
    }
};


int main()
{
    UG g;
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

    auto [e1, e1_was_created]  = AddEdge(g, 0, 1);
    SetWeight(g, e1, 1);
    auto [e2, e2_was_created]  = AddEdge(g, 0, 4);
    SetWeight(g, e2, 1);

    auto [e3, e3_was_created]  = AddEdge(g, 1, 2);
    SetWeight(g, e3, 1);
    auto [e4, e4_was_created]  = AddEdge(g, 1, 4);
    SetWeight(g, e4, 1);

    auto [e5, e5_was_created]  = AddEdge(g, 2, 3);
    SetWeight(g, e5, 1);
    auto [e6, e6_was_created]  = AddEdge(g, 2, 5);
    SetWeight(g, e6, 1);

    auto [e7, e7_was_created]  = AddEdge(g, 4, 5);
    SetWeight(g, e7, 1);
    auto [e8, e8_was_created]  = AddEdge(g, 4, 6);
    SetWeight(g, e8, 1);

    Print(g);
    std::cout << "breath first: ";
    BreathFirst(g, UG::vertex_t{0}, PrintVisitor());

    std::cout << std::endl;

    std::cout << "depth first: ";
    DepthFirst(g, UG::vertex_t{0}, PrintVisitor());

    return 0;
}
