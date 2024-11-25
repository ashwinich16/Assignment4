#ifndef PUBLICATION_H
#define PUBLICATION_H

#include <string>
#include <vector>

class Person {
    std::string name;
public:
    explicit Person(const std::string& name);
    std::string getName() const;
};

class Author : public Person {
    std::string affiliation;
public:
    Author(const std::string& name, const std::string& affiliation);
    std::string getAffiliation() const;
};

class Publication {
    std::string title;
    std::string venue;
    int year;
    std::vector<Author> authors;

public:
    Publication(const std::string& title, const std::string& venue, int year);

    void addAuthor(const Author& author);
    const std::vector<Author>& getAuthors() const;

    std::string getTitle() const;
    std::string getVenue() const;
    int getYear() const;

    void printPublication() const;
    int getCoAuthorCount() const;
};

#endif // PUBLICATION_H
