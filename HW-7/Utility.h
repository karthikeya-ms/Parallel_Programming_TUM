//
// Created by Vincent_Bode on 10.06.2020.
//

#ifndef ASSIGNMENTS_UTILITY_H
#define ASSIGNMENTS_UTILITY_H


class Utility {
public:
    static void generateProblemFromInput(unsigned char* document);

    static void readEncyclopedia(unsigned char *&document);

    static unsigned char *getQuery(unsigned int queryId);

    static size_t getQueryLength(unsigned int queryId);

    static unsigned char *getQueryBuffer();
};


#endif //ASSIGNMENTS_UTILITY_H
