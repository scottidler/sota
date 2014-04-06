grammar sota;

tokens { BOF, INDENT, DEDENT }
@lexer::header {
    import com.yuvalshavit.antlr4.DenterHelper;
}
@lexer::members {
    private final DenterHelper denter = new DenterHelper(NL, sotaParser.BOF, sotaParser.INDENT, sotaParser.DEDENT) {
        @Override
        public Token pullToken() {
            return sotaLexer.super.nextToken();
        }
    };

    @Override
    public Token nextToken() {
        return denter.nextToken();
    }

    public Token rawNextToken() {
        return super.nextToken();
    }
}

prog
    :   global
    ;

global  
    :   BOF statements EOF
    ;

block
    :   flow_block
    |   inline_block
    ;

flow_block
    :   INDENT statements DEDENT
    ;

match
    :   MATCH expression match_block
    ;

match_block
    :   INDENT match_options DEDENT
    ;

match_options
    :   PIPE match_option (NL PIPE match_option)* NL*
    ;

match_option
    :   match_expression block
    ;

match_expression
    :   literal
    |   match_expression (PIPE match_expression)+
    ;

inline_block
    :   ARROW 
        (   expression
        |   LBRACE statement (SEMI statement)* RBRACE
        )
    ;

namespace
    :   Identifier flow_block
    ;

statements
    :   statement ( (NL | SEMI) statement)* NL*
    ;

statement
    :   namespace
    |   enumeration
    |   if_block
    |   foreach_block
    |   while_block
    |   dowhile_block
    |   RETURN expression?
    |   PRINT expression?
    |   DEBUG expression?
    |   TRACE expression?
    |   expression
    ;

enumeration
    :   Identifier ASSIGN 
        (   elements
        |   INDENT elements DEDENT
        )
    ;

elements
    :   PIPE element (NL? PIPE element)* NL*
    ;

element
    :   Identifier (ASSIGN expression)?
    ;

if_block
    :   IF expression block (ELSIF expression block)* (ELSE block)?
    ;

foreach_block
    :   FOREACH Identifier IN expression block
    ;

while_block
    :   WHILE expression block
    ;

dowhile_block
    :   DO block WHILE expression
    ;

expressions
    :   expression ( (NL | COMMA) expression)*
    ;

expression
    :   primary
    |   PRINT '(' expression ')'
    |   DEBUG '(' expression ')'
    |   TRACE '(' expression ')'
    |   match
    |   func
    |   call
    |   expression '.' Identifier
    |   expression '[' expression ']'
    |   expression ('++' | '--')
    |   ('+'|'-'|'++'|'--') expression
    |   ('~'|'!') expression
    |   expression ('*'|'/'|'%') expression
    |   expression ('+'|'-') expression
    |   expression ('<' '<' | '>' '>' '>' | '>' '>') expression
    |   expression ('<=' | '>=' | '>' | '<') expression
    |   expression 'is' type
    |   expression ('==' | '!=') expression
    |   expression ELIPSES expression
    |   expression '&' expression
    |   expression '^' expression
    |   expression '|' expression
    |   expression '&&' expression
    |   expression '||' expression
    |   expression '?' expression ':' expression
    |   expression
        (   '='<assoc=right>
        |   '+='<assoc=right>
        |   '-='<assoc=right>
        |   '*='<assoc=right>
        |   '/='<assoc=right>
        |   '&='<assoc=right>
        |   '|='<assoc=right>
        |   '^='<assoc=right>
        |   '>>='<assoc=right>
        |   '>>>='<assoc=right>
        |   '<<='<assoc=right>
        |   '%='<assoc=right>
        )
        expression
    ;

assign
    :   Identifier '=' expression
    ;

primary
    :   '(' expression ')'
    |   literal
    |   Identifier
    ;

literal
    :   IntegerLiteral
    |   FloatingPointLiteral
    |   CharacterLiteral
    |   StringLiteral
    |   BooleanLiteral
    |   'null'
    ;

func
    :   '(' params ')'
        (   match_block
        |   block
        )
    ;

call
    :   Identifier '(' params ')'
    ;

params
    :   expressions?
    ;

type
    :   primitive ('[' ']')*
    ;

