#include <stdio.h>
#define MAX 100

int match(char pattern[], char text[]) {
    int i, j, k, counter = 0, topCount = 0, topIndex = 0;
    for (i = 0; text[i] != '\0'; i++) {
        counter = 0;
        for (j = 0, k = i; pattern[j] != '\0' && text[k] != '\0' && pattern[j] == text[k]; j++, k++, counter++);

        if (counter > topCount) {
            topCount = counter;
            topIndex = i;
        }
    }
    return topIndex;
}

int main(void) {
    char a[MAX], b[MAX];
    int position = 0;
        
        printf("Enter pattern to find\n");
        scanf("%s", a);

        printf("Enter text to look\n");
        scanf("%s", b);

        position = match(a, b);
        if (position == 0) {
            printf("The is perfect match between the pattern and the text.\n\n");
        } else {
            printf("The lowest number of un-match position is %d.\n\n", position);
        }

    return 0;
}