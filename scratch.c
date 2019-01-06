#include <stdio.h>
#include <stdlib.h>
//#include <ncurses.h>
//#include <unistd.h>

//char* d()  {
////    printf("\n\nin d\n");
//    char* dstr = "haphaphap";
////    printf("%s\n", dstr);
////    printf("end   %u\n", &dstr[8]);
////    printf("start %u\n", dstr);
////    printf("%u\n", &dstr);
////    printf("out d\n\n");
//    return dstr;
//}
//
const char* c() {
    char* cstr = "my string";
//    cstr[0] = 'z';
    return cstr;
}

int main() {
//    const char* str = c();
//    printf("%s\n", str);
    char* pa = malloc(1);
    *pa = 'a';
    char* pb = malloc(1);
    *pb = 'b';
    char* pc = malloc(1);
    *pc = 'c';
    char* pd = malloc(1);
    *pd = 'd';
    printf("%c, %c, %c, %c\n", *pa, *pb, *pc, *pd);
    //printf("%c, %c, %c, %c\n", *pa, *(pa+2), *pc, *pd);
    for (int i=0; i<100; i += 32) {
        printf("%c\n", *(pa+i));
    }

    return 0;
}
