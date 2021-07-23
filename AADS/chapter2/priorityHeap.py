class PQ:
    def __init__(self, elements=[]):
        # elements = (priority, element)
        self.elements = elements

    def Top(self):
        """
        returns element with highest priority
        """
        if(len(self.elements) > 0):
            first_element = self.elements[0][1]
            if(len(self.elements) > 1):
                self.elements[0] = self.elements.pop()
                self.BubbleDown(0)
            else:
                self.elements.pop()

            return first_element

        return -1

    def PushBack(self, new_element, priority):
        self.elements.append((priority, new_element))
        self.BubbleUp(len(self.elements)-1)

    def Update(self, element, new_priority):
        # if the element does not exist this will
        # be rather odd ;-)
        matches = [i for i in range(0, len(self.elements))
                   if self.elements[i][1] == element]

        # We support duplicates, and if the element is missing
        # this will not error... not sure if the latter is
        # a good idea.
        for matching_i in matches:
            old_priority = self.Priority(matching_i)
            self.elements[matching_i] = (new_priority, element)

            if(old_priority < new_priority):
                self.BubbleUp(matching_i)

            if(old_priority > new_priority):
                print("bubbledown on {0}".format(matching_i))
                self.BubbleDown(matching_i)

    @staticmethod
    def ParentIndex(i):
        if(i == 0):
            return -1  # no parent
        else:
            if(i % 2 == 0):
                return int(i/2) - 1
            else:
                return int((i-1)/2)

    def ChildIndices(self, i):
        left = (2 * i + 1)
        right = (2 * i + 2)
        if not (left < len(self.elements)):
            left = -1
        if not (right < len(self.elements)):
            right = -1
        return (left, right)

    def BubbleUp(self, i):
        """
        Keep swapping with position with parent untill
        the parent has higher priority
        """

        while(i != 0 and
              self.Priority(PQ.ParentIndex(i)) < self.Priority(i)):
            self.Swap(i, PQ.ParentIndex(i))
            i = PQ.ParentIndex(i)

    def Priority(self, i):
        return self.elements[i][0]

    def BubbleDown(self, i):
        """
        Keep swapping with position with childs untill
        the childs have both a lower priority
        """
        while(True):
            (left_child_index, right_child_index) = self.ChildIndices(i)

            new_i = i
            if(left_child_index != -1 and
               self.Priority(left_child_index) > self.Priority(i)):
                self.Swap(left_child_index, i)
                new_i = left_child_index
            if(right_child_index != -1 and
               self.Priority(right_child_index) > self.Priority(i)):
                self.Swap(right_child_index, i)
                new_i = right_child_index
            if(new_i == i):  # no swaps happened
                break
            else:
                i = new_i

    def Swap(self, i, j):
        self.elements[i], self.elements[j]\
            = self.elements[j], self.elements[i]


test = PQ()
test.PushBack('A', 0)
test.PushBack('B', 1)
test.PushBack('C', 5)
test.PushBack('D', 3)
test.elements
test.Top()
test.elements
test.Top()
test.elements
test.Top()
test.elements
test.Top()
test.elements

test = PQ([(7, '0'), (4, '1.1'), (6, '1.2'), (4, '1.1'), (3, '2.2'), (1, '2.3'), (5, '2.4'), (2, '3.1')])

test.Update('1.1', 8)

test.elements
