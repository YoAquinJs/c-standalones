#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int evalRPN(char **tokens, size_t token_count);

int main(int argc, char *argv[]) {
    int result = evalRPN(argv + 1, argc - 1);
    printf("RPN result: %d\n", result);

    return 0;
}

int recurseEvalRPN(char **tokens, size_t token, size_t *next_token) {
    char opt = *tokens[token];
    if (opt != '+' && opt != '*' && opt != '/' &&
        (opt != '-' || tokens[token][1] != '\0')) {
        *next_token = token - 1;
        return atoi(tokens[token]);
    }

    int operand2 = recurseEvalRPN(tokens, token - 1, next_token);
    int operand1 = recurseEvalRPN(tokens, *next_token, next_token);

    switch (opt) {
    case '+':
        return operand1 + operand2;
    case '-':
        return operand1 - operand2;
    case '*':
        return operand1 * operand2;
    case '/':
        return operand1 / operand2;
    }

    return 0;
}

int evalRPN(char **tokens, size_t token_count) {
    return recurseEvalRPN(tokens, token_count - 1, &token_count);
}
