// Phonebook search program with support of T9 coding
// returns list of all contacts containing sequence on stdin to stdout
// Vojtech Zmatlo (xzmatlv00), 10/2024

// TODO 
// testovani

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


void resetCounters(int *lenghtCounter, int* lineCounter, char *contactArray)
// resets variables keeping info about contact
{
  *lenghtCounter=0;
  *lineCounter=1;
  for (int i=0;i<201;i++) {
    contactArray[i]=0;
  }
}

bool isAdequateCharacter(char contactCharacter, char sequenceCharacter) {
  // returns if the contactCharacter is the same as sequenceCharacter in t9

  // in case when numbers equal
  if (contactCharacter==sequenceCharacter) return 1;

  // getting rid of 0 in sequence character
  if (sequenceCharacter=='0') {
    if (contactCharacter=='+') return 1;
    return 0;
  }

  int numberFromSequence = sequenceCharacter-'0';
  char representedCharacter;

  // adds i to numberFromSequence*3 to get all possible letters (skips i==4 if number represents
  // only 3 letters and adds one for numbers above 7, returns 1 if characters match
  for (int i=0;i<4;i++) {
    representedCharacter = 'a'+(numberFromSequence-2)*3+i;
    if ((i==3) && !(numberFromSequence==7 || numberFromSequence==9)) break;
    if (numberFromSequence>7) representedCharacter++;
    if (representedCharacter==contactCharacter) return 1;
  }

  // if nothing matched, return 0
  return 0;
}

bool isSuitable(char contact[], char sequenceToFind[], bool mustBeSubsequent) 
// Returns whether contact contains given sequence
{
  int sequenceLenght=strlen(sequenceToFind);
  int contactLenght=strlen(contact);

  if (sequenceLenght==0) return 1; // when blank sequence is passed for printing everything

  // loop through all characters in contact, try to find whole sequence when it finds the first
  for (int i=0;i<=contactLenght-sequenceLenght;i++) {
    if (isAdequateCharacter(tolower(contact[i]),sequenceToFind[0])) {
      int k=0;
      for (int j=1;j<=sequenceLenght;j++) 
      // loops through sequence to find, tries to find each character
      {
        if (j>=sequenceLenght) return 1; // when whole sequence is found
        if (mustBeSubsequent) // if sequence must be found as one piece 
        {
          // if letter isn't equal to number, otherwise continue
          if (!isAdequateCharacter(tolower(contact[i+j]), sequenceToFind[j])) break;
        }
        else // if sequence can be found with spaces between each character 
        {
          bool newLine = 0; // variable to break two loops
          while (!isAdequateCharacter(tolower(contact[i+j+k]), sequenceToFind[j])) 
          // if character doesn't match number, look on next character
          {
            if (contact[i+j+k]==',') 
            // resets progress in finding sequence if it reaches end of name
            {
              newLine = 1;
              break;
            }
            if (contact[i+j+k]==0) return 0; // returns 0 when it finds end of contact
            k++;
          }
          if (newLine) break;
        }
      }
    }
  }

  return 0;
}

bool handleContact
  (char contact[], char sequenceToFind[], int *lenght, int *lines, bool mustBeSubsequent) 
// handles contact for printing if sequence is found, resets temp variables
{
  bool suitable = 0;
  if (isSuitable(contact, sequenceToFind, mustBeSubsequent)) {
    contact[++*lenght] = 0;
    printf("%s\n", contact);
    suitable = 1;
  }
  resetCounters(lenght, lines, contact);
  return suitable;
}

