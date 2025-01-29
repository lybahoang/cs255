// The program converts a postfix expression into its corresponding infix expression
#include <iostream>
#include <string>
#include <cmath>
#include <cctype>
#include <vector>
using namespace std;

// Global constant
const char NUMBERS = 'n',
           OPERATOR = 'o',
           MINUS = '-',
           OPEN_PARENTHESIS = '(';
const string ZERO = "0";

// Function prototypes
void displayIntroduction();                                 // Function to display the introduction of the program
void displayMenu();                                         // Function to display the menu of the program
bool validatePostfix(string);                               // Function to validate the postfix expression
void parsePostfix(string, vector<string> &);                // Function to parse postfix into operands and operators
string postfixToInfix(vector<string>);                      // Function to convert from postfix to infix expression
double calculateInfix(vector<string>);                      // Function to calculate the infix expression

char takeAssociativity(string);                              // Function to return the associativity of an operator
void infixTokenization(string, vector<string> &);           // Function parses the infix notation
double calculateExpression(vector<string>);                 // Function to calculate an expression that do not contain parentheses
int locateOpenParenthesis(vector<string>);                  // Function to return the position of an open parenthesis in a vector
int locateCloseParenthesis(vector<string>, int);            // Function to return the position of a close parenthesis in a vector
int countSymbol(vector<string>, char);                      // Function to count the number of a specific character in a vector
bool areAllSymbolsValid(string);                            // Function to check if all characters in the postfix are valid
bool choiceValidation(string);                              // Function to check if user's choice
bool isOperand(string);                                     // Function to check if a string is an operand
bool isOperator(char);                                      // Function to check if a character is an operator
bool isOperator(string);                                    // Function to check if a string is an operator
int findLargest(vector<int>);                               // Function to return the highest number in a vector
int precedence(string);                                     // Function to determine the precedence of an operator

// The main Function
int main()
{
    // Constant for choices
    const int OPTION_1 = 1,
              OPTION_2 = 2,
              OPTION_3 = 3;

    // Variable declaration
    string postfixExpression,       // String to hold the postfix expression from user
           infixExpression,         // String to hold the infix expression
           userChoice;              // String to hold user's choice
    int choice;                     // Variable to store user's choice
    vector<string> postfixTokens,   // Vector to store the postfix tokens
                   infixTokens;     // Vector to store the infix tokens

    // Display the introduction of the program
    displayIntroduction();

    // The main program
    do
    {
        // Display the menu of the program
        displayMenu();
        // Get user's choice
        getline(cin, userChoice);
        // Validate the user's choice
        while(!choiceValidation(userChoice))
        {
            cout << "Invalid choice. Please enter 1 - 3.\n";
            displayMenu();
            getline(cin, userChoice);
        }

        // Convert userChoice into integer
        choice = stoi(userChoice);

        // If the user's choice is OPTION_1: Enter a postfix expression
        if (choice == OPTION_1)
        {
            // Clear the previous vectors and infix expression
            infixExpression.clear();
            postfixTokens.clear();
            infixTokens.clear();

            // Prompt user for a postfix
            cout << "Enter a postfix expression: ";
            getline(cin, postfixExpression);

            // If the postfix is valid, then display its infix
            if (validatePostfix(postfixExpression))
            {
                // Parsing the postfix into operands and operators
                parsePostfix(postfixExpression, postfixTokens);

                // Convert the postfix into infix
                infixExpression = postfixToInfix(postfixTokens);

                // Display the infix
                cout << "Infix: " << infixExpression << endl;
            }
            // Otherwise, if the postfix is invalid, display error message
            else
            {
                cout << "Invalid postfix.\n";
            }
        }
        // If the user's choice is OPTION_2: Evaluate the infix expression
        else if (choice == OPTION_2)
        {
            // Check if there is an infix to evaluate
            if (!infixExpression.empty())
            {
                // Parsing the infix
                infixTokenization(infixExpression, infixTokens);

                // Calculate the infix
                double result = calculateInfix(infixTokens);

                // Division by Zero such as 0 / 0 (nan); 3 / 0;...
                if (isinf(result) || isnan(result))
                {
                    cout << "ERROR: Division by zero\n";
                }
                // Display the result
                else
                {
                    cout << "Calculation result: " << result << endl;
                }

                // Clear the old infix and postfix expression
                infixExpression.clear();
                postfixTokens.clear();
                infixTokens.clear();
            }
            // Otherwise, if there is no any infix to evaluate
            else
            {
                cout << "There is no infix expression to evaluate.\n";
                cout << "Please enter a postfix.\n";
            }
        }
        // If the user's choice is OPTION_3: Exit the program
        else
        {
            cout << "Exit the program successfully. Goodbye!\n";
        }
    } while (choice != 3);

    return 0;
}

