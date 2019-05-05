#include "CurlRest.h"

CurlRest::CurlRest() {
    this->curl = curl_easy_init();
    if (!this->curl) {
        cerr << "CurlRest(): curl_easy_init() failed" << endl;
        abort();
    }
    /* set callback function - CURLOPT_WRITEFUNCTION(3) */
    curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, &CurlRest::write_callback);
}

CurlRest::~CurlRest() {
    curl_easy_cleanup(this->curl);
}

/**
 * \brief encode whiteSpaces as "%20" to make the string a valid URL
 */
string CurlRest::encodeWhiteSpaces(string str) {
    size_t pos;
    while ((pos = str.find(' ')) != string::npos)
        str.replace(pos, 1, "%20");
    return str;
}

/**
 * \brief sets the CURLOPT_URL value
 */
bool CurlRest::setCurlOptURL(string url) {
    curl_easy_setopt(curl, CURLOPT_URL, this->encodeWhiteSpaces(url).c_str());
    return true;
}

/**
 * \brief run curl_easy_perform with the currently configured options
 * \param pointer to string to be filled with the output from curl
 * \return true/false for success/error
 */
bool CurlRest::performCurl(string *output) {
    curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, output);
    CURLcode res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
             << endl;
        return false;
    }
    return true;
}

/**
 * \brief a callback function to be provided to CURLOPT_WRITEFUNCTION
 */
size_t CurlRest::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    string* data = (string*) userdata;
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

