#ifndef BIBPARSER_H
#define BIBPARSER_H

#include <string>
#include <vector>
#include <set>
#include "publication.h"

std::vector<Publication> parseBibFile(const std::string& filePath, const std::set<std::string>& faculty);
std::set<std::string> loadFaculty(const std::string& csvFilePath);

#endif // BIBPARSER_H