// **************************************************
// Definition of function displayIntroduction       *
// This function will display the introduction of   *
// the program to the screen.                       *
// **************************************************
void displayIntroduction()
{
    cout << "------ WELCOME TO POSTFIX-TO-INFIX CONVERTER ------\n\n";
    cout << "This program converts a mathematical expression written in postfix notation ";
    cout << "(operators after operands) into its equivalent infix notation ";
    cout << "(operators between operands with parentheses for clarity).\n";
    cout << "For example, the postfix notation of 3 + 5 is 3 5 +.\n";
    cout << "\nConventions:\n";
    cout << "1. Difference operands should be separated by spaces to avoid ambiguity. For example, 355 + can be interpreted as 3 55 + or 35 5 +, so you should add spaces to clarify you postfix expression.\n";
    cout << "2. The minus sign of a negative operand should be placed right in front of the number. For example, -30 5 +.\n";
    cout << "\nThe following is the menu of the program.\n";
}

// **************************************************
// Definition of function displayMenu               *
// This function will display the menu of the       *
// program to the screen and prompt user for choice.*
// **************************************************
void displayMenu()
{
    cout << "\n\n----------- MENU -----------\n";
    cout << "1. Enter a postfix expression.\n";
    cout << "2. Evaluate the infix expression.\n";
    cout << "3. Exit the program.\n\n";
    cout << "Enter your choice (1 - 3): ";
}

// **************************************************
// Definition of function validatePostfix           *
// This function validates a postfix expression. It *
// returns true if the postfix is valid, and false  *
// if the postfix is invalid.                       *
// **************************************************
bool validatePostfix(string postfix)
{
    int numOperators = 0,           // Variable to keep track of the number of operators
        numOperands = 0;            // Variable to keep track of the number of operands
    vector<string> postfixTokens;

    // Postfix expression must contain only operators and digits
    if (!areAllSymbolsValid(postfix))
    {
        cout << "ERROR: Invalid characters in postfix\n";
        return false;
    }

    // Parsing the postfix into operands and operators
    parsePostfix(postfix, postfixTokens);

    // Count the number of operators and operands
    numOperators = countSymbol(postfixTokens, OPERATOR);
    numOperands = countSymbol(postfixTokens, NUMBERS);

    if (numOperands == 0 && numOperators == 0)
    {
        cout << "ERROR: Empty postfix.\n";
        return false;
    }

    if (numOperands == 0)
    {
        cout << "ERROR: No any operands in the postfix.\n";
        return false;
    }

    if (numOperators == 0)
    {
        cout << "ERROR: No any operators in the postfix.\n";
        return false;
    }

    if (numOperands - numOperators != 1)
    {
        cout << "ERROR: Invalid numbers of operands and operators\n";
        return false;
    }

    // There must be at least two operands following an operator at the beginning
    const int OPERANDS_PER_OPERATOR = 2;
    int length = postfixTokens.size();
    numOperands = 0;

    for (int index = 0; index < length; index++)
    {
        if (isOperator(postfixTokens[index]))
        {
            for (int count = 0; count < index; count++)
            {
                if (isOperand(postfixTokens[count]))
                {
                    numOperands++;
                }
            }
            break;
        }
    }

    if (numOperands < OPERANDS_PER_OPERATOR)
    {
        cout << "ERROR: Lack of operands at the beginning.\n";
        return false;
    }

    // Operators must be behind the operands
    if (isOperator(postfixTokens[0]))
    {
        cout << "ERROR: Operators at the beginning\n";
        return false;
    }

    return true;
}

