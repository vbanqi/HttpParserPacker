#include "InteractGlobal.h"
#include "HttpRequestParser.h"
#include <string.h>
#include <stdlib.h>

using namespace Interact;


HttpRequestParser::HttpRequestParser()
{
}


HttpRequestParser::~HttpRequestParser()
{
}

int HttpRequestParser::ParserHead(const char* data, size_t len, char** content)
{
    
    if (len == 0) {
        return INTERACT_AGAIN;
    }
    Init();
    char mes[m_maxContentLength];
    size_t cl = len >= m_maxContentLength? m_maxContentLength-1: len;
    memcpy(mes, data, cl);
    mes[cl] = 0;
    const char* h_end = strstr(mes, HTTP_SPARATOR HTTP_SPARATOR);
    if (h_end == nullptr) {
        if (cl == m_maxContentLength - 1) {
            IError("The head is too max!");
            return INTERACT_ERROR;
        }
        return INTERACT_AGAIN;
    }
    const int h_len = h_end - mes;
    //the h_len is never more than len
    const char* start = mes;
    const char* end = strpbrk(mes, HTTP_SPARATOR);
    if (!end) {
        return INTERACT_ERROR;
    }
    *content = (char*)data + h_len + 4;
    //parser head
    size_t index = 0;
    while (start != end)
    {
        if (ParserHeadLine(index, start, end) == INTERACT_ERROR) {
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

int HttpRequestParser::ParserHeadLine(size_t index, const char* start, const char* end)
{
    size_t len = end - start;
    if (len >= m_maxContentLength) {
        IError("The head line length is too long!");
        return INTERACT_ERROR;
    }

    char line[m_maxContentLength];
    memcpy(line, start, len);
    line[len] = '\0';

    char* st = line;
    char* cursor;
    if (index == 0 )
    {
        //method
        cursor = strpbrk(st, " \t");
        len = cursor - st;
        if (len >= m_maxContentLength || len == 0) {
            IError("The method length is too long!");
            return INTERACT_ERROR;
        }
        SetPairs((char *)HTTP_METHOD, strlen(HTTP_METHOD), st, len);

        while (*cursor == ' ' || *cursor =='\t')
        {
            cursor++;
        }
        st = cursor;
        //dir
        cursor = strpbrk(st, " \t");
        len = cursor - st;
        if (len >= m_maxContentLength || len == 0) {
            return INTERACT_ERROR;
        }
        SetPairs((char *)HTTP_DIR, strlen(HTTP_DIR), st, len);
        while (*cursor == ' ' || *cursor == '\t')
        {
            cursor++;
        }
        st = cursor;
        //http version
        len = strnlen(st, m_maxContentLength);
        if (len >= m_maxContentLength|| len == 0) {
            return INTERACT_ERROR;
        }
        SetPairs((char *) HTTP_VERSION, strlen(HTTP_VERSION), st, len);
    }
    else
    {
        cursor = strpbrk(st, ":");
        if (!cursor) {
            return INTERACT_ERROR;
        }
        int kl = cursor - st;
        while (*(++cursor) == ' ' || *cursor == '\t'|| *cursor == ':');
        int vl = strlen(cursor);
        SetPairs(st, kl, cursor, vl);
    }
    return INTERACT_OK;
}

