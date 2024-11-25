#include "Publication.h"
using namespace std;

Person::Person(const string& name) : name(name) {}

string Person::getName() const { return name; }

Author::Author(const string& name, const string& affiliation)
    : Person(name), affiliation(affiliation) {}

string Author::getAffiliation() const { return affiliation; }

Publication::Publication(const string& title, const string& venue, int year)
    : title(title), venue(venue), year(year) {}

void Publication::addAuthor(const Author& author) { authors.push_back(author); }

const vector<Author>& Publication::getAuthors() const { return authors; }

string Publication::getTitle() const { return title; }

string Publication::getVenue() const { return venue; }

int Publication::getYear() const { return year; }

void Publication::printPublication() const {
    cout << "Title: " << title << "\n";
    cout << "Venue: " << venue << "\n";
    cout << "Year: " << year << "\n";
    cout << "Co-Authors:\n";
    for (const auto& author : authors) {
        cout << "- " << author.getName() << " (" << author.getAffiliation() << ")\n";
    }
}

int Publication::getCoAuthorCount() const { return authors.size() - 1; }
