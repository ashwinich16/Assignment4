#ifndef BIBPARSER_H
#define BIBPARSER_H

#include <vector>
#include <set>
#include <string>
#include "publication.h"

std::vector<Publication> parseBibFile(const std::string& filePath, const std::set<std::string>& faculty);
std::set<std::string> loadFaculty(const std::string& csvFilePath);

#endif
