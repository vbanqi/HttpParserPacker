#ifndef __POSEIDON_PROTOCOL_HTTPRESPONSEPACKER_H__
#define __POSEIDON_PROTOCOL_HTTPRESPONSEPACKER_H__
#include "IHttpPacker.h"
#include "InteractGlobal.h"

namespace Interact {

    class HttpResponsePacker:public IHttpPacker
    {
    public:
        HttpResponsePacker();
        virtual ~HttpResponsePacker();

        virtual void Init();

        virtual int PackerHead(char* data, size_t bufferSize, size_t &headLen) override;

        int SetCode(size_t code);

        int SetCodeDes(const char* des);
    private:
        
        static const int m_maxDesLength = 1024;

        size_t m_code;
        char m_httpDes[m_maxDesLength];
    };
}
#endif
