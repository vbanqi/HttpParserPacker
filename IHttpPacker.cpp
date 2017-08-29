
#include "InteractGlobal.h"
#include "IHttpPacker.h"

namespace Interact {

IHttpPacker::IHttpPacker()
{
    memset(m_pairs, 0, sizeof(m_pairs));
}

IHttpPacker::~IHttpPacker()
{
    Init(); 
}

void IHttpPacker::Init()
{
    for (size_t i = 0; i < m_maxPairs; i++) {
        if(m_pairs[i].value) {
            delete[] m_pairs[i].key;
            delete[] m_pairs[i].value;
            m_pairs[i].key = 0;
            m_pairs[i].value = 0;
        }
    }
}

int IHttpPacker::SetHead(const char *key, const char *value)
{
    size_t kl = strnlen(key, m_maxContentLength);
    size_t vl = strnlen(value, m_maxContentLength);

    return SetHead((char *)key, kl, (char *)value, vl);
}

int IHttpPacker::SetHead(char *key, size_t kl, char *value, size_t vl)
{
    if (kl >= m_maxContentLength || vl >= m_maxContentLength) {
        IError("Set the key %s or value %s is too long");
        return INTERACT_ERROR;
    }
    for (size_t i = 0; i < m_maxPairs; i++) {
        if (!m_pairs[i].key) {
            char *k = new char[kl + 1];
            char *v = new char[vl + 1];
            memcpy(k, key, kl);
            memcpy(v, value, vl);
            k[kl] = 0;
            v[vl] = 0;
            m_pairs[i].key = k;
            m_pairs[i].value = v;
            return INTERACT_OK;
        }
        else if (strcasestr(m_pairs[i].key, key)) {
            delete[] m_pairs[i].key;
            delete[] m_pairs[i].value;
            m_pairs[i].key = 0;
            m_pairs[i].value = 0;
            char *k = new char[kl + 1];
            char *v = new char[vl + 1];
            memcpy(k, key, kl);
            memcpy(v, value, vl);
            k[kl] = 0;
            v[vl] = 0;
            m_pairs[i].key = k;
            m_pairs[i].value = v;
            return INTERACT_OK;
        }
    }
    IError("The pairs is too max");
    return INTERACT_ERROR;
}
int IHttpPacker::SetHeadInt(const char *key, int value)
{
    int kl = strlen(key);
    return SetHeadInt((char *)key, kl, value);
}

int IHttpPacker::SetHeadInt(char *key, size_t kl, int v)
{
    char value[30];
    int vl = snprintf(value, 30, "%d", v);
    if (vl < 0 || vl >= (int)sizeof(value)) {
        IError("Set the value error!");
        return INTERACT_ERROR;
    }
    if (kl >= m_maxContentLength || vl >= (int)m_maxContentLength) {
        IError("Set the key %s or value %s is too long");
        return INTERACT_ERROR;
    }
    for (size_t i = 0; i < m_maxPairs; i++) {
        if (!m_pairs[i].key) {
            char *k = new char[kl + 1];
            char *v = new char[vl + 1];
            memcpy(k, key, kl);
            memcpy(v, value, vl);
            k[kl] = 0;
            v[vl] = 0;
            m_pairs[i].key = k;
            m_pairs[i].value = v;
            return INTERACT_OK;
        }
        else if (strcasestr(m_pairs[i].key, key)) {
            delete[] m_pairs[i].key;
            delete[] m_pairs[i].value;
            m_pairs[i].key = 0;
            m_pairs[i].value = 0;
            char *k = new char[kl + 1];
            char *v = new char[vl + 1];
            memcpy(k, key, kl);
            memcpy(v, value, vl);
            k[kl] = 0;
            v[vl] = 0;
            m_pairs[i].key = k;
            m_pairs[i].value = v;
            return INTERACT_OK;
        }
    }
    IError("The pairs is too max");
    return INTERACT_ERROR; 

}
}
