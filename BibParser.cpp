#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <set>
#include <cassert>  // For assert()
#include "publication.h"
#include "BibParser.h"

using namespace std;

// Function to check balanced braces
bool areBracesBalanced(const string& str) {
    int balance = 0;
    for (char c : str) {
        if (c == '{') balance++;
        if (c == '}') balance--;
        if (balance < 0) return false;
    }
    return balance == 0;
}

// Helper function to check if every line except the first and last ends with a comma
bool hasValidCommas(const string& entry) {
    stringstream entryStream(entry);
    string line;
    vector<string> lines;

    // Read all lines of the BibTeX entry
    while (getline(entryStream, line)) {
        lines.push_back(line);
    }

    // Check each line except the first and last for a trailing comma
    for (size_t i = 1; i < lines.size() - 1; ++i) {
        // Trim spaces from the end of the line
        string trimmedLine = regex_replace(lines[i], regex("\\s+$"), "");

        // Count the number of commas at the end of the line
        size_t commaCount = 0;
        while (!trimmedLine.empty() && trimmedLine.back() == ',') {
            commaCount++;
            trimmedLine.pop_back();  // Remove the last comma
        }

        // If there are more than one comma, return false (invalid format)
        if (commaCount != 1) {
            std::cerr << "Error: Line has invalid commas at the end (invalid format):\n" 
                      << lines[i] << std::endl;
            return false;  // More than one comma or no comma at all, invalid format
        }
    }

    return true;
}

// Helper function to parse an individual BibTeX entry
void processEntry(const string& currentEntry, vector<Publication>& publications, const set<string>& faculty) {
    string title, venue, authorsStr;
    int year = 0;
    vector<Author> authors;
    set<string> uniqueAuthors;

    // Assert that braces are balanced
    assert(areBracesBalanced(currentEntry) && "Mismatched braces in entry!");

    // Check if the entry has valid commas (i.e., every line except first and last must end with a comma)
    if (!hasValidCommas(currentEntry)) {
        cerr << "Error: Invalid comma usage found in the entry:\n" << currentEntry << endl;
        assert(false && "Invalid comma usage found in the entry!");  // Assert failure on invalid commas
    }

    // Extract fields from the current entry
    size_t titlePos = currentEntry.find("title=");
    size_t venuePos = currentEntry.find("venue=");
    size_t authorPos = currentEntry.find("author=");
    size_t yearPos = currentEntry.find("year=");

    if (titlePos != string::npos) {
        size_t start = currentEntry.find('{', titlePos);
        size_t end = currentEntry.find('}', start);
        if (start != string::npos && end != string::npos)
            title = currentEntry.substr(start + 1, end - start - 1);
    }

    if (venuePos != string::npos) {
        size_t start = currentEntry.find('{', venuePos);
        size_t end = currentEntry.find('}', start);
        if (start != string::npos && end != string::npos)
            venue = currentEntry.substr(start + 1, end - start - 1);
    }

    if (authorPos != string::npos) {
        size_t start = currentEntry.find('{', authorPos);
        size_t end = currentEntry.find('}', start);
        if (start != string::npos && end != string::npos)
            authorsStr = currentEntry.substr(start + 1, end - start - 1);
    }

    if (yearPos != string::npos) {
        size_t start = currentEntry.find('{', yearPos);
        size_t end = currentEntry.find('}', start);
        if (start != string::npos && end != string::npos)
            year = stoi(currentEntry.substr(start + 1, end - start - 1));
    }

    // Parse authors
    stringstream authorStream(authorsStr);
    string authorName;
    bool hasAffiliatedAuthor = false;

    while (getline(authorStream, authorName, ',')) {
        authorName = regex_replace(authorName, regex("^\\s+|\\s+$"), ""); // Trim spaces
        if (!uniqueAuthors.insert(authorName).second) {
            cerr << "Error: Duplicate author found in the entry:\n" << currentEntry << endl;
            assert(false && "Duplicate author found in the entry!");  // Assert failure on duplicate author
        };
        if (faculty.count(authorName)) {
            hasAffiliatedAuthor = true;
        }
        authors.emplace_back(authorName, faculty.count(authorName) ? "IIIT-Delhi" : "Unknown Affiliation");
    }

    // If no affiliated author found, trigger an error
    if (!hasAffiliatedAuthor) {
        cerr << "Error: No affiliated author found in the entry:\n" << currentEntry << endl;
        assert(false && "No affiliated author found in the entry!");  // Assert failure
    }

    // Add the publication to the list
    Publication publication(title, venue, year);
    for (const auto& author : authors) {
        publication.addAuthor(author);
    }
    publications.push_back(publication);
}

vector<Publication> parseBibFile(const string& filePath, const set<string>& faculty) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Could not open the BibTeX file.");
    }

    vector<Publication> publications;
    string line, currentEntry;

    while (getline(file, line)) {
        if (line.empty()) continue;

        // Start a new entry
        if (line[0] == '@') {
            if (!currentEntry.empty()) {
                // Process the previous entry
                processEntry(currentEntry, publications, faculty);
            }
            currentEntry.clear(); // Clear for the new entry
        }
        currentEntry += line + "\n";
    }

    // Process the last entry if present
    if (!currentEntry.empty()) {
        processEntry(currentEntry, publications, faculty);
    }

    file.close();
    return publications;
}

set<string> loadFaculty(const string& csvFilePath) {
    ifstream file(csvFilePath);
    if (!file.is_open()) {
        throw runtime_error("Could not open the faculty file.");
    }

    set<string> faculty;
    string line;

    while (getline(file, line)) {
        if (line.find("\xEF\xBB\xBF") == 0) { // Remove BOM
            line = line.substr(3);
        }
        if (line.empty() || line == "Authors") continue;
        faculty.insert(line);
    }

    file.close();
    return faculty;
}
