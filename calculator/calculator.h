#pragma once

#include <sstream>
#include <memory>
#include <string>
#include <iostream>

class Expression;
class Term;
class Tokenizer {
public:
    friend Term;
    friend Expression;
    explicit Tokenizer(std::istream* in) : in_{in} {
        Consume();
    }

    enum TokenType { kNumber, kSymbol, kEnd };

    void Consume() {
        char c;
        *in_ >> c;
        if (in_->eof()) {
            prev_type_ = type_;
            type_ = kEnd;
            return;
        }
        if (std::isdigit(c)) {
            in_->putback(c);
            prev_number_ = number_;
            *in_ >> number_;
            prev_type_ = type_;
            type_ = kNumber;
        } else {
            prev_symbol_ = symbol_;
            symbol_ = c;
            prev_type_ = type_;
            type_ = kSymbol;
        }
    }

    void PutBack() {
//        std::cout << "--------------PutBack--------------" << std::endl;
        if (type_ == kNumber) {
            type_ = prev_type_;
            auto str_num = std::to_string(number_);
            for (int i = str_num.size() - 1; i >= 0; --i) {
//                std::cout << str_num << std::endl;
                in_->putback(str_num[i]);
            }
            number_ = prev_number_;

        } else if (type_ == kSymbol) {
            type_ = prev_type_;
            in_->putback(symbol_);
            symbol_ = prev_symbol_;
        }
    }

    TokenType GetType() const {
        return type_;
    }

    int64_t GetNumber() const {
        return number_;
    }

    char GetSymbol() const {
        return symbol_;
    }

private:
    std::istream* in_;
    TokenType type_ = kNumber;
    TokenType prev_type_;
    int64_t number_ = 0;
    int64_t prev_number_ = 0;
    char symbol_;
    char prev_symbol_;
};

class ExpressionBase {
public:
    virtual ~ExpressionBase() = default;
    virtual int64_t Evaluate() const = 0;
};

    // void Stats(std::shared_ptr<Tokenizer> tokenizer) {
    //     std::cout << "<===Stats===>" << std::endl;
    //     std::cout << "type: " << (tokenizer->GetType() == 0 ? "number" : "symbol") << std::endl;
    //     std::cout << "symbol: " << tokenizer->GetSymbol() << std::endl;
    //     std::cout << "number: " << tokenizer->GetNumber() << std::endl;
    // }

class Expression : public ExpressionBase {
public:
    Expression(std::shared_ptr<Tokenizer> tokenizer) : tokenizer_(tokenizer) {
    }
    int64_t Evaluate() const override;

private:
    std::shared_ptr<Tokenizer> tokenizer_;
};

class Factor : public ExpressionBase {
public:
    Factor(std::shared_ptr<Tokenizer> tokenizer) : tokenizer_(tokenizer) {
    }

    int64_t Evaluate() const override {
        auto type = tokenizer_->GetType();
        if (type == Tokenizer::kSymbol) {
            char sym = tokenizer_->GetSymbol();
            tokenizer_->Consume();
            if (sym == '-') {
                int64_t value = Evaluate();
                return -value;
            } else if (sym == '(') {
                Expression expr(tokenizer_);
                int64_t value = expr.Evaluate();
                tokenizer_->Consume();  // current lexeme is right brace (if not it is an error)
                return value;
            }
        } else if (type == Tokenizer::kNumber) {
            int64_t value = tokenizer_->GetNumber();
            tokenizer_->Consume();
            return value;
        }
        throw;
    }

private:
    std::shared_ptr<Tokenizer> tokenizer_;
};

class Term : public ExpressionBase {
public:
    Term(std::shared_ptr<Tokenizer> tokenizer) : tokenizer_(tokenizer) {
    }

    int64_t Evaluate() const override {
        Factor factor(tokenizer_);
        int64_t value = factor.Evaluate();
        while (true) {
            auto type = tokenizer_->GetType();
            if (type == Tokenizer::kSymbol) {
                char sym = tokenizer_->GetSymbol();
                tokenizer_->Consume();
                if (sym == '*') {
                    value *= factor.Evaluate();
                } else if (sym == '/') {
                    value /= factor.Evaluate();
                } else {
                    tokenizer_->PutBack();
                    return value;
                }
            } else {
                break;
            }
        }
        return value;
    }

private:
    std::shared_ptr<Tokenizer> tokenizer_;
};

int64_t Expression::Evaluate() const {
    Term term(tokenizer_);
    int64_t value = term.Evaluate();
    while (true) {
        auto type = tokenizer_->GetType();
        if (type == Tokenizer::kSymbol) {
            char sym = tokenizer_->GetSymbol();
            tokenizer_->Consume();
            if (sym == '+') {
                value += term.Evaluate();
            } else if (sym == '-') {
                value -= term.Evaluate();
            } else {
                tokenizer_->PutBack();
                break;
            }
        } else {
            break;
        }
    }
    return value;
}

std::unique_ptr<ExpressionBase> ParseExpression(Tokenizer* tokenizer) {
    auto token_type = tokenizer->GetType();
    if (token_type == Tokenizer::kEnd) {
        return nullptr;
    }
    return std::make_unique<Expression>(Expression(std::make_shared<Tokenizer>(*tokenizer)));
}
