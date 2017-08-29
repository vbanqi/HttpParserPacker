#include "InteractGlobal.h"
#include "HttpResponsePacker.h"
#include <string.h>
#include <stdio.h>
using namespace Interact;

HttpResponsePacker::HttpResponsePacker()
    :  m_code(0)
{
    m_httpDes[0] = 0;
}


HttpResponsePacker::~HttpResponsePacker()
{
}
void HttpResponsePacker::Init()
{
    IHttpPacker::Init();
    m_code = 0;
    m_httpDes[0] = 0;
}


int HttpResponsePacker::PackerHead(char* data, size_t bufferSize, size_t &headLen)
{
    int ret = 0;
    int len = 0;
    if (m_code == 0) {
        IError("The code must to be set!");
        goto fail;
    }
    len = snprintf(data, bufferSize, "HTTP/1.1 %zu %s" HTTP_SPARATOR, m_code, &m_httpDes[0]);
    if (len < 0 || len >= (int)bufferSize) {
        IError("PackHead error");
        goto fail;
    }
    for (size_t i = 0; i < m_maxPairs; i ++) {
        if (m_pairs[i].key && m_pairs[i].value) {
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
    return INTERACT_OK;
fail:
    data[0] = 0;
    return INTERACT_ERROR;
}

int HttpResponsePacker::SetCode(size_t code)
{
    if (code > 1000){
        IError("http code is not illegal!");
        return INTERACT_ERROR;
    }
    m_code = code;
    return INTERACT_OK;
}

int HttpResponsePacker::SetCodeDes(const char* des)
{
    int l = strnlen(des, m_maxDesLength);
    if (l >= m_maxDesLength) {
        return INTERACT_ERROR;
    }
    memcpy(&m_httpDes[0], des, l);
    m_httpDes[l] = 0;
    return INTERACT_OK;
}

