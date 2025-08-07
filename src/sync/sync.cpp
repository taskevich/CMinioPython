#include <Python.h>
#include "../minio_api.h"

static PyObject* py_connect(PyObject* self, PyObject* args) {
    const char* endpoint;
    const char* access_key;
    const char* secret_key;
    bool secure;

    if (!PyArg_ParseTuple(args, "sssp", &endpoint, &access_key, &secret_key, &secure)) {
        Py_RETURN_NONE;
    }

    MinioClient* client = new MinioClient(endpoint, access_key, secret_key, secure);
    return PyCapsule_New(client, "MinioClient", [](PyObject* capsule) {
        delete reinterpret_cast<MinioClient*>(PyCapsule_GetPointer(capsule, "MinioClient"));
    });
}

static PyObject* py_check(PyObject* self, PyObject*) {
    PyObject* capsule;
    
    MinioClient* client = reinterpret_cast<MinioClient*>(PyCapsule_GetPointer(capsule, "MinioClient"));
    if (!client) Py_RETURN_NONE;

    printf("Client is NULL: client=%d; capsule=%d", client == NULL, capsule == NULL);

    Py_RETURN_NONE;
}

static PyMethodDef SyncMethods[] = {
    {"connect", py_connect, METH_VARARGS, "Connect to MinIO"},
    {"check", py_check, METH_NOARGS, "Check MinioClient created"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef sync_module = {
    PyModuleDef_HEAD_INIT,
    "sync",
    "Sync MinIO wrapper",
    -1,
    SyncMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_sync(void) {
    return PyModule_Create(&sync_module);
}
