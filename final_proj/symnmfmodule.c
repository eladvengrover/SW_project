#define PY_SSIZE_T_CLEAN
# include <Python.h>
# include "general.h"
# include "symnmf.h"

void read_points_from_python(vector *head_vec, PyObject *head_vec_obj, int d, int n) {
    vector *last_vec, *curr_vec;
    cord *head_cord, *curr_cord;
    PyObject *dp, *crd;
    double cord_value;
    head_cord = head_vec->cords;
    curr_cord = head_cord;

    curr_vec = head_vec;
    last_vec = curr_vec;

    int i, j;
    for (i = 0; i < n; i++) {
        dp = PyList_GetItem(head_vec_obj, i);
        for (j = 0; j < d; j++) {
            crd = PyList_GetItem(dp, j);
            cord_value = PyFloat_AsDouble(crd);
            if (j == d - 1) {
                curr_cord->value = cord_value;  /**Read last cord of vector*/
                curr_cord->next = NULL;
                curr_vec->cords = head_cord;
            } else {
                curr_cord->value = cord_value;
                curr_cord->next = malloc(sizeof(cord));
                if (curr_cord->next == NULL) {
                    handle_errors();
                }
                curr_cord->next->value = 0;
                curr_cord->next->next = NULL;

                curr_cord = curr_cord->next;
            }
        }
        last_vec = curr_vec;  /**Prepare the next vector*/
        head_cord = malloc(sizeof(struct cord));
        if (head_cord == NULL) {
            handle_errors();
        }
        head_cord->value = 0;
        head_cord->next = NULL;
        curr_cord = head_cord;

        curr_vec->next = malloc(sizeof(vector));
        if (curr_vec->next == NULL) {
            handle_errors();
        }
        curr_vec->next->cords = head_cord;
        curr_vec->next->next = NULL;

        curr_vec = curr_vec->next;
    }

    free_vec(last_vec->next);
    last_vec->next = NULL;
}



vector* py_obj_to_vector(PyObject *head_vec_obj, int d) {
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

    read_points_from_python(head_vec, head_vec_obj, d, n);

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
    int n, d;
    /* This parses the Python arguments into C variables*/
    if(!PyArg_ParseTuple(args, "Oi", &X_obj, &d)) {
        return NULL;
    }

    vector *X = py_obj_to_vector(X_obj, d);
    n = PyObject_Length(X_obj);
    vector *A = c_sym(X, n);

    py_output = vector_to_py_obj(A, n, n);

    /**Free reest of memory*/
    free_vec(X);
    free_vec(A);

    return Py_BuildValue("O", py_output);
}



static PyObject* ddg_wrapper(PyObject *self, PyObject *args)
{
    PyObject* X_obj;
    PyObject* py_output;
    int n, d;
    /* This parses the Python arguments into C variables*/
    if(!PyArg_ParseTuple(args, "Oi", &X_obj, &d)) {
        return NULL;
    }

    vector *X = py_obj_to_vector(X_obj, d);
    n = PyObject_Length(X_obj);
    vector *A = c_sym(X, n);
    vector *D = c_ddg(A, n);


    py_output = vector_to_py_obj(D, n, n);

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
    int n, d;
    /* This parses the Python arguments into C variables*/
    if(!PyArg_ParseTuple(args, "Oi", &X_obj, &d)) {
        return NULL;
    }

    vector *X = py_obj_to_vector(X_obj, d);
    n = PyObject_Length(X_obj);
    vector *A = c_sym(X, n);
    vector *D = c_ddg(A, n);
    vector *W = c_norm(A, D, n);


    py_output = vector_to_py_obj(W, n, n);

    /**Free reest of memory*/
    free_vec(X);
    free_vec(A);
    free_vec(D);
    free_vec(W);


    return Py_BuildValue("O", py_output);
}


static PyObject* symnmf_wrapper(PyObject *self, PyObject *args)
{
    PyObject* H_obj;
    PyObject* W_obj;
    PyObject* py_output;
    int n, d, k;
    /* This parses the Python arguments into C variables*/
    if(!PyArg_ParseTuple(args, "OOii", &H_obj, &W_obj, &d, &k)) {
        return NULL;
    }

    vector *W = py_obj_to_vector(W_obj, d);
    vector *H = py_obj_to_vector(H_obj, d);
    n = PyObject_Length(X_obj);
    vector *optimized_H = c_symnmf(H, W, n, k);

    py_output = vector_to_py_obj(W, n, k);

    /**Free reest of memory*/
    free_vec(W);
    free_vec(H);
    free_vec(optimized_H);

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
                (PyCFunction) norm_wrapper, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,
                PyDoc_STR("Returns normalized similarity matrix")},
        {"symnmf",                   /* the Python method name that will be used */
                (PyCFunction) symnmf_wrapper, /* the C-function that implements the Python function and returns static PyObject*  */
                     METH_VARARGS,
                PyDoc_STR("Returns optimized H")},
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
