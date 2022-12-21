#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int peek(FILE *fp, int index, char expected);
int find(FILE *fp, char* character_name);
char* retrieve_name(FILE *fp, int index);

// Peek forwards into the file, seeing what char is there. 
int peek(FILE *fp, int index, char expected) {
  char c = fgetc(fp);
  if (c == expected) {
    fseek(fp, index, SEEK_SET);
    return 1;
  }
  return 0;
}

int find(FILE *fp, char* character_name) {
  int index = 0;
  char c;
  
  while ((c = fgetc(fp)) != EOF) {
    // fgetc() will consume the character at index N, so the fp is really pointing to index + 1 after the call.
    // as a result, retrieve_name will start at the character after !, not at the first letter of the Character's name.
    // so, in retrieve_name, we add 1 to index to point it to the first letter of the Character's name. fseek() brings the fp there.
    
    if (c == '!' && peek(fp, index, 'C') == 1) {
      if (strcmp(retrieve_name(fp, index), character_name) == 0) {
	printf("Character %s found at index %d\n", character_name, index);
	return index;
      }
    }
    index++;
  }
  printf("Character %s not found.", character_name);
  return -1;
}

char* retrieve_name(FILE *fp, int index) {
  int length = 0;
  char c;
  index = index + 1;
  fseek(fp, index, SEEK_SET);
  
  while ((c = fgetc(fp)) != '!' && (c != EOF)) {
    printf("%c", c);
    if (c != '\n')
      length++;
  }

  fseek(fp, index, SEEK_SET);
  
  char* name = malloc(sizeof(char) * length);
  fread(name, sizeof(char), length, fp);

  fseek(fp, index - 1, SEEK_SET);

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

  int index = find(fp, "Anjin");
  //  retrieve_name(fp, index, "Toranaga");
  //  printf("Length: %d", length);
  
  fclose(fp);
}
