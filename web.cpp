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
    /* *res += std::string((char*)ptr);

    return size * nmemb;*/

    size_t totalSize = size * nmemb;
    res->append(static_cast<char*>(ptr), totalSize);
    return totalSize;
}

size_t callbackFile(void *ptr, size_t size, size_t nmemb, FILE *file)
{
    size_t written;

    written = fwrite(ptr, size, nmemb, file);

    return written;
}

std::string WebAPI::sendRequest(const std::string URI, WEB_HTTP_REQUEST type, std::map<std::string, std::string> paramsGet, std::map<std::string, std::string> paramsPost)
{
    std::string URI_completed = URI;

    if(paramsGet.size() != 0 && type == WEB_HTTP_GET)
    {
        URI_completed += "?";

        size_t i = 0;

        for(const auto& kv : paramsGet)
        {
            URI_completed += kv.first + "=" + kv.second;

            if(i < paramsGet.size() - 1)
            {
                URI_completed += "&";
            }

            i++;
        }
    }

    else
    if(paramsPost.size() != 0 && type == WEB_HTTP_POST)
    {
        //curl_easy_setopt(curl, CURLOPT_POST, 1L);

        std::string postData;
        size_t index = 0;
        for (const auto& kv : paramsPost)
        {
            postData += kv.first + "=" + kv.second;

            if (index < paramsPost.size() - 1)
            {
                postData += "&";
            }

            index++;
        }

        std::cout << "Données envoyées : " << postData << std::endl;

        size_t dataSize = postData.size();
        char* postDataCStr = new char[dataSize + 1];
        strcpy(postDataCStr, postData.c_str());

        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postDataCStr);

        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        //free(postData);
    }

    answer = "";

    //std::cout << URI_completed << std::endl;

    curl_easy_setopt(curl, CURLOPT_URL, URI_completed.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
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

std::string WebAPI::sendRequestFile(const std::string URI, const std::string path, WEB_HTTP_REQUEST type, std::map<std::string, std::string> paramsGet, std::map<std::string, std::string> paramsPost)
{
    std::string URI_completed = URI;

    if(paramsGet.size() != 0 && type == WEB_HTTP_GET)
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

    file = fopen(path.c_str(), "wb");

    curl_easy_setopt(curl, CURLOPT_URL, URI_completed.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    res = curl_easy_perform(curl);

    fclose(file);

    if(res != CURLE_OK)
    {
        std::cout << "Erreur requête Curl : " << curl_easy_strerror(res) << std::endl;

        return "";
    }

    return "OK";
}
