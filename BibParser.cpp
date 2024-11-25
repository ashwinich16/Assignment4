#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <set>
#include "Publication.h"
#include "BibParser.h"
using namespace std;

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
                // Process the current entry
                string title, venue, authorsStr;
                int year = 0;
                vector<Author> authors;
                set<string> uniqueAuthors;

                // Check for mismatched braces
                int openBraces = 0;
                for (char c : currentEntry) {
                    if (c == '{') openBraces++;
                    if (c == '}') openBraces--;
                }
                if (openBraces != 0) {
                    cout << "Warning: Mismatched braces in entry:\n" << currentEntry << "\n";
                    currentEntry.clear();
                    continue;
                }

                // Extract fields
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
                    authorName = authorName.substr(authorName.find_first_not_of(' ')); // Trim spaces
                    if (!uniqueAuthors.insert(authorName).second) {
                        cout << "Warning: Duplicate author found: " << authorName << "\n";
                    }
                    if (faculty.count(authorName)) {
                        hasAffiliatedAuthor = true;
                    }
                    authors.emplace_back(authorName, faculty.count(authorName) ? "IIIT-Delhi" : "Unknown Affiliation");
                }

                // Check for missing affiliated authors
                if (!hasAffiliatedAuthor) {
                    cout << "Warning: No IIIT-Delhi affiliated authors in entry:\n" << currentEntry << "\n";
                    currentEntry.clear();
                    continue;
                }

                // Create and store the publication
                Publication publication(title, venue, year);
                for (const auto& author : authors) {
                    publication.addAuthor(author);
                }
                publications.push_back(publication);

                currentEntry.clear(); // Clear for the next entry
            }
        }
        currentEntry += line + "\n"; // Accumulate lines
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

    // Read the file line by line
    while (getline(file, line)) {
        // Remove BOM if present
        if (line.find("\xEF\xBB\xBF") == 0) { // BOM for UTF-8
            line = line.substr(3);
        }

        // Skip the header line or empty lines
        if (line.empty() || line == "Authors") {
            continue;
        }

        // Add the cleaned-up line to the faculty set
        faculty.insert(line);
    }

    file.close();
    return faculty;
}