// **************************************************
// Definition of function parsePostfix              *
// This function will parse the postfix into its    *
// operands and operators only, and put them into   *
// a vector.                                        *
// **************************************************
void parsePostfix(string postfix, vector<string> &postfixTokens)
{
    int count = 0;      // Loop counter variable
    string token;

    // Read the postfix from its left to right
    while (postfix[count] != '\0')
    {
        token.clear();
        token += postfix[count];

        // If the character is a digit
        if (isdigit(postfix[count]))
        {
            // Check for a multiple-digit operand
            count++;
            // While the next character is not null and is a digit
            while (postfix[count] != '\0' && isdigit(postfix[count]))
            {
                // Add it to the token
                token += postfix[count];
                count++;
            }
            count--;
            // Push the token into the postfixToken vector
            postfixTokens.push_back(token);
        }
        // If the character is a minus and at the start of the postfix
        else if (count == 0 && postfix[count] == '-')
        {
            // Check for a multiple-digit negative number
            count++;
            // While the next character is not null and is a digit
            while (postfix[count] != '\0' && isdigit(postfix[count]))
            {
                // Add it to token
                token += postfix[count];
                count++;
            }
            count--;
            // Push the token back into the postfixToken vector
            postfixTokens.push_back(token);
        }
        // Else if the character is a minus, preceded by a space, and in the middle
        else if (count > 0 && postfix[count] == '-' && isspace(postfix[count - 1]))
        {
            // Check for a multiple-digit negative number
            count++;
            // While the next character is not null and is a digit
            while (postfix[count] != '\0' && isdigit(postfix[count]))
            {
                // Add it to token
                token += postfix[count];
                count++;
            }
            count--;
            // Push the token back into the postfixToken vector
            postfixTokens.push_back(token);
        }
        // Else if the character is not a whitespace character
        else if (!isspace(postfix[count]))
        {
            // Push the token back into the postfixToken vector
            postfixTokens.push_back(token);
        }

        // Increment the loop counter
        count++;
    }
}

// **************************************************
// Definition of function postfixToInfix            *
// This function converts a postfix into an infix,  *
// and returns the string of infix.                 *
// **************************************************
string postfixToInfix(vector<string> postfixTokens)
{
    vector<int> operandPrecedence;      // A vector to hold the precedence of operand
    vector<string> infixTokens;         // Vector to hold infix tokens
    string token,
           subExpression,               // String to hold the subexpression of the infix
           infixExpression;             // String to hold the infix expression
    int operatorPrecedence;             // Variable to hold the precedence of an operator

    // Scanning the postfixTokens vector from the left to right
    for (int index = 0; index < postfixTokens.size(); index++)
    {
        token = postfixTokens[index];

        // If the token is an operand, push it back into the infixTokens vector
        if (isOperand(token))
        {
            // Because token is an operand, then its precedence is 0
            operandPrecedence.push_back(0);
            // Push the token into the infixTokens vector
            infixTokens.push_back(token);
        }
        // If the token is an operator
        else if (isOperator(token))
        {
            // Get the precedence of the operator
            operatorPrecedence = precedence(token);
            // Get the associativity of the operator
            char associativity = takeAssociativity(token);

            // Take the first operand in the infixTokens and its precedence
            string operand_2 = infixTokens.back();
            infixTokens.pop_back();
            int operandPrecedence_2 = operandPrecedence.back();
            operandPrecedence.pop_back();

            // Take the second operand in the infixTokens vector and its precedence
            string operand_1 = infixTokens.back();
            infixTokens.pop_back();
            int operandPrecedence_1 = operandPrecedence.back();
            operandPrecedence.pop_back();

            // Add parentheses if the precedence of the operator > the precedence of operand 1
            if (operatorPrecedence > operandPrecedence_1 && operandPrecedence_1 != 0)
            {
                operand_1 = "(" + operand_1 + ")";
            }

            // Add parentheses if the precedence of the operator > the precedence of operand 2
            if (operatorPrecedence > operandPrecedence_2 && operandPrecedence_2 != 0)
            {
                operand_2 = "(" + operand_2 + ")";
            }

            // Add parentheses in case the precedence of the operator = the precedence of the operand
            // but the operand is required to perform first. For example, 2 - (3 + 1) + 3 != 2 - 3 + 1 + 3
            if (associativity == 'L' && operatorPrecedence > 0 && operatorPrecedence == operandPrecedence_2)
            {
                operand_2 = "(" + operand_2 + ")";
            }
            if (associativity == 'R' && operatorPrecedence > 0 && operatorPrecedence == operandPrecedence_1)
            {
                operand_1 = "(" + operand_1 + ")";
            }

            // Add parentheses for negative numbers
            if (operandPrecedence_1 == 0 && stoi(operand_1) < 0)
            {
                operand_1 = "(" + operand_1 + ")";
            }

            if (operandPrecedence_2 == 0 && stoi(operand_2) < 0)
            {
                operand_2 = "(" + operand_2 + ")";
            }

            // Build the subexpression
            subExpression = operand_1 + " " + token + " " + operand_2;
            // Push the subexpression into the infix
            infixTokens.push_back(subExpression);
            // Push the precedence of the subexpression in the operandPrecedence vector
            operandPrecedence.push_back(operatorPrecedence);
        }
    }

    // Build the string of infix expression
    for (string token : infixTokens)
    {
        infixExpression += token;
    }

    // Return the infix expression
    return infixExpression;
}

