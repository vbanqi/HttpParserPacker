#include "InteractGlobal.h"
#include "HttpRequestPacker.h"
#include <stdio.h>
#include <string.h>

using namespace Interact;

#define CHECK_SIZE_ERROR(mem) if (strnlen(mem,COM_CONTENT_LENGTH) == 0) return INTERACT_ERROR

HttpRequestPacker::HttpRequestPacker()
    : m_url(nullptr)
    , m_dir(nullptr)
    , m_length(0)
{
    strcpy(m_method, "GET");
}


HttpRequestPacker::~HttpRequestPacker()
{
    ResetUrl();
}

void HttpRequestPacker::Init() 
{
    IHttpPacker::Init();
    ResetUrl();
    strcpy(m_method, "GET");

}

void HttpRequestPacker::ResetUrl()
{
    if (m_url) {
        delete[] m_url;
    }
    if (m_dir) {
        delete[] m_dir;
    }
    m_url = nullptr;
    m_dir = nullptr;
}
int HttpRequestPacker::SetUrl(const char* url, size_t len)
{
    m_url = new char[len + 1];
    memcpy(m_url, url, len);
    m_url[len] = 0;

    size_t index = 0;
    if (!strncmp(m_url, "http://", 7)) {
        index = 7;
    }
    else if (!strncmp(m_url, "https://", 8)) {
        index = 8;
    }
    m_dir = new char[len + 1];
    if (index == 0) {
        memcpy(m_dir, url, len);
        m_dir[len] = 0;
        delete[] m_url;
        m_url = nullptr;
        return INTERACT_OK;
    }
    const char* ind_host = m_url + index;
    const char* ind_host_last = strstr(ind_host, "/");
    size_t host_len;
    if (ind_host_last == nullptr) {
        host_len = len - index;
    }
    else {
        host_len = ind_host_last - ind_host;
    }
    int ret = SetHead((char *)HTTP_HOST, strlen(HTTP_HOST), (char *)ind_host, host_len);
    if (ret == INTERACT_ERROR) {
        ResetUrl();
        return INTERACT_ERROR;
    }

    size_t dl = len - index - host_len;
    const char* ind_dir = m_url + index + host_len;
    if(dl == 0) {
        strcpy(m_dir, "/");
    }
    else {
        memcpy(m_dir, ind_dir, dl);
        m_dir[dl] = '\0';
    }
    return INTERACT_OK;
}

int HttpRequestPacker::SetMethod(const char* method)
{
    strcpy(m_method, method);
    return INTERACT_OK;
}
int HttpRequestPacker::SetDir(const char* dir, size_t len)
{
    if (m_dir) {
        delete[] m_dir;
    }
    m_dir = new char[len + 1];
    memcpy(m_dir, dir, len);
    m_dir[len] = 0;
    return INTERACT_OK;

}
    
int HttpRequestPacker::PackerHead(char* data, size_t bufferSize, size_t &headLen)
{
    int len = snprintf(data, bufferSize, "%s %s HTTP/1.1" HTTP_SPARATOR , m_method, m_dir);
    int ret = 0;

    if (len < 0 || len >= (int)bufferSize) {
        IError("Pack error!");
        goto fail;
    }

    for (size_t i = 0; i < m_maxPairs; i ++) {
        if (m_pairs[i].key && m_pairs[i].value) {
            if (len > (int )bufferSize) {
                IError("PackHead is too long");
                goto fail;
            }
            ret = snprintf(data + len, bufferSize - len, "%s: %s" HTTP_SPARATOR, m_pairs[i].key, m_pairs[i].value); 
            len += ret;
            if (ret < 0 || len >= (int)bufferSize) {
                IError("PackHead error");
                goto fail;
            }
        }
    }
    ret = snprintf(data + len, bufferSize - len, HTTP_SPARATOR); 
    len += ret;
    if (ret < 0 || len >= (int)bufferSize) {
        IError("PackHead error");
        goto fail;
    }
    headLen = len;
    Init();
    return INTERACT_OK;
fail:
    Init();
    return INTERACT_ERROR;
}

