#ifndef __POSEIDON_PROTOCOL_HTTPPAKCER_H__
#define __POSEIDON_PROTOCOL_HTTPPAKCER_H__

#include "IHttpParsePack.h"

namespace Interact {

    class IHttpPacker : public IHttpParsePack
    {
    public:
        IHttpPacker();
        virtual ~IHttpPacker();
        virtual void Init();
        virtual int PackerHead(char* data, size_t bufferSize, size_t &headLen) = 0;

        int SetHead(const char *key, const char *value);
        int SetHead(char *key, size_t kl, char *value, size_t vl);
        int SetHeadInt(const char *key, int value);
        int SetHeadInt(char *key, size_t kl, int value);

//        virtual int SetHead(char* key, size_t kl, char* value, size_t vl);
    protected:
        static const size_t m_maxContentLength = 1024;
        static const size_t m_maxPairs = 20;
        HttpPairs m_pairs[m_maxPairs];
    };

}
#endif