// **************************************************
// Definition of function calculateInfix            *
// This function evaluates the value of an infix    *
// and returns that value.                          *
// **************************************************
double calculateInfix(vector<string> infixTokens)
{
    // Vector to hold the tokens of expression enclosed by a pair of parentheses
    vector<string> subExpressionTokens;
    int startPosition,      // Variable to hold the starting position of the subexpression
        endPosition;        // Variable to hold the end position of the subexpression

    // Evaluate all the subexpression enclosed by a pair of parenthesis
    do
    {
        // Clear the previous subexpression
        subExpressionTokens.clear();
        // Getting the starting location of the subexpression enclosed by a pair of parentheses
        startPosition = locateOpenParenthesis(infixTokens);

        if (startPosition != -1)
        {
            // Getting the corresponding end location of the subexpression enclosed by a pair of parentheses
            endPosition = locateCloseParenthesis(infixTokens, startPosition);

            // Push the tokens of the subexpression into the subExpressionTokens vector
            for (int index = startPosition + 1, count = 0; index < endPosition; index++, count++)
            {
                subExpressionTokens.push_back(infixTokens[index]);
            }

            // Evaluate the subexpression and convert the result into a string
            string operand = to_string(calculateExpression(subExpressionTokens));

            // Replace the result into the subexpression in the infixTokens vector
            infixTokens[startPosition] = operand;
            // Remove the subexpression from the infixTokens vector
            infixTokens.erase(infixTokens.begin() + startPosition + 1, infixTokens.begin() + endPosition + 1);
        }
    } while (startPosition != -1);

    // Evaluate the infix after evaluating all the subexpressions.
    double result = calculateExpression(infixTokens);

    // Return the evaluation result
    return result;
}

// **************************************************
// Definition of function takeAssociativity         *
// This function returns L if left associativity   *
// R if right associativity                        *
// **************************************************
char takeAssociativity(string op)
{
    if (op == "+" || op == "-" || op == "*" || op == "/")
        return 'L';
    else
        return 'R';
}

// **************************************************
// Definition of function infixTokenization         *
// This function parses a string of infix notation  *
// into its operators, operands, parentheses        *
// **************************************************
void infixTokenization(string infix, vector<string> &infixTokens)
{
    string token;

    // Split the infix into operands, operators, and parentheses
    int length = infix.length();

    for (int index = 0; index < length; index++)
    {
        // Clear the content of the string
        token.clear();
        char currentChar = infix[index];
        token += currentChar;

        // If token is a negative number and the number is at the beginning
        if (currentChar == MINUS && index == 0 && index + 1 < length && isdigit(infix[index + 1]))
        {
            index += 1;
            // If the token is a multiple-digit negative number
            while (index < length && isdigit(infix[index]))
            {
                token += infix[index];
                index++;
            }
            index -= 1;
            // Push the token into the vector
            infixTokens.push_back(token);
        }
        // If the expression has an expression like: -(-3) = 0 - (-3),...
        else if (currentChar == MINUS && index == 0)
        {
            // Push zero in front of the minus sign
            infixTokens.push_back(ZERO);
            // Push the token into the vector
            infixTokens.push_back(token);
        }
        // If token is a negative number and the number is inside the expression
        else if (currentChar == MINUS && infix[index - 1] == OPEN_PARENTHESIS && index > 0 && isdigit(infix[index + 1]))
        {
            index += 1;
            // If the token is a multiple-digit negative number
            while (index < length && isdigit(infix[index]))
            {
                token += infix[index];
                index++;
            }
            index -= 1;
            // Push the token into the vector
            infixTokens.push_back(token);
        }
        // If the token is formated as 0 - ...
        else if (currentChar == MINUS && infix[index - 1] == OPEN_PARENTHESIS && index > 0 && !isdigit(infix[index + 1]) && index < length)
        {
            // Push zero in front of the minus sign
            infixTokens.push_back(ZERO);
            // Push the token into the vector
            infixTokens.push_back(token);
        }
        // If the token is a number
        else if (isdigit(currentChar))
        {
            index += 1;
            // If the number is a multiple-digit number
            while (index < length && isdigit(infix[index]))
            {
                token += infix[index];
                index++;
            }
            index -= 1;
            // Push the token into the vector
            infixTokens.push_back(token);
        }
        // Otherwise
        else if (!isspace(currentChar))
        {
            infixTokens.push_back(token);
        }
    }
}

