#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_ID_SIZE 30
#define MAX_INT_SIZE 10
#define MAX_BUFFER_SIZE 1000

typedef enum {
    Identifier,
    IntConst,
    Operator,
    LeftPar,
    RightPar,
    LeftSquareBracket,
    RightSquareBracket,
    LeftCurlyBracket,
    RightCurlyBracket,
    StringConst,
    Keyword,
    EndOfLine,
    Comment
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_BUFFER_SIZE];
} Token;

char *keywords[] = {"break", "case", "char", "const", "continue", "do", "else", "enum", "float",
    "for", "goto", "if", "int", "long", "record", "return", "static", "while"};

char *operators[] = {"+", "-", "*", "/", "++", "--", ":="};


int main() {
    FILE *file = fopen("code.psi", "r");

    if (file == NULL) {
        printf("Error: Could not open file\n");
        return 1;
    }
    FILE *output = fopen("code.lex", "w");
       if (output == NULL) {
        printf("Error: Could not open output file\n");
        fclose(file);
        return 1;
    }

    char c;
    while ((c = fgetc(file)) != EOF) {
        if(isalpha(c)){
            char identifier[MAX_BUFFER_SIZE+1] = {0};
            int idx=0;
            do{
                    if(idx<MAX_ID_SIZE){
                        //add the character to identifier
                        identifier[idx++]=tolower(c);
                    }
                    else{
                        idx++;

                    }
                    c=fgetc(file);
            } while (isalnum(c) || c == '_');
            //after reading the entire identifier, checks if it's a keyword
            int is_keyword=0;
            for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
                if (strcmp(identifier, keywords[i]) == 0) {
                    // If the identifier matches a keyword, set the flag to 1 and break the loop
                    is_keyword = 1;
                    break;
                }
            }
            if(idx>MAX_ID_SIZE){
                printf("Error: Identifier too long.");

            }
            else if(is_keyword){
                  fprintf(output, "Keyword(%s)\n", identifier);
            } else {
               fprintf(output, "Identifier(%s)\n", identifier);
            }
            ungetc(c,file);
        }
        //TODO: add other things

    }

    fclose(file);
    return 0;
}
