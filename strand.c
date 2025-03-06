#include "strand.h"

// REQUIRED

void create_dictionary_array(strand_t * s, FILE * dictFP) {

  //count finds dictionary file size
  int count = 0;
  char dictWord[100];

  //iterates through dictionary file
  while (fscanf(dictFP, "%s", dictWord) != EOF) {
    bool validWord = true;
    int length = strlen(dictWord);
    //Checks that all letters in the dictionary word are lowercase and are in the alphabet
    for (int i = 0; i < length; ++i) {
      if (!isalpha(dictWord[i]) || !islower(dictWord[i])) {
        validWord = false;
      }
    }

    //If the word is valid and equal to desired word length, iterates the count
    if (length  == s->wordLen && validWord) {
      count++;
    }
  }

  //Allocates memory to dictionary array based on count
  s->dictionary = (char**)malloc(count * sizeof(char*));

  //Sets dictSize to 0
  s->dictSize = 0;

  //Resets dictionary file to first line
  fseek(dictFP, 0, SEEK_SET);

  //Iterates through dictionary file again
  while (fscanf(dictFP, "%s", dictWord) != EOF) {
    bool validWord = true;
    int length = strlen(dictWord);
    //Checks same conditions as above loop
    for (int i = 0; i < length; ++i) {
      if (!isalpha(dictWord[i]) || !islower(dictWord[i])) {
	validWord = false;
      }
    }
    //If valid word and desired word length, adds it to the dictionary array stored in s and increases dictSize
    if (length  == s->wordLen && validWord) {
      s->dictionary[s->dictSize] = strdup(dictWord);
      s->dictSize++;
    }
  }
  
}

int is_word_in_dictionary_array(strand_t * s, const char* word) {

  //Iterates through dictionary array
  for (int i = 0; i < s->dictSize; i++) {
    //Checks if word is equal to word at dictionary array at all indexes
    if (strcmp(word, s->dictionary[i]) == 0) {
      return 1;
    }
  }
  
  return 0;
}

char** allocate_2d_array(int rows, int cols) {
  //Allocates memory for 2d letter array
    char** array = (char**)malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; ++i) {
      //Allocates memory for each point in array
        array[i] = (char*)malloc(cols * sizeof(char));
    }
    return array;
}

void free_2d_array(char** array, int rows) {
  //Frees memory for each point in 2d array, then frees memory for whole array
    for (int i = 0; i < rows; ++i) {
        free(array[i]);
    }
    free(array);
}

void build_words_from_letter(strand_t* s, int row, int col, FILE * dictFP) {
  
  //Allocates memory for word the size of wordLen, with one space for the null character
  char* word = (char*)malloc((s->wordLen + 1) * sizeof(char));
  
  //Creates first letter in word with position given
  word[0] = s->letters[row][col];

  //Sets next letter in word to null character
  word[1] = '\0';
  
  //Calls recursive function to build rest of word
  generate_word(s, row, col, word, 1, dictFP);

}

void generate_word(strand_t* s, int row, int col, char* word, int pos, FILE * dictFP) {
  
  //If word building has been completed, end recursive loop
  if (pos == s->wordLen) {

    bool duplicate = false;
    //Iterates through words already added to check if word is a duplicate
    for (int i = 0; i < s->numWords; ++i) {
      if (strcmp(s->words[i], word) == 0) {
	duplicate = true;
      }
    }
    //If word is not a duplicate and is in dictionary array, adds to the s->words array and increments s->numWords
    if (is_word_in_dictionary_array(s, word) == 1 && !duplicate) {
      strcpy(s->words[s->numWords], word);
      s->numWords += 1;
    }
    return;
  }
  
  //Try to go to all 8 directions in clockwise order

  //Up, left
  if (row > 0 && col > 0) {
    word[pos] = s->letters[row - 1][col - 1];
    word[pos + 1] = '\0';
    generate_word(s, row - 1, col - 1, word, pos + 1, dictFP);
  }

  //Straight up
  if (row > 0) {
    word[pos] =	s->letters[row - 1][col];
    word[pos + 1] = '\0';
    generate_word(s, row - 1, col, word, pos + 1,  dictFP);
  }

  //Up, right
  if (row > 0 && col + 1 < s->cols) {
    word[pos] =	s->letters[row - 1][col + 1];
    word[pos + 1] = '\0';
    generate_word(s, row - 1, col + 1, word, pos + 1, dictFP);
  }

  //Straight right
  if (col + 1 < s->cols) {
    word[pos] =	s->letters[row][col + 1];
    word[pos + 1] = '\0';
    generate_word(s, row, col + 1, word, pos + 1,  dictFP);
  }

  //Down, right
  if (row + 1 < s->rows && col + 1 < s->cols) {
    word[pos] = s->letters[row + 1][col + 1];
    word[pos + 1] = '\0';
    generate_word(s, row + 1, col + 1, word, pos + 1, dictFP);
  }

  //Straight down
  if (row + 1 < s->rows) {
    word[pos] = s->letters[row + 1][col];
    word[pos + 1] = '\0';
    generate_word(s, row + 1, col, word, pos + 1, dictFP);
  }

  //Down, left
  if (row + 1 < s->rows && col > 0) {
    word[pos] = s->letters[row + 1][col - 1];
    word[pos + 1] = '\0';
    generate_word(s, row + 1, col - 1, word, pos + 1, dictFP);
  }

  //Straight left
  if (col > 0) {
    word[pos] = s->letters[row][col - 1];
    word[pos + 1] = '\0';
    generate_word(s, row, col - 1, word, pos + 1, dictFP);
  }

}

