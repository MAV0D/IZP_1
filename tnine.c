// Phonebook search program with support of T9 coding
// returns list of all contacts containing sequence on stdin to stdout
// Vojtech Zmatlo (xzmatlv00), 10/2024

// TODO 
// testovani
// komentare
// prerusene posloupnosti znaku

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

void printAllContacts()
// Prints all contacts, used in case of no input sequence
{
  int c;

  while ((c=getchar())!=EOF)
  {
    putc(c,stdout);
  }
}

void resetCounters(int *lenghtCounter, int* lineCounter, char *contactArray)
// resets temp variables
{
  *lenghtCounter=0;
  *lineCounter=1;
  for (int i=0;i<201;i++) {
    contactArray[i]=0;
  }
}

bool isAdequateCharacter(char contactCharacter, char sequenceCharacter) {
  // returns if the contactCharacter is the same as sequenceCharacter in t9
  if (contactCharacter==sequenceCharacter) return 1;

  int numberFromSequence = sequenceCharacter-'0';
  char representedCharacter;

  for (int i=0;i<4;i++) {
    representedCharacter = 'a'+(numberFromSequence-2)*3+i;
    if ((i==3) && !(numberFromSequence==7 || numberFromSequence==9)) break;
    if (numberFromSequence>7) representedCharacter++;
    if (representedCharacter==contactCharacter) return 1;
  }

  return 0;
}

bool isSuitable(char contact[], char sequenceToFind[], bool mustBeSubsequent) 
// Returns whether contact contains given sequence
{
  
  int sequenceLenght=0;
  while (sequenceToFind[++sequenceLenght]!=0);
  int contactLenght=0;
  while (contact[++contactLenght]!=0);
  for (int i=0;i<contactLenght-sequenceLenght;i++) {
    if (isAdequateCharacter(tolower(contact[i]),sequenceToFind[0])) {
      int k=0;
      for (int j=1;j<=sequenceLenght;j++) {
        if (j==sequenceLenght) return 1; // when whole sequence is found
        if (mustBeSubsequent) {
          if (!isAdequateCharacter(tolower(contact[i+j]), sequenceToFind[j])) break;
        }
        else {
          bool newLine = 0;
          while (!isAdequateCharacter(tolower(contact[i+j+k]), sequenceToFind[j])) {
            k++;
            if (contact[i+j+k]==0) return 0;
            if (contact[i+j+k]==' ') {
              newLine = 1;
              break;
            }
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
// handles contact for printing, resets temp variables
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


int findSuitableContacts(char sequenceToFind[], bool mustBeSubsequent)
// loops through input string, splits it into contacts and calls handleContact on each
{
  bool isSomeContactSuitable = 0;
  bool isCurrentContactSuitable = 0;
  int c;
  int contactLenght=0;
  int contactLines=1;
  char currentContact[201];
  resetCounters(&contactLenght, &contactLines, currentContact);
  while ((c=getchar())!=EOF)
  {
    currentContact[contactLenght++]=c;
    if (c=='\n') {
      if (contactLines==1) {
        currentContact[contactLenght-1] = ',';
        currentContact[contactLenght] = ' ';
        contactLenght++;
      } else {
        currentContact[contactLenght-1] = 0;
      }
      contactLines++;
    }
    if (contactLines>2) {
      isCurrentContactSuitable = handleContact(
        currentContact, sequenceToFind, &contactLenght, &contactLines, mustBeSubsequent);
      isSomeContactSuitable = (isCurrentContactSuitable || isSomeContactSuitable);
      continue;
    }
    if (contactLenght>=200) {
      fprintf(stderr, "Invalid input data. \n");
      return -1;
    }
  }

  isCurrentContactSuitable = 
    handleContact(currentContact, sequenceToFind, &contactLenght, &contactLines, mustBeSubsequent);
  isSomeContactSuitable = (isCurrentContactSuitable || isSomeContactSuitable);

  return isSomeContactSuitable;
}

int main(int argc, char *argv[]) {

  if (argc==1) // edge case if no seqeunce is entered
  {
    printAllContacts();
    return 0;
  }

  int inputValidity;

  if (argc == 3 && !strcmp(argv[1], "-s")) {
    inputValidity = findSuitableContacts(argv[2], 0);
  } else {
    inputValidity = findSuitableContacts(argv[1], 1);

  }

  if (!inputValidity) printf("Not found\n");  

  return inputValidity;
}