// **************************************************
// Definition of function calculateExpression       *
// This function calculates the value of an         *
// expression not being enclosed by a pair of       *
// parentheses and returns the result.              *
// **************************************************
double calculateExpression(vector<string> expressionTokens)
{
    vector<double> operandStack;        // Vector to hold the operands
    vector<int> operatorPrecedence;     // Vector to hold the precedence of operators
    vector<string> operatorStack;       // Vector to hold the operators
    string token,                       // Variable to hold infix tokens
           op;                          // Variable to hold an operator
    double result;                      // Variable to hold the calculation result

    // Read the expressionTokens vector from the left to right
    // to put operands into operandStack, operators into operatorStack,
    // and the precedence of operators into operatorPrecedence
    for (int index = 0; index < expressionTokens.size(); index++)
    {
        token = expressionTokens[index];

        // If the token is an operand, push it into the operandStack vector
        if (isOperand(token))
        {
            // Convert numeric string into a double
            double operand = stod(token);
            operandStack.push_back(operand);
        }
        // Otherwise
        else
        {
            // Push the operator into the operatorStack vector
            operatorStack.push_back(token);
            // Push the precedence of the operator into operatorPrecedence vector
            operatorPrecedence.push_back(precedence(token));
        }
    }

    // Calculate the expression
    while (!operatorStack.empty())
    {
        // Find the highest precedence
        int highestPrecedence = findLargest(operatorPrecedence);

        // Calculate the operation with the highest precedence first
        for (int index = 0; index < operatorPrecedence.size(); index++)
        {
            if (operatorPrecedence[index] == highestPrecedence)
            {
                // Pop the operator
                op = operatorStack[index];

                // Pop the operands
                double operand_1 = operandStack[index];
                double operand_2 = operandStack[index + 1];

                // Perform calculation
                if (op == "+") result = operand_1 + operand_2;
                else if (op == "-") result = operand_1 - operand_2;
                else if (op == "*") result = operand_1 * operand_2;
                else if (op == "/") result = operand_1 / operand_2;
                else if (op == "^") result = pow(operand_1, operand_2);

                // Push the calculation result into the position of the operand_1
                // in the operandStack vector
                operandStack[index] = result;
                // Remove the operator that has just been used from the operatorStack vector
                operatorStack.erase(operatorStack.begin() + index);
                // Remove the corresponding precedence of the operator as well
                operatorPrecedence.erase(operatorPrecedence.begin() + index);
                // Remove the operand_2 that has been used as well
                operandStack.erase(operandStack.begin() + index + 1);
                // Break the loop and start a new cycle
                break;
            }
        }
    }

    // Return the calculation result
    return operandStack[0];
}

// **************************************************
// Definition of function locateOpenParenthesis     *
// This function finds and returns the position of  *
// an open parenthesis in a vector.                 *
// **************************************************
int locateOpenParenthesis(vector<string> infixTokens)
{
    // A sentinel if there is no open parenthesis
    int position = -1;

    // Find the innermost open parenthesis
    for (int index = infixTokens.size() - 1; index >= 0; index--)
    {
        if (infixTokens[index] == "(")
        {
            position = index;
            break;
        }
    }

    // Return the position of the innermost open parenthesis
    return position;
}

// **************************************************
// Definition of function locateCloseParenthesis    *
// This function finds and returns the position of  *
// the corresponding close parenthesis in a vector  *
// with an open parenthesis.                        *
// **************************************************
int locateCloseParenthesis(vector<string> infixTokens, int startPosition)
{
    int position;

    // Find the corresponding close parenthesis starting from the
    // position of an open parenthesis
    for (int index = startPosition; index < infixTokens.size(); index++)
    {
        if (infixTokens[index] == ")")
        {
            position = index;
            break;
        }
    }

    // Return the position
    return position;
}

