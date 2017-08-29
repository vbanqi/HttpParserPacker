#ifndef __POSEIDON_PROTOCOL_HTTPRESPONSEPARSER_H__
#define __POSEIDON_PROTOCOL_HTTPRESPONSEPARSER_H__

#include "IHttpParser.h"
#include "InteractGlobal.h"
namespace Interact {

    class HttpResponseParser : public IHttpParser
    {
    public:
        HttpResponseParser();
        virtual ~HttpResponseParser();

        virtual int ParserHead(const char* head, size_t len, char **data) override;

    private:
        int ParserHeadLine(size_t index, const char* start, const char* end);

    };
}
#endif
