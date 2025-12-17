#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define MAX_LENGTH 10000 //Allow large files

//-----------------------------------------------
// 1.Check If a Charachter Is a Delimeter
//-----------------------------------------------

bool isDelimeter(char chr){
    return (chr == ' ' || chr == '\n' || chr == '\t' || 
            chr == '+' || chr == '-' || chr == '*' || chr == '/' ||
            chr == ',' || chr == ';' || chr == '%' || chr == '>' ||
            chr == '<' || chr == '=' || chr == '(' || chr == ')' ||
            chr == '[' || chr == ']' || chr == '{' || chr == '}' || 
            chr == ':' || chr == '?' || chr == '!' || chr == '.'|| 
            chr == '\r'|| chr == '#' || chr == '\0');
}

//-----------------------------------------------
// 2.Check If a Charachter Is an Operator
//-----------------------------------------------

bool isOperator(char chr){
    return (chr == '+' || chr == '-' || chr == '*' ||
            chr == '/' || chr == '>' || chr == '<' || 
            chr == '=');
}

//-----------------------------------------------
// 3.Check If a Charachter Is a Punctuation
//-----------------------------------------------

bool isPunctuation(char chr){
    return( chr == ',' || chr == ';' || chr == ':' || 
            chr == '(' || chr == ')' || chr == '[' || 
            chr == ']' || chr == '{' || chr == '}' || 
            chr == '?' || chr == '!' || chr == '.'
            || chr == '#' );
}

//-----------------------------------------------
// 4.Check If a String Is a valid Identifier
//-----------------------------------------------

bool isValidIdentifier(char* str){
    if (str == NULL || *str == '\0' )
       return false;
    if (isdigit(str[0]) || isDelimeter(str[0]))
       return false;
    for (int i=1; str[i] !='\0'; i++){
        if(!(isalnum(str[i]) || str[i]=='_'))
           return false;
    }
    return true;
}

//---------------------------------------
// 5.Check If a String Is a Keyword
//---------------------------------------

bool isKeyword(char* str){
    const char* keywords[] =
        {"auto","break","case","char","const","continue","default","do",
         "double","else","enum","extern","float","for","goto","if","int",
         "long","register","return","short","signed","sizeof","static",
         "struct","switch","typedef","union","unsigned","void","volatile",
         "while","printf","scanf","main"};
    int keywordCount= sizeof(keywords)/ sizeof(keywords[0]);
    for (int i=0;i<keywordCount;i++){
        if(strcmp(str,keywords[i])==0)
          return true;
    }
    return false;
}

//-----------------------------------------------
// 6.Check If a String represents an Integer
//-----------------------------------------------

bool isInteger(char* str) {
    if (str == NULL || *str == '\0')
        return false;

    // Binary: 0b...
    if (strlen(str) > 2 && str[0] == '0' && (str[1]=='b' || str[1]=='B')) {
        for (int i=2; str[i]!='\0'; i++)
            if (str[i]!='0' && str[i]!='1') return false;
        return true;
    }

    // Hexadecimal: 0x...
    if (strlen(str) > 2 && str[0] == '0' && (str[1]=='x' || str[1]=='X')) {
        for (int i=2; str[i]!='\0'; i++)
            if (!isxdigit(str[i])) return false;
        return true;
    }

    // Octal: starts with 0 (but not 0 itself)
    if (str[0]=='0' && strlen(str) > 1) {
        for (int i=1; str[i]!='\0'; i++)
            if (str[i]<'0' || str[i]>'7') return false;
        return true;
    }

    // Decimal integer
    for (int i=0; str[i]!='\0'; i++)
        if (!isdigit(str[i])) return false;

    return true;
}

//-----------------------------------------------
// 7.Check If a String represents a Float
//-----------------------------------------------

bool isFloat(char* str) {
    if (str == NULL || *str == '\0') return false;

    int dotCount = 0;
    for (int i=0; str[i]!='\0'; i++) {
        if (str[i] == '.') {
            dotCount++;
            if (dotCount > 1) return false; // only one dot allowed
        }
        else if (!isdigit(str[i])) {
            return false;
        }
    }
    return dotCount == 1; // must have exactly one dot
}

//----------------------------
// 8.Skip string literals
//----------------------------

int skipCharLiteral(const char *input, int pos) {
    pos++; 
    if (input[pos] == '\\') pos += 2; 
    else pos++; 
    if (input[pos] == '\'') pos++; 
    return pos;
}

//-----------------------------------------------
// 9.Return Substring Between Start and End
//-----------------------------------------------

char* getSubstring(char* str,int start, int end){
    int subLength = end - start+1;
    char* subStr = (char*)malloc(subLength+1);
    strncpy(subStr,str + start, subLength);
    subStr[subLength]='\0';
    return subStr;
}

//--------------------------------
// 10.Skip Comments in C Code
//--------------------------------

int skipComments(char* input, int index) {
    // Single-line comment: //
    if (input[index] == '/' && input[index + 1] == '/') {
        index += 2;
        while (input[index] != '\0' && input[index] != '\n') {
            index++;
        }
    }
    // Multi-line comment: /* ... */
    else if (input[index] == '/' && input[index + 1] == '*') {
        index += 2;
        while (input[index] != '\0' && !(input[index] == '*' && input[index + 1] == '/')) {
            index++;
        }
        if (input[index] != '\0') index += 2; // Skip the closing */
    }
    return index;
}

