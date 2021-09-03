import math as m
import hashlib as hl


class Bloomfilter:

    def __init__(self, number_of_elements=100, false_positives=0.1):
        self.false_positives_ = false_positives
        self.number_of_elements_ = number_of_elements

        # From AADS page 143:
        p = false_positives
        n = number_of_elements

        self.number_of_hashes_ = m.ceil(-m.log(p)/m.log(2))
        self.number_of_bits_ = m.ceil(-n * (m.log(p)/(m.log(2)**2)))
        self.Reset()

    def h1(self, x): return int(hl.sha3_512(x).hexdigest(), 16)
    def h2(self, x): return int(hl.blake2b(x).hexdigest(), 16)

    def Hashes(self, element):

        def h(x, i):
            return (self.h1(x) + i*self.h2(x) + i*i) % self.number_of_bits_

        return [h(bytes(element), i) for i in range(0, self.number_of_hashes_)]

    def Insert(self, element):
        hs = self.Hashes(element)
        for i in hs:
            self.bits_[i] = True

    def Contains(self, element):
        hs = self.Hashes(element)
        return all([i for i in hs if self.bits_[i]])

    def Reset(self):
        self.bits_ = list([False for x in range(0, self.number_of_bits_)])

    def FalsePositives(self):
        return self.false_positives


b = Bloomfilter()

b.Insert(b"test")
