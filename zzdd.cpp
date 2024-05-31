#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include <stack>
#include <iomanip>  // For setw()
#include <cstdlib>  // For system()

using namespace std;

// Function to perform the calculation
double calculate(double a, char op, double b) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if(b != 0)
                return a / b;
            else {
                cout << "Error: Division by zero!" << endl;
                exit(EXIT_FAILURE);
            }
        default:
            cout << "Error: Invalid operator!" << endl;
            exit(EXIT_FAILURE);
    }
}

// Function to determine the precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to apply an operator to the top two elements in the value stack
void applyOp(stack<double>& values, char op) {
    double b = values.top(); values.pop();
    double a = values.top(); values.pop();
    double result;

    switch(op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break; // Corrected subtraction operation
        case '*': result = a * b; break;
        case '/':
            if(b != 0)
                result = a / b;
            else {
                cout << "Error: Division by zero!" << endl;
                exit(EXIT_FAILURE);
            }
            break;
        default:
            cout << "Error: Invalid operator!" << endl;
            exit(EXIT_FAILURE);
    }

    values.push(result);
}

// Function to evaluate the expression
double evaluate(const string& expression) {
    stack<double> values;
    stack<char> ops;
    stringstream ss(expression);
    char c;
    bool negative = false;
    bool expectOperator = false;

    while (ss >> c) {
        if (isdigit(c) || c == '.') {
            ss.putback(c);
            double value;
            ss >> value;
            if (negative) {
                value = -value;
                negative = false;
            }
            values.push(value);
            expectOperator = true;
        } else if (c == '-' && !expectOperator) {
            // Check for unary minus (negative number)
            negative = true;
        } else if (c == '(') {
            ops.push(c);
            expectOperator = false;
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                applyOp(values, ops.top());
                ops.pop();
            }
            ops.pop(); // Remove the '(' from stack
            expectOperator = true;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                applyOp(values, ops.top());
                ops.pop();
            }
            ops.push(c);
            expectOperator = false;
        }
    }

    while (!ops.empty()) {
        applyOp(values, ops.top());
        ops.pop();
    }

    return values.top();
}

// Function to format the result as a string
string formatResult(double result) {
    stringstream ss;
    ss << result;
    string resStr = ss.str();

    // Remove trailing zeros and decimal point if the result is an integer
    if (resStr.find('.') != string::npos) {
        resStr.erase(resStr.find_last_not_of('0') + 1, string::npos);
        if (resStr.back() == '.') {
            resStr.pop_back();
        }
    }
    return resStr;
}

// Function to show the calculator interface
void show_calculator(const string& input, const string& result = "") {
    system("cls"); // clear the screen, use "clear" for Unix-based systems
    cout << "-------------------------------------------------------------" << endl;
    cout << "----                                                     ----" << endl;
    cout << "--           The current output is: " << setw(20) << left << result << "----" << endl;
    cout << "--                                                         --" << endl;
    cout << "----                                                     ----" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "--     +        |           -            |        *        --" << endl;
    cout << "--  addition    |      subtraction       | Multiplication  --" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "--    /         |          cls           |          =      --" << endl;
    cout << "--   division   |         clear          |       result    --" << endl;
    cout << "-------------------------------------------------------------" << endl;
}

int main() {
    string input;
    string result;

    while (true) {
        show_calculator(input, result);
        cout << "Enter: ";
        getline(cin, input);

        if (input == "cls") {
            input.clear();
            result.clear();
            continue;
        }

        double res = evaluate(input);
        result = formatResult(res);

        show_calculator(input, result);
        input.clear();
    }

    return 0;
}
