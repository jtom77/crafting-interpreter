#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include "object.h"


static void repl() {
  char line[1024];
  for (;;) {
    printf("> ");

    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\". \n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\". \n", path);
    exit(74);
  }

  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    fprintf(stderr, "Coould not read file \"%s\". \n", path);
    exit(74);
  }

  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

static void runFile(const char* path) {
  char* source = readFile(path);
  InterpretResult result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR) exit(65);
  if (result == INTERPRET_RUNTIME_ERROR) exit(70);

}

int testTable(void) {
  initVM();                 // required: sets up vm.strings + object list

  Table table;
  initTable(&table);

  char* entries[3][2] = {
    {"key1", "val1"},
    {"key2", "val2"}, 
    {"key3", "val3"}
  };

  for (int i=0; i < 3; i++) {
    // printf("Key=%s, Value=%s\n", entries[i][0], entries[i][1]);

    // key and value are both ObjString*, created from C literals
    ObjString* key = copyString(entries[i][0], 4);
    ObjString* value = copyString(entries[i][1], 4);

    // store: key is ObjString*, value is a Value wrapping the ObjString*
    tableSet(&table, key, OBJ_VAL(value));

    // read it back
    Value retrieved;
    // if (tableGet(&table, key, &retrieved) && IS_STRING(retrieved)) {
    //   printf("%s = %s\n", key->chars, AS_CSTRING(retrieved));
    // }

    debugTable(&table);
  }
  freeTable(&table);
  freeVM();
  return 0;
}

int main(int argc, const char* argv[]) {
  initVM();

  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }

  Chunk chunk;
  initChunk(&chunk);

  freeVM();
  freeChunk(&chunk);

  return 0;
}
