#ifndef __poseidon_protocol_http_test__
#define __poseidon_protocol_http_test__

#include "InteractGlobal.h"
#include "HttpRequestPacker.h"
#include "HttpRequestParser.h"
#include "HttpResponsePacker.h"
#include "HttpResponseParser.h"
#include "gtest/gtest.h"

const static int g_httpTestContentLength = 4096;
TEST(HttpRequestPacker, PackerConrrect) {
    const char *packerstr = "POST /live/xyz HTTP/1.1\r\n"
        "Host: 127.0.0.1:1688\r\n"
        "User-Agent: windows nt\r\n"
        "Content-Type: text/json\r\n"
        "Content-Length: 11\r\n"
        "\r\n";
    Interact::HttpRequestPacker reqpacker;

    const char *url = "http://127.0.0.1:1688/live/xyz";
    ASSERT_EQ(INTERACT_OK, reqpacker.SetUrl(url, strlen(url)));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetMethod("POST"));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHead((char *)HTTP_USERAGENT, strlen(HTTP_USERAGENT), (char *)"windows nt", strlen("windows nt")));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHead((char *)HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE), (char *)"text/json", strlen("text/json")));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHeadInt((char *)HTTP_CONTENT_LENGTH, strlen(HTTP_CONTENT_LENGTH),11));
    //ASSERT_EQ(INTERACT_OK,reqpacker.SetContent((char*)"hello word!"));
    char content[g_httpTestContentLength];
    char* ch = content;
    size_t len = 0;
    ASSERT_EQ(INTERACT_OK,reqpacker.PackerHead(ch, g_httpTestContentLength, len));

    ASSERT_EQ(len , strlen(ch));
    ASSERT_STREQ(packerstr, content);
}

TEST(HttpRequestPacker, PackerInconrrectLength) {
    const char *packerstr = "POST /live/xyz HTTP/1.1\r\n"
        "Host: 127.0.0.1:1688\r\n"
        "User-Agent: windows nt\r\n"
        "Content-Type: text/json\r\n"
        "Content-Length: -11\r\n"
        "\r\n";
    Interact::HttpRequestPacker reqpacker;
    const char *url = "http://127.0.0.1:1688/live/xyz";
    ASSERT_EQ(INTERACT_OK,reqpacker.SetUrl(url, strlen(url)));
    ASSERT_EQ(INTERACT_OK,reqpacker.SetMethod("POST"));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHead((char *)HTTP_USERAGENT, strlen(HTTP_USERAGENT), (char *)"windows nt", strlen("windows nt")));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHead((char *)HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE), (char *)"text/json", strlen("text/json")));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHeadInt((char *)HTTP_CONTENT_LENGTH, strlen(HTTP_CONTENT_LENGTH), -11));
    //ASSERT_EQ(INTERACT_OK,reqpacker.SetContent((char*)"hello word!"));
    char content[g_httpTestContentLength];
    char* ch = content;
    size_t len = 0;
    ASSERT_EQ(INTERACT_OK,reqpacker.PackerHead(ch, g_httpTestContentLength, len));

    ASSERT_EQ(len, strlen(ch));
    ASSERT_STREQ(packerstr, content);
}

TEST(HttpRequestPacker, PackerInconrrectUrl) { const char *packerstr = "POST /live/xyz HTTP/1.1\r\n"
        "Host: 127.0.0.1:1688\r\n"
        "User-Agent: windows nt\r\n"
        "Content-Type: text/json\r\n"
        "Content-Length: 11\r\n"
        "\r\n";
    Interact::HttpRequestPacker reqpacker;
    const char* url = "http://127.0.0.1:1688";
    ASSERT_EQ(INTERACT_OK,reqpacker.SetUrl(url, strlen(url)));
    ASSERT_EQ(INTERACT_OK,reqpacker.SetMethod("POST"));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHead((char *)HTTP_USERAGENT, strlen(HTTP_USERAGENT), (char *)"windows nt", strlen("windows nt")));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHead((char *)HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE), (char *)"text/json", strlen("text/json")));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHeadInt((char *)HTTP_CONTENT_LENGTH, strlen(HTTP_CONTENT_LENGTH),11));
    char content[g_httpTestContentLength];
    size_t len = 0;
    ASSERT_EQ(INTERACT_OK,reqpacker.PackerHead(content, g_httpTestContentLength, len));

    ASSERT_EQ(len, strlen(content));
    ASSERT_STRNE(packerstr, content);
}

