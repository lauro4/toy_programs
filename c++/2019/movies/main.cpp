#include <iostream>
#include <getopt.h>
#include "OmdbApi.h"

using namespace std;
/*
 * print "help"
 */
 void printHelp() {
    cout << "To run this program, use the options below: \n\n"
         << "-h, --help\n"
         << "\t Print this help message and exit\n\n"
         << "-s, --search <MEDIA TITLE>\n"
         << "\t Perform a search by MEDIA TITLE\n\n"
         << "-t, --type\n"
         << "\t Modify the output so only the type of the media is printed"
         << " after the search is performed\n\n"
         << "Examples:\n./movies --search star wars\n\n"
         << "Outputs the full results search for a match of a media with the"
         << " title \"star wars\"\n\n./movies --search star wars -t\n\n"
         << "Outputs only the TITLE and TYPE from the search result for a"
         << "match of a media with the title \"star wars\"" << endl;
 }

/*
 * print search results on stdout
 */
void printResultsAsInExample(OmdbApi *omdbApi, bool showMediaTypeOnly) {
    /* check if the search has failed */
    if (omdbApi->getLastResult("Response") == "False") {
        cout << "Error - " << omdbApi->getLastResult("Error") << endl;
        return;
    }

    /* assuming this is what was asked with the -t option */
    if (showMediaTypeOnly) {
        cout << "Title - " << omdbApi->getLastResult("Title") << endl;
        cout << "Type - " << omdbApi->getLastResult("Type") << endl;
        return;
    }
//    cout << omdbApi->dumpLastSearchResults();
    /*
     * make it as similar as possible to the example
     * instead of just dumping the results
     */
    /* first 14 items */
    for (string key : { "Title", "Year", "Rated", "Released", "Runtime",
                        "Genre", "Director", "Writer", "Actors", "Plot",
                        "Language", "Country", "Awards", "Poster"}) {
        cout << key << " - " << omdbApi->getLastResult(key) << endl;
    }
    /* ratings (special case as it is an array of ratings from different sources*/
    cout << "Ratings -" << endl;
    unsigned int n = omdbApi->getSearchResultsArrayLenght("Ratings");
    for (unsigned int i = 0; i < n; i++) {
        cout << omdbApi->getLastResult("Ratings", i, "Source") << " : "
             << omdbApi->getLastResult("Ratings", i, "Value") << endl;
    }
    /* remainming 5 items */
    for (string key : {
                "Type", "DVD", "BoxOffice", "Production", "Website"
            }) {
        cout << key << " - " << omdbApi->getLastResult(key) << endl;
    }
}

/**
 * \brief main function for command line version of the program
 * \return  0: success
 * \return -1: search  didn't receive a valid response from the server
 * \return -2: invalid parameter
 * \return -3: missing mandatory parameter
 */
int main(int argc, char **argv) {
    string mediaTitle;
    bool showMediaTypeOnly = false;
    bool res;
    int c, optionIndex;
    static struct option longOptions[] = {
        {"help",      no_argument,        0, 'h'},
        {"search",    required_argument,  0, 's'},
        {"type",      no_argument,        0, 't'},
        {0, 0, 0, 0}
    };
    /* ****Command line parsing loop: ******/
    while ((c = getopt_long (argc, argv, "hs:t", longOptions, &optionIndex)) != -1) {
        switch (c) {
        /* display help and exit immediately */
        case 'h':
            printHelp();
            return 0;
        /* -s media tile or --search media title or -s "media title" */
        case 's':
            mediaTitle = optarg;
            /*
             * manually append the following words until next option
             * to get the full title (in case it was not passed within quotes)
             */
            for(int i = optind; i < argc && argv[i][0] != '-'; i++) {
                mediaTitle.append(" ");
                mediaTitle.append(argv[i]);
            }
            /*set flags*/
            break;
        /* user only wants to know the 'type' of the media he is searching */
        case 't':
            showMediaTypeOnly = true;
            break;
        case '?':
            cerr << "Incorrect parameters\n\n" << endl;
            printHelp();
            return -2;
        }
    }

    if (mediaTitle.empty()) {
        cerr << "It's mandatory to inform a MEDIA_TITLE to perform a search\n"
             << endl;
        printHelp();
        return -3;
    }

    /* ****** Run the search and print results: ********/
    OmdbApi* omdbApi = new OmdbApi();
    res = omdbApi->searchMediaByTitle(mediaTitle);
    if (res) {
        printResultsAsInExample(omdbApi, showMediaTypeOnly);
    } else {
        cerr << "Something went wrong and we didn't receive a valid response"
             << " from the server" << endl;
    }
    delete omdbApi;

    return res ? 0 : -1;
}
