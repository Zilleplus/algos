class Node:
    def __init__(self, key=None, priority=-1):
        self.priority = priority
        self.key = key
        self.left = None
        self.right = None
        self.parent = None

    def SetLeft(self, left_node):
        self.left = left_node
        if(left_node is not None):
            left_node.parent = self

    def SetRight(self, right_node):
        self.right = right_node
        if(right_node is not None):
            right_node.parent = self

    def IsLeaf(self):
        return self.left is None\
               and self.right is None

    def IsRoot(self):
        return self.parent is None

    def IsLeft(self):
        if(self.parent is None):
            return False

        return self.parent.left == self

    def IsRight(self):
        if(self.parent is None):
            return False

        return self.parent.right == self

    def ToString(self):
        l_str = "*" if self.left is None else self.left.ToString()
        r_str = "*" if self.right is None else self.right.ToString()

        if(self.left is None and self.right is None):
            return "[" + str(self.key) + "]"

        out = "(" + str(self.key) + " "
        out = out + l_str
        out = out + ", " + r_str
        out = out + ")"

        return out


def RotateLeft(treap, x):
    """
    right becomes parent of it's parent
    """
    if(x is None):
        return
    if(x.parent is Node):
        return

    y = x.parent
    p = y.parent

    # If this invariant doesn't hold
    # you can't do a left rotation.
    assert y.right == x

    if p is not None:
        if not y.IsRoot():
            if(y.IsLeft()):
                p.SetLeft(x)
            else:
                p.SetRight(x)
    else:
        treap.root = x
        treap.root.parent = None

    y.SetRight(x.left)
    x.SetLeft(y)


def RotateRight(treap, x):
    """
    left becomes parent of it's parent
    """
    if(x is None):
        print("early exit x is None")
        return
    if(x.parent is None):
        print("early exit x.parent is None")
        return

    y = x.parent
    p = y.parent

    # If this invariant doesn't hold
    # you can't do a right rotation.
    assert y.left == x

    if p is not None:
        if not y.IsRoot():
            if(y.IsLeft()):
                p.SetLeft(x)
            else:
                p.SetRight(x)
    else:
        treap.root = x
        treap.root.parent = None

    y.SetLeft(x.right)
    x.SetRight(y)


class SortedPriorityQueue:
    def __init__(self):
        self.root = None

    def Top(self):
        if(self.root is None):
            return None
        old_top = self.root.key

        # todo: adjust tree using rotate operations

        return old_top

    def Peek(self):
        if(self.root is None):
            return None
        return self.root.key

    def Insert(self, new_element, priority):
        if(self.root is None):
            self.root = Node(key=new_element, priority=priority)
            return

        pos = self.root

        # go down the tree as far as possible
        # smalles is on the left, largest on the right
        while(True):
            if(pos.key > new_element):
                if(pos.left is None):
                    pos.SetLeft(Node(key=new_element, priority=priority))
                    pos = pos.left
                    break
                else:
                    pos = pos.left
            else:
                if(pos.right is None):
                    pos.SetRight(Node(key=new_element, priority=priority))
                    pos = pos.right
                    break
                else:
                    pos = pos.right

        # key invariant is ok, but priority invariant might not be ok
        # rotate it upward till priority is ok
        while((not pos.IsRoot()) and pos.parent.priority > pos.priority):
            if(pos.IsLeft()):
                RotateRight(self, pos)
            else:
                RotateLeft(self, pos)

    def Update(self, to_update_element, new_priority):
        # There is a potentially more efficient
        # way to do this.
        # Maybe we should introduce a bubble-up/down
        # operation here?
        self.Delete(to_update_element)
        self.Insert(to_update_element, new_priority)

    def Delete(self, element):
        """
        Delete the node with key=element,
        returns true if the node was found and deleted
        """
        # The idea is to push the node
        # outward to a leaf, then disconnect
        # the leaf.

        n = self.Search(element)
        if n is None:
            return False
        if n.IsLeaf() and n.IsRoot():
            self.root = None
            return True

        while not n.IsLeaf():
            if n.left is None:
                RotateLeft(self, n.right)
            elif n.right is None:
                RotateRight(self, n.left)
            elif n.left.priority < n.right.priority:
                RotateRight(self, n.left)
            else:
                RotateLeft(self, n.right)

        if n.IsLeft():
            n.parent.left = None
        else:
            n.parent.right = None
        n.parent = None

        return True

    def Search(self, element):
        """
        Find the node in the tree with key=element
        returns None if not found
        """
        pos = self.root
        while pos is not None:
            if pos.key == element:
                return pos

            if pos.key < element:
                pos = pos.right
            else:
                pos = pos.left

        return None

    def Contains(self, element):
        return self.Search(element) is not None

    def Max(self):
        """
        return the maximum key
        """
        pos = self.root
        if pos is None:
            return None
        while pos.right is not None:
            pos = pos.right

        return pos.key

    def Min(self):
        """
        return the minimum key
        """
        ...
        pos = self.root
        if pos is None:
            return None
        while pos.left is not None:
            pos = pos.left

        return pos.key

    def ToString(self):
        if self.root is None:
            return "Empty"
        return self.root.ToString()


q = SortedPriorityQueue()
q.Insert('A', 4)
q.Insert('B', 3)
q.Insert('C', 5)
q.Insert('D', 2)

print(q.ToString())
q.Delete('A')
print(q.ToString())

# python treap.py
