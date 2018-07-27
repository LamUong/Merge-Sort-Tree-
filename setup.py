from distutils.core import setup, Extension
setup(
		name="mergeSortTree", 
		version="1.0",
		ext_modules=[Extension("mergeSortTree", ["merge_sort_tree.cpp"])],
      )