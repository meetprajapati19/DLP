%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int char_count = 0, word_count = 0, line_count = 0;
int whitespace_count = 0, tab_count = 0, special_char_count = 0;
int keyword_count = 0, operator_count = 0, identifier_count = 0, constant_count = 0, punctuation_count = 0;

// C keywords array
char *keywords[] = {"int", "float", "char", "double", "if", "else", "while", "for", "return", "void", 
                    "switch", "case", "break", "continue", "struct", "union", "typedef", "long", 
                    "short", "signed", "unsigned", "const", "static", "extern", "sizeof", "volatile"};
#define NUM_KEYWORDS 24

int is_keyword(const char *word) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_constant(const char *word) {
    char *endptr;
    strtol(word, &endptr, 10);  // Try to parse as an integer
    if (*endptr == '\0') return 1;

    strtod(word, &endptr);  // Try to parse as a float
    if (*endptr == '\0') return 1;

    return 0;
}
%}

%%
// Match keywords
"int"|"float"|"char"|"double"|"if"|"else"|"while"|"for"|"return"|"void"|
"switch"|"case"|"break"|"continue"|"struct"|"union"|"typedef"|"long"|
"short"|"signed"|"unsigned"|"const"|"static"|"extern"|"sizeof"|"volatile" {
    keyword_count++; 
    char_count += yyleng; 
}

// Match identifiers (variables, function names)
[a-zA-Z_][a-zA-Z0-9_]* {
    if (!is_keyword(yytext)) { 
        if (!is_constant(yytext)) {
            identifier_count++;
        }
    }
    word_count++;
    char_count += yyleng;
}

// Match constants (integer, float, scientific notation)
[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? {
    constant_count++; 
    char_count += yyleng; 
}

// Match string literals
\"([^\"]*)\" {
    constant_count++; 
    char_count += yyleng; 
}

// Match operators
[\+\-\*/=<>!&|^%] {
    operator_count++; 
    char_count += yyleng; 
}

// Match punctuation
[{}();,] {
    punctuation_count++; 
    char_count += yyleng; 
}

// Match whitespace and tabs
[ \t] {
    char_count += yyleng; 
    whitespace_count++; 
    if (yytext[0] == '\t') tab_count++; 
}

// Match new lines
\n {
    line_count++; 
    char_count++; 
}

// Match special characters
[@$%^&] {
    special_char_count++; 
    char_count++; 
}

// Catch-all rule to avoid infinite loops
.|\n { char_count++; }

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
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

    // Print results
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    printf("Whitespaces: %d\n", whitespace_count);
    printf("Tabs: %d\n", tab_count);
    printf("Special Characters: %d\n", special_char_count);
    printf("Keywords: %d\n", keyword_count);
    printf("Operators: %d\n", operator_count);
    printf("Identifiers: %d\n", identifier_count);
    printf("Constants: %d\n", constant_count);
    printf("Punctuation: %d\n", punctuation_count);

    return 0;
}

int yywrap() {
    return 1;
}
