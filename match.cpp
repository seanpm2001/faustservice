#include "match.hh"
#include <iostream>
#include <string>
#include <vector>

// Boost libraries
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;

extern int gVerbosity;

//----------------------------------------------------------------
// simplifyURL(), remove duplicated '/' in the URL

string simplifyURL(const char* url)
{
    const char* p = url;  // current char in url
    int         n = 0;    // number of successive '/'
    string      r;        // resulting simplified URL

    for (char c = *p; c != 0; c = *(++p)) {
        n = (c == '/') ? n + 1 : 0;
        if (n < 2) r += c;
    }
    if (gVerbosity >= 2) cerr << "Simplify url " << url << " --> " << r << endl;
    return r;
}

//----------------------------------------------------------------
// decomposeURL(), decompose an URL into a vector of strings.
// Used internally by matchURL. Trailing / are removed

vector<string> decomposeURL(const string& url)
{
    boost::filesystem::path U(url);
    vector<string>          decomposition;
    for (auto n : U) {
        string s = n.string();
        if (s != ".") decomposition.push_back(s);
    }
    return decomposition;
}

//----------------------------------------------------------------
// matchURL() returns true if the url and the pattern match
// To match they must have the same number of elements
// and all these elements must be identical, or wildcards ( '*' ).
// Data contains the decomposition of the URL

bool matchURL(const string& url, const std::string& pat, vector<string>& data)
{
    vector<string> U = decomposeURL(url);
    vector<string> P = decomposeURL(pat);
    if (P.size() == U.size()) {
        for (size_t i = 0; i < P.size(); i++) {
            if ((P[i] != "*") && (P[i] != U[i])) {
                return false;
            }
        }
        data = U;
        if (gVerbosity >= 2) cout << "PATTERN " << pat << " MATCHES URL " << url << endl;
        return true;
    } else {
        return false;
    }
}

//----------------------------------------------------------------
// matchURL() returns true if the url and the pattern match.
// To match they must have the same number of elements
// and all these elements must be identical, or wildcards ( '*' ).

bool matchURL(const string& url, const std::string& pat)
{
    vector<string> ignore;
    bool           r = matchURL(url, pat, ignore);
    if (gVerbosity >= 2) {
        if (r)
            cout << "MATCH " << pat << " <== " << url << endl;
        else
            cout << "DOES NOT MATCH " << pat << " <== " << url << endl;
    }
    return r;
}

bool matchExtension(const string& url, const std::string& ext)
{
    size_t u = url.length();
    size_t e = ext.length();
    if (u > e) {
        size_t p = u - e;
        return url.find(ext, p) == p;
    } else {
        return false;
    }
}

bool matchBeginURL(const string& url, const std::string& pat)
{
    vector<string> U = decomposeURL(url);
    vector<string> P = decomposeURL(pat);
    if (P.size() <= U.size()) {
        for (size_t i = 0; i < P.size(); i++) {
            if ((P[i] != "*") && (P[i] != U[i])) {
                return false;
            }
        }
        if (gVerbosity >= 2) cout << "PATTERN " << pat << " MATCHES URL " << url << endl;
        return true;
    } else {
        if (gVerbosity >= 2) cout << "PATTERN " << pat << " DOES NOT MATCH URL " << url << endl;
        return false;
    }
}
