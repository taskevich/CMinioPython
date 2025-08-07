#include "minio_api.h"

MinioClient::MinioClient(const char* endpoint, const char* access_key, const char* secret_key, bool secure)
    : endpoint(endpoint), access_key(access_key), secret_key(secret_key), secure(secure),
      base_url(minio::s3::BaseUrl(endpoint, secure)),
      provider(minio::creds::StaticProvider(access_key, secret_key)),
      client(base_url, &provider) {
}

minio::s3::ListBucketsResponse MinioClient::ListBuckets() {
    return this->client.ListBuckets();
}

/// @brief 
/// @param bucket Name of the bucket
/// @param key Path to directory 
/// @return ListObjectsV2Args -> PyList objects
minio::s3::ListObjectsResult MinioClient::ListObjects(const char* bucket, const char* key, bool recursive) {
    minio::s3::ListObjectsArgs args;
    args.bucket = bucket;
    if (key)
        args.prefix = key;
    args.recursive = recursive;
    return this->client.ListObjects(args);
}

MinioClient::~MinioClient() {}