// **************************************************
// Definition of function countSymbol               *
// The function counts and returns the numbers of   *
// a specific character in a vector.                *
// **************************************************
int countSymbol(vector<string> postfixTokens, char symbol)
{
    int count = 0;

    // Count the number of operands
    if (symbol == NUMBERS)
    {
        for (string token : postfixTokens)
        {
            if (isOperand(token))
            {
                count++;
            }
        }
    }
    // Count the number of operators
    else if (symbol == OPERATOR)
    {
        for (string token : postfixTokens)
        {
            if (token == "+" || token == "-" || token == "/" || token == "*" || token == "^")
            {
                count++;
            }
        }
    }

    return count;
}

// **************************************************
// Definition of function areAllSymbolsValid        *
// The function checks if a postfix contains only   *
// valid characters or not.                         *
// **************************************************
bool areAllSymbolsValid(string postfix)
{
    char character;

    for (int index = 0; index < postfix.length(); index++)
    {
        character = postfix[index];

        // If the character is not a digit, not a whitespace character,
        // and not an operator => return false
        if (!isdigit(character) && !isspace(character) && !isOperator(character))
        {
            return false;
        }
    }

    // Otherwise, return true
    return true;
}

// **************************************************
// Definition of function choiceValidation          *
// This function checks whether users enter a valid *
// or invalid option.                               *
// **************************************************
bool choiceValidation(string userChoice)
{
    // Constant for choices
    const int OPTION_1 = 1,
              OPTION_2 = 2,
              OPTION_3 = 3;
    vector<char> choiceTokens;

    // Determine the length of user's choice,
    // except a whitespace character
    for (char character : userChoice)
    {
        if (!isspace(character))
        {
            choiceTokens.push_back(character);
        }
    }

    // If the length of user's choice is not 1, returns false
    if (choiceTokens.size() != 1)
    {
        return false;
    }

    // If the length of user's choice is 1, but not a digit, returns false
    if (!isdigit(choiceTokens[0]))
    {
        return false;
    }

    // If the user's choice is not in the valid range, returns false
    if (choiceTokens[0] - '0' > OPTION_3 || choiceTokens[0] < OPTION_1)
    {
        return false;
    }

    // Otherwise, returns true
    return true;
}

// **************************************************
// Definition of function isOperand                 *
// This function determines if a string is an       *
// operand or not                                   *
// **************************************************
bool isOperand(string token)
{
    if (isdigit(token[0]))
        return true;
    else if (token[0] == MINUS && token.length() > 1 && isdigit(token[1]))
        return true;
    return false;
}

// **************************************************
// Definition of function isOperator                *
// This function determines if a character is an    *
// operator or not.                                 *
// **************************************************
bool isOperator(char character)
{
    const int SIZE = 5;
    char charOperators[SIZE] = {'+', '-', '*', '/', '^'};

    for (int i = 0; i < SIZE; i++)
    {
        if (character == charOperators[i])
            return true;
    }

    return false;
}

// **************************************************
// Definition of function isOperator                *
// This function determines if a string is an       *
// operator or not.                                 *
// **************************************************
bool isOperator(string str)
{
    const int SIZE = 5;
    string strOperators[SIZE] = {"+", "-", "*", "/", "^"};

    for (int i = 0; i < SIZE; i++)
    {
        if (str == strOperators[i])
            return true;
    }

    return false;
}

// **************************************************
// Definition of function findLargest               *
// This function finds and returns the largest      *
// element in a vector.                             *
// **************************************************
int findLargest(vector<int> operatorPrecedence)
{
    int largest = 0;

    // Scanning through each element in the vector
    for (int opPrecedence : operatorPrecedence)
    {
        if (opPrecedence > largest)
        {
            largest = opPrecedence;
        }
    }

    // Return the largest element
    return largest;
}

// **************************************************
// Definition of function precedence                *
// This function returns the precedence of an       *
// operator.                                        *
// **************************************************
int precedence(string c)
{
    if (c == "+" || c == "-") return 1;
    else if (c == "*" || c == "/") return 2;
    else if (c == "^") return 3;
    return 0;
}
