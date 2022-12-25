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
    if (c == '!' && peek(fp, index, 'C') == 1) {
      if (strcmp(retrieve_name(fp, index+1), character_name) == 0) {
	printf("Character %s found at index %d\n", character_name, index);
	return index;
      }
    }
    index++;
  }
  return -1;
}

char* retrieve_name(FILE *fp, int index) {
  int length = 0;
  char c;
  index = index;
  fseek(fp, index, SEEK_SET);
  
  while ((c = fgetc(fp)) != '!' && (c != EOF)) {
    if (c != '\n')
      length++;
  }
  
  printf("\n");
  
  fseek(fp, index, SEEK_SET);
  
  char* name = malloc(sizeof(char) * length);
  fread(name, sizeof(char), length, fp);

  fseek(fp, index, SEEK_SET);
  
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
