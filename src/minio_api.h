#pragma once
#include <Python.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

class MinioClient {
public:
    MinioClient(const char* enpoint, const char* access_key, const char* secret_key, bool secure = false);
    ~MinioClient();

private:
    const char* endpoint;
    const char* access_key;
    const char* secret_key;
    bool secure;
};

#ifdef __cplusplus
}
#endif
