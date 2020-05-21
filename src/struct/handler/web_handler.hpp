#ifndef __WEB_HANDLER_HPP__
#define __WEB_HANDLER_HPP__

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class WebHandler{
    public:
        const char* url;
        const WebRequestMethodComposite method; 
        const ArRequestHandlerFunction handler;

        WebHandler(const char* _url, 
            const WebRequestMethodComposite _method,
            const ArRequestHandlerFunction _handler) : url(_url), method(_method), handler(_handler) {

        }
};

#endif