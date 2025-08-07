// minio_api.h

#pragma once
#include <Python.h>
#include <miniocpp/client.h>

#ifdef __cplusplus
extern "C" {
#endif

class MinioClient {
public:
    MinioClient(const char* endpoint, const char* access_key, const char* secret_key, bool secure = false);
    minio::s3::ListBucketsResponse ListBuckets();
    minio::s3::ListObjectsResult MinioClient::ListObjects(const char* bucket, const char* key, bool recursive);
    ~MinioClient();

    private:
    minio::s3::BaseUrl base_url;
    minio::creds::StaticProvider provider;
    minio::s3::Client client;
    const char* endpoint;
    const char* access_key;
    const char* secret_key;
    bool secure;
};

extern MinioClient* global_client;

#ifdef __cplusplus
}
#endif
