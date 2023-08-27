#define PY_SSIZE_T_CLEAN
# include <Python.h>
# include "general.h"
# include "symnmf.h"


vector* py_obj_to_verctor(PyObject *head_vec_obj, int d) {
    vector *head_vec;
    int n = PyObject_Length(head_vec_obj);
    if (n < 0) {
        return NULL;
    }

    head_vec = malloc(sizeof(vector));
    if (head_vec == NULL) {
        handle_errors();
    }
    head_vec->next = NULL;
    head_vec->cords = malloc(sizeof(cord));
    if (head_vec->cords == NULL) {
        handle_errors();
    }
    head_vec->cords->value = 0;
    head_vec->cords->next = NULL;

    read_points(head_vec, head_vec_obj, d, n);

    return head_vec;
}

PyObject* vector_to_py_obj(vector *vec, int n, int d) {
    PyObject* python_vectors_lst;
    PyObject* python_vector;
    PyObject* crd;
    cord *curr_cord;
    vector *curr_vec = vec;
    int i, j;
    python_vectors_lst = PyList_New(n);

    for(i = 0; i < n; i++) {
        python_vector = PyList_New(d);
        curr_cord = curr_vec->cords;
        j = 0;
        while (curr_cord != NULL) {
            crd = Py_BuildValue("d", curr_cord->value);
            PyList_SetItem(python_vector, j, crd);
            curr_cord = curr_cord->next;
            j++;
        }
        PyList_SetItem(python_vectors_lst, i, python_vector);
        curr_vec = curr_vec->next;
    }

    return python_vectors_lst;
}



static PyObject* sym_wrapper(PyObject *self, PyObject *args)
{
    PyObject* X_obj;
    PyObject* py_output;
    int n, d = 0;
    /* This parses the Python arguments into C variables*/
    if(!PyArg_ParseTuple(args, "O", &X_obj)) {
        return NULL;
    }

    vector *X = py_obj_to_verctor(X_obj, &d);
    n = PyObject_Length(X_obj);
    vector *A = sym(X, n);

    py_output = vector_to_py_obj(A, n, d);

    /**Free reest of memory*/
    free_vec(X);
    free_vec(A);


    return Py_BuildValue("O", py_output);
}



static PyObject* ddg_wrapper(PyObject *self, PyObject *args)
{
    PyObject* X_obj;
    PyObject* py_output;
    int n, d = 0;
    /* This parses the Python arguments into C variables*/
    if(!PyArg_ParseTuple(args, "O", &X_obj)) {
        return NULL;
    }

    vector *X = py_obj_to_verctor(X_obj, &d);
    n = PyObject_Length(X_obj);
    vector *A = sym(X, n);
    vector *D = ddg(A, n);


    py_output = vector_to_py_obj(D, n, d);

    /**Free reest of memory*/
    free_vec(X);
    free_vec(A);
    free_vec(D);


    return Py_BuildValue("O", py_output);
}

static PyObject* norm_wrapper(PyObject *self, PyObject *args)
{
    PyObject* X_obj;
    PyObject* py_output;
    int n, d = 0;
    /* This parses the Python arguments into C variables*/
    if(!PyArg_ParseTuple(args, "O", &X_obj)) {
        return NULL;
    }

    vector *X = py_obj_to_verctor(X_obj, &d);
    n = PyObject_Length(X_obj);
    vector *A = sym(X, n);
    vector *D = ddg(A, n);
    vector *W = norm(A, D, n);


    py_output = vector_to_py_obj(W, n, d);

    /**Free reest of memory*/
    free_vec(X);
    free_vec(A);
    free_vec(D);
    free_vec(W);


    return Py_BuildValue("O", py_output);
}

static PyMethodDef cMethods[] = {
        {"sym",                   /* the Python method name that will be used */
                (PyCFunction) sym_wrapper, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,
                PyDoc_STR("Returns similarity matrix")},
        {"ddg",                   /* the Python method name that will be used */
                (PyCFunction) ddg_wrapper, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,
                PyDoc_STR("Returns diagonal degree matrix")},
        {"norm",                   /* the Python method name that will be used */
                (PyCFunction) norn_wrapper, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,
                PyDoc_STR("Returns normalized similarity matrix")},
        {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};

static struct PyModuleDef cmodule = {
        PyModuleDef_HEAD_INIT,
        "symmnfAPI", /* name of module */
        NULL, /* module documentation, may be NULL */
        -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
        cMethods /* the PyMethodDef array from before containing the methods of the extension */
};

PyMODINIT_FUNC PyInit_symmnfAPI(void)
{
    PyObject *m;
    m = PyModule_Create(&cmodule);
    if (!m) {
        return NULL;
    }
    return m;
}
