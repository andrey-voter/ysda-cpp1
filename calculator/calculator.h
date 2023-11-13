#pragma once

#include <sstream>
#include <memory>
#include <format>
#include <string>

class Tokenizer {
public:
    explicit Tokenizer(std::istream* in) : in_{in} {
        Consume();
    }

    enum TokenType { kNumber, kSymbol, kEnd };

    void Consume() {
        char c;
        *in_ >> c;
        if (in_->eof()) {
            type_ = kEnd;
            return;
        }
        if (std::isdigit(c)) {
            in_->putback(c);
            *in_ >> number_;
            type_ = kNumber;
        } else {
            symbol_ = c;
            type_ = kSymbol;
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

    TokenType type_;
    int64_t number_;
    char symbol_;
};

class ExpressionBase {
public:
    virtual ~ExpressionBase() = default;
    virtual int64_t Evaluate() const = 0;
};

class Term;

class Expression : public ExpressionBase {
public:
    Expression(Tokenizer* tokenizer) : tokenizer_(tokenizer) {
    }
    int64_t Evaluate() const override {
        Term term(tokenizer_);
        int64_t value = term.Evaluate();
    }

private:
    Tokenizer* tokenizer_;
};

class Factor : public ExpressionBase {
public:
    Factor(Tokenizer* tokenizer) : tokenizer_(tokenizer) {
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
                tokenizer_->Consume();  // current lexeme is right brace
                return value;
            }
        } else if (type == Tokenizer::kNumber) {
            int64_t value = tokenizer_->GetNumber();
            tokenizer_->Consume();
            return value;
        }
        return 0;  //? eof
    }

private:
    Tokenizer* tokenizer_;
};

class Term : public ExpressionBase {
public:
    Term(Tokenizer* tokenizer) : tokenizer_(tokenizer) {
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
                    break;
                }
            } else { // error, there should be symbol, not number (it can be eof)
                break;
            }
        }
        return value;
    }

private:
    Tokenizer* tokenizer_;
};




//class Constant : public ExpressionBase {
//public:
//    Constant(int64_t value) : value_(value) {
//    }
//
//    int64_t Evaluate() const override {
//        return value_;
//    }
//
//private:
//    int64_t value_;
//};
//
//class UnaryExpression : public ExpressionBase {
//public:
//    UnaryExpression(char operation, std::unique_ptr<ExpressionBase> operand)
//        : operation_(operation), operand_(std::move(operand)) {
//    }
//
//    int64_t Evaluate() const override {
//        if (operation_ == '-') {
//            return -operand_->Evaluate();
//        } else {
//            throw std::runtime_error("Unknown unary expression");
//        }
//    }
//
//private:
//    char operation_;
//    std::unique_ptr<ExpressionBase> operand_;
//};
//
//class BinaryExpression : public ExpressionBase {
//public:
//    BinaryExpression(char operation, std::unique_ptr<ExpressionBase> left,
//                     std::unique_ptr<ExpressionBase> right)
//        : operation_(operation), left_(std::move(left)), right_(std::move(right)) {
//    }
//
//    int64_t Evaluate() const override {
//        switch (operation_) {
//            case '+':
//                return left_->Evaluate() + right_->Evaluate();
//            case '-':
//                return left_->Evaluate() - right_->Evaluate();
//            case '*':
//                return left_->Evaluate() * right_->Evaluate();
//            case '/':
//                return left_->Evaluate() / right_->Evaluate();
//            default:
//                throw std::runtime_error("Unknown binary expression");
//        }
//    }
//
//private:
//    char operation_;
//    std::unique_ptr<ExpressionBase> left_;
//    std::unique_ptr<ExpressionBase> right_;
//};

std::unique_ptr<ExpressionBase> ParseExpression(Tokenizer* tokenizer) {
    //    auto token_type = tokenizer->GetType();
    //    if (token_type == Tokenizer::kNumber) {
    //        auto number = tokenizer->GetNumber();
    //        tokenizer->Consume();
    //        return std::make_unique<Constant>(number);
    //    } else if (token_type == Tokenizer::kSymbol) {
    //        char symbol = tokenizer->GetSymbol();
    //        tokenizer->Consume();
    //        if (symbol == '(') {
    //            auto inner_expression = ParseExpression(tokenizer);
    //            // error handling
    //            tokenizer->Consume();
    //            return inner_expression;
    //        }
    //        if (symbol == '-') {
    //            tokenizer->Consume();
    //            auto operand = ParseExpression(tokenizer);
    //            return std::make_unique<UnaryExpression>(symbol, std::move(operand));
    //        }
    //    }
    return nullptr;
}
