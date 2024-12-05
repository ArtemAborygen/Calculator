#include <iostream>

struct Token {
    char type;
    int value;
    char op;
};

// 'n' - число
// 'o' - операія
// 'p' - дужки

int factorial(int n) {
    if (n==0 || n==1) return 1;

    int result = 1;

    for (int i = 2; i<=n; i++) {
        result *= i;
    }

    return result;
}


int tokenize(const char* input, Token* tokens) {
    int i = 0, t = 0;

    while (input[i]!='\0') {
        if (input[i]>='0' && input[i]<='9') {
            int value = 0;

            while (input[i]>='0' && input[i]<='9') {
                value = value * 10 + (input[i] - '0');
                i++;
            }

            tokens[t++] = {'n', value, '\0'};

        } else if (input[i]=='+' || input[i]=='-' || input[i]=='*' || input[i]=='/' || input[i]=='^' || input[i]=='!') {
            tokens[t++] = {'o', 0, input[i]};
            i++;

        } else if (input[i]=='(' || input[i]==')') {
            tokens[t++] = {'p', 0, input[i]};
            i++;

        } else {
            return 0;
        }
    }

    return t;
}


int precedence(char op) {
    if (op=='+' || op=='-') return 1;
    if (op=='*' || op=='/') return 2;
    if (op=='^') return 3;
    if (op=='!') return 4;

    return 0;
}


// Reverse Polish notation
int RPN(Token* tokens, int tokenCount, Token* output) {
    Token stack[100];
    int stackIndex = -1, outIndex = 0;

    for (int i = 0; i<tokenCount; i++) {
        if (tokens[i].type=='n') {
            output[outIndex++] = tokens[i];

        } else if (tokens[i].type=='o') {
            while (stackIndex>=0 && precedence(stack[stackIndex].op)>=precedence(tokens[i].op)) {
                output[outIndex++] = stack[stackIndex--];
            }

            stack[++stackIndex] = tokens[i];

        } else if (tokens[i].type=='p') {
            if (tokens[i].op=='(') {
                stack[++stackIndex] = tokens[i];

            } else if (tokens[i].op==')') {
                while (stackIndex>=0 && stack[stackIndex].op!='(') {
                    output[outIndex++] = stack[stackIndex--];
                }

                stackIndex--;
            }
        }
    }

    while (stackIndex>=0) {
        output[outIndex++] = stack[stackIndex--];
    }

    return outIndex;
}


int evaluateRPN(Token* rpnTokens, int rpnCount) {
    int stack[100];
    int stackIndex = -1;

    for (int i = 0; i<rpnCount; i++) {
        if (rpnTokens[i].type=='n') {
            stack[++stackIndex] = rpnTokens[i].value;

        } else if (rpnTokens[i].type=='o') {
            if (rpnTokens[i].op=='!') {
                int a = stack[stackIndex--];
                stack[++stackIndex] = factorial(a);

            } else {
                int b = stack[stackIndex--];
                int a = stack[stackIndex--];

                if (rpnTokens[i].op=='+') stack[++stackIndex] = a+b;
                if (rpnTokens[i].op=='-') stack[++stackIndex] = a-b;
                if (rpnTokens[i].op=='*') stack[++stackIndex] = a*b;
                if (rpnTokens[i].op=='/') stack[++stackIndex] = a/b;
                if (rpnTokens[i].op=='^') {
                    int result = 1;

                    for (int j = 0; j<b; j++) {
                        result *= a;
                    }

                    stack[++stackIndex] = result;
                }
            }
        }
    }

    return stack[stackIndex];
}


int main() {
    char input[100];
    Token tokens[100], rpnTokens[100];

    while (std::cin >> input) {
        int tokenCount = tokenize(input, tokens);
        int rpnCount = RPN(tokens, tokenCount, rpnTokens);
        int result = evaluateRPN(rpnTokens, rpnCount);

        std::cout << result << std::endl;
    }
}
