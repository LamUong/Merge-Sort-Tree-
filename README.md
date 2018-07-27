# Merge-Sort-Tree-
Merge Sort Tree in C++/Python

# Introduction
This is a C++ library with Python bindings for Merge Sort Tree.

# Prequisites
This should works for both Python 2 and Python 3

# Install
To install, please clone the repository and run ``python setup.py install``

# Code Example
.. code-block:: python
	from mergeSortTree import MergeSortTree
	a = MergeSortTree()
	a.build([i for i in range(100000)])
	a.range_query(1000,50000,30000)

# API
* ``MergeSortTree()`` return a new merge sort tree.
* ``a.build()`` take in an integer array which you want to build the merge sort tree on.
* ``aa.range_query(x,y,k)`` return the number of elements in range x to y which is smaller or equal to k.

