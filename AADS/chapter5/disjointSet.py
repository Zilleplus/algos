from typing import List, Optional


class Node:
    parent: Optional[Node]
    rank: int

    def __init__(self):
        self.parent = None
        self.rank = 1  # only updated for root nodes


class DisjointSet:
    nodes_: dict[str, Node]
    last_id_: int

    def __init__(self):
        self.nodes_ = {}
        self.last_id_ = 0

    def AddElement(self, element: str):
        self.nodes_[element] = Node()

    def FindPartition(self, n: Node):
        if n.parent is None:
            return n

        root = self.FindPartition(n.parent)
        # while we backtrack our traces, set
        # this root to the parent root, shortcutting
        # for the next call
        if n.parent != root:
            n.parent = root

        return root

    def Merge(self, element_x: str, element_y: str):
        x = self.nodes_[element_x]
        y = self.nodes_[element_y]

        p_x = self.FindPartition(x)
        p_y = self.FindPartition(y)

        new_rank = p_x.rank + p_y.rank
        if p_x.rank < p_y.rank:
            p_y.parent = p_x
            p_x.rank = new_rank
        else:
            p_x.parent = p_y
            p_y.rank = new_rank

    def AreDisjoint(self, element_x: str, element_y: str):
        x = self.nodes_[element_x]
        y = self.nodes_[element_y]

        p_x = self.FindPartition(x)
        p_y = self.FindPartition(y)

        return not p_x == p_y


d = DisjointSet()
d.AddElement("a")
d.AddElement("b")
d.AddElement("c")

d.Merge("a", "b")
print(d.AreDisjoint("a", "b"))
