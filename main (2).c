#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

int countWordsOfLength(char* filename, int wordSize) { 
    //---------------------------------------------------------
    // TODO - write countWordsOfLength()    
    //      open a file with name <filename> and count the 
    //      number of words in the file that are exactly 
    //      <wordSize> letters long, where a "word" is ANY set
    //      of characters that falls between two whitespaces 
    //      (or tabs, or newlines, etc.)
    //          return the count, if filename is valid
    //          return -1 if the file cannot be opened
    //---------------------------------------------------------
    FILE* file = fopen(filename, "r"); // open file in raed mode
    if (file == NULL) {
        return -1; // check if file couldn't be opened
    }
    int count = 0; // initialize word count to 0
    char word[100]; //hold each word read from the file
    while (fscanf(file, "%s", word) == 1) { //read words from file one by one
        if (strlen(word) == wordSize) { //check if word length matches wordSize
            count++; // increment count if word matches size
        }
    }
    fclose(file); //close file
    return count; //return number of matching words
}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    //---------------------------------------------------------
    // TODO - write buildWordArray()    
    //      open a file with name <filename> and fill the 
    //      pre-allocated word array <words> with only words
    //      that are exactly <wordSize> letters long;
    //      the file should contain exactly <numWords> words 
    //      that are the correct number of letters; thus, 
    //      <words> is pre-allocated as <numWords> char* ptrs, 
    //      each pointing to a C-string of legnth <wordSize>+1;
    //          return true iff the file is opened successfully
    //                      AND the file contains exactly 
    //                      <numWords> words of exactly 
    //                      <wordSize> letters, and those words
    //                      are stored in the <words> array
    //          return false otherwise
    //---------------------------------------------------------
    FILE* file = fopen(filename, "r"); //open file in read mode
    if (file == NULL) { 
        return false; // check if file couldn't be opened
    }
    int count = 0; // initialize count of found words to 0
    char word[100]; //hold each word read from the file
    while (fscanf(file, "%s", word) == 1) { // read words from file one by one
        if (strlen(word) == wordSize) { // check if word length matches wordSize
            if (count < numWords) { // check if we haven't filled the array yet
                strcpy(words[count], word); // copy word to words array
                count ++; // for each valid word, increment count
            } else {
                fclose(file); // close the file if array is full
                return false;
            }
        }
    }
    fclose(file); //close the file
    if (count != numWords) {
        return false; // return false if count doesn't match numWords
    } else {
        return true; // return true if count matches numWords
    }
}

int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    //---------------------------------------------------------
    // TODO - write findWord()
    //          binary search for string <aWord> in an 
    //          alphabetically sorted array of strings <words>, 
    //          only between <loInd> & <hiInd>
    //              return index of <aWord> if found
    //              return -1 if not found b/w loInd & hiInd
    //---------------------------------------------------------
    if (loInd > hiInd) { //check range
        return -1; // word not found
    }
    int midInd = loInd + (hiInd - loInd) / 2; //find middle index
    int compare = strcmp(words[midInd], aWord); // compare middle word with aWord
    if (compare == 0) { //words match
        return midInd; //return the index where aWord was found
    } else if (compare < 0) { // middle word is less than aWord
        return findWord(words, aWord, midInd + 1, hiInd); //upper half search
    } else {
        return findWord(words, aWord, loInd, midInd - 1); //lower half search
    }
}

void freeWords(char** words, int numWords) {
    //---------------------------------------------------------
    // TODO - write freeWords()
    //          free up all heap-allocated space for <words>,
    //          which is an array of <numWords> C-strings
    //           - free the space allocated for each C-string 
    //           - then, free the space allocated for the array
    //                  of pointers, <words>, itself
    //---------------------------------------------------------
    for (int i = 0; i < numWords; i++) {
        free(words[i]); // free each word's memory
    }
    free(words);
}

void insertWordAtFront(WordNode** ladder, char* newWord) {
    //---------------------------------------------------------
    // TODO - write insertWordAtFront()
    //          allocate space for a new [WordNode], set its 
    //          [myWord] subitem using <newWord> and insert
    //          it to the front of <ladder>, which is a 
    //          pointer-passed-by-pointer as the head node of 
    //          ladder changes inside this function; 
    //          <newWord> is a pointer to a C-string from the 
    //          full word array, already heap-allocated
    //---------------------------------------------------------
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode)); // allocate memory for new node
    newNode->myWord= newWord; // set the word for the new node
    newNode->next = *ladder; // point new node to the current head
    *ladder = newNode; // update ladder to point to the new head
}

int getLadderHeight(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write getLadderHeight()
    //          find & return number of words in <ladder> list
    //---------------------------------------------------------
    int ladderHeight = 0; // initialize height counter to 0
    while(ladder) { // loop until null ladder
        ladderHeight++; //increment height
        ladder = ladder->next; // next node
    }
    return ladderHeight; // return the total height of the ladder
}

WordNode* copyLadder(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write copyLadder()
    //          make a complete copy of <ladder> and return it;
    //          the copy ladder must have new space allocated
    //          for each [WordNode] in <ladder>, BUT the
    //          C-string pointers to elements of the full word  
    //          array can be reused; i.e. the actual words do 
    //          NOT need another allocation here
    //---------------------------------------------------------
    if (ladder == NULL) { //ladder is empty
        return NULL; // return null
    }
    WordNode* nodeCopy = malloc(sizeof(WordNode)); // allocate memory for new node
    nodeCopy->myWord = ladder->myWord; // copy the word pointer
    nodeCopy->next = copyLadder(ladder->next); //copy the rest of the ladder
    
    return nodeCopy; //return copied ladder
}

