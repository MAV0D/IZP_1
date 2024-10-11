// Phonebook search program with support of T9 coding
// returns list of all contacts containingsequence on stdin to stdout
// Vojtech Zmatlo (xzmatlv00), 10/2024


#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void printAllContacts() // Prints all contacts, used in case of no input sequence
{
  int c;

  while ((c=getchar())!=EOF)
  {
    putc(c,stdout);
  }
}

void resetCounters(int *lenghtCounter, int* lineCounter, char *contactArray) {
  *lenghtCounter=0;
  *lineCounter=1;
  for (int i=0;i<201;i++) {
    contactArray[i]=0;
  }
}

bool isSuitable(char contact[], char sequenceToFind[]) {
  int sequenceLenght=0;
  while (sequenceToFind[++sequenceLenght]!=0);
  int contactLenght=0;
  while (contact[++contactLenght]!=0);
  for (int i=0;i<contactLenght-sequenceLenght;i++) {
    if (contact[i]==sequenceToFind[0]) {
      for (int j=1;j<sequenceLenght;j++) {
        if (contact[i+j]!=sequenceToFind[j]) break;
        return 1;
      }
    }
  }

  return 0;
}

void handleContact(char contact[], char sequenceToFind[], int *lenght, int *lines) {
  if (isSuitable(contact, sequenceToFind)) {
    contact[++*lenght] = 0;
    printf("%s", contact);
  }
  resetCounters(lenght, lines, contact);
}

char findSuitableContacts(char sequenceToFind[]) {
  int c;
  int contactLenght=0;
  int contactLines=1;
  char currentContact[201];
  resetCounters(&contactLenght, &contactLines, currentContact);
  while ((c=getchar())!=EOF)
  {
    currentContact[contactLenght++]=c;
    if (c=='\n') contactLines++;
    if (contactLines>2) {
      handleContact(currentContact, sequenceToFind, &contactLenght, &contactLines);
      continue;
    }
    if (contactLenght>=200) {
      printf("Invalid input data. \n");
      return 1;
    }
  }

  handleContact(currentContact, sequenceToFind, &contactLenght, &contactLines);
  
  return 0;
}

int main(int argc, char *argv[]) {

  if (argc==1) // edge case if no seqeunce is entered
  {
    printAllContacts();
    return 0;
  }

  ///////////////////////////////////////////////////

  char sequence[101];
  strcpy(sequence, argv[1]);

  findSuitableContacts(sequence);

  return 0;
}