//---------------------------------
// 11.Skip string literals "..."
//---------------------------------

int skipString(char* input, int index) {
    if (input[index] == '"') {
        index++;
        while (input[index] != '\0' && input[index] != '"') {
            if (input[index] == '\\' && input[index + 1] == '"') {
                index += 2;
                continue;
            }
            index++;
        }
        if (input[index] == '"') index++;
    }
    return index;
}

//-----------------------------------------------
// 12.Perform Lexical Analysis on Input string
//-----------------------------------------------

int lexiAnalyzer(char* input) {
    int left = 0, right = 0;
    int len = strlen(input);
    printf("+----------------------+------------------+\n");
    printf("| %-20s | %-16s |\n", "TOKEN TYPE", "VALUE");
    printf("+----------------------+------------------+\n");
    while (right <= len && left <= right) {

        // Skip comments
        if ((input[right] == '/' && input[right + 1] == '/') ||
            (input[right] == '/' && input[right + 1] == '*')) {
            right = skipComments(input, right);
            left = right;
            continue;
        }

        // Handle string literals
        if (input[right] == '"') {
            int start = right;
            right = skipString(input, right);
            int len = right - start;
            char strLit[1024]; // adjust size if needed
            if(len > 1023) len = 1023; // prevent overflow
            strncpy(strLit, input + start, len);
            strLit[len] = '\0';
            printf("| %-20s | %-16s |\n", "String Literal",strLit);
            left = right;
            continue;
        }

        // Handle character literals
        if (input[right] == '\'') {
            int start = right;
            right = skipCharLiteral(input, right);
            char charLit[10]; // enough to store 'A' style literals
            int len = right - start;
            if(len > 9) len = 9; // prevent overflow
            strncpy(charLit, input + start, len);
            charLit[len] = '\0';
            printf("| %-20s | %-16s |\n", "Char Literal",charLit);
            left = right;
            continue;
        }

        // If current char is NOT a delimiter, keep moving
        if (!isDelimeter(input[right])) {
            right++;
            continue;
        }

        // Handle word-like token before this delimiter
        if ((isDelimeter(input[right]) && left != right) || (right == len && left != right)) {

            // Handle float pattern 
            if (right > left && isdigit((unsigned char)input[right - 1]) &&
                input[right] == '.' && isdigit((unsigned char)input[right + 1])) {
                right++;
                while (isdigit((unsigned char)input[right])) right++;
            }

            char* subStr = getSubstring(input, left, right - 1);

            if (isKeyword(subStr))
                printf("| %-20s | %-16s |\n","Keyword", subStr);
            else if (isInteger(subStr))
                printf("| %-20s | %-16s |\n","Integer", subStr);
            else if (isFloat(subStr))
                printf("| %-20s | %-16s |\n","Float", subStr);
            else if (isValidIdentifier(subStr))
                printf("| %-20s | %-16s |\n","Identifier", subStr);
            else
                printf("| %-20s | %-16s |\n","Unidentified", subStr);

            free(subStr);
            left = right;
        }

        // Handle operators
        if (isOperator(input[right])) {
            if ((input[right] == '+' && input[right + 1] == '+') ||
                (input[right] == '-' && input[right + 1] == '-') ||
                (input[right] == '=' && input[right + 1] == '=') ||
                (input[right] == '!' && input[right + 1] == '=') ||
                (input[right] == '<' && input[right + 1] == '=') ||
                (input[right] == '>' && input[right + 1] == '=')) {

                char opStr[3] = {input[right], input[right + 1], '\0'};
                printf("| %-20s | %-16s |\n", "Operators",opStr);
                right += 2;
            } else {
                printf("| %-20s | %-16c |\n","Operator", input[right]);
                right++;
            }
            left = right;
            continue;
        }

        // Handle punctuation
        if (isPunctuation(input[right])) {
            printf("| %-20s | %-16c |\n","Punctuation", input[right]);
            right++;
            left = right;
            continue;
        }

        // Skip whitespace and reset left pointer
        if (isspace((unsigned char)input[right])) {
            right++;
            left = right;
            continue;
        }

        right++;
    }
    printf("+----------------------+------------------+\n");

    return 0;
}

//-------------------------------------------
// 13.Reads a File into a Buffer Safely
//-------------------------------------------

char* readFile(const char*filename)
{
    FILE* file =fopen(filename,"rb");
    if(!file)
    {
        perror("Error opening file");
        exit(1);
    }
    char* buffer=(char*)malloc(MAX_LENGTH);
    if(!buffer)          
    {
        perror("Memory allocation failed");
        exit(1);
    }
    buffer[0]='\0';
    char line[512];
    while (fgets(line,sizeof(line),file))
    {
        if(strlen(buffer) + strlen(line) >= MAX_LENGTH-1)
        {
            fprintf(stderr,"File too large for buffer limit.\n");
            exit(1);
        }
        strcat(buffer,line);
    }
    fclose(file);
    return buffer;
}

//----------------------
// 14.Main Function
//-----------------------

int main()
{
    char filename[]="input.c";
    // printf("Enter file name to analyze (e.g. input.c):");
    // scanf("%255s",filename);

    char* code=readFile(filename);
    printf("\nAnalyzing file: %s\n\n",filename);

    lexiAnalyzer(code);

    free(code);
    return 0;
}

//---------------------
//     Now Run It :) 
//---------------------