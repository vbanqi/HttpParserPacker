#ifndef __POSEIDON_PROTOCOL_HTTPPARSE_PAKCER_H__
#define __POSEIDON_PROTOCOL_HTTPPARSE_PAKCER_H__

#define HTTP_VERSION "HttpVersion"
#define HTTP_DES "HttpDes"
#define HTTP_HOST "Host"
#define HTTP_CODE "Code"
#define HTTP_METHOD "Method"
#define HTTP_DIR "Dir"
#define HTTP_USERAGENT "User-Agent"
#define HTTP_CONTENT_LENGTH "Content-Length"
#define HTTP_CONTENT_TYPE "Content-Type"
#define HTTP_SEQUENCE_NUMBER "Sequence-Number"
#define HTTP_X_RELAY_SEQ "X-RelayServer-Seq"
#define HTTP_X_RELAY_TYPE "X-RelayServer-Type"
#define HTTP_X_RELAY_VERSION "X-API-Version"

#define HTTP_RESPONSE "Response"
namespace Interact {
    typedef struct{
        char *key;
        char *value;
    }HttpPairs;

    class IHttpParsePack : public InteractObject
    {
    public:
        
        virtual ~IHttpParsePack(){}
        virtual void Init() = 0;
    };

}
#endif