TEST(HttpRequestPacker, PackerInconrrectUserAgent) {
    const char *packerstr = "POST /live/xyz HTTP/1.1\r\n"
        "Host: 127.0.0.1:1688\r\n"
        "Content-Type: text/json\r\n"
        "Content-Length: 11\r\n"
        "\r\n";
    Interact::HttpRequestPacker reqpacker;
    const char *url = "http://127.0.0.1:1688/live/xyz";
    ASSERT_EQ(INTERACT_OK,reqpacker.SetUrl(url, strlen(url)));
    ASSERT_EQ(INTERACT_OK,reqpacker.SetMethod("POST"));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHead((char *)HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE), (char *)"text/json", strlen("text/json")));
    ASSERT_EQ(INTERACT_OK, reqpacker.SetHeadInt((char *)HTTP_CONTENT_LENGTH, strlen(HTTP_CONTENT_LENGTH), 11));
    char content[g_httpTestContentLength];
    char* ch = content;
    size_t len = 0;
    ASSERT_EQ(INTERACT_OK,reqpacker.PackerHead(ch, g_httpTestContentLength, len));

    ASSERT_EQ(len, strlen(ch));
    ASSERT_STREQ(packerstr, content);
}

TEST(HttpRequestParser, HttpRequestParserTest) {
    const char* str = "GET /gywm/aboutus/ HTTP/1.1\r\n"
        "Host: www.100tal.com\r\n"
        "Cache-Control: max-age=0\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
        "Accept-Encoding: gzip, deflate, sdch\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 10\r\n"
        "Accept-Language: zh-CN,zh;q=0.8\r\n\r\n"
        "hello word";
    char *he = strstr((char *)str, "\r\n\r\n");
    Interact::HttpRequestParser reqparser;
    char *content = nullptr;
    ASSERT_EQ(he - str + 4,reqparser.ParserHead(str, strlen(str), &content));
    ASSERT_STREQ("GET", reqparser.GetHeadString(HTTP_METHOD)); 
    ASSERT_STREQ("www.100tal.com", reqparser.GetHeadString(HTTP_HOST));
    ASSERT_STREQ("text/html", reqparser.GetHeadString(HTTP_CONTENT_TYPE));
    ASSERT_EQ(10, reqparser.GetHeadInt(HTTP_CONTENT_LENGTH));
    ASSERT_STREQ("hello word", content);
}

TEST(HttpRequestParser, HttpRequestParserNoLength) {
    const char* str = "GET /gywm/aboutus/ HTTP/1.1\r\n"
        "Host: www.100tal.com\r\n"
        "Cache-Control: max-age=0\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
        "Accept-Encoding: gzip, deflate, sdch\r\n"
        "Content-Type: text/html\r\n"
        "Accept-Language: zh-CN,zh;q=0.8\r\n\r\n"
        "hello word";
    char *he = strstr((char *)str, "\r\n\r\n");
    Interact::HttpRequestParser reqparser;
    char *content = nullptr;
    ASSERT_EQ(he - str + 4,reqparser.ParserHead(str, strlen(str), &content));
    ASSERT_STREQ("GET", reqparser.GetHeadString(HTTP_METHOD)); 
    ASSERT_STREQ("www.100tal.com", reqparser.GetHeadString(HTTP_HOST));
    ASSERT_STREQ("text/html", reqparser.GetHeadString(HTTP_CONTENT_TYPE));
    ASSERT_NE(10, reqparser.GetHeadInt(HTTP_CONTENT_LENGTH));
    ASSERT_STREQ("hello word", content);
}


