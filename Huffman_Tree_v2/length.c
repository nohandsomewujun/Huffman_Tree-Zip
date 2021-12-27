#include <stdio.h>
#include <string.h>
int main()
{
    FILE *fp;
    char *str;
    long sz;
    scanf("%s",str);
    fp = fopen(str,"rb");
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    printf("%ld",sz);
    printf("%ld",strlen("sahdiuashcfijasnckjansjkcnaksjncjasncijasnckjasncjksa"));
}