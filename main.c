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
    bool in_comment = false;
    int line = 1;
    int column = 1;
    char op[3] = {0};
    while ((c = fgetc(file)) != EOF) {
        column++;
        if (c == '\n') {
            line++;
            column = 1;
        }

        if(isalpha(c)){
            char identifier[MAX_BUFFER_SIZE+1] = {0};
            int idx = 0;
            int token_start_column = column;
            do {
                if(idx < MAX_ID_SIZE){
                    identifier[idx++]=tolower(c);
                } else {
                    idx++;
                }
                c = fgetc(file);
                column++;
            } while (isalnum(c) || c == '_');
            ungetc(c, file);
            column--;

            int is_keyword = 0;
            for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
                if (strcmp(identifier, keywords[i]) == 0) {
                    is_keyword = 1;
                    break;
                }
            }
            if(idx > MAX_ID_SIZE){
                printf("Error: Identifier too long at line %d, column %d\n", line, token_start_column-1);
            } else if (is_keyword) {
                fprintf(output, "Keyword(%s)\n", identifier);
            } else {
                fprintf(output, "Identifier(%s)\n", identifier);
            }
        }
        else if (isdigit(c)) {
            char int_const[MAX_INT_SIZE+1] = {0};
            int idx = 0;
            int token_start_column = column;
            do {
                if (idx < MAX_INT_SIZE) {
                    int_const[idx++] = c;
                } else {
                    idx++;
                }
                c = fgetc(file);
                column++;
            } while (isdigit(c));
            ungetc(c, file);
            column--;

            if (idx > MAX_INT_SIZE) {
                printf("Error: Integer constant too long at line %d, column %d\n", line, token_start_column-1);
            } else {
                fprintf(output, "IntConst(%s)\n", int_const);
            }
        }
        else if (c == '/') {
            c = fgetc(file);
            column++;
            if (c == '*') {
                in_comment = true;
            } else {
                ungetc(c, file);
                column--;
            }
        }
        else if (c == '"') {
            char string_const[MAX_BUFFER_SIZE+1] = {0};
            int idx = 0;
            int token_start_column = column;
            c = fgetc(file);
            column++;
            while (c != '"' && c != EOF) {
                string_const[idx++] = c;
                c = fgetc(file);
                column++;
            }

            if (c == EOF) {
                printf("Error: String constant not terminated at line %d, column %d\n", line, token_start_column);
            } else {
                fprintf(output, "StringConst(%s)\n", string_const);
            }
        }
        else if(strchr("+-*/:", c)){
            op[0] = c;
            op[1] = fgetc(file);
            column++;
            if ((op[0] == '+' && op[1] == '+') || (op[0] == '-' && op[1] == '-')
                || (op[0] == ':' && op[1] == '=')){
                    fprintf(output, "Operator(%c%c)\n", op[0], op[1]);
                }
                else{
                    ungetc(op[1], file);
                    column--;
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
            int token_start_column = column;

            do {
                if (idx < MAX_BUFFER_SIZE) {
                    buffer[idx++] = c;
                } else {
                    idx++;
                }
                c = fgetc(file);
                column++;
            } while (isalnum(c) || c == '_');
            ungetc(c, file);
            column--;

            printf("Error: Unrecognized token '%s' at line %d, column %d\n", buffer, line, token_start_column-1);
        }

        while(in_comment){
            int comment_start_line = line;
            int comment_start_column = column;
            c = fgetc(file);
            column++;
            if (c == '\n') {
                line++;
                column = 1;
            }
            if(c=='*'){
                c=fgetc(file);
                column++;
                if(c=='/'){
                    in_comment = false;
                } else {
                    ungetc(c, file);
                    column--;
                }
            } else if (c == EOF) {
                printf("Error: Comment not terminated before end of file at line %d, column %d\n", comment_start_line, comment_start_column);
                break;
            }
        }
    }

    fclose(file);
    return 0;
}
