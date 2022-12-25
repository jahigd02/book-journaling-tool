#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int peek(FILE *fp, int index, char expected);
int find(FILE *fp, char* character_name);
char* retrieve_name(FILE *fp, int index);

// Peek forwards into the file, seeing what char is there
int peek(FILE *fp, int index, char expected) {
  char c = fgetc(fp);
  //  printf("Next character: %c\n", c);
  if (c == expected) {
    fseek(fp, index, SEEK_SET);
    return 1;
  }
  fseek(fp, index, SEEK_SET);
  return 0;
}

int find(FILE *fp, char* character_name) {
  int index = 0;
  char c;
  
  while ((c = fgetc(fp)) != EOF) {
    // fp points to character after !, not ! itself, after the fgetc(fp) in the while
    if (c == '!' && peek(fp, index, 'C') == 1) {
      // so we do index + 1 to tell it to start not at the fp (!+1) but the name (!+2)/(C+1)
      char *temp_name;
      if (strcmp(temp_name = retrieve_name(fp, index+1), character_name) == 0) {
	printf("Character %s found at index %d\n", character_name, index);
	return index;
      } else {
	printf("Character %s not found, found %s instead\n", character_name, temp_name);
      }
    }
    index++;
  }
  return -1;
}

char* retrieve_name(FILE *fp, int index) {
  int length = 0;
  char c;

  fseek(fp, index, SEEK_SET);

  while ((c = fgetc(fp)) != '!' && (c != EOF)) {
    length++;
  }
  fseek(fp, index, SEEK_SET);
  
  char* name = malloc(sizeof(char) * length);
  fread(name, sizeof(char), length-1, fp);
  // in the event that it fails to find the name, we need to seek back to the entry indicator ('C', etc), which is index-1
  fseek(fp, index-1, SEEK_SET);
  
  return name;
}

int main (int argc, char* argv[]) {
  FILE *fp;
  if ((fp = fopen(argv[1], "r+")) != NULL) {
    printf("Data file \"%s\" opened.\n", argv[1]);
  } else {
    printf("Error opening file \"%s\"\n", argv[1]);
    fclose(fp);
    exit(-1);
  }
  int index = find(fp, argv[2]);
  fclose(fp);
}
