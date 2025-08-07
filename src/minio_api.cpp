#include "minio_api.h"

MinioClient::MinioClient(const char* enpoint, const char* access_key, const char* secret_key, bool secure)
    : endpoint(endpoint), access_key(access_key), secret_key(secret_key), secure(secure) {}

MinioClient::~MinioClient() {}
