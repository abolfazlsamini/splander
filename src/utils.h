#ifndef _UTILS_H_
#define _UTILS_H_

typedef struct {
    int size;
    int len;
    char* chars;
} String;

String StringNew();
String StringFromConst(const char* string);
void StringClear(String* str);

#endif
