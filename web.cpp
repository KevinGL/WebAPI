#include <iostream>
#include "web.h"

WebAPI::WebAPI()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl == nullptr)
    {
        std::cout << "Erreur initialisation CURL" << std::endl;
    }
}

WebAPI::~WebAPI()
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

size_t getResAPI(void *ptr, size_t size, size_t nmemb, std::string *res)
{
    *res += std::string((char*)ptr);

    return size * nmemb;
}

std::string WebAPI::sendRequest(const std::string URI, WEB_HTTP_REQUEST type, std::map<std::string, std::string> paramsGet, std::map<std::string, std::string> paramsPost)
{
    std::string URI_completed = URI;

    if(paramsGet.size() != 0)
    {
        URI_completed += "?";

        size_t i = 0;

        for(const auto& kv : paramsGet)
        {
            URI_completed += kv.first + "=" + kv.second;

            if(i < paramsGet.size()-1)
            {
                URI_completed += "&";
            }

            i++;
        }
    }

    curl_easy_setopt(curl, CURLOPT_URL, URI_completed.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResAPI);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
    {
        std::cout << "Erreur requête Curl : " << curl_easy_strerror(res) << std::endl;

        return "";
    }

    return answer;
}
