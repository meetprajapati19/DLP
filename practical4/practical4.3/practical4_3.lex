%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int char_count = 0, word_count = 0, line_count = 0;
int in_word = 0;
%}

%%

.          { char_count++; }  
\n         { char_count++; line_count++; in_word = 0; }  
[ \t]+     { char_count++; in_word = 0; }  
[a-zA-Z0-9]+ { char_count += yyleng; if (!in_word) { word_count++; in_word = 1; } }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    yyin = file;
    yylex();
    fclose(file);

    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);

    return 0;
}

int yywrap() {
    return 1;
}