primitive
    :   'bool'
    |   'char'
    |   'int'
    |   'float'
    |   'double'
    ;

// LEXER

// keywords
BASE    :   'base';
BOOL    :   'bool';
CHAR    :   'char';
DEBUG   :   'debug';
DO      :   'do';
ELSE    :   'else';
ELIF    :   'elif';
FOREACH :   'foreach';
IF      :   'if';
IN      :   'in';
IS      :   'is';
LOG     :   'log';
MATCH   :   'match';
OPEN    :   'open';
PRINT   :   'print';
RETURN  :   'return';
THIS    :   'this';
TRACE   :   'trace';
TRY     :   'try';
TYPE    :   'Type' | 'type';
WITH    :   'with';
WHILE   :   'while';


// §3.10.1 Integer Literals

IntegerLiteral
    :   DecimalIntegerLiteral
    |   HexIntegerLiteral
    |   OctalIntegerLiteral
    |   BinaryIntegerLiteral
    ;

fragment
DecimalIntegerLiteral
    :   DecimalNumeral IntegerTypeSuffix?
    ;

fragment
HexIntegerLiteral
    :   HexNumeral IntegerTypeSuffix?
    ;

fragment
OctalIntegerLiteral
    :   OctalNumeral IntegerTypeSuffix?
    ;

fragment
BinaryIntegerLiteral
    :   BinaryNumeral IntegerTypeSuffix?
    ;

fragment
IntegerTypeSuffix
    :   [lL]
    ;

fragment
DecimalNumeral
    :   '0'
    |   NonZeroDigit (Digits? | '_'* Digits)
    ;

fragment
Digits
    :   Digit ( (Digit | '_')* Digit)?
    ;

fragment
Digit
    :   '0'
    |   NonZeroDigit
    ;

fragment
NonZeroDigit
    :   [1-9]
    ;

fragment
HexNumeral
    :   '0' [xX] HexDigits
    ;

fragment
HexDigits
    :   HexDigit ( (HexDigit | '_')* HexDigit)?
    ;

fragment
HexDigit
    :   [0-9a-fA-F]
    ;

fragment
OctalNumeral
    :   '0' '_'* OctalDigits
    ;

fragment
OctalDigits
    :   OctalDigit ( (OctalDigit | '_')* OctalDigit)?
    ;

fragment
OctalDigit
    :   [0-7]
    ;

fragment
BinaryNumeral
    :   '0' [bB] BinaryDigits
    ;

fragment
BinaryDigits
    :   BinaryDigit ( (BinaryDigit | '_')* BinaryDigit)?
    ;

fragment
BinaryDigit
    :   [01]
    ;

// §3.10.2 Floating-Point Literals

FloatingPointLiteral
    :   DecimalFloatingPointLiteral
    |   HexadecimalFloatingPointLiteral
    ;

fragment
DecimalFloatingPointLiteral
    :   Digits '.' Digits? ExponentPart? FloatTypeSuffix?
    |   '.' Digits ExponentPart? FloatTypeSuffix?
    |   Digits ExponentPart FloatTypeSuffix?
    |   Digits FloatTypeSuffix
    ;

fragment
ExponentPart
    :   ExponentIndicator SignedInteger
    ;

fragment
ExponentIndicator
    :   [eE]
    ;

fragment
SignedInteger
    :   Sign? Digits
    ;

fragment
Sign
    :   [+-]
    ;

fragment
FloatTypeSuffix
    :   [fFdD]
    ;

fragment
HexadecimalFloatingPointLiteral
    :   HexSignificand BinaryExponent FloatTypeSuffix?
    ;

fragment
HexSignificand
    :   HexNumeral '.'?
    |   '0' [xX] HexDigits? '.' HexDigits
    ;

fragment
BinaryExponent
    :   BinaryExponentIndicator SignedInteger
    ;

fragment
BinaryExponentIndicator
    :   [pP]
    ;

// §3.10.3 Boolean Literals

BooleanLiteral
    :   'true'
    |   'false'
    ;

// §3.10.4 Character Literals

CharacterLiteral
    :   '\'' SingleCharacter '\''
    |   '\'' EscapeSequence '\''
    ;

