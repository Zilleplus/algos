class Node:
    def __init__(self):
        self.children = {}
        self.KeyNode = False


class Trie:
    def __init__(self):
        self.root = Node()

    @staticmethod
    def addNewBranch(node: Node, key):
        for c in key:
            new_node = Node()
            node.children[c] = new_node
            node = new_node
        node.KeyNode = True

    def insert(self, key: str):
        current_node = self.root
        for i, c in enumerate(key):
            if c in current_node.children:
                # if a node already exists, continue down
                current_node = current_node.children[c]
            else:
                # If the node does not exist, create a new
                # branch.
                Trie.addNewBranch(current_node, key[i:])
                return True

        return False  # key already exists in trie

    def searchNode(self, key):
        node = self.root
        for c in key:
            if c in node.children:
                node = node.children[c]
            else:
                return None

        return node

    def contains(self, key):
        n = self.searchNode(key)
        if n is None:
            return None
        return n.KeyNode

    def _allKeys(self, node, prefix=""):
        for c, child in node.children.items():
            if child.KeyNode:
                yield prefix + c
            for k in self._allKeys(child, prefix=prefix + c):
                yield k

    def keyStartingWith(self, prefix):
        n = self.searchNode(prefix)
        if n is None:
            print("nothing to find with prefix " + prefix)
            return
        for k in self._allKeys(n, prefix):
            yield k

    # find the longest match of key
    def longestPrefix(self, key):
        node = self.root
        longestPrefix = None
        prefix = ""
        for c in key:
            if c in node.children:
                prefix = prefix + c
                if node.KeyNode:
                    longestPrefix = prefix
                node = node.children[c]
            else:
                break

        return longestPrefix

    def removeWithoutPruning(self, key):
        node = self.searchNode(key)
        if node is None:
            return False

        # If the keynode was already false
        # return false.
        returnVal = node.KeyNode
        node.KeyNode = False
        return returnVal

    def remove(self, key):
        # The books does this with recursion,
        # but I prefer flatten it out.
        # As python has no tail-recursive optimization
        if key == "":
            return False
        visited_nodes = []
        visited_nodes.append(('', self.root))
        node = self.root
        for c in key:
            if c in node.children:
                node = node.children[c]
                visited_nodes.append((c, node))
            else:
                print("can't find")
                return False  # Can't find it.

        previous_char, previous_node = visited_nodes.pop()
        previous_node.KeyNode = False
        while(len(visited_nodes) > 0):
            current_char, current_node = visited_nodes.pop()
            if previous_node.KeyNode:
                break
            current_node.children.pop(previous_char)
            previous_char = current_char
            previous_node = current_node

        return True


t = Trie()
t.insert("test")
t.insert("testing")
t.remove("testing")
