from golwrapper import init
from golwrapper import dest
from golwrapper import gen
import random
import pygame
from collections import namedtuple
import sys
import time
from collections import defaultdict
from copy import deepcopy

Dim = namedtuple("Dimension", ["width", "height"])
Grid = namedtuple("Grid", ["dim", "cells"])
Neighbours = namedtuple("Neighbours", ["alive", "dead"])

gosper_glider = Grid(Dim(50, 50), {(22, 8), (12, 7), (36, 7), (17, 9), (11, 8), (1, 9), (25, 4), (2, 8), (16, 7),
                                   (25, 10), (21, 6), (23, 9), (14, 6), (36, 6), (22, 7), (14, 12), (17, 8), (11, 10),
                                   (25, 9), (35, 7), (1, 8), (18, 9), (22, 6), (21, 8), (23, 5), (12, 11), (17, 10),
                                   (11, 9), (35, 6), (25, 5), (2, 9), (13, 6), (13, 12), (15, 9), (16, 11), (21, 7)})

  
# Number of pairs
N = 10
  
# range of pairs
R = 5
  
# N Random Tuples list
# Using list comprehension + sample()

listOfTuples = [divmod(ele, R + 1) for ele in random.sample(range((R + 1) * (R + 1)), N)]
  
res = init(listOfTuples)
print("Initial state: " + str(res))
for x in range(0, 2):
    print(gen())




