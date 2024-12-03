#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define DEFAULT_STRING_SIZE 16

String StringFromConst(const char* string) {
    int size = strlen(string);
    String self = {
        .size = size,
        .len = 0,
        .chars = malloc(sizeof(char) * size)
    };
    strcpy(self.chars, string);
    return self;
}

String StringNew() {
    return (String) {
        .size = DEFAULT_STRING_SIZE,
        .len = 0,
        .chars = malloc(sizeof(char) * DEFAULT_STRING_SIZE)
    };
}

void StringClear(String* str) {
    str->len = 0;
    str->size = 0;
    str->len = 0;
}
