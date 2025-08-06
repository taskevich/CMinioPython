#include "minio_api.h"

PyMethodDef funcs[] = {
    {
        "connect",
        (PyCFunction)connect,
        METH_VARARGS,
        "Read file", 
    },
    {
        NULL, NULL, 0, NULL
    }
};

PyModuleDef cminiopy_module = {
    PyModuleDef_HEAD_INIT,
    "cminiopy",
    "cminiopy module",
    -1,
    funcs,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_cminiopy(void)
{
    return PyModule_Create(&cminiopy_module);
}
