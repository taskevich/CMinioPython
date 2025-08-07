#include <Python.h>

PyMethodDef funcs[] = {
    {
        NULL, NULL, 0, NULL
    }
};

PyModuleDef cminiopy_module = {
    PyModuleDef_HEAD_INIT,
    "cminiopy",
    "CMinioPy - Minio client written on C",
    -1,
    funcs,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_cminiopy(void) {
    return PyModule_Create(&cminiopy_module);
}
