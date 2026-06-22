#include "../TreeSimplifier.h"

void TreeSimplifier::simplify(ExprNode*& root) {
    //Проверить, что указатель не является пустым
    if (!root) return;

    //Для каждого узла, что является дочерним для переданного узла
    for (int i = 0; i < root->operands.size(); i++)
        //Вызвать рекурсивный метод упрощения
        simplify(root->operands[i]);

    //Если тип узла поддерживает упрощение
    //Вызвать соответствующий метод упрощения узла
    if (root->type == ExprNode::PLUS)
        simplifyPlus(root);
    else if (root->type == ExprNode::MINUS)
        simplifyMinus(root);
    else if (root->type == ExprNode::MULTIPLY)
        simplifyMultiply(root);
    else if (root->type == ExprNode::DIVIDE)
        simplifyDivide(root);
    else if (root->type == ExprNode::ADDRESS_OF || root->type == ExprNode::DEREFERENCE)
        simplifyPointer(root);
    else if (root->type == ExprNode::UNARY_MINUS)
        simplifyUnaryMinus(root);
}

void TreeSimplifier::simplifyPlus(ExprNode* root) {
    if (root->type != ExprNode::PLUS) return;

    //Создать список дочерних узлов
    QVector<ExprNode*> children;

    //Для каждого узла, что привязан к переданному узлу
    for (ExprNode* child : root->operands) {
        //Проверить тип узла
        //Если узел типа плюс
        if (child->type == ExprNode::PLUS) {
            //Добавить все дочерние узлы, что привязаны к узлу типа плюс в список дочерних узлов
            children.append(child->operands);
            //Отвязать узел от входного переданного узла
            child->operands.clear();
            delete child;
        }
        //Если узел типа минус
        else if (child->type == ExprNode::MINUS) {
            //Добавить первый дочерний узел, что привязан к узлу типа минус в список дочерних узлов
            children.append(child->operands[0]);
            //Изменить второй дочерний узел на отрицательный ему же
            //Добавить отрицательный дочерний узел в список дочерних узлов
            children.append(makeNegative(child->operands[1]));
            child->operands.clear();
            delete child;
        }
        //Иначе добавить узел в список дочерних узлов
        else {
            children.append(child);
        }
    }
    //Заменить список, привязанных узлов, у переданного узла на новый список дочерних узлов
    root->operands = children;
}

void TreeSimplifier::simplifyMinus(ExprNode* root) {
    if (root->type != ExprNode::MINUS) return;

    //Проверить тип первого дочернего узла, привязанного к переданному узлу для упрощения
    //Если тип первого привязанного узла равен плюсу
    if (root->operands[0]->type == ExprNode::PLUS) {
        //Создать новый список узлов
        QVector<ExprNode*> terms;
        //Добавить в созданный список все привязанные узлы от первого дочернего узла
        terms.append(root->operands[0]->operands);
        //Сделать второй дочерний узел отрицательным и также добавить в созданный список
        terms.append(makeNegative(root->operands[1]));

        root->operands[0]->operands.clear();
        delete root->operands[0];
        //Изменить тип переданного узла на плюс
        root->type = ExprNode::PLUS;
        //Заменить старый список узлов, привязанных к переданному узлу, на новый созданный список
        root->operands = terms;
    }
    //Иначе если тип первого дочернего узла равен вычитанию
    else if (root->operands[0]->type == ExprNode::MINUS) {
        //Взять указатель на первый привязанный узел к переданному узлу и сохранить его как "внутренний узел"
        ExprNode* inner = root->operands[0];
        //Создать новый узел типа сложение
        ExprNode* sum = new ExprNode(ExprNode::PLUS);
        //Привязать к новому узлу сложения второй дочерний узел от "внутреннего узла" и второй привязанный узел от переданного узла
        sum->operands.append(inner->operands[1]);
        sum->operands.append(root->operands[1]);

        //Вызвать функцию упрощения узла типа плюс для нового созданного узла суммы
        simplifyPlus(sum);

        root->operands[0] = inner->operands[0];
        //Назначить вторым привязанным узлом переданного узла созданный новый узел сложения
        root->operands[1] = sum;

        inner->operands.clear();
        delete inner;
    }
    //Иначе если тип второго дочернего узла равен вычитанию
    else if (root->operands[1]->type == ExprNode::MINUS) {
        //Взять указатель на второй привязанный узел к переданному узлу и сохранить его как "внутренний узел"
        ExprNode* inner = root->operands[1];
        //Создать новый узел типа сложение
        ExprNode* sum = new ExprNode(ExprNode::PLUS);
        //Привязать к новому узлу сложения первый дочерний узел  от переданного узла и второй привязанный узел от "внутреннего узла
        sum->operands.append(root->operands[0]);
        sum->operands.append(inner->operands[1]);

        //Назначить первым привязанным узлом переданного узла созданный новый узел сложения
        root->operands[0] = sum;
        //Назначить вторым привязанным узлом переданного узла первый узел "внутреннего узла
        root->operands[1] = inner->operands[0];

        inner->operands.clear();
        delete inner;
    }
}

void TreeSimplifier::simplifyMultiply(ExprNode* root) { }

void TreeSimplifier::simplifyDivide(ExprNode* root) {}

void TreeSimplifier::simplifyPointer(ExprNode*& root) {}

void TreeSimplifier::simplifyUnaryMinus(ExprNode*& root) {}

ExprNode* TreeSimplifier::makeNegative(ExprNode* op) {
    ExprNode();
}
