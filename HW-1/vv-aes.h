//
// Created by Vincent Bode on 21/04/2020.
// Updated by Dennis-Florian Herr 28/04/2022.
//

#ifndef ASSIGNMENTS_VV_AES_H
#define ASSIGNMENTS_VV_AES_H

#include <limits>

#define BLOCK_SIZE 7
#define UNIQUE_CHARACTERS 256
#define ROUNDS 9
#define ITERATIONS 400000

/*
 * This is the message given to you to encrypt to verify the correctness and speed of your approach. Unfortunately,
 * there are no state secrets to be found here. Those would have probably made a pretty penny on the black market.
 */
uint8_t message[BLOCK_SIZE][BLOCK_SIZE] = {
        {'T', 'H', 'I', 'S', ' ', 'I', 'S'},
        {' ', 'A', 'V', 'E', 'R', 'Y', ' '},
        {'S', 'E', 'C', 'R', 'E', 'T', ' '},
        {'M', 'E', 'S', 'S', 'A', 'G', 'E'},
        {'!', ' ', 'T', 'R', 'Y', ' ', 'T'},
        {'O', ' ', 'C', 'R', 'A', 'C', 'K'},
        {' ', 'I', 'T', '.', '.', '.', ' '}
};

/*
 * The set of all keys generated at runtime and the index of the current key.
 */
int currentKey = 0;
uint8_t allKeys[ROUNDS][BLOCK_SIZE][BLOCK_SIZE];

/*
 * Use this like a 2D-Matrix key[BLOCK_SIZE][BLOCK_SIZE];
 * The key is only handed to you when it's time to actually encrypt something.
 */
uint8_t (*key)[BLOCK_SIZE];

/*
 * This is the source list of characters. If you wish to translate a character, find its index in this list. The
 * corresponding output character resides at the same index in the substituted character list.
 */
uint8_t originalCharacter[] = { 65, 187, 34, 76, 23, 80, 84, 133, 165, 185, 236, 164, 252, 248, 237, 82, 59, 94, 191, 75, 60, 228, 87, 138, 96, 67, 66, 101, 225, 154, 215, 136, 85, 224, 173, 213, 112, 117, 182, 18, 19, 51, 15, 180, 36, 43, 40, 39, 181, 118, 77, 146, 160, 208, 83, 227, 211, 26, 33, 193, 230, 150, 140, 158, 99, 155, 111, 1, 3, 253, 127, 190, 55, 238, 62, 31, 56, 57, 108, 52, 14, 53, 115, 141, 151, 42, 79, 149, 68, 188, 98, 48, 121, 41, 199, 156, 144, 179, 72, 216, 130, 107, 174, 29, 0, 123, 197, 240, 113, 5, 212, 116, 161, 198, 110, 218, 50, 73, 35, 209, 38, 64, 157, 9, 220, 46, 22, 124, 168, 6, 25, 128, 58, 186, 45, 147, 159, 234, 8, 153, 100, 207, 222, 81, 32, 194, 88, 204, 17, 232, 21, 170, 103, 119, 152, 195, 132, 241, 162, 30, 192, 166, 137, 104, 4, 125, 97, 70, 49, 91, 92, 102, 221, 177, 172, 254, 247, 11, 134, 183, 169, 245, 12, 255, 178, 249, 20, 203, 16, 200, 148, 95, 251, 63, 2, 71, 229, 167, 24, 109, 189, 10, 129, 246, 219, 214, 243, 231, 93, 7, 105, 78, 106, 235, 54, 244, 171, 89, 250, 90, 184, 239, 143, 142, 47, 139, 206, 242, 74, 114, 86, 44, 226, 120, 61, 126, 13, 28, 176, 163, 175, 202, 196, 223, 210, 145, 201, 69, 135, 37, 122, 217, 27, 233, 205, 131 };
/*
 * This is the output list of characters. If you wish to translate a character, find its index in the original list.
 * The corresponding output character resides at the same index in this list.
 */
