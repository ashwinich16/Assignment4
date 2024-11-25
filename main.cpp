#include <iostream>
#include <vector>
#include "BibParser.h"
#include "publication.h"
using namespace std;

void printAuthorPublications(const vector<Publication>& publications, const string& authorName) {
    int totalCoAuthors = 0, paperCount = 0;

    cout << "\nPublications by " << authorName << ":\n";
    for (const auto& publication : publications) {
        for (const auto& author : publication.getAuthors()) {
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
        cout << "Average co-authors per paper: " 
             << (int)(totalCoAuthors / paperCount) << "\n";
    } else {
        cout << "No publications found for " << authorName << ".\n";
    }
}

int main(int argc, char* argv[]) {
    string bibFilePath = "publist.bib";
    string facultyFilePath = "IIITDelhi.csv";

    // Ensure that at least one author name is provided as a command-line argument
    if (argc < 2) {
        cerr << "Error: Please provide at least one author name as a command-line argument." << endl;
        return 1;
    }

    try {
        // Load faculty list
        set<string> faculty = loadFaculty(facultyFilePath);
        
        // Parse the BibTeX file
        auto publications = parseBibFile(bibFilePath, faculty);

        // Loop through all the author names passed as command-line arguments
        for (int i = 1; i < argc; ++i) {
            string authorName = argv[i]; // Get author name from command-line arguments
            printAuthorPublications(publications, authorName); // Print publications for each author
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
