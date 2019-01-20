/*
 * Nhan Phan
 * Program 1
 * Professor Carroll
 * CS570
 * cssc0072
 */

#include <stdlib.h>
#include "getword.h"


typedef int symbol;                                  // Pre-defined data type for special character
#define none 0                                       
#define dollar 1							       	 // '$'
#define backslash 2									 // '\'
#define tilde 3										 // '~'
#define lessthan 4									 // '<'
extern int backslashCheck;



int getword(char *w) {
    int iochar;                                      // Obtain character from the input stream 
    int countChar = 0;								 // Count everytime a character is put into a single word 
    char *streamPtr = w;                             // For (indirectly) return single word from memory
    symbol initialCheck = none;                      // Check if '$' appears at the beginning of a word 
    //char *homePath = getenv("HOME");
	symbol middleCheck = none;						 // Check if the special character appears in the middle of a word 
	int checkTilde = 0;

	while ( ( iochar = getchar() ) != EOF ) {
	 	if (countChar == 254){						 // Only accept word less than STORAGE size, the rest will be on next call
			ungetc(iochar,stdin);					 // Put back the 255th character of a word to the stream for next call
			*streamPtr = '\0';						 // End the word with null terminator for returning the word (indirectly)
			return countChar;
		}
		// This comment is used for debugging purposes only since this program is written completely in the vi editor
		//(void) printf("iochar=%c....count=%d...middleCheck=%d\n", iochar, countChar, middleCheck);
		if (middleCheck != none) {					 // Check if the special character is right before the currently obtained character 
			if (middleCheck == backslash) {		 	 // Every character after '/' is considered as normal character
				if (iochar != '\n') {				 // Except newline and EOF
					*streamPtr++ = iochar; 			 // Add the character to the word (for returning)
					countChar++;
				}
				middleCheck = none;					 // Must reset the flag to avoid infinite loop
				continue;
			}
			else if (middleCheck == lessthan) {		 // '<' case
				middleCheck = none;					 
				if (iochar == '<') {				 // '<<' case
					*streamPtr++ = iochar;
					countChar++;
				}
				else if (iochar != ' ')   
					ungetc(iochar, stdin);		 	 // Have to put back or else it will be ignored
				*streamPtr++ = '<';					 // Adding the previous '<'
				countChar++;
				*streamPtr = '\0';
				return (initialCheck == dollar) ? (countChar - 1) * -1 : countChar;
													 // Returns the negative count only the beginning of the word is '$'
			}
		}
		if (iochar == '\\') {						 // Look for '\' and turn on the flag once found
			middleCheck = backslash;
			backslashCheck = 1;
		}
		else if (checkTilde == 1) {
		}
        else if (iochar == ' '|| iochar == '\t') {   // Parse the word once reaching the normal delimiter
            if (countChar == 0)                      // Ignore leading spaces,tabs 
                continue;
            *streamPtr = '\0';
            return (initialCheck == dollar) ? (countChar - 1) * -1 : countChar;
        }
        else if (iochar == '\n'|| iochar == ';' ||  iochar == '<' || iochar == '>' || iochar == '|' || iochar == '&' || iochar =='~') {  
													 // Special delimiter parsing case, which are newline and metacharacters
            if (countChar != 0) {					 // When these delimiter is found after the word 
				if (iochar == '~') {				 // The tilde in the middle of the word is treat as normal character
					*streamPtr++ = iochar;
					countChar++;
					continue;
				}
                ungetc(iochar, stdin);               // Reaching end of a word, must put it back to input stream for next call  
				*streamPtr = '\0';
                return (initialCheck == dollar) ? (countChar - 1) * -1 : countChar;
            }
			else if (countChar == 0) {				 // When there's no word found before these special delimiter 
				/*if (iochar == '~' && checkTilde == 0) {
					checkTilde = 1;
					continue;
				}*/
				if (iochar == '~') {				 // The	tilde at the beginning will be replaced with the home directory
					/*int i;
					printf("Something....=%s=...\n", getenv("~"));
					for (i = 0; i < (int)strlen(homePath); i++) {
						*streamPtr++ = homePath[i];
						countChar++;
					}*/
					*streamPtr++ = iochar;
					countChar++;
					continue;
				}
				else if (iochar == '<') {	
					middleCheck = lessthan;
				}
				else if (iochar == ';' || iochar == '\n') {
				 									 // These are "end-of-word" delimiters
					*streamPtr = '\0';
					return 0;
				}
				else {								 // The rest are metacharacters, which should be treat as a word
					*streamPtr++ = iochar;
					*streamPtr = '\0';
					return 1;
				}
			}
        }
        else {                                       // Assembling string case
            if (countChar == 0 && iochar == '$')  
               	initialCheck = dollar;
			else 
                *streamPtr++ = iochar;
            countChar++;
        }
    }											     // Reach EOF
    *streamPtr = '\0';
    if (countChar == 0) 							 // When no more word is found 
        return -255;
    return (initialCheck == dollar) ? (countChar - 1) * -1 : countChar;
} 