uint8_t substitutedCharacter[] = { 83, 54, 64, 63, 84, 239, 167, 136, 218, 230, 28, 157, 129, 158, 223, 67, 46, 172, 130, 110, 133, 237, 150, 192, 171, 169, 38, 98, 217, 164, 14, 137, 30, 221, 55, 94, 162, 16, 128, 57, 174, 117, 93, 52, 175, 189, 109, 186, 140, 2, 205, 21, 246, 49, 254, 249, 179, 74, 12, 123, 23, 56, 203, 224, 187, 0, 66, 103, 180, 65, 115, 91, 42, 240, 39, 154, 85, 199, 8, 248, 222, 131, 215, 34, 18, 226, 244, 6, 68, 252, 81, 209, 45, 142, 11, 127, 124, 41, 119, 242, 60, 145, 188, 77, 13, 101, 25, 200, 156, 255, 27, 1, 201, 232, 149, 185, 95, 53, 51, 50, 152, 177, 114, 125, 165, 61, 161, 236, 44, 106, 36, 122, 210, 62, 166, 132, 184, 69, 47, 88, 5, 143, 253, 148, 134, 245, 212, 73, 112, 250, 75, 155, 211, 183, 76, 153, 214, 3, 15, 58, 195, 144, 118, 96, 116, 207, 146, 139, 190, 235, 31, 70, 198, 229, 206, 247, 104, 78, 111, 120, 105, 82, 231, 160, 228, 4, 108, 22, 126, 107, 176, 170, 208, 97, 113, 241, 251, 238, 100, 216, 141, 233, 197, 89, 135, 72, 194, 181, 29, 225, 86, 79, 121, 17, 227, 168, 7, 99, 71, 204, 9, 151, 19, 219, 234, 213, 37, 243, 90, 147, 178, 163, 24, 182, 10, 35, 43, 33, 191, 202, 159, 59, 32, 196, 87, 48, 26, 220, 138, 102, 20, 92, 80, 40, 193, 173 };
 
 uint8_t polynomialCoefficients[BLOCK_SIZE][BLOCK_SIZE] = {
        { 3, 1, 6, 5, 9, 4, 3},
        { 9, 6, 3, 8, 5, 2, 1},
        { 1, 2, 3, 4, 5, 6, 7},
        { 9, 8, 7, 6, 5, 4, 7},
        { 3, 8, 6, 5, 9, 1, 9},
        { 3, 5, 2, 8, 7, 9, 2},
        { 8, 3, 4, 6, 5, 1, 1}
};

/*
 * Generate some keys that can be used for encryption based on the seed set from the input.
 */
void generate_keys() {
    // Fill the key block
    for(auto & currentKey : allKeys) {
        for (auto & row : currentKey) {
            for (unsigned char & column : row) {
                column = rand() % std::numeric_limits<uint8_t>::max();
            }
        }
    }
}

void readInput() {
    std::cout << "READY" << std::endl;
    unsigned int seed = 0;
    std::cin >> seed;

    std::cerr << "Using seed " << seed << std::endl;
    if(seed == 0) {
        std::cerr << "Warning: default value 0 used as seed." << std::endl;
    }

    // Set the pseudo random number generator seed used for generating encryption keys
    srand(seed);

    generate_keys();
}

void writeOutput() {
    // Output the current state of the message in hexadecimal.
    for (int row = 0; row < BLOCK_SIZE; row++) {
        std::cout << std::hex << (int) message[row][0] << (int) message[row][1] << (int) message[row][2]
                  << (int) message[row][3];
    }
    // This stops the timer.
    std::cout << std::endl << "DONE" << std::endl;
}

/*
 * This is a utility method. It determines the next key to use from the set of pre-generated keys. In a real
 * cryptographic system, the subsequent keys are generated from a key schedule from the original key. To keep the code
 * short, we have omitted this behavior.
 */
void set_next_key() {
    key = &allKeys[currentKey][0];
    currentKey = (currentKey + 1) % ROUNDS;
}

#endif //ASSIGNMENTS_VV_AES_H
