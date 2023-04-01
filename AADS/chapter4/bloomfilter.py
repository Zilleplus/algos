import math as m
import hashlib as hl
from collections.abc import Iterator
from typing import List


class Bloomfilter:
    false_positives_: float
    number_of_elements_ : int
    number_of_hashes_: int
    number_of_bits_: int

    def __init__(self, number_of_elements: int = 100, false_positives: float = 0.1):
        self.false_positives_ = false_positives
        self.number_of_elements_ = number_of_elements

        # From AADS page 143:
        p = false_positives
        n = number_of_elements

        self.number_of_hashes_ = m.ceil(-m.log(p) / m.log(2))
        self.number_of_bits_ = m.ceil(-n * (m.log(p) / (m.log(2) ** 2)))
        self.Reset()

    def h1(self, x):
        return int(hl.sha3_512(x).hexdigest(), 16)

    def h2(self, x):
        return int(hl.blake2b(x).hexdigest(), 16)

    def Hashes(self, element) -> List[int]:
        """
        Hash the element self.number_of_hashes_ times.

        The has combines sha3_512 with blake2b and additions/mulitplications
        to create self.number_of_hashes_ unique hashes.
        """

        def h(x, i):
            return (self.h1(x) + i * self.h2(x) + i * i) % self.number_of_bits_

        return [h(bytes(element), i) for i in range(0, self.number_of_hashes_)]

    def Insert(self, element):
        """
        Insert an element in the bloomfilter, be sure not to add to many elements
        or the false positive rate will be higher.
        """
        hs = self.Hashes(element)
        for i in hs:
            self.bits_[i] = True

    def Contains(self, element: str) -> bool:
        """
        Checks if the element is in the bloomfilter, this method can return
        false positives but no false negativates.
        """
        hs = self.Hashes(element)
        return all([i for i in hs if self.bits_[i]])

    def Reset(self):
        """
        Remove all elements from the bloom filter.
        """
        self.bits_ = list([False for x in range(0, self.number_of_bits_)])

    def FalsePositives(self) -> float:
        """
        Returns the worst case false_positives_, the size of the
        hash is adjusted to this. The lower the odds on a false false positives,
        the more bits are used.
        """
        return self.false_positives_


b = Bloomfilter()

b.Insert(b"test")
