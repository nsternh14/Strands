#ifndef STRAND_H
#define STRAND_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// REQUIRED STRUCT - DO NOT REMOVE STRUCT MEMBERS
typedef struct strand
{
  char** words;
  char** letters;
  int rows, cols;
  int numWords;
  int wordLen;
  char** dictionary; //2d pointer array to store all valid dictionary words (makes program run much faster)
  int dictSize; //Keeps size of dictionary array

} strand_t;

// REQUIRED FUNCTIONS
strand_t * create_strand(FILE * dataFP, FILE * dictFP);
bool is_word_in_strand(strand_t * strand, char * word);
void print_strand_matrix(strand_t * strand);
void free_strand(strand_t * strand);

//HELPER FUNCTIONS

//Generates complete word in clockwise direction
void generate_word(strand_t* s, int row, int col, char* word, int pos, FILE * dictFP);

//Builds word from given letter in strand 2d letter array
void build_words_from_letter(strand_t* s, int row, int col, FILE * dictFP);

//Iterates through dictionary file and creates dictionary array with all valid words
void create_dictionary_array(strand_t * s, FILE * dictFP);

//Returns 1 if word is found in dictionary array, 0 if not
int is_word_in_dictionary_array(strand_t * s, const char* word);

//Allocates memory to 2d array
char** allocate_2d_array(int rows, int cols);

//Frees memory of 2d array
void free_2d_array(char** array, int rows);

#endif