int printAllContacts()
// Prints all contacts, used in case of no input sequence
{

  // variable initialization
  int c;
  int contactLenght=0;
  int lineLenght=0;
  int contactLines=1;
  char currentContact[201];
  resetCounters(&contactLenght, &contactLines, currentContact);
  
  
  while ((c=getchar())!=EOF)
  // loops through whole input
  {
    currentContact[contactLenght++]=c;
    lineLenght++;
    if (c=='\n') 
    // replace \n character on first line end with ", ", end contact on second line end
    {
      if (contactLines==1) {
        currentContact[contactLenght-1] = ',';
        currentContact[contactLenght] = ' ';
        contactLenght++;
      } else {
        currentContact[contactLenght-1] = 0;
      }
      contactLines++;
      lineLenght=0;
    }
    
    if (contactLines>2) 
    // special call of handleContact with no sequence
    {
      handleContact(currentContact, "", &contactLenght, &contactLines, 0);
      continue;
    }

    // handle longer line or contact than expected
    if (contactLenght>200||lineLenght>100) {
      fprintf(stderr, "Invalid input data. \n");
      return -1;
    }
  }

  // handles last contact
  handleContact(currentContact, "", &contactLenght, &contactLines, 0);
  return 0;
}

int findSuitableContacts(char sequenceToFind[], bool mustBeSubsequent)
// loops through input string, splits it into contacts and calls handleContact on each
{
  // check if sequenceToFind is numeric only
  int sequenceLenght=strlen(sequenceToFind);
  for (int i=0;i<sequenceLenght;i++) {
    if (!isdigit(sequenceToFind[i])) {
      fprintf(stderr, "Invalid argument, must be number");
      return -1;
    }
  }

  // variable initialization
  bool isSomeContactSuitable = 0;
  bool isCurrentContactSuitable = 0;
  int c;
  int contactLenght=0;
  int lineLenght=0;
  int contactLines=1;
  char currentContact[201];
  resetCounters(&contactLenght, &contactLines, currentContact);
  
  
  while ((c=getchar())!=EOF)
  // loops through whole input
  {
    if (contactLines == 1) { // checks if character is number or '+' while reading phone number
      currentContact[contactLenght++]=c;
    } else {
      if (!isdigit(c) && c!='+' && c!='\n') {
        fprintf(stderr, "Invalid phone number, it can only contain 0-9 and +");
        return -1;
      }
      currentContact[contactLenght++]=c;
    }
    lineLenght++;
    if (c=='\n')
    // replace \n character on first line end with ", ", end contact on second line end
    {
      if (contactLines==1) {
        currentContact[contactLenght-1] = ',';
        currentContact[contactLenght] = ' ';
        contactLenght++;
      } else {
        currentContact[contactLenght-1] = 0;
      }
      contactLines++;
      lineLenght=0;
    }
    
    if (contactLines>2)
    // pass contact to handleContact, update isSomeContactSuitable
    {
      isCurrentContactSuitable = handleContact(
        currentContact, sequenceToFind, &contactLenght, &contactLines, mustBeSubsequent);
      isSomeContactSuitable = (isCurrentContactSuitable || isSomeContactSuitable);
      continue;
    }

    // handle longer line or contact than expected
    if (contactLenght>200||lineLenght>100) {
      fprintf(stderr, "Invalid input data. \n");
      return -1;
    }
  }

  // handles last contact
  isCurrentContactSuitable = 
    handleContact(currentContact, sequenceToFind, &contactLenght, &contactLines, mustBeSubsequent);
  isSomeContactSuitable = (isCurrentContactSuitable || isSomeContactSuitable);

  return isSomeContactSuitable;
}

int main(int argc, char *argv[]) {
  if (argc>3) // edge case for invalid number of arguments
  {
    fprintf(stderr, "Invalid number of arguments (%i, maximum is 3)", argc);
    return -1;
  }

  int inputValidity; // variable for storing if input is valid and if some contacts has been found

  if (argc==1) // edge case if no seqeunce is entered
  {
    inputValidity = printAllContacts();
    return 0;
  }


  // switch between modes
  if (argc == 3 && !strcmp(argv[1], "-s")) {
    inputValidity = findSuitableContacts(argv[2], 0);
  } else if (argc==3) {
    fprintf(stderr, "Invalid number of arguments (3, but first isn't -s)");
    return -1;
  }
  
  else {
    inputValidity = findSuitableContacts(argv[1], 1);
  }

  if (!inputValidity) printf("Not found\n");  

  return (inputValidity<0)?inputValidity:0;
}
