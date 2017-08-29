#include "InteractGlobal.h"
#include "IHttpParser.h"

namespace Interact {
    

IHttpParser::IHttpParser()
    :m_index(0)
{
    memset(m_pairs, 0, sizeof(m_pairs));
}

IHttpParser::~IHttpParser()
{
    Init();
}

void IHttpParser::Init()
{
    m_index = 0;
    for (size_t i = 0; i < m_maxPairs; i++) {
        if(m_pairs[i].value) {
            delete[] m_pairs[i].key;
            delete[] m_pairs[i].value;
            m_pairs[i].key = 0;
            m_pairs[i].value = 0;
        }
    }
}
char *IHttpParser::GetHeadString(const char* key)
{
    for(size_t i = 0; i < m_maxPairs; i++) {
        if (m_pairs[i].key && !strncasecmp(m_pairs[i].key,key, strlen(key))) {
            return m_pairs[i].value;
        }
    }
    return nullptr;
}
int IHttpParser::GetHeadInt(const char* key)
{
    for(size_t i = 0; i < m_maxPairs; i++) {
        if (m_pairs[i].key && !strncasecmp(m_pairs[i].key,key, strlen(key))) {
            size_t len;
            char *cursor = m_pairs[i].value;
            char length[30];
            len = strlen(cursor);
            if (len >= 30)
                return 0;
            memcpy(length, cursor, len);
            length[len] = '\0';
            size_t maxlen = atoi(length);
            if (maxlen > 4096) {
                IError("The %s is too max!", m_pairs[i].key); 
                return 0;
            }
            return maxlen;
        }
    }
    return 0;
}

int IHttpParser::SetPairs(char *key, size_t kl, char *value, size_t vl)
{
    if (kl >= m_maxContentLength || kl >= m_maxContentLength) {
        IError("The key length %zu or vl length %zu is too long!", kl, vl);
        return INTERACT_ERROR;
    }
    if (m_index >= (int)m_maxPairs) {
        IError("Max head line is %zu", m_maxPairs);
        return INTERACT_ERROR;
    }
    char *k = new char[kl + 1];
    char *v = new char[vl + 1];
    memcpy(k, key, kl);
    memcpy(v, value, vl);
    k[kl] = 0;
    v[vl] = 0;
    m_pairs[m_index].key = k;
    m_pairs[m_index].value = v;
    m_index ++;
    return INTERACT_OK;
}
}
