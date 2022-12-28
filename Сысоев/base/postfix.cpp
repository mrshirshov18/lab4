
#include "postfix.h"


bool TPostfix::isNumber(char c) {
    return '0' <= c && c <= '9';
}
bool TPostfix::isLetter(char c) {
    return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
}
bool TPostfix::isOperation(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}
bool TPostfix::isMinus(char c) {
    return c == '-';
}
bool TPostfix::isOperator_open(char c) {
    return c == '(';
}
bool TPostfix::isOperator_close(char c) {
    return c == ')';
}
bool TPostfix::isPoint(char c) {
    return c == '.';
}
bool TPostfix::isSin(string s) {
    return s == "sin";
}
bool TPostfix::isCos(string s) {
    return s == "cos";
}
bool TPostfix::isLog(string s) {
    return s == "log";
}
bool TPostfix::isFunc(string s) {
    return isSin(s) || isCos(s) || isLog(s);
}




void TPostfix::Parse() {
    lexemType type = nothing;
    lexemType prev_type = type;
    int brackets = 0;// open++, close--
    int left = 0;//левая граница лексемы, i-будет свегда правой границей
    int start_arg = 0;
    int bracket_for_func;
    string name_variable;
    for (int i = 0; i < input_expression.size(); i++) {
        char c = input_expression[i];
        if (c == ' ')
            continue;
        switch (type) {
        case (nothing):
            prev_type = type;
            // начало выражения
            if (isOperator_open(c)) {
                left = i;
                type = operator_open;
            }
            else if (isNumber(c) || isPoint(c) || isMinus(c)) {
                left = i;
                type = number;
            }
            else if (isLetter(c)) {
                left = i;
                type = variable;
            }

            else {
                throw invalid_argument("Invalid expression");
            }
            break;
        case (number):
            prev_type = type;
            if ((isPoint(c)) & (isPoint(input_expression[i - 1])))//две точки подряд нельзя
                throw invalid_argument("Invalid expression");
            if ((isPoint(c) & (!isMinus(input_expression[i - 1]))) || isNumber(c))
                continue;//всё ещё число
            infix.emplace_back(number, input_expression.substr(left, i - left));// добавили
            if (isOperator_close(c)) {
                left = i;
                type = operator_close;
            }
            else if (isOperation(c)) {
                left = i;
                type = operation;
            }
            else {
                throw invalid_argument("Invalid expression");
            }
            break;
        case (variable):
            if (isLetter(c) && prev_type != func)
                continue;
            name_variable = input_expression.substr(left, i - left);

            prev_type = type;

            if (isOperator_close(c)) {
                left = i;
                type = operator_close;

                infix.emplace_back(variable, name_variable);
                operands.insert({ name_variable, 0.0 });
            }
            else if (isOperation(c)) {
                if (i != left) {
                    infix.emplace_back(variable, name_variable);
                    operands.insert({ name_variable, 0.0 });
                }
                left = i;
                type = operation;

            }
            else if (isOperator_open(c) && isFunc(name_variable)) {
                start_arg = i + 1;
                type = func;
                infix.emplace_back(variable, "%");
                if (isSin(name_variable))
                    expressions.emplace_back(Sin, TPostfix());

                if (isCos(name_variable))
                    expressions.emplace_back(Cos, TPostfix());

                if (isLog(name_variable))
                    expressions.emplace_back(Log, TPostfix());

                bracket_for_func = 1;
            }
            else {
                throw invalid_argument("Invalid expression");
            }
            break;
        case (operation):
            prev_type = type;
            infix.emplace_back(operation, input_expression.substr(left, i - left));
            if (isOperator_open(c)) {
                left = i;
                type = operator_open;
            }
            else if (isNumber(c) || isPoint(c)) {
                left = i;
                type = number;
            }
            else if (isLetter(c)) {
                left = i;
                type = variable;
            }
            else {
                throw invalid_argument("Invalid expression");
            }
            break;
        case (operator_open):
            prev_type = type;
            brackets++;
            infix.emplace_back(operator_open, input_expression.substr(left, i - left));
            if (isOperator_open(c)) {
                left = i;
                type = operator_open;
            }
            else if (isNumber(c) || isPoint(c) || isMinus(c)) {
                left = i;
                type = number;
            }
            else if (isLetter(c)) {
                left = i;
                type = variable;
            }
            else {
                throw invalid_argument("Invalid expression");
            }
            break;
        case (operator_close):
            prev_type = type;
            brackets--;
            infix.emplace_back(operator_close, input_expression.substr(left, i - left));
            if (isOperator_close(c)) {
                left = i;
                type = operator_close;
            }
            else if (isOperation(c)) {
                left = i;
                type = operation;
            }
            else {
                throw invalid_argument("Invalid expression");
            }
            break;
        case (func):
            if (isOperator_open(c)) {
                bracket_for_func++;
            }
            else if (isOperator_close(c)) {
                bracket_for_func--;
            }
            if (bracket_for_func == 0) {
                expressions.back().second = TPostfix(input_expression.substr(start_arg, i - start_arg));
                left = i + 1;
                prev_type = type;
                type = variable;
            }
            break;
        }
    }

    if (prev_type!=func)
        infix.emplace_back(type, input_expression.substr(left, input_expression.size() - left));//последняя лексема
    if (type == variable && prev_type != func)
        operands.insert({ input_expression.substr(left,input_expression.size() - left), 0.0 });
    if (type == operator_close && prev_type != func)
        brackets--;
    if (type == operator_open)
        brackets++;
    if (type == func)
        expressions.back().second = TPostfix(input_expression.substr(start_arg, input_expression.size() - start_arg-1));
    //string y = input_expression.substr(start_arg, input_expression.size() - start_arg-1);

    if (brackets != 0)
        throw invalid_argument("Invalid expression: troubles with brackets");
    if ((type != operator_close && type != number && type != variable && prev_type != func)) {
        throw invalid_argument("Invalid expression: invalid ending");
    }

    //for (auto i : infix) {
    //    std::cout << i.second << "_";
    //}
    //std::cout << '\n';
}

