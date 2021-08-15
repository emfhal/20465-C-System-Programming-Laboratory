#include <stdio.h>
#define MAX 100

void f_sequence(char str[]) {
    int increase = 0, decrease = 0, equals = 0, i = 0;
    if (!str[0])
        printf("The string is empty\n");
    else if (!str[1])
        printf("The string must have at least 2 char in a row\n");
    else {
        for (i = 1; str[i]; i++) {
            if (str[i] == str[i - 1])
                equals = 1;
            if (str[i] > str[i - 1])
                increase = 1;
            if (str[i] < str[i - 1])
                decrease = 1;
        }
        if ((decrease == 1) && (increase == 0) && (equals == 0))
            printf("We have a very decreasing sequence!");
        else if ((decrease == 0) && (increase == 1) && (equals == 0))
            printf("We have a very increasing sequence!");
        else if ((decrease == 1) && (increase == 0) && (equals == 1))
            printf("We have a decreasing sequence!");
        else if ((decrease == 0) && (increase == 1) && (equals == 1))
            printf("We have an increasing sequence!");
        else if ((decrease == 0) && (increase == 0) && (equals == 1))
            printf("We have an increasing sequence!");
        else if ((decrease == 1) && (increase == 1))
            printf("We have not an increasing/decreasing sequence!");
        puts("");
    }
}

int main(void) {
    char str[MAX];

    printf("Please input your string:\n");
    while (scanf("%s", str) != EOF) {
        f_sequence(str);
    }
    return 0;
}