fragment
SingleCharacter
    :   ~['\\]
    ;

// §3.10.5 String Literals

StringLiteral
    :   '"' StringCharacters? '"'
    ;

fragment
StringCharacters
    :   StringCharacter+
    ;

fragment
StringCharacter
    :   ~["\\]
    |   EscapeSequence
    ;

// §3.10.6 Escape Sequences for Character and String Literals

fragment
EscapeSequence
    :   '\\' [btnfr"'\\]
    |   OctalEscape
    |   UnicodeEscape
    ;

fragment
OctalEscape
    :   '\\' OctalDigit
    |   '\\' OctalDigit OctalDigit
    |   '\\' [0-3] OctalDigit OctalDigit
    ;

fragment
UnicodeEscape
    :   '\\' 'u' HexDigit HexDigit HexDigit HexDigit
    ;

// §3.10.7 The Null Literal

NullLiteral
    :   'null'
    ;

// §3.11 Separators

LPAREN          :   '(';
RPAREN          :   ')';
LBRACE          :   '{';
RBRACE          :   '}';
LBRACK          :   '[';
RBRACK          :   ']';
SEMI            :   ';';
COMMA           :   ',';
DOT             :   '.';

// §3.12 Operators

ARROW           :   '->';
ELIPSES         :   '..';

ASSIGN          :   '=';
GT              :   '>';
LT              :   '<';
BANG            :   '!';
TILDE           :   '~';
QUESTION        :   '?';
COLON           :   ':';
EQUAL           :   '==';
LE              :   '<=';
GE              :   '>=';
NOTEQUAL        :   '!=';
AND             :   '&&';
OR              :   '||';
INC             :   '++';
DEC             :   '--';
ADD             :   '+';
SUB             :   '-';
MUL             :   '*';
SLASH           :   '/';
BITAND          :   '&';
PIPE            :   '|';
CARET           :   '^';
MOD             :   '%';

ADD_ASSIGN      :   '+=';
SUB_ASSIGN      :   '-=';
MUL_ASSIGN      :   '*=';
DIV_ASSIGN      :   '/=';
AND_ASSIGN      :   '&=';
OR_ASSIGN       :   '|=';
XOR_ASSIGN      :   '^=';
MOD_ASSIGN      :   '%=';
LSHIFT_ASSIGN   :   '<<=';
RSHIFT_ASSIGN   :   '>>=';
URSHIFT_ASSIGN  :   '>>>=';

// §3.8 Identifiers (must appear after all keywords in the grammar)

Identifier
    :   JavaLetter JavaLetterOrDigit*
    ;

fragment
JavaLetter
    :   [a-zA-Z$_] // these are the "java letters" below 0xFF
    |   // covers all characters above 0xFF which are not a surrogate
        ~[\u0000-\u00FF\uD800-\uDBFF]
        {Character.isJavaIdentifierStart(_input.LA(-1))}?
    |   // covers UTF-16 surrogate pairs encodings for U+10000 to U+10FFFF
        [\uD800-\uDBFF] [\uDC00-\uDFFF]
        {Character.isJavaIdentifierStart(Character.toCodePoint((char)_input.LA(-2), (char)_input.LA(-1)))}?
    ;

fragment
JavaLetterOrDigit
    :   [a-zA-Z0-9$_] // these are the "java letters or digits" below 0xFF
    |   // covers all characters above 0xFF which are not a surrogate
        ~[\u0000-\u00FF\uD800-\uDBFF]
        {Character.isJavaIdentifierPart(_input.LA(-1))}?
    |   // covers UTF-16 surrogate pairs encodings for U+10000 to U+10FFFF
        [\uD800-\uDBFF] [\uDC00-\uDFFF]
        {Character.isJavaIdentifierPart(Character.toCodePoint((char)_input.LA(-2), (char)_input.LA(-1)))}?
    ;

//
// Additional symbols not defined in the lexical specification
//

AT : '@';
ELLIPSIS : '...';

//
// Whitespace and comments
//

WS  :   [ \t]+ -> skip
    ;

NL  :   '\r'? '\n' ' '*
    ;

COMMENT
    :   '(#' .*? '#)' -> skip
    ;

LINE_COMMENT
    :   '#' ~[\r\n]* -> skip
    ;