void TPostfix::ToPostfix() {
    Parse();
    Stack<pair<lexemType, string>> st;
    pair<lexemType, string> stelem;
    for (auto& item : infix) {
        switch (item.first) {
        case operator_open:
            st.push(item);
            break;
        case operator_close:
            stelem = st.top();
            st.pop();
            while (stelem.first != operator_open) {
                postfix.emplace_back(stelem);
                stelem = st.top();
                st.pop();
            }
            break;
        case operation:
            while (!st.empty()) {
                stelem = st.top();
                st.pop();
                if (priority[item.second[0]] <= priority[stelem.second[0]])
                    postfix.emplace_back(stelem);
                else {
                    st.push(stelem);
                    break;
                }
            }
            st.push(item);
            break;
        default:
            postfix.emplace_back(item);
        }
    }
    while (!st.empty()) {
        stelem = st.top();
        st.pop();
        postfix.emplace_back(stelem);
    }
}

void TPostfix::GetValues(istream& input, ostream& output) {
    output << "enter values" << endl;
    for (auto& op : operands) {
        output << "Enter value of " << op.first << ": ";
        //input>>op.second;
        double val;
        input >> val;
        op.second = val;
    }
}

double TPostfix::Calculate(istream& input, ostream& output) { // Ввод переменных, вычисление по постфиксной форме
    GetValues(input, output);

    double val;

    int i = 0;
    for (auto& lexem : postfix) {
        if (lexem.second[0] == '%') {
            lexem.first = number;
            val = expressions[i].second.Calculate(input, output);
            if (expressions[i].first == Cos)
                val = cos(val);
            if (expressions[i].first == Sin)
                val = sin(val);
            if (expressions[i].first == Log)
                val = log(val);
            lexem.second = "";
            if (val < 0) {
                lexem.second += "!";
            }
            lexem.second += to_string(val);
            i++;
        }
    }

    Stack<double> st;
    double leftoperand, rightoperand;
    for (auto& lexem : postfix) {
        switch (lexem.second[0]) {
        case'+':
            rightoperand = st.top();
            st.pop();
            leftoperand = st.top();
            st.pop();
            st.push(leftoperand + rightoperand);
            break;
        case '-':
            rightoperand = st.top();
            st.pop();
            leftoperand = st.top();
            st.pop();
            st.push(leftoperand - rightoperand);
            break;
        case '*':
            rightoperand = st.top();
            st.pop();
            leftoperand = st.top();
            st.pop();
            st.push(leftoperand * rightoperand);
            break;
        case '/':
            rightoperand = st.top();
            st.pop();
            leftoperand = st.top();
            st.pop();
            st.push(leftoperand / rightoperand);
            break;
        case '!':
            st.push(stod(lexem.second.substr(1)));
            break;
        default:

            if (lexem.first == variable)
                st.push(operands[lexem.second]);
            else
                st.push(stod(lexem.second));
        }
    }
    return st.top();
}
