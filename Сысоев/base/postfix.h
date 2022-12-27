
#include<iostream>
#include <string>
#include "stack.h"
#include <vector>
#include <map>
#include <cmath>
//#include <stack>
using namespace std;

class TPostfix
{
    string input_expression;
    enum lexemType {
        nothing,//начало выражения
        number, // 4; -4; .4; 
        variable, //переменная____ a;my_argument - right_____-a;a1 - wrong!
        operation, //+, -, *, /
        operator_open, // (
        operator_close,
        func
        // )
    };
    enum funcType {
        Sin,
        Cos,
        Log
    };
    vector<pair<lexemType, string>> infix; //набор лексем со значениями
    vector<pair<lexemType, string>> postfix;
    //static 
    map<char, int> priority;
    map<string, double> operands; //набор переменных(variable) со значениями (по умолчанию в них нули)
    vector<pair<funcType, TPostfix>> expressions;

    bool isNumber(char c);      // 0 .. 9
    bool isLetter(char c); // a ... z, A ... Z, _
    bool isOperation(char c);   // +, -, *, /
    bool isMinus(char c);       // -
    bool isPoint(char c);       // .
    bool isOperator_open(char c);       // (
    bool isOperator_close(char c);

    bool isSin(string s);
    bool isCos(string s);
    bool isLog(string s);
    bool isFunc(string s);

    void Parse(); //разложить на лексемы

    void GetValues(istream& input, ostream& output);
public:
    void ToPostfix();//сделать обратную польскую запись для подсчета
    TPostfix() = default;
    TPostfix(string _input_expression) :input_expression(_input_expression) {
        this->priority = { {'+',1},{'-',1},{'*',2},{'/',2}};
        if (input_expression.empty())
            throw invalid_argument("Creating arithmetic expression from an empty string");
        ToPostfix();
    }
    string GetInfix() { return input_expression; }

    string GetPostfix() {
        string ans;
        int n = 0;
        for (auto& i : postfix) {
            if (i.second[0] == '%') {
                if (expressions[n].first == Sin)
                    ans += "sin(";
                if (expressions[n].first == Cos)
                    ans += "cos(";
                if (expressions[n].first == Log)
                    ans += "log(";
                ans += expressions[n].second.GetPostfix();
                ans += ")";
                n++;
            }
            else
                ans += i.second;
            ans += ' ';
        }
        return ans;
    }

    double Calculate(istream& input, ostream& output);// Ввод переменных, вычисление по постфиксной форме
};
