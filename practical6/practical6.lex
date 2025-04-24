%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pos = 0;
char user_input[100];

int parse_S();
int parse_L();
int parse_L_prime();
int match(char expected);

%}

%%

[a()]     ;  // These characters will be handled in main()
[,]       ;
[ \t\n]   ;  // Ignore spaces and newlines
.         { printf("Invalid string\n"); exit(0); }

%%

int main() {
    printf("Enter input string: ");
    scanf("%s", user_input);  // Read input without spaces
    pos = 0;

    if (parse_S() && user_input[pos] == '\0') {
        printf("Valid string\n");
    } else {
        printf("Invalid string\n");
    }

    return 0;
}

int parse_S() {
    if (match('a')) {
        return 1;
    } else if (match('(')) {
        if (parse_L() && match(')')) {
            return 1;
        }
    }
    return 0;
}

int parse_L() {
    if (parse_S()) {
        return parse_L_prime();
    }
    return 0;
}

int parse_L_prime() {
    if (match(',')) {
        if (parse_S()) {
            return parse_L_prime();
        }
        return 0;
    }
    return 1; // ε case (empty allowed)
}

int match(char expected) {
    if (user_input[pos] == expected) {
        pos++;
        return 1;
    }
    return 0;
}

int yywrap() { return 1; }
