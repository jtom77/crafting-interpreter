#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
  Chunk chunk;
  initChunk(&chunk);
  writeChunk(&chunk, OP_RETURN, 123);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  constant = addConstant(&chunk, 1.5);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  constant = addConstant(&chunk, 77.7);
  writeChunk(&chunk, OP_CONSTANT, 124);
  writeChunk(&chunk, constant, 125);
 
  for (int i=0; i < 100; i++) {
    constant = addConstant(&chunk, 100.0);
    writeChunk(&chunk, OP_CONSTANT, i * 12);
    writeChunk(&chunk, constant, i * 12);
  }

  disassembleChunk(&chunk, "test_chunk");
  freeChunk(&chunk);
  return 0;
}
