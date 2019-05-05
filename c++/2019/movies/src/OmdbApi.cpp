#include "OmdbApi.h"

OmdbApi::OmdbApi() {
    this->cr = new CurlRest;
    this->holdingValidResult = false;
}

OmdbApi::~OmdbApi() {
    delete this->cr;
}

/**
 * \brief perform a search by title on omdb's api (OP 't')
 * \brief the search result is stored in a json, available to be consumed later
 * \param title: the title of the movie
 * \return true in case a valid search result is succesfully received
 */
bool OmdbApi::searchMediaByTitle(string title) {
    string url = this->baseUrl;
    string jsonData;

    url.append("/?apikey=");
    url.append(this->apiKey);
    url.append("&r=json");
    url.append("&t=");
    url.append(title);
    cr->setCurlOptURL(url);

    if (cr->performCurl(&jsonData)) {
        Json::Reader reader;
        if (!reader.parse(jsonData, this->searchResults, false)) {
            cerr  << "Error parsing the search result"
                  << reader.getFormattedErrorMessages() << endl
                  << jsonData << endl << endl;
            /*
             * when this fails, 'jsonData' might be an HTML with an error message
             * TODO: parse the HTML to extract the error messages
             */
            return false;
        }
    }
    this->holdingValidResult = true;
    return true;
}

/**
 * \brief dump all search results data into a string
 */
string OmdbApi::dumpLastSearchResults() {
    return this->searchResults.toStyledString();
}

/**
 * \brief get the corresponding string 'value' for 'key' in last saved results
 * \return "Not available" if the key doesn't exist in the results
 * \return a string with the value of result[key], if a valid string
 * \return '####' in case of errors
 */
string OmdbApi::getLastResult(string key) {
    if (!this->searchResults.isMember(key))
        return "Not available";
    if (! this->searchResults[key].isString()) {
        cerr << "OmdbApi::getLastResult: key " << key
             << " not of type std::string" << endl;
        //abort();
        return "####";
    }
    return this->searchResults[key].asString();
}

/**
 * \brief get a fraction of an Array 'key' in last saved results
 * \return a string with the value of result[key][indx][subkey], if a valid string
 * \return '####' in case of errors
 */
string OmdbApi::getLastResult(string key, unsigned int indx, string subKey) {
    if (!this->searchResults.isMember(key)) {
        return "Not available";
    }
    if (! this->searchResults[key].isArray()) {
        cerr << "OmdbApi::getLastResult: key " << key
             << " not of type Array" << endl;
        //abort();
        return "####";
    }
    if (indx < 0 || indx >= this->searchResults[key].size()) {
        cerr << "OmdbApi::getLastResult: indx " << indx
             << " out of Array bounds" << endl;
        //abort();
        return "####";
    }
    if (! this->searchResults[key][indx][subKey].isString()) {
        cerr << "OmdbApi::getLastResult: key " << key
             << " indx " << indx << "subkey " << subKey
             << " not of type std::string" << endl;
        //abort();
        return "####";
    }

    return this->searchResults[key][indx][subKey].asString();
}

/**
 * \return lenght of array if key is an array, 0 otherwise
 */
unsigned int OmdbApi::getSearchResultsArrayLenght(string key) {
    if (this->searchResults.isMember(key) && this->searchResults[key].isArray())
        return this->searchResults[key].size();
    else
        return 0;
}
