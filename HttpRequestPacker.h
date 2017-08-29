#ifndef __POSEIDON_PROTOCOL_HTTPREQUESTPACKER_H__
#define __POSEIDON_PROTOCOL_HTTPREQUESTPACKER_H__

#include "IHttpPacker.h"
#include "InteractGlobal.h"

namespace Interact {

    class HttpRequestPacker :public IHttpPacker
    {
    public:
        HttpRequestPacker();
        virtual ~HttpRequestPacker();


        int SetUrl(const char* url, size_t len);

        int SetDir(const char* dir, size_t len);
    
        int SetMethod(const char* method);

        virtual void Init();

        virtual int PackerHead(char* data, size_t bufferSize, size_t &headLen) override;

    private:
        void ResetUrl();
        
        static const size_t m_maxMethodLength = 20;
            
        char *m_url; 
        char *m_dir;
        char m_method[m_maxMethodLength];
        size_t m_length;
    };

}
#endif
