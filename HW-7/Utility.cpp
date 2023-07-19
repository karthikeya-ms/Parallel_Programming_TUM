//
// Created by Vincent_Bode on 10.06.2020.
//

#include <iostream>
#include <random>
#include <cstring>
#include "Utility.h"
#include "StringSearch.h"

// Stores the individual queries in a square matrix. Any excess bytes after the end of each query are set to zero.
unsigned char queryStorage[NUM_QUERIES][MAX_QUERY_LENGTH];

// Generates the problem as usual by getting a seed.
void Utility::generateProblemFromInput(unsigned char* document) {
    unsigned int seed = 0;
    std::cout << "READY" << std::endl;
    std::cin >> seed;

    std::cerr << "Using seed " << seed << std::endl;
    if (seed == 0) {
        std::cerr << "Warning: default value 0 used as seed." << std::endl;
    }

    std::mt19937 random(seed);
    std::uniform_int_distribution distribution(0, DOCUMENT_SIZE - MAX_QUERY_LENGTH - 1);
    for (unsigned int queryId = 0; queryId < NUM_QUERIES; queryId++) {
        unsigned int queryStart = distribution(random);
        std::memcpy(queryStorage[queryId], document + queryStart, getQueryLength(queryId));
    }
}

// Loads the encyclopedia into memory completely.
void Utility::readEncyclopedia(unsigned char *&document) {
    document = new unsigned char[DOCUMENT_SIZE];

    FILE *file = fopen(FILE_NAME, "rb");
    if(!file) {
        fprintf(stderr, "Failed to open file " FILE_NAME ". Are you in the right working directory?\n");
        exit(-1);
    }
    if(fread(document, 1, DOCUMENT_SIZE, file) != DOCUMENT_SIZE) {
        fprintf(stderr, "Failed to read requested number of bytes from file.\n");
        exit(-1);
    }
    fclose(file);
}

// Get the length of the query by id. Can be accessed from any rank.
std::size_t Utility::getQueryLength(unsigned int queryId) {
    return (queryId / 4) + 5;
}

// Access the query by its id. Just dereferences the 2D array to return the proper offset to the query start.
// Can only be used if the query storage was filled.
unsigned char* Utility::getQuery(unsigned int queryId) {
    return queryStorage[queryId];
}

// This lends you direct access to a char buffer of size NUM_QUERIES * MAX_QUERY_LENGTH. This can be used as a 2D array,
// where each query starts at the beginning of a line. Any excess data is set to zero.
// Can be useful for distributing work.
unsigned char* Utility::getQueryBuffer() {
    return *queryStorage;
}
