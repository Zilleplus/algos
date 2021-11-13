def longestPrefix(left: str, right: str):
    endMatch = 0
    for i in range(0, min(len(left), len(right))):
        if(left[i] == right[i]):
            endMatch = endMatch + 1
        else:
            break

    return left[0:endMatch]


class Node:
    def __init__(self, key, keyNode):
        self.keyNode = keyNode
        self.key = key
        # The children dict uses the first char,
        # this makes efficient matching without looping
        # possible.
        self.children = {}

    def matchOnChildren(self, key: str) -> tuple:
        """
        Find a child that matches with this key. The key
        returned shares the prefix returned. But might
        be larger then the prefix.
        If prefix == key -> it was an exact match.
        If prefix != key -> only partial match len(key) > len(prefix)

        returns: tuple(prefix, key)
        """
        if key[0] in self.children:
            k = self.children[key[0]].key
            lp = longestPrefix(k, key)
            if lp is not None:
                return (lp, k)

        return ("", "")


class RadixTrie:
    def __init__(self):
        self.root = Node(key=None, keyNode=False)

    def search(self, key: str):
        currentPosition = 0
        node = self.root
        while currentPosition < len(key):
            remainingKey = key[currentPosition:]
            (prefix, childKey) = node.matchOnChildren(remainingKey)
            if prefix == "" or len(prefix) != len(childKey):
                return None

            if prefix == childKey:
                node = node.children[childKey[0]]
            currentPosition = currentPosition + len(prefix)

        return node

    def remove(self, key: str):
        # same code as search, but keep track of the visited nodes
        currentPosition = 0
        node = self.root
        visited = [node]
        while currentPosition < len(key):
            remainingKey = key[currentPosition:]
            (prefix, childKey) = node.matchOnChildren(remainingKey)
            if prefix == "" or len(prefix) != len(childKey):
                return False

            if prefix == childKey:
                node = node.children[childKey[0]]
            currentPosition = currentPosition + len(prefix)

            visited.append(node)

        # visited contains all the nodes up to the one that needs removal.
        # Pop off the back and remove the nodes while they have no other
        # children, and are not a keyvalue.
        current = visited.pop()
        current.keyNode = False
        while(len(visited) > 0):
            previous = visited.pop()
            if not current.keyNode and len(current.children) == 0:
                assert current.key[0] in previous.children
                del previous.children[current.key[0]]
            current = previous

    def contains(self, key: str):
        n = self.search(key)
        return (n is not None) and (n.keyNode)

    def insert(self, key: str):
        currentPosition = 0
        node = self.root
        while currentPosition < len(key):
            remainingKey = key[currentPosition:]
            (prefix, childKey) = node.matchOnChildren(remainingKey)
            if prefix == "":
                # no match found -> add new edge
                assert remainingKey[0] not in node.children
                node.children[remainingKey[0]] = \
                    Node(key=remainingKey, keyNode=True)
                return
            elif prefix == childKey:
                # either partial match or full match
                node = node.children[childKey[0]]
                if prefix == remainingKey:
                    node.keyNode = True
                    return
            elif prefix != childKey:
                # requires split!
                interNode = Node(key=prefix, keyNode=False)

                oldNode = Node(key=childKey[len(prefix):], keyNode=True)
                interNode.children[oldNode.key[0]] = oldNode

                newNode = Node(key=remainingKey[len(prefix):], keyNode=True)
                if interNode.key != newNode.key:
                    interNode.children[newNode.key[0]] = newNode

                del node.children[childKey[0]]
                node.children[interNode.key[0]] = interNode
            else:
                assert False

            currentPosition = currentPosition + len(prefix)


t = RadixTrie()
t.insert("hello")
t.insert("helloworld")
t.insert("china")
t.insert("chinese")
assert t.contains("china")
t.remove("china")
assert not t.contains("china")
