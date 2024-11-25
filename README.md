# Assignment4
# Description
It takes the Multiple Author names as input arguments and then Parses the given .bib file to fetch the published Data and also validate the .bib file where the are followinf these conditions:
(a) Formatting mistake, with incorrect braces or commas
(b) Same author repeated twice in any bib entry
(c) No author belonging to the institute’s affiliation.
It stops the excecution if the .bib file does not follow any one of these conditions.If the .bib is correct it will output all the publication of the Authors

Commands to buils the file
g++ -c output main.cpp BibParser.cpp publication.cpp
g++ -g -o output main.o BibParser.o publication.o 
g++ -O3 -o optimize main.o BibParser.o publication.o