void freeLadder(WordNode* ladder) {
    //---------------------------------------------------------
    // TODO - write freeLadder()
    //          free up all heap-allocated space for <ladder>;
    //          this does NOT include the actual words, 
    //          instead just free up the space that was 
    //          allocated for each [WordNode]
    //---------------------------------------------------------
    while (ladder != NULL) { //loop until ladder is empty
        WordNode* temporaryNode = ladder; // store current node
        ladder = ladder -> next; // next node
        free(temporaryNode); //free current node
    }
}

void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    //---------------------------------------------------------
    // TODO - write insertLadderAtBack()
    //          allocate space for a new [LadderNode], set its 
    //          [topWord] subitem using <newLadder>; then, find
    //          the back of <list> and append the newly created
    //          [LadderNode] to the back; Note that <list> is a 
    //          pointer-passed-by-pointer, since this function 
    //          must handle the edge case where <list> is empty
    //          and the new [LadderNode] becomes the head node
    //---------------------------------------------------------        
    while (*list != NULL) { // loop until end of the list
        list = &(*list)->next; //next node
    }
    *list = (LadderNode*)malloc(sizeof(LadderNode)); // allocate memory for new node
    (*list)->topWord = newLadder; // set the topWord to newLadder
    (*list)->next = NULL; // set next pointer to null
}

WordNode* popLadderFromFront(LadderNode** list) {
    //---------------------------------------------------------
    // TODO - write popLadderFromFront()
    //          pop the first ladder from the front of the list
    //          by returning the pointer to the head node of 
    //          the ladder that is the subitem of the head node
    //          of <list> AND updating the head node of <list>
    //          to the next [LadderNode]; Note that <list> is a 
    //          pointer-passed-by-pointer, since this function 
    //          updates the head node to be one down the list;
    //          the [LadderNode] popped off the front must have
    //          its memory freed in this function, since it 
    //          will go out of scope, but the ladder itself, 
    //          i.e. the head [WordNode], should NOT be freed. 
    //---------------------------------------------------------
    WordNode* ladder = (*list)->topWord; // get the ladder from the first node
    LadderNode* followingNode = (*list)->next; // get the next node
    free (*list); // free the first node
    *list = followingNode; // update the head of the list
    
    return ladder; // return ladder from the popped node
}

void freeLadderList(LadderNode* myList) {
    //---------------------------------------------------------
    // TODO - write freeLadderList()
    //          free up all heap-allocated space for <myList>;
    //          for each ladder in <myList>:
    //           - free the space allocated for each [WordNode] 
    //                  in the ladder using freeLadder()
    //           - then, free the space allocated for the 
    //                  [LadderNode] itself 
    //---------------------------------------------------------
    while(myList != NULL){ // loop until list is empty
        LadderNode* tempNode = myList; // store current node
        freeLadder(myList->topWord); // free ladder in current node
        myList = myList->next; // next node
        free(tempNode);// free current node
    }
}

WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord,  
                                    char* finalWord ) {
    //---------------------------------------------------------
    // TODO - write findShortestWordLadder()
    //          run algorithm to find the shortest word ladder
    //          from <startWord> to <finalWord> in the <words>
    //          word array, where each word is <wordSize> long 
    //          and there are <numWords> total words;
    //          <usedWord> also has size <numWords>, such that
    //          usedWord[i] is only true if words[i] has 
    //          previously be entered into a ladder, and should
    //          therefore not be added to any other ladders; 
    //          the algorithm creates partial word ladders, 
    //          which are [WordNode] linked lists, and stores 
    //          them in a [LadderNode] linked list. 
    //              return a pointer to the shortest ladder;
    //              return NULL if no ladder is possible;
    //                  before return, free all heap-allocated 
    //                  memory that is created here that is not 
    //                  the returned ladder
    //---------------------------------------------------------
    LadderNode* followingLadder = NULL; //initialize list of ladders
    WordNode* initialLadder = NULL; // initialize first ladder
    insertWordAtFront(&initialLadder, startWord); //insert start word to ladder
    insertLadderAtBack(&followingLadder, initialLadder); // insert first ladder to list
    while(followingLadder != NULL) { // loop until no ladders are left
        WordNode* currentLadder = popLadderFromFront(&followingLadder); // get first ladder
        if (strcmp(currentLadder -> myWord, finalWord) == 0) { // check if reached the final word
            freeLadderList(followingLadder); // free all other ladders
            return currentLadder; // return shortest ladder found
        }
        for (int i = 0; i < numWords; i++) { // loop through each word
            if (usedWord[i] == false) { // check if the word has not been used
                int differentChar = 0; // initialize different character count
                for (int j = 0; (j < wordSize) && (differentChar <= 1); j++) { // loop through each character
                    if (currentLadder->myWord[j] != words[i][j]) { // characters differ
                        differentChar++; // increment difference count
                    }
                }
                if (differentChar == 1) { //exactly one character is different
                    usedWord[i] = true; // mark word as used
                    WordNode* newLadder = copyLadder(currentLadder); // copy current ladder
                    insertWordAtFront(&newLadder, words[i]); // add the new word to the front
                    insertLadderAtBack(&followingLadder, newLadder); // add the new ladder to the list
                }
            }
        }
        freeLadder(currentLadder); // free the current ladder after using it
    }
    return NULL; // null if no ladder was found
}

// interactive user-input to set a word; 
//  ensures sure the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}


//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main(), except for 
//  testing-related purposes (such as command-line 
//  arguments for "TESTING MODE" to call a test case 
//  master function, or something similar)
//-----------------------------------------------------
int main() {
    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
