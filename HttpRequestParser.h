#ifndef __POSEIDON_PROTOCOL_HTTPREQUESTPARSER_H__
#define __POSEIDON_PROTOCOL_HTTPREQUESTPARSER_H__

#include "IHttpParser.h"
#include "InteractGlobal.h"

namespace Interact {

    class HttpRequestParser :public IHttpParser
    {
    public:
        HttpRequestParser();
        virtual ~HttpRequestParser();

        virtual int ParserHead(const char* head, size_t len, char** data) override;

    private:
        int ParserHeadLine(size_t index, const char* start, const char* end);

    };
}
#endif
