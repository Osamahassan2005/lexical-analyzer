 #  Objectives
 1. Build a single-file C program that demonstrates modular programming (functions for discrete 
tasks).
 2. Ensure correct handling of comments, string/char literals, escape sequences, and numeric 
formats.
 3. Provide safe file reading with buffer-limit checks to avoid overflow.
 4. Produce a readable token list printed to the console.
    
# Core components and responsibilities

 isDelimeter(char)
 Recognizes characters that separate tokens (spaces, newlines, punctuation, many operators). Used to 
determine token boundaries.
 isOperator(char)
 Identifies one-character operators.
 isPunctuation(char)
 Identifies punctuation characters printed as separate tokens.
 isValidIdentifier(char*)
 Validates C-like identifiers: must not start with a digit and may contain letters, digits, and 
underscores.
 isKeyword(char*)
 Checks whether a token matches a table of common C keywords and selected library functions 
(e.g., printf, scanf, main). The list can be adjusted for strict language adherence.
 isInteger(char*)
 Detects integer literals in multiple bases: binary (0b...), hexadecimal (0x...), octal (leading 0), and 
decimal. Returns true if the string represents a valid integer literal under these formats.
 isFloat(char*)
 Detects floating-point literals that contain exactly one decimal point and otherwise digits. Note: 
scientific notation is not supported in the current implementation.
 skipCharLiteral(const char*, int)
 Advances the scanning index past a character literal, correctly handling escapes such as ' '.
 skipString(char*, int)
 Advances the scanning index past string literals while respecting escaped quotes and other escape 
sequences.
 skipComments(char*, int)
 Skips both single-line (//) and multi-line (/* ... */) comments.
 getSubstring(char*, int, int)
 Returns a dynamically allocated copy of a substring for token classification.
 readFile(const char*)
 Reads the entire file into a dynamically allocated buffer while preventing overflow beyond 
MAX_LENGTH.
 lexiAnalyzer(char*)
 The main scanning function. Uses a two-pointer technique (left and right) to identify tokens and 
classifies them by calling the helper functions above.

# Edge Cases & Implementation Notes
 The implementation explicitly addresses several tricky cases:
 String literals: The scanner collects entire string literals, including escaped quotes, so contents 
such as " are preserved and the literal is not split.
 Character literals: Handles both simple characters ('A') and escaped forms (' ', '\'). The skip routine 
prevents accidental splitting.
 Comments: Both // single-line and /* ... */ multi-line comments are recognized and ignored in 
token output.
 Numeric bases: Integer recognition supports binary (0b/0B), hexadecimal (0x/0X), octal (leading 
0), and decimal. Floats are recognized when a single decimal point occurs with digits on both sides.
 Multi-character operators: The scanner detects ++, --, ==, !=, <=, and >= using lookahead.
 # Conclusion
 This Lexical Analyzer provides a compact and practical demonstration of lexical scanning 
techniques in C. It cleanly separates concerns into helper functions, handles common C language 
features, and produces a clear token listing. The design is intentionally simple but extensible, 
making it suitable as a learning tool or as the first stage in a small compiler or static analysis 
project
