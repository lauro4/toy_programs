#ifndef OMDBAPI_H
#define OMDBAPI_H

#include <iostream>
#include <json/json.h>
#include "CurlRest.h"

using namespace std;

/**
 * \brief simplified class to manage media searches using Omdb's REST API.
 * \brief An instance of this class will hold the results of the last search
 * \brief in a JSON
 */
class OmdbApi {
  public:
    OmdbApi();
    ~OmdbApi();
    bool holdingValidResult;
    bool searchMediaByTitle(string title);
    string getLastResult(string key);
    string getLastResult(string key, unsigned int indx, string subKey);
    unsigned int getSearchResultsArrayLenght(string key);
    string dumpLastSearchResults();

  private:
    CurlRest* cr;
    /* hardcoded for simplicity, should probably come through constructor from a cfg file */
    const string baseUrl = "http://www.omdbapi.com";
    const string apiKey = "310ad53";
    Json::Value searchResults;

};

#endif // OMDBAPI_H
