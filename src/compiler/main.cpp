#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <cctype>
#include <string>

enum class TokenType {
    _return,
    int_lit,
    semi,
    _print,
    string_lit,
    _plus,
    _minus,
    comment,
    _int,        // palabra clave int
    _identifier, // nombre de variable
    assign    
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string &str) {
    std::vector<Token> tokens;

    std::string buf;
    for (size_t i = 0; i < str.length(); i++) {
        char c = str.at(i);

        if (std::isalpha(static_cast<unsigned char>(c))) {
            buf.clear();
            buf.push_back(c);
            i++;
            while (i < str.length() && std::isalnum(static_cast<unsigned char>(str.at(i)))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--; // nos pasamos una
            if (buf == "return") {
                tokens.push_back({TokenType::_return, std::nullopt});
            } else if (buf == "print") {
                tokens.push_back({TokenType::_print, std::nullopt});
            } else if (buf == "int") {
                tokens.push_back({TokenType::_int, std::nullopt});
            } else {
                tokens.push_back({TokenType::_identifier, buf});
            }
        }
        else if (c == '!' && i + 1 < str.length() && str.at(i + 1) == '!') {
            i += 2; // saltar los dos !!
            buf.clear();
        
            while (i < str.length() && str.at(i) != ';') {
                buf.push_back(str.at(i));
                i++;
            }
        
            if (i >= str.length()) {
                std::cerr << "You messed up: unterminated comment\n";
                exit(EXIT_FAILURE);
            }
        
            tokens.push_back({TokenType::comment, buf});
        }
        else if (std::isdigit(static_cast<unsigned char>(c))) {
            buf.clear();
            buf.push_back(c);
            i++;
            while (i < str.length() && std::isdigit(static_cast<unsigned char>(str.at(i)))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--; // nos pasamos una
            tokens.push_back({TokenType::int_lit, buf});
        }
        else if (c == '"') {
            // string literal
            buf.clear();
            i++;
            while (i < str.length() && str.at(i) != '"') {
                buf.push_back(str.at(i));
                i++;
            }
            if (i >= str.length()) {
                std::cerr << "You messed up: unterminated string literal\n";
                exit(EXIT_FAILURE);
            }
            tokens.push_back({TokenType::string_lit, buf});
        }
        else if (c == '+') {
            tokens.push_back({TokenType::_plus, std::nullopt});
        }
        else if (c == ';') {
            tokens.push_back({TokenType::semi, std::nullopt});
        }
        else if (c == '-') {
            tokens.push_back({TokenType::_minus, std::nullopt});
        }
        else if (std::isspace(static_cast<unsigned char>(c))) {
            continue; // ignorar espacios, tabs, \n, \r
        }
        else if (c == '=') {
            tokens.push_back({TokenType::assign, std::nullopt});
        }
        else {
            std::cerr << "You messed up: unexpected char '" << c << "'\n";
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}


static std::string escape_for_nasm(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        if (ch == '"' || ch == '\\') {
            out.push_back('\\');
            out.push_back(ch);
        } else {
            out.push_back(ch);
        }
    }
    return out;
}

std::string tokens_to_asm(const std::vector<Token> &tokens) {
    std::stringstream data, text, out;
    int msg_id = 0;

    // Recorremos tokens
    for (size_t i = 0; i < tokens.size(); i++) {
        const Token &tok = tokens.at(i);

        // return <int>;
        if (tok.type == TokenType::_return) {
            if (i + 2 < tokens.size() &&
                tokens.at(i + 1).type == TokenType::int_lit &&
                tokens.at(i + 2).type == TokenType::semi) {

                std::string code = tokens.at(i + 1).value.value();
                text << "    ; return " << code << "\n";
                text << "    mov rax, 60\n";
                text << "    mov rdi, " << code << "\n";
                text << "    syscall\n";
            }
        }

        // print ... ;
        else if (tok.type == TokenType::_print) {
            // print "texto";
            if (i + 2 < tokens.size() &&
                tokens.at(i + 1).type == TokenType::string_lit &&
                tokens.at(i + 2).type == TokenType::semi) {

                std::string val = tokens.at(i + 1).value.value();
                std::string label = "msg" + std::to_string(msg_id++);
                std::string esc = escape_for_nasm(val);
                data << label << " db \"" << esc << "\", 0xA\n";
                text << "    ; print \"" << esc << "\"\n";
                text << "    mov rax, 1\n";
                text << "    mov rdi, 1\n";
                text << "    mov rsi, " << label << "\n";
                text << "    mov rdx, " << (val.size() + 1) << "\n";
                text << "    syscall\n";
            }
            // print <int>;
            else if (i + 2 < tokens.size() &&
                     tokens.at(i + 1).type == TokenType::int_lit &&
                     tokens.at(i + 2).type == TokenType::semi) {

                std::string val = tokens.at(i + 1).value.value();
                std::string label = "msg" + std::to_string(msg_id++);
                data << label << " db \"" << val << "\", 0xA\n";
                text << "    ; print " << val << "\n";
                text << "    mov rax, 1\n";
                text << "    mov rdi, 1\n";
                text << "    mov rsi, " << label << "\n";
                text << "    mov rdx, " << (val.size() + 1) << "\n";
                text << "    syscall\n";
            }
            
            
            // print <int> + <int> ;
            else if (i + 4 < tokens.size() &&
                     tokens.at(i + 1).type == TokenType::int_lit &&
                     tokens.at(i + 2).type == TokenType::_plus &&
                     tokens.at(i + 3).type == TokenType::int_lit &&
                     tokens.at(i + 4).type == TokenType::semi) {

                int lhs = std::stoi(tokens.at(i + 1).value.value());
                int rhs = std::stoi(tokens.at(i + 3).value.value());
                int sum = lhs + rhs;
                std::string sval = std::to_string(sum);

                std::string label = "msg" + std::to_string(msg_id++);
                data << label << " db \"" << sval << "\", 0xA\n";
                text << "    ; print " << lhs << " + " << rhs << " = " << sval << "\n";
                text << "    mov rax, 1\n";
                text << "    mov rdi, 1\n";
                text << "    mov rsi, " << label << "\n";
                text << "    mov rdx, " << (sval.size() + 1) << "\n";
                text << "    syscall\n";
            }
            //print <int> - <int> ;
            else if (i + 4 < tokens.size() &&
                     tokens.at(i + 1).type == TokenType::int_lit &&
                     tokens.at(i + 2).type == TokenType::_minus &&
                     tokens.at(i + 3).type == TokenType::int_lit &&
                     tokens.at(i + 4).type == TokenType::semi) {

                int lhs = std::stoi(tokens.at(i + 1).value.value());
                int rhs = std::stoi(tokens.at(i + 3).value.value());
                int minus = lhs - rhs;
                std::string sval = std::to_string(minus);

                std::string label = "msg" + std::to_string(msg_id++);
                data << label << " db \"" << sval << "\", 0xA\n";
                text << "    ; print " << lhs << " - " << rhs << " = " << sval << "\n";
                text << "    mov rax, 1\n";
                text << "    mov rdi, 1\n";
                text << "    mov rsi, " << label << "\n";
                text << "    mov rdx, " << (sval.size() + 1) << "\n";
                text << "    syscall\n";
            }

            else if (tok.type == TokenType::_print) {
                if (i + 2 < tokens.size() &&
                    tokens.at(i + 1).type == TokenType::_identifier &&
                    tokens.at(i + 2).type == TokenType::semi) {
            
                    std::string varname = tokens.at(i + 1).value.value();
                    std::string label = "buf" + std::to_string(msg_id++);
            
                    data << label << " db 20 dup(0)\n";
            
                    text << "    ; print variable " << varname << "\n";
                    text << "    mov rax, [" << varname << "]\n";  
                    text << "    mov rdi, " << label << "\n";      
                    text << "    mov rcx, 0\n";                    

                    text << label << "_convert_loop:\n";
                    text << "    xor rdx, rdx\n";
                    text << "    mov rbx, 10\n";
                    text << "    div rbx\n";
                    text << "    add dl, '0'\n";
                    text << "    mov [rdi + rcx], dl\n";
                    text << "    inc rcx\n";
                    text << "    test rax, rax\n";
                    text << "    jnz " << label << "_convert_loop\n";

                    text << "    mov rsi, rdi\n"; 
                    text << "    add rsi, rcx\n"; 

                    text << label << "_print_loop:\n";
                    text << "    dec rsi\n";
                    text << "    mov al, [rsi]\n";
                    text << "    mov [rdi], al\n"; 
                    text << "    loop " << label << "_print_loop\n";

                    text << "    mov rax, 1\n";
                    text << "    mov rdi, 1\n";       
                    text << "    mov rsi, " << label << "\n"; 
                    text << "    mov rdx, rcx\n";    
                    text << "    syscall\n";

                }
            }
            
            
            
            else {
                std::cerr << "You messed up: malformed print statement\n";
                exit(EXIT_FAILURE);
            }
        }

        // comment = !! <string> ;
        else if (tok.type == TokenType::comment) {
            text << "    ; " << tok.value.value() << "\n";
        }

        else if (tok.type == TokenType::_int) {
            if (i + 3 < tokens.size() &&
                tokens.at(i + 1).type == TokenType::_identifier &&
                tokens.at(i + 2).type == TokenType::assign &&
                tokens.at(i + 3).type == TokenType::int_lit &&
                tokens.at(i + 4).type == TokenType::semi) {
                
                std::string varname = tokens.at(i + 1).value.value();
                std::string intval  = tokens.at(i + 3).value.value();
        
                data << varname << " dq " << intval << "\n";
                text << "    ; int " << varname << " = " << intval << "\n";
        
                i += 4; 
            }
            else {
                std::cerr << "You messed up: malformed int declaration\n";
                exit(EXIT_FAILURE);
            }
        }
        
        
        
    }

    // Ensamblado final
    out << ";copyright (c) 2025 Nod\n";
    out << "global _start\n";
    out << "section .data\n";
    out << data.str();
    out << "section .text\n";
    out << "_start:\n";
    out << text.str();
    return out.str();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage. Correct use is:\n";
        std::cerr << "compiler <input.nod>\n";
        return EXIT_FAILURE;
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    std::vector<Token> tokens = tokenize(contents);

    {
        std::fstream file("./out.asm", std::ios::out);
        file << tokens_to_asm(tokens);
    }

    system("nasm -f elf64 ./out.asm -o ./out.o");
    system("ld ./out.o -o ./out");
    system("./out");

    return EXIT_SUCCESS;
}
