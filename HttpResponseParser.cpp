#include "InteractGlobal.h"
#include "HttpResponseParser.h"
#include "MessageParserUtils.h"

using namespace Interact;

HttpResponseParser::HttpResponseParser()
{

}


HttpResponseParser::~HttpResponseParser()
{
}

int HttpResponseParser::ParserHead(const char* data, size_t len, char **content)
{
    if (len == 0) {
        return INTERACT_AGAIN;
    }
    Init();
    //first copy the max head data to cache
    size_t cl = len >= m_maxContentLength? m_maxContentLength-1: len;
    char mes[m_maxContentLength];
    memcpy(mes, data, cl);
    mes[cl] = 0;
    //second check the \r\n\r\n chars
    const char* h_end = strstr(mes, HTTP_SPARATOR HTTP_SPARATOR);
    if (h_end == nullptr)
        return INTERACT_AGAIN;
    int h_len = h_end - mes;
    const char* start = mes;
    const char* end = strpbrk(mes, HTTP_SPARATOR);
    if (end == nullptr) {
        return INTERACT_ERROR;
    }
    //parser head
    SetPairs((char *)HTTP_METHOD, strlen(HTTP_METHOD), (char *)HTTP_RESPONSE, strlen(HTTP_RESPONSE));
    *content = (char*)data + h_len + 4;
    size_t index = 0;
    while (start != end)
    {
        if (this->ParserHeadLine(index, start, end) == INTERACT_ERROR) {
            Init();
            return INTERACT_ERROR;
        }
        end++;
        if (end > mes + len)
        {
            break;
        }
        if (*end == '\r' || *end == '\n')
        {
            end++;
        }
        start = end;
        end = strpbrk(start, HTTP_SPARATOR);
        if (end == nullptr)
            end = mes + len;
        index++;
    }
    return h_len + 4;
}

int HttpResponseParser::ParserHeadLine(size_t index, const char* start, const char* end)
{
    size_t len = end - start;
    if (len >= m_maxContentLength) {
        return INTERACT_ERROR;
    }

    char line[m_maxContentLength];
    memcpy(line, start, len);
    line[len] = '\0';

    char* st = line;
    char* cursor;
    if (index == 0)
    {
        //version
        cursor = strpbrk(st, " \t");
        len = cursor - st;
        if (len >= m_maxContentLength || len == 0) {
            return INTERACT_ERROR;
        }

        SetPairs((char *)HTTP_VERSION, strlen(HTTP_VERSION), st, len);
        
        while (*cursor == ' ' || *cursor == '\t')
        {
            cursor++;
        }
        st = cursor;
        //code
        cursor = strpbrk(st, " \t");
        len = cursor - st;
        if (len >= m_maxContentLength || len == 0) {
            return INTERACT_ERROR;
        }
        SetPairs((char *)HTTP_CODE, strlen(HTTP_CODE), st, len);

        while (*cursor == ' ' || *cursor == '\t')
        {
            cursor++;
        }
        st = cursor;
        //http desc 
        len = strlen(cursor);
        if (len >= m_maxContentLength || len == 0) {
            return INTERACT_ERROR;
        }
        SetPairs((char *)HTTP_DES, strlen(HTTP_DES), st, len);
    }
    else
    {
        cursor = strpbrk(st, ":");
        if (!cursor) {
            return INTERACT_ERROR;
        }
        int kl = cursor - st;
        while (*(++cursor) == ' ' || *cursor == '\t'|| *cursor == ':');
        int vl = len - kl - 1;
        SetPairs(st, kl, cursor, vl);
    }
    return INTERACT_OK;
}

