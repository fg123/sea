#include "Parser.h"
#include "Error.h"

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
        default: break;
    }
    throw GeneralError("Invalid token to TokenToOperator: " + std::string(TokenTypeString[(int) t]));
}

Expression* Parser::ParsePrimaryExpression() {
    if (Match(TokenType::LPAREN)) {
        Expression* result = ParsePrimaryExpression();
        Expect(TokenType::RPAREN, "Expected ')' to close '('");
        return result;
    }
    else if (Match(TokenType::IDENTIFIER)) {
        NameExpression* result = MakeNode<NameExpression>();
        result->name = tokenList[ptr - 1].value;
        return result;
    }
    else if (
        Match(TokenType::INT_LITERAL) ||
        Match(TokenType::LONG_LITERAL) ||
        Match(TokenType::DOUBLE_LITERAL) ||
        Match(TokenType::FLOAT_LITERAL) ||
        Match(TokenType::CHAR_LITERAL) ||
        Match(TokenType::NULL_LITERAL) ||
        Match(TokenType::TRUE_LITERAL) ||
        Match(TokenType::FALSE_LITERAL) ||
        Match(TokenType::STRING_LITERAL)) {
        LiteralExpression* result = MakeNode<LiteralExpression>();
        result->token = tokenList[ptr - 1];
        return result;
    }
    // Collection Literal
    // Object Literal 
    // Function Literal
    else if (Match(TokenType::THIS)) {
        return MakeNode<ThisExpression>();
    }
    else if (Match(TokenType::SUPER)) {
        return MakeNode<SuperExpression>();
    }
    else if (Match(TokenType::IF)) {
        IfExpression* ifExpression = MakeNode<IfExpression>();
        Expect(TokenType::LPAREN, "Expected '(' after 'if'");
        ifExpression->condition = ParseExpression();
        Expect(TokenType::RPAREN, "Expected ')' after condition");
        ifExpression->trueStatement = ParseStatement();
        if (Match(TokenType::ELSE)) {
            ifExpression->falseStatement = ParseStatement();
        }
        return ifExpression;
    }
    else if (Match(TokenType::WHEN)) {
        WhenExpression* whenExpression = MakeNode<WhenExpression>();
        if (Match(TokenType::LPAREN)) {
            // When has a subject
            if (Match(TokenType::CONST)) {
                // When has a binding
                const Token& idTok = Expect(TokenType::IDENTIFIER, "Expected identifier for 'when' subject binding");
                whenExpression->bindingId = idTok.value;
                Expect(TokenType::ASSIGNMENT, "Expected '=' after 'when' subject binding");
            }
            whenExpression->subject = ParseExpression();
            Expect(TokenType::RPAREN, "Expected ')' after 'when' subject");
        }
        Expect(TokenType::LBRACE, "Expected '{' to start 'when' entries");
        while (Peek() != TokenType::RBRACE) {
            if (Match(TokenType::ELSE)) {
                Expect(TokenType::RANGE, "Expected '->' for 'when' else case");
                whenExpression->elseEntry = ParseStatement();
                break;
            }
            WhenEntry entry;
            do {
                entry.tests.push_back(ParseExpression());
            } while (Match(TokenType::COMMA));
            Expect(TokenType::RANGE, "Expected '->' for 'when' entry");
            entry.body = ParseStatement();
            whenExpression->entries.push_back(entry);
        }
        Expect(TokenType::RBRACE, "Expected '}' to end 'when' entries");
        return whenExpression;
    }
    // Try Expression TODO
    throw SeaError(tokenList[ptr].fileName,
        tokenList[ptr].lineNumber, tokenList[ptr].colNumber, "Could not parse expression!");
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
    else if (Match(TokenType::LPAREN)) {
        // Call
        lhs = ParseCallSuffix(lhs);
    }
    else if (Match(TokenType::LBRACKET)) {
        // Index
        Expression* expr = MakeBinary(lhs, ParseExpression(), BinaryExpression::Operator::GET);
        Expect(TokenType::RBRACKET, "Expected ']' after indexing operator");
        lhs = expr;
    }
    else if (Match(TokenType::DOT) || Match(TokenType::SAFE_DOT)) {
        // Navigation
        NavigationExpression* expr = MakeNode<NavigationExpression>();
        expr->lhs = lhs;
        expr->safeNavigation = (lastMatch == TokenType::SAFE_DOT);
        const Token& idTok = Expect(TokenType::IDENTIFIER, "Expected identifier after '.'");
        expr->id = idTok.value;
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
        TokenType savedLastMatch = lastMatch;
        Expression* rhs = ParseAsExpression();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(savedLastMatch));
    }
    return lhs;
}

Expression* Parser::ParseAdditiveExpression() {
    Expression* lhs = ParseMultiplicativeExpression();
    while (Match(TokenType::ADD) || Match(TokenType::SUB)) {
        TokenType savedLastMatch = lastMatch;
        Expression* rhs = ParseMultiplicativeExpression();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(savedLastMatch));
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

Expression* Parser::ParseCallSuffix(Expression* lhs) {
    CallExpression* result = MakeNode<CallExpression>();
    result->lhs = lhs;
    if (Peek() != TokenType::RPAREN) {
        do {
            result->args.push_back(ParseExpression());
        } while (Match(TokenType::COMMA));
    }
    Expect(TokenType::RPAREN, "Expected ')' to end argument list");
    return result;
}

Expression* Parser::ParseCallExpression() {
    Expression* lhs = ParseInfixOperation();
    while (Match(TokenType::LPAREN)) {
        lhs = ParseCallSuffix(lhs);
    }
    return lhs;
}

Expression* Parser::ParseComparison() {
    Expression* lhs = ParseCallExpression();
    while (Match(TokenType::LT) ||
           Match(TokenType::GT) ||
           Match(TokenType::LE) || 
           Match(TokenType::GE)) {
        TokenType savedLastMatch = lastMatch;
        Expression* rhs = ParseCallExpression();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(savedLastMatch));
    }
    return lhs;
}

Expression* Parser::ParseEquality() {
    Expression* lhs = ParseComparison();
    while (Match(TokenType::EQ) || Match(TokenType::NE)) {
        TokenType savedLastMatch = lastMatch;
        Expression* rhs = ParseComparison();
        lhs = MakeBinary(lhs, rhs, TokenToOperator(savedLastMatch));
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
    return ParseDisjunction();
}