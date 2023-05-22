#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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
    int in_string=0;
    bool in_comment=false;
    int line = 1;
    int column = 1;
    char op[3] = {0};
    while ((c = fgetc(file)) != EOF) {
        column++;
        if (c == '\n') {
            line++;
            column = 0;
        }

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
        else if (isdigit(c)) {
            char int_const[MAX_INT_SIZE+1] = {0};
            int idx = 0;
            do {
                if (idx < MAX_INT_SIZE) {
                    int_const[idx++] = c;
                } else {
                    idx++;
                }
                c = fgetc(file);
            } while (isdigit(c));

            if (idx > MAX_INT_SIZE) {
                printf("Error: Integer constant too long at line %d, column %d\n", line, column);
            } else {
                fprintf(output, "IntConst(%s)\n", int_const);
            }
            ungetc(c, file);
}
        else if(c=='/'){
             c=fgetc(file);
             if (c == '*') {  // This is the start of a comment
                in_comment = true;
            } else {  // This is not a comment, just a regular '/'
                ungetc(c, file);  // Put the character back, it will be processed in the next iteration
            }
        }
        else if (c == '"') {
            char string_const[MAX_BUFFER_SIZE+1] = {0};
            int idx = 0;
            c = fgetc(file);
            while (c != '"' && c != EOF) {
                string_const[idx++] = c;
                c = fgetc(file);
            }

            if (c == EOF) {
                printf("Error: String constant not terminated at line %d, column %d\n", line, column);
            } else {
                fprintf(output, "StringConst(%s)\n", string_const);
            }
        }
        else if(strchr("+-*/:", c)){
            op[0] = c;
            op[1] = fgetc(file);
            if ((op[0] == '+' && op[1] == '+') || (op[0] == '-' && op[1] == '-')
                || (op[0] == ':' && op[1] == '=')){
                    fprintf(output, "Operator(%c%c)\n", op[0], op[1]);
                }
                else{
                    ungetc(op[1], file);
                    fprintf(output, "Operator(%c)\n", op[0]);
                }
        }
        else if (c == '(') {
                fprintf(output, "LeftPar\n");
        } else if (c == ')') {
            fprintf(output, "RightPar\n");
        } else if (c == '{') {
            fprintf(output, "LeftCurlyBracket\n");
        } else if (c == '}') {
            fprintf(output, "RightCurlyBracket\n");
        } else if (c == '[') {
            fprintf(output, "LeftSquareBracket\n");
        } else if (c == ']') {
            fprintf(output, "RightSquareBracket\n");
        } else if(c==';'){
            fprintf(output, "EndOfLine\n");
        }
        else if (!isspace(c)) {
            char buffer[MAX_BUFFER_SIZE+1] = {0};
            int idx = 0;

            do {
                if (idx < MAX_BUFFER_SIZE) {
                    buffer[idx++] = c;
                } else {
                    idx++;
                }

                c = fgetc(file);
                column++;

            } while (isalnum(c) || c == '_');

            printf("Error: Unrecognized token '%s' at line %d, column %d\n", buffer, line, column - strlen(buffer));
            ungetc(c, file);
        }
        while(in_comment){
            int c=fgetc(file);
            if(c=='*'){
                c=fgetc(file);
                if(c=='/'){
                    in_comment = false;
                } else {
                    ungetc(c,file);
                }
            } else if (c == EOF) {
                printf("Error: Comment not terminated before end of file\n");
                break;
            }

        }

    }

    fclose(file);
    return 0;
}
