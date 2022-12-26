
#include<iostream>
#include <string>
#include "stack.h"
#include <vector>
#include <map>
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
        operator_close // )
    };
    vector<pair<lexemType, string>> infix; //набор лексем со значениями
    vector<pair<lexemType, string>> postfix;
    //static 
    map<char, int> priority;
    map<string, double> operands; //набор переменных(variable) со значениями (по умолчанию в них нули)

    bool isNumber(char c);      // 0 .. 9
    bool isLetter(char c); // a ... z, A ... Z, _
    bool isOperation(char c);   // +, -, *, /
    bool isMinus(char c);       // -
    bool isPoint(char c);       // .
    bool isOperator_open(char c);       // (
    bool isOperator_close(char c);         // )

    void Parse(); //разложить на лексемы

    void GetValues(istream& input, ostream& output);
public:
    void ToPostfix();//сделать обратную польскую запись для подсчета
    TPostfix(string _input_expression) :input_expression(_input_expression) {
        this->priority = { {'+',1},{'-',1},{'*',2},{'/',2}};
        if (input_expression.empty())
            throw invalid_argument("Creating arithmetic expression from an empty string");
        ToPostfix();
    }
    string GetInfix() { return input_expression; }

    string GetPostfix() {
        string ans;
        for (auto& i : postfix) {
            ans += ' ';
            ans += i.second;
        }
        return ans;
    }

    double Calculate(istream& input, ostream& output);// Ввод переменных, вычисление по постфиксной форме
};
