#ifndef CURLREST_H
#define CURLREST_H

#include <iostream>
#include <curl/curl.h>

using namespace std;

/**
 * \brief basic class to perform http GETs using CURL
 */
class CurlRest {
  public:
    CurlRest();
    ~CurlRest();
    bool setCurlOptURL(string url);
    bool performCurl(string *output);

  private:
    CURL *curl;
    string encodeWhiteSpaces(string str);
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

};

#endif // CURLREST_H
