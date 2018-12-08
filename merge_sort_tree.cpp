#include <Python.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#endif

#ifndef Py_TYPE
    #define Py_TYPE(ob) (((PyObject*)(ob))->ob_type)
#endif

#ifdef IS_PY3K
    #define PyInt_FromLong PyLong_FromLong 
#endif

class mergeSortTree
{
  private:
    vector< vector<int> >tree;
    vector<int> A;
    void build_tree( int cur , int l , int r )
    {
        if (l==r) 
        {
          tree[cur].push_back(A[l]); 
          return ;
        }
        int mid = l+(r-l)/2;
        build_tree(2*cur+1, l, mid); // Build left tree 
        build_tree(2*cur+2, mid+1, r); // Build right tree
        vector<int> arr1 = tree[2*cur+1];
        vector<int> arr2 = tree[2*cur+2];
        vector<int> arr3(arr1.size()+ arr2.size());
        merge(arr1.begin(), arr1.end(), arr2.begin(), arr2.end(), arr3.begin());
        tree[cur] = arr3;
    }
    int query( int cur, int l, int r, int x, int y, int k)
    {
        if( r < x || l > y )
          return 0;
        if( x<=l && r<=y )
        //Binary search over the current sorted vector to find elements smaller than K
          return upper_bound(tree[cur].begin(), tree[cur].end(), k) - tree[cur].begin();
        int mid=l+(r-l)/2;
        return query(2*cur+1, l, mid, x, y, k)+query(2*cur+2, mid+1, r, x, y, k);
    }
  public:
    int range_query(int x, int y, int k)
    {
      return query(0, 0, A.size()-1, x, y, k);
    }
    int get_size()
    {
      return A.size();
    }
    void build(vector<int> arr)
    {
//       cout << "The tree is being made "<< "\n";
      A=arr;
      tree.clear();
      tree.resize(A.size()*4);
      build_tree( 0 , 0, A.size()-1);
    }
    mergeSortTree()
    {
//       cout << "The object is going to be constructed "<< "\n";
    }
    ~mergeSortTree() {
//       std::cout << "The object is going to be deallocated.\n";
    }
};

typedef struct 
{
    PyObject_HEAD
    mergeSortTree* ptr;
} py_merge_sort_tree;

bool check_constraints(py_merge_sort_tree *self, int32_t item) {
  if (item < 0) {
    PyErr_SetString(PyExc_IndexError, "Item index can not be negative");
    return false;
  } else if (item >= self->ptr->get_size()) {
    PyErr_SetString(PyExc_IndexError, "Item index larger than the largest item index");
    return false;
  } else {
    return true;
  }
}

static void
py_mst_dealloc(py_merge_sort_tree* self)
{
    delete self->ptr;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

bool
convert_list_to_vector(PyObject* v, int f, vector<int>* w)
{
  if (PyObject_Size(v) != f) {
    PyErr_SetString(PyExc_IndexError, "Vector has wrong length");
    return false;
  }
  for (int z = 0; z < f; z++) {
    PyObject *key = PyInt_FromLong(z);
    PyObject *pf = PyObject_GetItem(v, key);
    (*w)[z] = PyFloat_AsDouble(pf);
    Py_DECREF(key);
    Py_DECREF(pf);
  }
  return true;
}

static PyObject *
py_mst_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
  py_merge_sort_tree *self = (py_merge_sort_tree *)type->tp_alloc(type, 0);

  if (self == NULL) {
      return NULL;
  }
  self->ptr = new mergeSortTree();
  return (PyObject *)self;
}

static int
py_mst_init(py_merge_sort_tree *self, PyObject *args, PyObject *kwds)
{
    return 0;
}

static PyObject* 
py_mst_build(py_merge_sort_tree *self, PyObject *args, PyObject* kwargs) 
{
  PyObject* v;
  if (!self->ptr) 
    return NULL;
  static char const * kwlist[] = {"vector", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", (char**)kwlist, &v))
    return NULL;

  vector<int> w(PyObject_Size(v));
  if (!convert_list_to_vector(v, PyObject_Size(v) , &w)) {
    return NULL;
  }
  self->ptr->build(w);

  Py_RETURN_NONE;
}

static PyObject* 
py_mst_range_query(py_merge_sort_tree *self, PyObject *args, PyObject* kwargs) 
{
  int x;
  int y;
  int k;

  if (!self->ptr) 
    return NULL;
  
  static char const * kwlist[] = {"x","y","k", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "iii", (char**)kwlist, &x, &y, &k))
    return NULL;

  if (!check_constraints(self, x) && !check_constraints(self, y)) {
    return NULL;
  }
  if (x>y) {
    PyErr_SetString(PyExc_ValueError, "y must be larger or equal to x");
    return NULL;
  }

  int n = self->ptr->range_query(x,y,k);

  return PyInt_FromLong(n);
}

static PyMethodDef merge_sort_methods[] = {
  {"build", (PyCFunction)py_mst_build, METH_VARARGS | METH_KEYWORDS, "Initialize merge sort tree from list of integers"},
  {"range_query", (PyCFunction)py_mst_range_query, METH_VARARGS | METH_KEYWORDS, "range query the merge sort tree"},
  {NULL, NULL, 0, NULL}      /* Sentinel */
};

static PyTypeObject py_merge_sort_treeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "mergeSortTree.MergeSortTree",             /* tp_name */
  sizeof(py_merge_sort_tree), /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)py_mst_dealloc,/* tp_dealloc */
  0,                         /* tp_print */
  0,                         /* tp_getattr */
  0,                         /* tp_setattr */
  0,                         /* tp_compare */
  0,                         /* tp_repr */
  0,                         /* tp_as_number */
  0,                         /* tp_as_sequence */
  0,                         /* tp_as_mapping */
  0,                         /* tp_hash */
  0,                         /* tp_call */
  0,                         /* tp_str */
  0,                         /* tp_getattro */
  0,                         /* tp_setattro */
  0,                         /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,        /* tp_flags */
  "merge sort tree objects",           /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  merge_sort_methods,        /* tp_methods */
  0,                         /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)py_mst_init,      /* tp_init */
  0,                         /* tp_alloc */
  py_mst_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
  {NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3
static PyModuleDef mst2module = {
  PyModuleDef_HEAD_INIT,
  "merge_sort_tree",
  "Example module that creates an extension type.",
  -1,
  module_methods, 
  NULL, 
  NULL, 
  NULL, 
  NULL,
};
#endif

PyObject *create_module(void) 
{
  PyObject *m;

  if (PyType_Ready(&py_merge_sort_treeType) < 0)
    return NULL;

  #if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&mst2module);
  #else
    m = Py_InitModule("MergeSortTree", module_methods);
  #endif

  if (m == NULL)
    return NULL;

  Py_INCREF(&py_merge_sort_treeType);
  PyModule_AddObject(m, "MergeSortTree", (PyObject *)&py_merge_sort_treeType);
  return m;
}

#if PY_MAJOR_VERSION >= 3
  PyMODINIT_FUNC PyInit_mergeSortTree(void) {
    return create_module();      // it should return moudule object in py3
  }
#else
  PyMODINIT_FUNC initmergeSortTree(void) {
    create_module();
  }
#endif
