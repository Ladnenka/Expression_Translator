#include "../AbstractTranslator.h"

AbstractTranslator::TranslateContext::~TranslateContext() {}

AbstractTranslator::AbstractTranslator(TranslateContext* ctx) : context(ctx) {
    context->translator = this;
}

AbstractTranslator::~AbstractTranslator() { delete context; }

AbstractTranslator::TranslateContext::LoadedData::LoadedData() {}

QString AbstractTranslator::translateExpression(ExprNode* root) {

    preLines.clear();
    postLines.clear();

    QString mainResult = translate(root);

    return (preLines + QStringList{mainResult} + postLines).join("\n");
}

void AbstractTranslator::initTranslateTable() {
    translateTable = {
                      { ExprNode::PLUS,         &AbstractTranslator::translateSum         },
                      { ExprNode::MINUS,        &AbstractTranslator::translateSub         },
                      { ExprNode::MULTIPLY,     &AbstractTranslator::translateMultiply    },
                      { ExprNode::DIVIDE,       &AbstractTranslator::translateDivide      },
                      { ExprNode::MODULO,       &AbstractTranslator::translateModulo      },
                      { ExprNode::POWER,        &AbstractTranslator::translatePower       },
                      { ExprNode::UNARY_MINUS,  &AbstractTranslator::translateUnaryMinus  },
                      { ExprNode::LOGICAL_AND,  &AbstractTranslator::translateLogicalAnd  },
                      { ExprNode::LOGICAL_OR,   &AbstractTranslator::translateLogicalOr   },
                      { ExprNode::LESS,         &AbstractTranslator::translateLess        },
                      { ExprNode::GREATER,      &AbstractTranslator::translateGreater     },
                      { ExprNode::LESS_EQ,      &AbstractTranslator::translateLessEq     },
                      { ExprNode::GREATER_EQ,   &AbstractTranslator::translateGreaterEq  },
                      { ExprNode::EQUAL,        &AbstractTranslator::translateEqual       },
                      { ExprNode::NOT_EQUAL,    &AbstractTranslator::translateNotEqual    },
                      { ExprNode::ASSIGN,       &AbstractTranslator::translateAssign      },
                      { ExprNode::PLUS_ASSIGN,  &AbstractTranslator::translatePlusAssign  },
                      { ExprNode::MINUS_ASSIGN, &AbstractTranslator::translateMinusAssign },
                      { ExprNode::MULT_ASSIGN,  &AbstractTranslator::translateMultAssign  },
                      { ExprNode::DIV_ASSIGN,   &AbstractTranslator::translateDivAssign   },
                      { ExprNode::MOD_ASSIGN,   &AbstractTranslator::translateModAssign   },
                      { ExprNode::PRE_INC,      &AbstractTranslator::translatePreInc      },
                      { ExprNode::POST_INC,     &AbstractTranslator::translatePostInc     },
                      { ExprNode::PRE_DEC,      &AbstractTranslator::translatePreDec      },
                      { ExprNode::POST_DEC,     &AbstractTranslator::translatePostDec     },
                      { ExprNode::ADDRESS_OF,   &AbstractTranslator::translateAddressOf   },
                      { ExprNode::INDEX,        &AbstractTranslator::translateIndex        },
                      };
}

QString AbstractTranslator::translate(ExprNode* expr) {
    //Проверить существует ли переданный узел
    //Если указатель на узел отсутствует
    //Вернуть пустую строку
    if (!expr) return "";

    //Перевести узлы, не требующие построения списка операндов
    if (expr->type == ExprNode::NUMBER)   return translateNumber(expr);
    if (expr->type == ExprNode::VARIABLE) return context->getVarDescription(expr->varName);
    if (expr->type == ExprNode::FUNCTION) return context->getFuncDescription(expr->funcName, expr->operands);

    //Построить список строк операндов
    QVector<QString> operandStrings;
    for (ExprNode* operand : expr->operands) {
        QString operandStr = translate(operand);
        if (needParentheses(expr->type, operand->type))
            operandStr = "(" + operandStr + ")";
        operandStrings.push_back(operandStr);
    }

    //Перевести узлы, требующие доступа к самому узлу помимо строк операндов
    if (expr->type == ExprNode::LOGICAL_NOT) return translateLogicalNot(expr, operandStrings);
    if (expr->type == ExprNode::DEREFERENCE) return translateDereference(expr, operandStrings);

    //Переводиим все остальные операторы — через таблицу диспетчеризации
    auto it = translateTable.find(expr->type);
    return it != translateTable.end() ? (this->*it.value())(operandStrings) : "";
}

bool AbstractTranslator::needParentheses(ExprNode::ExprType exprType, ExprNode::ExprType operandType) {return false;}

QString AbstractTranslator::translateNumber(const ExprNode* expr) { return expr->value; }