strand_t* create_strand(FILE* dataFP, FILE* dictFP) {

  //Allocate data for strand
  strand_t* s = (strand_t*)malloc(sizeof(strand_t));
  
  //Scan data file for num rows, num cols, and wordLen
  fscanf(dataFP, "%d %d", &s->rows, &s->cols);
  fscanf(dataFP, "%d", &s->wordLen);

  //Calls function to build dictionary array
  create_dictionary_array(s, dictFP);
  
  //Allocate memory for letters array and words
  s->letters = allocate_2d_array(s->rows, s->cols);

  s->words = (char**)malloc(s->dictSize* sizeof(char*));
  for (int i = 0; i < s->dictSize; i++) {
    s->words[i] = (char*)malloc((s->wordLen + 1)*sizeof(char));
  }

  //Scan in letters and populate letters array
  for (int i = 0; i < s->rows; ++i) {
    for (int j = 0; j < s->cols; ++j) {
      fscanf(dataFP, " %c", &s->letters[i][j]);
    }
  }

  s->numWords = 0;
  
  //Populate strand->words
  for (int i = 0; i < s->rows; ++i) {
    for (int j = 0; j < s->cols; ++j) {
      build_words_from_letter(s, i, j, dictFP);
    }
  }

  //Close data file and dict file
  fclose(dataFP);
  fclose(dictFP);

  //Return fully populated strand
  return s;
}

// REQUIRED

bool is_word_in_strand(strand_t* strand, char* word) {

  //Given word, checks if it is in the strand word array
  for (int i = 0; i < strand->numWords; ++i) {
    if (strcmp(strand->words[i], word) == 0) {
      return true;
    }
  }
  return false;
}

// REQUIRED

void print_strand_matrix(strand_t* strand) {
  
  int x = 0;
  //Prints strand matrix based on rows and columns
  for (int i = 0; i < ((2 * strand->rows) + 1); ++i) {
    printf("\t");
    if (i % 2 == 0) {
      //Every second line, prints the dashes and pluses
      for (int j = 0; j < (strand->cols - 1) * 4 + 5; ++j) {
	if (j % 4 == 0) {
	  printf("+");
	}
	else {
	  printf("-");
	}
      }
      printf("\n");
    }
    else {
      int y = 0;
      //If not a dashes and pluses line, prints out the bars and letters
      for (int j = 0; j < (strand->cols - 1) * 4 + 5; ++j) {
	if ((j % 2 == 0) && (j % 4 == 0)) {
	  printf("|");
	}
	else if ((j % 2 == 0) && (j % 4 != 0)) {
	  printf("%c", strand->letters[x][y++]);
	}
	else {
	  printf(" ");
	}
      }
      x++;
      printf("\n");
    }
  }
  
}

// REQUIRED

void free_strand(strand_t* strand) {
  
  //Free memory for all parts of the strand
  for (int i = 0; i < strand->numWords; ++i) {
    free(strand->words[i]);
  }
  free(strand->words);
  //Free memory for letters array
  free_2d_array(strand->letters, strand->rows);
  //Free memory within dictionary array
  for (int i = 0; i < strand->dictSize; ++i) {
    free(strand->dictionary[i]);
  }
  //Free memory for dictionary
  free(strand->dictionary);
  //Free entire strand struct
  free(strand);
}
