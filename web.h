#ifndef WEB_H
#define WEB_H
#endif

#include <Curl/curl/curl.h>
#include <map>

enum WEB_HTTP_REQUEST
{
    WEB_HTTP_GET,
    WEB_HTTP_POST,
};

class WebAPI
{
    private :

    CURL *curl;
    CURLcode res;
    std::string answer = "";

    size_t callback(void *ptr, size_t size, size_t nmemb, std::string *res);

    public :

    WebAPI();
    ~WebAPI();

    std::string sendRequest(const std::string URI, WEB_HTTP_REQUEST type, std::map<std::string, std::string> paramsGet = {}, std::map<std::string, std::string> paramsPost = {});
};

size_t getResAPI(void *ptr, size_t size, size_t nmemb, std::string *res);
