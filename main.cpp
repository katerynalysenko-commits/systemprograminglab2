#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <unordered_set>
using namespace std;

enum class TokenType {
    INT, FLOAT, HEX, STRING, CHAR, IDENT, RESERVED, OPERATOR, DELIM,
    COMMENT, PREPROCESSOR, ERROR, END
};

struct Token {
    string lexeme;
    TokenType type;
};

const unordered_set<string> keywords = {
    "break", "case", "chan", "const", "continue", "default", "defer", "else",
    "fallthrough", "for", "func", "go", "goto", "if", "import", "interface",
    "map", "package", "range", "return", "select", "struct", "switch", "type", "var"
};

const vector<pair<TokenType, regex>> lexem_patterns = {
    {TokenType::COMMENT, regex(R"(//.*|/\*[\s\S]*?\*/)")},
    {TokenType::PREPROCESSOR, regex(R"(#.*)")},
    {TokenType::FLOAT, regex(R"(\b\d*\.\d+([eE][-+]?\d+)?\b)")},
    {TokenType::HEX, regex(R"(\b0[xX][0-9a-fA-F]+\b)")},
    {TokenType::INT, regex(R"(\b\d+\b)")},
    {TokenType::STRING, regex(R"("([^"\\]|\\.)*")")},
    {TokenType::CHAR, regex(R"('([^'\\]|\\.)')")},
    {TokenType::OPERATOR, regex(R"(\=\=|\!\=|\+\+|\-\-|\&\&|\|\||\+|\-|\*|\/|=|<|>|<=|>=)")},
    {TokenType::DELIM, regex(R"(;|,|\.|\(|\)|\{|\}|\[|\]|:)")},
    {TokenType::IDENT, regex(R"([a-zA-Z_][a-zA-Z0-9_]*)")}
};

string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::INT: return "INTEGER";
        case TokenType::FLOAT: return "FLOAT";
        case TokenType::HEX: return "HEX";
        case TokenType::STRING: return "STRING";
        case TokenType::CHAR: return "CHAR";
        case TokenType::IDENT: return "IDENTIFIER";
        case TokenType::RESERVED: return "KEYWORD";
        case TokenType::OPERATOR: return "OPERATOR";
        case TokenType::DELIM: return "DELIMITER";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::PREPROCESSOR: return "PREPROCESSOR";
        case TokenType::ERROR: return "ERROR";
        case TokenType::END: return "END";
        default: return "UNKNOWN";
    }
}

Token nextToken(const string& code, size_t& start) {
    while (start < code.length() && isspace(code[start])) ++start;
    if (start >= code.length()) return {"", TokenType::END};

    for (const auto& [type, pat] : lexem_patterns) {
        smatch m;
        string s = code.substr(start);
        if (regex_search(s, m, pat, regex_constants::match_continuous)) {
            string lex = m.str();

            if (type == TokenType::IDENT && keywords.count(lex)) {
                start += lex.length();
                return {lex, TokenType::RESERVED};
            }
            start += lex.length();
            return {lex, type};
        }
    }
    return { string(1, code[start++]), TokenType::ERROR };
}

vector<Token> lexAnalyze(const string& code) {
    vector<Token> result;
    size_t pos = 0;
    while (true) {
        Token t = nextToken(code, pos);
        if (t.type == TokenType::END) break;
        result.push_back(t); // тепер НЕ відкидаємо коментарі
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: lexer <go-file>\n";
        return 1;
    }
    ifstream f(argv[1]);
    string prog((istreambuf_iterator<char>(f)), {});
    auto tokens = lexAnalyze(prog);

    cout << "Lexical Analysis Result\n";
    for (const auto& t : tokens) {
        cout << "Lexeme: " << t.lexeme
             << "\tType: " << tokenTypeToString(t.type) << endl;
    }
    return 0;
}
