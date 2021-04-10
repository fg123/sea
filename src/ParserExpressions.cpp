#include "Parser.h"

Expression* Parser::MakeBinary(Expression* lhs, Expression* rhs, BinaryExpression::Operator op) {
    BinaryExpression* result = MakeNode<BinaryExpression>();
    result->lhs = lhs;
    result->rhs = rhs;
    result->op = op;
    return result;
}

BinaryExpression::Operator TokenToOperator(TokenType t) {
    switch (t) {
        case TokenType::LT: return BinaryExpression::Operator::LT;
        case TokenType::GT: return BinaryExpression::Operator::GT;
        case TokenType::LE: return BinaryExpression::Operator::LTE;
        case TokenType::GE: return BinaryExpression::Operator::GTE;
        case TokenType::EQ: return BinaryExpression::Operator::EQ;
        case TokenType::NE: return BinaryExpression::Operator::NEQ;
        
        case TokenType::ADD: return BinaryExpression::Operator::PLUS;
        case TokenType::SUB: return BinaryExpression::Operator::MINUS;

        case TokenType::MULT: return BinaryExpression::Operator::TIMES;
        case TokenType::DIV: return BinaryExpression::Operator::DIVIDE;
        case TokenType::MOD: return BinaryExpression::Operator::MODULO;
    }
}

Expression* Parser::ParsePrimaryExpression() {
}

Expression* Parser::ParseUnaryExpression() {
    // Unary Prefix
    if (Match(TokenType::NOT) || Match(TokenType::SUB) ||
        Match(TokenType::INC) || Match(TokenType::DEC)) {
        // Left Recursive
        TokenType savedLastMatch = lastMatch;
        Expression* lhs = ParseUnaryExpression();
        UnaryExpression* expr = MakeNode<UnaryExpression>();
        expr->lhs = lhs;
        if (savedLastMatch == TokenType::NOT) {
            expr->op = UnaryExpression::Operator::NOT;
        }
        else if (savedLastMatch == TokenType::SUB) {
            expr->op = UnaryExpression::Operator::NEGATE;
        }
        else if (savedLastMatch == TokenType::INC) {
            expr->op = UnaryExpression::Operator::PRE_INC;
        }
        else if (savedLastMatch == TokenType::DEC) {
            expr->op = UnaryExpression::Operator::PRE_DEC;
        }
        return expr;
    }
    Expression* lhs = ParsePrimaryExpression();
    if (Match(TokenType::INC) || Match(TokenType::DEC)) {
        UnaryExpression* expr = MakeNode<UnaryExpression>();
        expr->lhs = lhs;
        if (lastMatch == TokenType::INC) {
            expr->op = UnaryExpression::Operator::POST_INC;
        }
        else {
            expr->op = UnaryExpression::Operator::POST_DEC;
        }
        lhs = expr;
    }
    return lhs;
}

Expression* Parser::ParseAsExpression() {
    Expression* lhs = ParseUnaryExpression();
    while (Match(TokenType::AS) || Match(TokenType::SAFE_AS)) {
        BinaryTypeExpression* result = MakeNode<BinaryTypeExpression>();
        result->lhs = lhs;
        result->rhs = ParseType();
        if (lastMatch == TokenType::AS) {
            result->op = BinaryTypeExpression::Operator::AS;
        }
        else {
            result->op = BinaryTypeExpression::Operator::SAFE_AS;
        }

        lhs = result;
    }
    return lhs;
}

Expression* Parser::ParseMultiplicativeExpression() {
    Expression* lhs = ParseAsExpression();
    while (Match(TokenType::MULT) || Match(TokenType::DIV) || Match(TokenType::MOD)) {
        Expression* rhs = ParseAsExpression();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(lastMatch));
    }
    return lhs;
}

Expression* Parser::ParseAdditiveExpression() {
    Expression* lhs = ParseMultiplicativeExpression();
    while (Match(TokenType::ADD) || Match(TokenType::SUB)) {
        Expression* rhs = ParseMultiplicativeExpression();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(lastMatch));
    }
    return lhs;
}

Expression* Parser::ParseRangeExpression() {
    Expression* lhs = ParseAdditiveExpression();
    while (Match(TokenType::RANGE)) {
        Expression* rhs = ParseAdditiveExpression();
        lhs = MakeBinary(lhs, rhs, BinaryExpression::Operator::RANGE);
    }
    return lhs;
}

Expression* Parser::ParseInfixFunctionCall() {
    Expression* lhs = ParseRangeExpression();
    while (Match(TokenType::IDENTIFIER)) {
        InfixCallExpression* expr = MakeNode<InfixCallExpression>();
        expr->lhs = lhs;
        expr->functionName = tokenList[ptr - 1].value;
        expr->rhs = ParseRangeExpression();
        lhs = expr;
    }
    return lhs;
}

Expression* Parser::ParseElvisExpression() {
    Expression* lhs = ParseInfixFunctionCall();
    while (Match(TokenType::ELVIS)) {
        Expression* rhs = ParseInfixFunctionCall();
        lhs = MakeBinary(lhs, rhs, BinaryExpression::Operator::ELVIS);
    }
    return lhs;
}

Expression* Parser::ParseInfixOperation() {
    Expression* lhs = ParseElvisExpression();
    while (Match(TokenType::IN) ||
           Match(TokenType::IS)) {
        if (lastMatch == TokenType::IN) {
            Expression* rhs = ParseElvisExpression();
            lhs = MakeBinary(lhs, rhs, BinaryExpression::Operator::CONTAINS);
        }
        else {
            BinaryTypeExpression* result = MakeNode<BinaryTypeExpression>();
            result->lhs = lhs;
            result->rhs = ParseType();
            result->op = BinaryTypeExpression::Operator::IS;

            lhs = result;
        }
    }
    return lhs;
}

Expression* Parser::ParseComparison() {
    Expression* lhs = ParseInfixOperation();
    while (Match(TokenType::LT) ||
           Match(TokenType::GT) ||
           Match(TokenType::LE) || 
           Match(TokenType::GE)) {
        Expression* rhs = ParseInfixOperation();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(lastMatch));
    }
    return lhs;
}

Expression* Parser::ParseEquality() {
    Expression* lhs = ParseComparison();
    while (Match(TokenType::EQ) || Match(TokenType::NE)) {
        Expression* rhs = ParseComparison();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(lastMatch));
    }
    return lhs;
}

Expression* Parser::ParseConjunction() {
    Expression* lhs = ParseEquality();
    while (Match(TokenType::AND)) {
        Expression* rhs = ParseEquality();
        lhs = MakeBinary(lhs, rhs, BinaryExpression::Operator::AND);
    }
    return lhs;
}

Expression* Parser::ParseDisjunction() {
    Expression* lhs = ParseConjunction();
    while (Match(TokenType::OR)) {
        Expression* rhs = ParseConjunction();
        lhs = MakeBinary(lhs, rhs, BinaryExpression::Operator::OR);
    }
    return lhs;
}

Expression* Parser::ParseExpression() {
    Expression* outExpression;
    return ParseDisjunction();
}