TEST(HttpRequestParser, HttpRequestParserNativeLength) {
    const char* str = "GET /gywm/aboutus/ HTTP/1.1\r\n"
        "Host: www.100tal.com\r\n"
        "Cache-Control: max-age=0\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
        "Accept-Encoding: gzip, deflate, sdch\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: -10\r\n"
        "Accept-Language: zh-CN,zh;q=0.8\r\n\r\n"
        "hello word";
    char *he = strstr((char *)str, "\r\n\r\n");
    Interact::HttpRequestParser reqparser;
    char *content = nullptr;
    ASSERT_EQ(he - str + 4,reqparser.ParserHead(str, strlen(str), &content));
    ASSERT_STREQ("GET", reqparser.GetHeadString(HTTP_METHOD)); 
    ASSERT_STREQ("www.100tal.com", reqparser.GetHeadString(HTTP_HOST));
    ASSERT_STREQ("text/html", reqparser.GetHeadString(HTTP_CONTENT_TYPE));
    ASSERT_EQ(0, reqparser.GetHeadInt(HTTP_CONTENT_LENGTH));
    ASSERT_STREQ("hello word", content);

}


TEST(HttpResponsePacker, HttpResponsePackerTest) {
    const char *str = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/json\r\n"
        "Content-Length: 11\r\n"
        "\r\n";
    Interact::HttpResponsePacker respacker;
    ASSERT_EQ(INTERACT_OK,respacker.SetCode(200));
    ASSERT_EQ(INTERACT_OK,respacker.SetCodeDes("OK"));
    ASSERT_EQ(INTERACT_OK, respacker.SetHead((char *)HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE), (char *)"text/json", strlen("text/json")));
    ASSERT_EQ(INTERACT_OK, respacker.SetHeadInt((char *)HTTP_CONTENT_LENGTH, strlen(HTTP_CONTENT_LENGTH), 11));

    char rescontent[g_httpTestContentLength];
    size_t len = 0;
    ASSERT_EQ(INTERACT_OK,respacker.PackerHead(rescontent, g_httpTestContentLength, len));
    ASSERT_EQ(len, strlen(rescontent));
    ASSERT_STREQ(str, rescontent);
    
}
    
TEST(HttpResponsePacker, HttpResponsePackerNativeLength) {
    const char *str = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/json\r\n"
        "Content-Length: -11\r\n"
        "\r\n";
    Interact::HttpResponsePacker respacker;
    ASSERT_EQ(INTERACT_OK,respacker.SetCode(200));
    ASSERT_EQ(INTERACT_OK,respacker.SetCodeDes("OK"));
    ASSERT_EQ(INTERACT_OK, respacker.SetHead((char *)HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE), (char *)"text/json", strlen("text/json")));
    ASSERT_EQ(INTERACT_OK, respacker.SetHeadInt((char *)HTTP_CONTENT_LENGTH, strlen(HTTP_CONTENT_LENGTH), -11));

    char rescontent[g_httpTestContentLength];
    size_t len = 0;
    ASSERT_EQ(INTERACT_OK,respacker.PackerHead(rescontent, g_httpTestContentLength, len));
    ASSERT_EQ(len, strlen(rescontent));
    ASSERT_STREQ(str, rescontent);
    
}


TEST(HttpResponsePacker, HttpResponsePackerSetNativeCode) {
    const char *str = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/json\r\n"
        "Content-Length: 11\r\n"
        "\r\n";
    Interact::HttpResponsePacker respacker;
    ASSERT_EQ(INTERACT_ERROR,respacker.SetCode(-200));
    ASSERT_EQ(INTERACT_OK,respacker.SetCodeDes("OK"));
    ASSERT_EQ(INTERACT_OK, respacker.SetHead((char *)HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE), (char *)"text/json", strlen("text/json")));
    ASSERT_EQ(INTERACT_OK, respacker.SetHeadInt((char *)HTTP_CONTENT_LENGTH, strlen(HTTP_CONTENT_LENGTH), 11));

    char rescontent[g_httpTestContentLength];
    size_t len = 0;
    ASSERT_EQ(INTERACT_ERROR,respacker.PackerHead(rescontent, g_httpTestContentLength, len));
    ASSERT_STRNE(str, rescontent);
    ASSERT_EQ(len, strlen(rescontent));

    
}

