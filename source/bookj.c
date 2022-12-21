#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int peek(FILE *fp, int index, char expected) {
  char c = fgetc(fp);
  if (c == expected) {
    return 1;
  }
  fseek(fp, index, SEEK_SET);
  return 0;
}

int find(FILE *fp, char* character_name) {
  int index = 0;
  char c; 
  while ((c = fgetc(fp)) != EOF) {
    if (c == character_name[0]) {
      if (peek(fp, index, character_name[1]) == 1) {
	index++;
	return index;
      } else {	
	c = fgetc(fp);
      }
    }
    index++;
  }

  return -1;
}

char* retrieve_name(FILE *fp, int index, char* name) {
  int length = 0;
  char c;

  fseek(fp, index+1, SEEK_SET);
  
  while ((c = fgetc(fp)) != '!' && (c != EOF)) {
    length++;
  }

  fseek(fp, index+1, SEEK_SET);
  
  char* name = malloc(sizeof(char) * length);
  fread(name, sizeof(char), length, fp);

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

  int index = find(fp, "!C");
  retrieve_name(fp, index);
  //  printf("Length: %d", length);
  
  fclose(fp);
}

/* Data file format

   The tool should be able to be:
       Be sectioned into: Books | Characters + Events | Relationships between Characters/Events | Relationship Notes 


   Build a structure for Characters/Events, holding Character/Event name and ID, relationship list (pointers to relationship structures)
   Build a structure for Relationships (identified by Name1!Name2), holding notes array
   Each Entity struct has a hash map, mapping names to Relationship structures
   Example:
   Toranaga has ID !E12
   Anjin-san has ID !E19

   "Toranaga" maps to struct Entity {
                                      name = "Toranaga";
                                      id = !E12;
				      relationships = ["Anjin-san" -> struct Relationship{}];
				    }
   

   To look through an Entity's relationships for a queried relationship:
   while (END CONDITION || relationship.name has ENTITY NAME) {}
   
   !B Book
   !E Entity

   Example operation:

   $Add book: Shogun

   --
   !BShogun
   --

   $Add character: "Toranaga"

   --
   !BShogun
   !EToranaga
   !ID0
   !R
   --

   $Add character: "Anjin-san"

   --
   !BShogun
   !EToranaga
   !ID0
   !R
   !EAnjin-san
   !ID1
   !R
   --

   $Add relationship: Toranaga and Anjin-san

   --
   !BShogun
   !EToranaga
   !ID0
   !RAnjin-san
   !EAnjin-san
   !ID1
   !RToranaga
   --

   $Update relationship: Toranaga and Anjin-san
   $Relationship notes: Empty
   $Add note: Toranaga meets Anjin-san at Osaka Castle

   --
   !BShogun
   !EToranaga
   !ID0
   !RAnjin-san
   !EAnjin-san
   !ID1
   !RToranaga
   !RS
   !RAnjin-san+Toranaga
   !N1Toranaga meets Anjin-san at Osaka Castle
   --
   
*/
