#include "game.h"

// REQUIRED
//returns an opened data file
FILE * open_data_file(int argc, char** argv) {
  
  //Checks if data file is included in arguments
  if (argc >= 2) {
    FILE * data_file = fopen(argv[1], "r");
    return data_file;
  }
  //If data file not included, opens data file based on user input
  char fileName[100];
  scanf(" %s", fileName);
  FILE * data_file = fopen(fileName, "r");
  if (data_file!= NULL) {
    return data_file;
  }
  else {
    return NULL;
  }
}

// REQUIRED
//returns an opened dictionary file
FILE * open_dict_file(int argc, char** argv) {
  
  //Checks if dict file is included in arguments
  if (argc >= 3) {
    FILE * dict_file = fopen(argv[2], "r");
    return dict_file;
  }
  //Returns default dict file if not included in argv
  FILE * dict_file = fopen(DICT_PATH , "r");
  return dict_file;

}

// REQUIRED - DO NOT EDIT
void print_instructions(strand_t * strand) {
    printf("\n  +-- Welcome to Strands! --+\n");
    printf("\nInstructions:\n");
    printf("\tFind all %d %d-letter words\n", strand->numWords, strand->wordLen);
    printf("\tEach word's letters must touch\n");
    printf("\tA letter can be repeated in a word\n");
    printf("\tType a word then press 'Enter'\n");
    printf("\tType 'quit' when done\n");
}

// REQUIRED
//Runs full game, utilizing is_word_in_strand() to check user guesses
void play_game(strand_t * s) {

  //Creates and allocates memory for 2d pointer array previousGuesses that stores all user guesses
  char** previousGuesses = (char**)malloc(s->dictSize * sizeof(char*));
  for (int i = 0; i < s->dictSize; ++i) {
    //Allocates memory for each point in array
    previousGuesses[i] = (char*)malloc(100 * sizeof(char));
  }

  //integer to store number of correct words guessed
  int numCorrect = 0;

  //stores whether user wants to quit or not
  bool wantsToPlay = true;

  //Stores int of how many words have been guessed, useful for previousGuesses array
  int indexGuessed = 0;

  //continues running until user chooses "quit" option
  while (wantsToPlay) {

    //prompts user for guess or "quit"
    printf("Guess a word or 'quit':\n");
    char guess[100];
    scanf("%s", guess);

    //If user input is "quit"
    if (strcmp(guess, "quit") == 0) {
    wantsToPlay = false;
    }
    //Prints ending messages
    if (!wantsToPlay) {
      printf("You found %d out of %d %d-letter words.\n", numCorrect, s->numWords, s->wordLen);
      printf("Thanks for playing!\n");
    }

    //If user input is not quit, checks if guess is a duplicate
    bool validGuess = true;
    if (wantsToPlay) {
      for (int i = 0; i < indexGuessed; ++i) {
	if (strcmp(guess, previousGuesses[i]) == 0) {
	  validGuess = false;
	}
      }

      //If guess is a duplicate
      if (!validGuess) {
	printf("You have already guessed %s!\n", guess);
	printf("You have found %d of %d %d-letter words in the strand so far.\n\n", numCorrect, s->numWords, s->wordLen);
      }
      //If guess is correct
      else if (is_word_in_strand(s, guess)) {
	numCorrect++;
	printf("%s is a valid word in the strand!\n", guess);

	//If all correct words have been found
	if (numCorrect == s->numWords) {
	  wantsToPlay = false;
	  printf("You found all %d %d-letter words!\n", s->numWords, s->wordLen);
	  printf("Great job! Thanks for playing!\n");
	}
	else {
	  printf("You have found %d of %d %d-letter words in the strand so far.\n\n", numCorrect, s->numWords, s->wordLen);
	}
      }
      //If guess is incorrect
      else {
	printf("%s is not a valid word in the strand.\n", guess);
	printf("You have found %d of %d %d-letter words in the strand so far.\n\n", numCorrect, s->numWords, s->wordLen);
      }

      //Copies guess into the previousGuesses array
      strcpy(previousGuesses[indexGuessed++], guess);
      
    }
  }
  
}
