#include <Python.h>
#include "../minio_api.h"

MinioClient* global_client = NULL;

static PyObject* py_connect(PyObject* self, PyObject* args) {
    const char* endpoint;
    const char* access_key;
    const char* secret_key;
    bool secure;

    if (!PyArg_ParseTuple(args, "sssp", &endpoint, &access_key, &secret_key, &secure)) {
        Py_RETURN_NONE;
    }

    if (global_client) {
        delete global_client;
    }

    global_client = new MinioClient(endpoint, access_key, secret_key, secure);

    return PyCapsule_New(global_client, "MinioClient", [](PyObject* capsule) {
        delete reinterpret_cast<MinioClient*>(PyCapsule_GetPointer(capsule, "MinioClient"));
    });
}

static PyObject* py_list_objects(PyObject* self, PyObject* args) {
    if (!global_client) {
        PyErr_SetString(PyExc_RuntimeError, "Minio client isn't initialized");
        Py_RETURN_NONE;
    }

    const char* bucket;
    PyObject* key_obj = NULL;
    int recurse = 0;

    if (!PyArg_ParseTuple(args, "sOp", &bucket, &key_obj, &recurse)) {
        Py_RETURN_NONE;
    }

    const char* key = NULL;
    if (key_obj != Py_None) {
        if (!PyUnicode_Check(key_obj)) {
            PyErr_SetString(PyExc_TypeError, "key must be str or None");
            Py_RETURN_NONE;
        }
        key = PyUnicode_AsUTF8(key_obj);
    }

    PyObject* py_list = PyList_New(0);

    for (auto result = global_client->ListObjects(bucket, key, recurse); result; ++result) {
        const minio::s3::Item& item = *result;
        if (!item || item.name.empty() || item.name.back() == '/') {
            continue;
        }

        PyObject* py_obj = PyDict_New();
        if (!py_obj) {
            Py_DECREF(py_list);
            Py_RETURN_NONE;
        }

        PyObject* py_key = PyUnicode_FromStringAndSize(item.name.c_str(), item.name.size());
        PyObject* py_obj_name = PyUnicode_FromStringAndSize(item.object_name.c_str(), item.object_name.size());
        PyObject* py_etag = PyUnicode_FromStringAndSize(item.etag.c_str(), item.etag.size());
        std::string mod_str = item.last_modified.ToISO8601UTC();
        PyObject* py_modified = PyUnicode_FromStringAndSize(mod_str.c_str(), mod_str.size());
        PyObject* py_size = PyLong_FromUnsignedLongLong(item.size);

        if (!py_key || !py_obj_name || !py_etag || !py_modified || !py_size) {
            Py_XDECREF(py_key);
            Py_XDECREF(py_obj_name);
            Py_XDECREF(py_etag);
            Py_XDECREF(py_modified);
            Py_XDECREF(py_size);
            Py_DECREF(py_obj);
            Py_DECREF(py_list);
            Py_RETURN_NONE;
        }

        PyDict_SetItemString(py_obj, "key", py_key);
        PyDict_SetItemString(py_obj, "object_name", py_obj_name);
        PyDict_SetItemString(py_obj, "etag", py_etag);
        PyDict_SetItemString(py_obj, "last_modified", py_modified);
        PyDict_SetItemString(py_obj, "size", py_size);

        Py_DECREF(py_key);
        Py_DECREF(py_obj_name);
        Py_DECREF(py_etag);
        Py_DECREF(py_modified);
        Py_DECREF(py_size);

        if (PyList_Append(py_list, py_obj) == -1) {
            Py_DECREF(py_obj);
            Py_DECREF(py_list);
            Py_RETURN_NONE;
        }

        Py_DECREF(py_obj);
    }

    return py_list;
}

static PyObject* py_list_buckets(PyObject* self, PyObject* args) {
    if (!global_client) {
        PyErr_SetString(PyExc_RuntimeError, "Minio client isn't initialized");
        Py_RETURN_NONE;
    }

    minio::s3::ListBucketsResponse resp = global_client->ListBuckets();
    if (!resp) {
        std::cerr << "ERROR: " << resp.Error().String() << std::endl;
        PyErr_SetString(PyExc_RuntimeError, resp.Error().String().c_str());
        Py_RETURN_NONE;
    }

    PyObject* py_bucket_list = PyList_New(0);
    for (const auto& bucket : resp.buckets) {
        PyObject* py_bucket = Py_BuildValue(
            "{s:s, s:s}",
            "name", bucket.name.c_str(),
            "creation_date", bucket.creation_date.ToISO8601UTC().c_str()
        );
        PyList_Append(py_bucket_list, py_bucket);
        Py_DECREF(py_bucket);
    }

    return py_bucket_list;
}

static PyMethodDef SyncMethods[] = {
    {"connect", py_connect, METH_VARARGS, "Connect to MinIO"},
    {"list_objects", py_list_objects, METH_VARARGS, "Get list objects"},
    {"list_buckets", py_list_buckets, METH_NOARGS, "Get list buckets"},
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