TEST(HttpResponseParser, HttpResponseParserNavigateCode) {
        Interact::HttpResponseParser resparser;
        const char* testresponse = "HTTP/1.1 -200 OK\r\n"
            "Server: nginx\r\n"
            "Date: Wed, 04 Jan 2017 02:46:05 GMT\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 14\r\n"
            "Vary: Accept-Encoding\r\n"
            "Content-Encoding: gzip\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Proxy-Connection: Close\r\n\r\n"
            "Hello poseidon";

        char *he = strstr((char *)testresponse, "\r\n\r\n");
        char *content = nullptr;
        ASSERT_EQ(he - testresponse + 4,resparser.ParserHead(testresponse, strlen(testresponse), &content));
        ASSERT_NE(200, resparser.GetHeadInt(HTTP_CODE));
        ASSERT_STREQ("text/html", resparser.GetHeadString(HTTP_CONTENT_TYPE));
        ASSERT_EQ(14, resparser.GetHeadInt(HTTP_CONTENT_LENGTH));
}
TEST(HttpResponseParser, HttpResponseParserNoDes) {
        Interact::HttpResponseParser resparser;
        const char* testresponse = "HTTP/1.1 200\r\n"
            "Server: nginx\r\n"
            "Date: Wed, 04 Jan 2017 02:46:05 GMT\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 14\r\n"
            "Vary: Accept-Encoding\r\n"
            "Content-Encoding: gzip\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Proxy-Connection: Close\r\n\r\n"
            "Hello poseidon";

        char *content = nullptr;
        ASSERT_EQ(INTERACT_ERROR,resparser.ParserHead(testresponse, strlen(testresponse), &content));
        ASSERT_NE(200, resparser.GetHeadInt(HTTP_CODE));
        ASSERT_STRNE("text/html", resparser.GetHeadString(HTTP_CONTENT_TYPE));
        ASSERT_EQ(0, resparser.GetHeadInt(HTTP_CONTENT_LENGTH));
}


TEST(HttpResponseParser, HttpResponseParserTestNoLength) {
        Interact::HttpResponseParser resparser;
        const char* testresponse = "HTTP/1.1 200 OK\r\n"
            "Server: nginx\r\n"
            "Date: Wed, 04 Jan 2017 02:46:05 GMT\r\n"
            "Content-Type: text/html\r\n"
            "Vary: Accept-Encoding\r\n"
            "Content-Encoding: gzip\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Proxy-Connection: Close\r\n\r\n"
            "Hello poseidon";

        char *he = strstr((char *)testresponse, "\r\n\r\n");
        char *content = nullptr;
        ASSERT_EQ(he - testresponse + 4,resparser.ParserHead(testresponse, strlen(testresponse), &content));
        ASSERT_EQ(200, resparser.GetHeadInt(HTTP_CODE));
        ASSERT_STREQ("text/html", resparser.GetHeadString(HTTP_CONTENT_TYPE));
        ASSERT_EQ(0, resparser.GetHeadInt(HTTP_CONTENT_LENGTH));
}

TEST(HttpResponseParser, HttpResponseParserTest) {
        Interact::HttpResponseParser resparser;
        const char* testresponse = "HTTP/1.1 200 OK\r\n"
            "Server: nginx\r\n"
            "Date: Wed, 04 Jan 2017 02:46:05 GMT\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 14\r\n"
            "Vary: Accept-Encoding\r\n"
            "Content-Encoding: gzip\r\n"
            "Transfer-Encoding: chunked\r\n"
            "Proxy-Connection: Close\r\n\r\n"
            "Hello poseidon";

        char *he = strstr((char *)testresponse, "\r\n\r\n");
        char *content = nullptr;
        ASSERT_EQ(he - testresponse + 4,resparser.ParserHead(testresponse, strlen(testresponse), &content));
        ASSERT_EQ(200, resparser.GetHeadInt(HTTP_CODE));
        ASSERT_STREQ("text/html", resparser.GetHeadString(HTTP_CONTENT_TYPE));
        ASSERT_EQ(14, resparser.GetHeadInt(HTTP_CONTENT_LENGTH));

}

#endif // !__poseidon_protocol_http_test__
