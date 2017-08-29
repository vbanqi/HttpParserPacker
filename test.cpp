#include "HttpRequestPacker.h"
#include <stdio.h>

int main()
{
    
    printf("connected by peer");
    const char* ip = "192.168.1.103";
    const char* name = "media";
    char data[COM_CONTENT_LENGTH];
/*
 *    DispatchRegisterParser parser;
 *
 *    DispatchRegister data_register;
 *
 *    strcpy(data_register.serverIp, ip);
 *    strcpy(data_register.appName, name);
 *    parser.ParseData(&data_register, data, COM_CONTENT_LENGTH);
 *    int len = strnlen(data, COM_CONTENT_LENGTH);
 *    printf("Register len:%d, data:%s", len, data);
 *
 */
    const char* src ="{\"inUse\":0,\"serverIp\":\"192.168.1.103\",\"appName\":\"media\"}"; 
    int len = strlen(src);
    memcpy(data, src, strlen(src)); 
    Interact::HttpRequestPacker packer;
    packer.SetContentType("application/json");
    packer.SetContentLength(len);
    packer.SetContent(data);
    packer.SetUrl("http://192.168.1.105:9980/register");
    packer.SetMethod(Interact::IHttpPacker::POST);
    
    if (packer.Packer(data, COM_CONTENT_LENGTH) == INTERACT_ERROR) {
        printf("http packer error");
    }

    printf("Http data:%s", data);
}
