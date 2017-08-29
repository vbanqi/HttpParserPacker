#ifndef __POSEIDON_PROTOCOL_HTTPPARSE_H__
#define __POSEIDON_PROTOCOL_HTTPPARSE_H__
#include "IHttpParsePack.h"

namespace Interact {
    

    class IHttpParser : public IHttpParsePack
    {
    public:
        IHttpParser();
        virtual ~IHttpParser();
        virtual void Init();
        virtual int ParserHead(const char* head, size_t len, char **data) = 0;
        virtual char *GetHeadString(const char* key);
        virtual int GetHeadInt(const char* key);

    protected:
        int SetPairs(char *key, size_t kl, char *value, size_t vl);
        static const size_t m_maxContentLength = 1024;

    private:
        static const size_t m_maxPairs = 20;
        int m_index;
        HttpPairs m_pairs[m_maxPairs];


    };

}

#endif
