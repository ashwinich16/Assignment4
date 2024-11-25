#include <iostream>
#include <vector>
#include "BibParser.h"
#include "Publication.h"
using namespace std;

void printAuthorPublications(const vector<Publication>& publications, const string& authorName) {
    int totalCoAuthors = 0, paperCount = 0;

    cout << "\nPublications by " << authorName << ":\n";
    for (const auto& publication : publications) {
        for (const auto& author : publication.getAuthors()) {
            // Perform a case-insensitive partial match
            if (author.getName().find(authorName) != string::npos) {
                publication.printPublication();
                totalCoAuthors += publication.getCoAuthorCount();
                paperCount++;
                cout << "----\n";
                break;
            }
        }
    }

    if (paperCount > 0) {
        double avgCoAuthors = double(totalCoAuthors) / paperCount;
        cout << "Average co-authors per paper: " << avgCoAuthors << "\n";
    } else {
        cout << "No publications found for " << authorName << ".\n";
    }
}


int main(int argc, char* argv[]) {
    string bibFilePath = "publist.bib";
    string facultyFilePath = "IIITDelhi.csv";

    try {
        // Load faculty names
        set<string> faculty = loadFaculty(facultyFilePath);

        // Parse and validate the BibTeX file
        auto publications = parseBibFile(bibFilePath, faculty);

        // Query for an author's publications
        string authorName;
        cout << "Enter author name to search publications: ";
        getline(cin, authorName);

        printAuthorPublications(publications, authorName);

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
