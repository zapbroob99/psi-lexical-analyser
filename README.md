# Lexical Analyzer for Psi++

This project involves the development of a lexical analyzer for the programming language Psi++. The program reads a Psi++ code file, identifies the tokens according to the lexical rules of the language, and produces a list of tokens in a .lex output file.

## Lexical Rules

- **Identifiers**: Identifiers in Psi++ begin with a letter and can comprise one or more letters, digits, or underscores (_). The maximum length for an identifier is 30 characters. Psi++ is case-insensitive, and all identifiers are standardized to lower case.
  - Example Token: `Identifier(my_var_1)`

- **Integer Constants**: The maximum size of an integer is 10 digits. Negative values are not permitted.
  - Example Token: `IntConst(1290)`

- **Operators**: The valid operators in Psi++ are +, -, *, /, ++, --, and :=.
  - Example Token: `Operator(++)`

- **Brackets**: Brackets in Psi++ include (, ), [, ], {, }.
  - Example Token: `LeftCurlyBracket`

- **String Constants**: String constants are enclosed in double quotes ("), can be of unlimited size, and may not contain the double quote character. If a string constant does not terminate before the end of the file, a lexical error is generated.

- **Keywords**: The keywords in Psi++ are break, case, char, const, continue, do, else, enum, float, for, goto, if, int, long, record, return, static, while. Psi++ is case-insensitive, and all keywords are standardized to lower case.
  - Example Token: `Keyword(while)`

- **End of Line**: The end of line character in Psi++ is ;.
  - Example Token: `EndOfLine`

- **Comments**: Comments in Psi++ start with /* and end with */. No tokens are produced for comments. If a comment does not terminate before the end of the file, a lexical error is generated.

## How to Use

The program takes a source file named `code.psi` as input and generates an output file named `code.lex` containing all the tokens from the input file listed sequentially.

### Example:

**Input** (code.psi):

```c
hi:=hello+25; /*addition */
hello++; /*increment*/

```
**Output** (code.lex):

```c
Identifier(hi)
Operator(:=)
Identifier(hello)
Operator(+)
IntConst(25)
EndOfLine
Identifier(hello)
Operator(++)
EndOfLine


```



