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

void TreeSimplifier::simplifyMultiply(ExprNode* root) {
    if (root->type != ExprNode::MULTIPLY) return;

    //Создать список дочерних узлов
    QVector<ExprNode*> factors;

    //Для каждого узла, что привязан к переданному узлу
    for (ExprNode* child : root->operands) {
        //Проверить тип узла
        //Если узел типа умножение
        if (child->type == ExprNode::MULTIPLY) {
            //Добавить все дочерние узлы, что привязаны к узлу типа умножение в список дочерних узлов
            factors.append(child->operands);
            //Отвязать узел от входного переданного узла
            child->operands.clear();
            delete child;
        }
        //Иначе добавить узел в список дочерних узлов
        else {
            factors.append(child);
        }
    }
    //Заменить список, привязанных узлов, у переданного узла на новый список дочерних узлов
    root->operands = factors;
}

void TreeSimplifier::simplifyDivide(ExprNode* root) {
    if (root->type != ExprNode::DIVIDE) return;
    //Проверить количество  узлов, привязанных к переданному узлу
    //Если количество дочерних-привязанных узлов не равно двум
    if (root->operands.size() != 2) return;

    //Если тип первого привязанного узла равен делению
    if (root->operands[0]->type == ExprNode::DIVIDE) {
        //Взять указатель на первый привязанный к переданному узлу и сохранить как "внутренний узел"
        ExprNode* inner = root->operands[0];
        //Создать новый узел типа умножения
        ExprNode* product = new ExprNode(ExprNode::MULTIPLY);
        //Привязать к новому узлу умножения второй привязанный к "внутреннему узлу" узел и второй привязанный узел от переданного узла
        product->operands.append(inner->operands[1]);
        product->operands.append(root->operands[1]);

        //Вызвать метод упрощения умножения для созданного узла
        simplifyMultiply(product);

        //Назначить первым привязанным узлом переданного узла первый дочерний узел "внутреннего узла"
        root->operands[0] = inner->operands[0];
        //Назначить вторым привязанным узлом переданного узла новый созданный узел умножения
        root->operands[1] = product;

        inner->operands.clear();
        delete inner;
    }
    //Иначе если тип второго дочернего узла равен делению
    else if (root->operands[1]->type == ExprNode::DIVIDE) {
        //Взять указатель на второй привязанный узел к переданному узлу и сохранить его как "внутренний узел"
        ExprNode* inner = root->operands[1];
        //Создать новый узел типа умножения
        ExprNode* product = new ExprNode(ExprNode::MULTIPLY);
        //Привязать к новому узлу умножения первый привязанный узел от переданного узла и второй привязанный узел от "внутреннего узла"
        product->operands.append(root->operands[0]);
        product->operands.append(inner->operands[1]);

        //Вызвать метод упрощения умножения для созданного узла
        simplifyMultiply(product);

        //Назначить первым привязанным узлом переданного узла новый созданный узел умножения
        root->operands[0] = product;
        //Назначить вторым привязанным узлом переданного узла первый узел "внутреннего узла"
        root->operands[1] = inner->operands[0];

        inner->operands.clear();
        delete inner;
    }
}

void TreeSimplifier::simplifyPointer(ExprNode*& root) {
    //Проверить количество привязанных узлов к переданному узлу
    //Если количество привязанных узлов не равно одному
    if (!root || root->operands.size() != 1) return;


    //Достать указатель на первый привязанный узел к переданному узлу и сохранить его как узел ребёнок
    ExprNode* child = root->operands[0];


    //Установить флаг первой проверки, что тип переданного узла разыменование и тип узла ребёнка взятие адреса
    bool needSimplify1 = root->type == ExprNode::DEREFERENCE && child->type == ExprNode::ADDRESS_OF;
    //Установить флаг второй проверки, что тип переданного узла взятие адреса и тип узла ребёнка разыменование
    bool needSimplify2 = root->type == ExprNode::ADDRESS_OF && child->type == ExprNode::DEREFERENCE;

    //Если хотя бы один из флагов проверок установлен в истину
    if (needSimplify1 || needSimplify2) {
        //Заменить переданный узел на первый дочерний узел ребёнка
        ExprNode* inner = child->operands[0];

        child->operands.clear();
        delete child;
        root->operands.clear();
        delete root;
        root = inner;
        return;
    }
}

void TreeSimplifier::simplifyUnaryMinus(ExprNode*& root) {
    if (root->type != ExprNode::UNARY_MINUS) return;
    if (root->operands[0]->type != ExprNode::UNARY_MINUS) return;

    ExprNode* doubleNeg = root->operands[0];
    ExprNode* inner = doubleNeg->operands[0];

    doubleNeg->operands.clear();
    delete doubleNeg;
    root->operands.clear();
    delete root;
    root = inner;
}

ExprNode* TreeSimplifier::makeNegative(ExprNode* op) {
    ExprNode();
}
