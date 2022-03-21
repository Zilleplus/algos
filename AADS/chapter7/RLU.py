from collections import defaultdict


class Node:
    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.next = None
        self.previous = None

    def to_string(self):
        return "[key=" + str(self.key) + ";value=" + str(self.value) + "]"


class LinkedNodes:
    def __init__(self):
        self._head = None
        self._back = None

    def unlink(self, node):
        if node is self._head:
            assert node.previous is None
            self._head = node.next
        else:
            # not the head so link previous
            node.previous.next = node.next
        if node is self._back:
            assert node.next is None
            self._back = node.previous
        else:
            # not the back so linke next
            node.next.previous = node.previous

    def push_front(self, node):
        if self._head is None:
            self._head = node
            self._back = node
        else:
            self._head.previous = node
            node.next = self._head

            self._head = node

    def pop_back(self):
        n = self._back
        if n is None:
            return None

        assert n.next is None
        if n.previous is None:
            # last element on the list, set head to none
            self._head = None
            self._back = None
        else:
            n.previous.next = None
            self._back = n.previous

        # clear out the node itself
        n.previous = None
        n.next = None
        return n

    def back(self):
        return self._back

    def to_list(self):
        it = self._head

        lst = []
        while True:
            if it is None:
                break
            lst.append(it)
            it = it.next

        return lst

    def str(self) -> str:
        output: str = "LinkedList={"
        for el in self.to_list():
            output += el.to_string()
        output += "}"
        return output

    def print(self):
        print(self.str())

# l = LinkedNodes()
# n1 = Node(1, "first")
# n2 = Node(2, "second")
# n3 = Node(3, "third")
# l.push_front(n1)
# l.push_front(n2)
# l.push_front(n3)
# l.print()


class LRU:
    def __init__(self, maxNumberOfElements):
        self._maxNumberOfElements = maxNumberOfElements
        self._map = defaultdict(lambda: None)
        self._elements = LinkedNodes()

    def get(self, key):
        n = self._map[key]
        if n is not None:
            self._elements.unlink(n)
            # Add accessed element to the front.
            self._elements.push_front(n)

        return n

    def contains(self, key):
        return key in self._map

    def set(self, key, value):
        if not self.contains(key) and \
                self.activate_size() + 1 > self._maxNumberOfElements:
            n = self._elements.pop_back()
            self._map.pop(n.key)

        n = Node(key, value)
        self._map[key] = n
        self._elements.push_front(n)

    def size(self):
        return self._maxNumberOfElements

    def activate_size(self):
        return len(self._map)

    def oldest_entry(self):
        return self._elements.back()

    def print(self):
        print("RLU:")
        print(self._elements)
        print(self._map)


c = LRU(maxNumberOfElements=2)
c.set(1, "first")
c.set(2, "second")
c.set(3, "tirth")
c.print()

c.get(1)  # should not be here
