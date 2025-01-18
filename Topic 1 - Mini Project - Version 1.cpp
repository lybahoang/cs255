// The program converts an infix notation into prefix notation
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <cctype>
using namespace std;

// Global constant
const char NUMBERS = 'n',
           OPEN_PARENTHESIS = '(',
           CLOSE_PARENTHESIS = ')',
           OPERATOR = 'o',
           MINUS = '-',
           PLUS = '+';
const string ZERO = "0";

// Major function prototypes
void displayIntroduction();                                 // Function to display the introduction of the program
void displayMenu();                                         // Function to display the menu
string processInfix(string);                                // Function to process the infix
bool infixValidation(string);                               // Function checks if an infix notation is valid or invalid
void infixTokenization(string, vector<string> &);           // Function parses the infix notation
void infixToPrefix(const vector<string>, vector<string> &); // Function converts infix to prefix
double prefixCalculation(const vector<string>);             // Function to evaluate prefix

// Other function prototypes
bool isOperand(string);                                     // Function to determine if a string is an operand or not
int precedence(string);                                     // Function to return the precedence of operator
bool areConsecutiveOperands(string);                        // Function to check if an infix containing two consecutive operands without operators
bool isOpenParenthesisValid(char, char &, int &);           // Function to check if an open parenthesis is valid
bool isCloseParenthesisValid(char, char &, int &);          // Function to check if an close parenthesis is valid
bool isOperatorValid(char, char &);                         // Function to check if an operator is valid
bool isDigitValid(char, char &);                            // Function to check if an operand is valid
bool isOpeningParenthesis(char);                            // Function to check if a char is an open parenthesis
bool isClosingParenthesis(char);                            // Function to check if a char is an close parenthesis
bool isOperator(char);                                      // Function to check if a char is an operator
bool choiceValidation(string);                              // Function to check if user's choice

// The main Function
int main()
{
    // Constant for choices
    const int OPTION_1 = 1,
              OPTION_2 = 2,
              OPTION_3 = 3;

    // Variable declaration
    string infixExpression;                                 // A variable to hold the user's infix
    string userChoice;                                      // A variable to hold the user's choice
    int choice;
    vector<string> infixTokens;                             // Vector to hold the tokens of infix notation
    vector<string> prefixTokens;                            // Vector to hold the tokens of prefix notatio

    // Introduction of the program
    displayIntroduction();

    // The main program
    do
    {
        // Display the menu and prompt for choice
        displayMenu();
        getline(cin, userChoice);

        // User's choice validation
        while (!choiceValidation(userChoice))
        {
            cout << "Invalid choice. Please enter (1 - 3)\n";
            displayMenu();
            getline(cin, userChoice);
        }

        // Convert userChoice into integer
        choice = stoi(userChoice);

        // If the user's choice is OPTION_1: Enter an infix expression
        if (choice == OPTION_1)
        {
            infixTokens.clear();
            prefixTokens.clear();

            // Get user's infix expression
            cout << "Enter an infix expression: ";
            getline(cin, infixExpression);

            // Infix expression validation
            // If the infix expression is true, then display the prefix expression
            if (infixValidation(infixExpression))
            {
                // Infix tokenization
                infixTokenization(infixExpression, infixTokens);
                // Convert infix to prefix
                infixToPrefix(infixTokens, prefixTokens);
                // Display the prefix
                cout << "INFIX: ";
                for (int index = 0; index < infixTokens.size(); index++)
                {
                    if (infixTokens[index] == "(")
                    {
                        cout << infixTokens[index];
                        continue;
                    }
                    else if (index + 1 < infixTokens.size() && infixTokens[index + 1] == ")")
                    {
                        cout << infixTokens[index];
                        continue;
                    }
                    cout << infixTokens[index] << " ";
                }
                cout << "\nPREFIX: ";
                for (int index = 0; index < prefixTokens.size(); index++)
                {
                    cout << prefixTokens[index] << " ";
                }
            }
            // If the infix expression is false, display an error message
            else
            {
                cout << "Invalid infix expression.\n";
            }
        }
        // If the user's option is OPTION_2: Calculate the prefix
        else if (choice == OPTION_2)
        {
            // If there is no prefix expression
            if (prefixTokens.empty())
            {
                cout << "There is no prefix expression to calculate.\n"
                     << "You should type in an infix expression.\n";
            }
            // If there is a prefix expression, calculate and display result
            else
            {
                double result = prefixCalculation(prefixTokens);

                // Division by Zero such as 0 / 0 (nan); 3 / 0;...
                if (isinf(result) || isnan(result))
                {
                    cout << "ERROR: Division by zero\n";
                }
                else
                {
                    cout << "Calculation result: " << result << endl;
                }

                // Clear the old infix and prefix expression
                infixTokens.clear();
                prefixTokens.clear();
            }
        }
        // If the user's choice is OPTION_3: Exit the program
        else if (choice == OPTION_3)
        {
            cout << "Exit the program. Goodbye!\n";
        }
        // Other options
        else
        {
            cout << "Invalid choice. Please enter (1 - 3)\n";
        }
    } while (choice != 3);

    return 0;
}

// **************************************************
// Definition of function displayIntroduction       *
// This function displays the introduction of the   *
// program and some conventions                     *
// **************************************************
void displayIntroduction()
{
    cout << "--- GROUP 1: WELCOME TO INFIX-PREFIX CONVERSION ---\n\n";
    cout << "This program converts mathematical expressions from infix notation to prefix notation.\n\n";
    cout << "Guidelines:\n";
    cout << "1. Infix notation places operators between operands (e.g., A + B).\n";
    cout << "2. Prefix notation places operators before operands (e.g., + A B), eliminating the need for parentheses.\n\n";
    cout << "Conventions:\n";
    cout << "1. A negative number at the beginning of the expression does not require parentheses.\n";
    cout << "2. Negative numbers inside the expression must be enclosed in parentheses to avoid being treated as subtraction (e.g., A + (-B) instead of A + -B).\n\n";
    cout << "The following is the menu of the program.\n";
}

// **************************************************
// Definition of function displayMenu               *
// This function display the menu of the program    *
// **************************************************
void displayMenu()
{
    cout << "\n\n----------- MENU -----------\n";
    cout << "1. Enter infix expression\n";
    cout << "2. Calculate the prefix expression\n";
    cout << "3. Exit\n\n";
    cout << "Enter your choice (1 - 3): ";
}

// **************************************************
// Definition of function processInfix              *
// This function removes whitespace characters and  *
// simplifies sequences of pluses and minuses in    *
// the infix notation                               *
// **************************************************
string processInfix(string infix)
{
    string output1;
    // Removes all whitespace characters in the infix
    for (char character : infix)
    {
        // If the character is not a whitespace and not a tabs
        if (!isspace(character))
        {
            output1 += character;
        }
    }

    // Simplifies sequences of pluses and minuses
    int minusCount = 0;
    char character;
    string output2;

    for (int index = 0; index < output1.length(); index++)
    {
        character = output1[index];
        // If the character is a minus or a plus sign
        if (character == MINUS || character == PLUS)
        {
            // If minus, update the number of minuses
            if (character == MINUS)
            {
                minusCount += 1;
            }
            index++;
            // If the next character is + or -, update the number of minuses
            while (index < output1.length() && output1[index] == MINUS || output1[index] == PLUS)
            {
                minusCount += (output1[index] == MINUS) ? 1 : 0;
                index++;
            }
            index--;

            // Convert the sequences of minuses and pluses
            output2 += (minusCount % 2 == 0) ? PLUS : MINUS;
            // Reset the number of minuses
            minusCount = 0;
        }
        // If the character is other characters, push it back to the processCharacter vector
        else
        {
            output2 += character;
        }
    }

    return output2;
}

// **************************************************
// Definition of function infixValidation           *
// This function determines if an infix expression  *
// is valid or invalid                              *
// **************************************************
bool infixValidation(string infix)
{
    char previousChar = '\0';
    int numOfOpenParentheses = 0;            // Variable to keep track of the number of open parentheses
    bool result;
    string processedInfix;

    // Check if there are two consecutive operands without operators
    if (areConsecutiveOperands(infix))
    {
        cout << "ERROR: Two consecutive operands without operators.\n";
        return false;
    }

    // Process the infix expression (removing whitespace and reduce sequences of minuses and pluses)
    processedInfix = processInfix(infix);

    // Check if the infix is empty
    if (processedInfix.empty())
    {
        cout << "ERROR: There is no infix expression.\n";
        return false;
    }

    // A loop to validate each character
    for (char currentChar : processedInfix)
    {
        // If the current character is an open parenthesis
        if (isOpeningParenthesis(currentChar))
        {
            // Call the function isOpenParenthesisValid
            if (!isOpenParenthesisValid(currentChar, previousChar, numOfOpenParentheses))
            {
                return false;
            }
        }

        // If the current character is a close parenthesis
        else if (isClosingParenthesis(currentChar))
        {
            // Call the function isCloseParenthesisValid
            if (!isCloseParenthesisValid(currentChar, previousChar, numOfOpenParentheses))
            {
                return false;
            }
        }

        // If the current character is an operator
        else if (isOperator(currentChar))
        {
            // Call the function is OperatorValid
            if (!isOperatorValid(currentChar, previousChar))
            {
                return false;
            }
        }

        // If the current character is a digit
        else if (isdigit(currentChar))
        {
            // Call the function isDigitValid
            if (!isDigitValid(currentChar, previousChar))
            {
                return false;
            }
        }
        // If the current character is not a digit, parentheses, operators => ERROR
        else
        {
            cout << "ERROR: Contain invalid characters.\n";
            return false;
        }
    }
    // After all, if the number of ('s is larger than )'s => ERROR
    if (numOfOpenParentheses > 0)
    {
        cout << "ERROR: Unmatched parenthesis.\n";
        return false;
    }
    // If the last character is an open parenthesis => ERROR
    if (previousChar == OPEN_PARENTHESIS)
    {
        cout << "ERROR: Open parentheses at the end.\n";
        return false;
    }
    else if (previousChar == OPERATOR)
    {
        cout << "ERROR: Operators at the end.\n";
        return false;
    }

    // Return true if the infix is valid
    return true;
}

// **************************************************
// Definition of function infixTokenization         *
// This function parses the infix expression into   *
// operators, operands, parentheses                 *
// **************************************************
void infixTokenization(string infix, vector<string> &infixTokens)
{
    string processedInfix = processInfix(infix);
    string token;

    // Split the infix into operands, operators, and parentheses
    int length = processedInfix.length();

    for (int index = 0; index < length; index++)
    {
        char currentChar = processedInfix[index];
        token += currentChar;

        // If token is a negative number and the number is at the beginning
        if (currentChar == MINUS && index == 0 && index + 1 < length && isdigit(processedInfix[index + 1]))
        {
            index += 1;
            // If the token is a multiple-digit negative number
            while (index < length && isdigit(processedInfix[index]))
            {
                token += processedInfix[index];
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
        else if (currentChar == MINUS && processedInfix[index - 1] == OPEN_PARENTHESIS && index > 0 && isdigit(processedInfix[index + 1]))
        {
            index += 1;
            // If the token is a multiple-digit negative number
            while (index < length && isdigit(processedInfix[index]))
            {
                token += processedInfix[index];
                index++;
            }
            index -= 1;
            // Push the token into the vector
            infixTokens.push_back(token);
        }
        // If the token is formated as 0 - ...
        else if (currentChar == MINUS && processedInfix[index - 1] == OPEN_PARENTHESIS && index > 0 && !isdigit(processedInfix[index + 1]) && index < length)
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
            while (index < length && isdigit(processedInfix[index]))
            {
                token += processedInfix[index];
                index++;
            }
            index -= 1;
            // Push the token into the vector
            infixTokens.push_back(token);
        }
        // Otherwise
        else
        {
            infixTokens.push_back(token);
        }

        // Clear the content of the string
        token.clear();
    }
}

// **************************************************
// Definition of function infixToPrefix             *
// This function converts infix to prefix           *
// **************************************************
void infixToPrefix(const vector<string> infixTokens, vector<string> &prefixTokens)
{
    // Reverse the infix expression
    vector<string> opStack;
    vector<string> temp(infixTokens.size());

	reverse_copy(infixTokens.begin(), infixTokens.end(), temp.begin());

	// Scan the infix tokens from left to right
	for (string token : temp)
    {
        // If the token is an operand, add it to prefixTokens
		if (isOperand(token))
        {
			prefixTokens.push_back(token);
		}

		// If the token is ")" push it to the opStack.
		else if (token == ")")
        {
            opStack.push_back(token);
		}

		// If the token is a "(" pop operator in the stack to
		// prefix expression until meet ")"
		else if (token == "(")
        {
			while (!opStack.empty() && opStack.back() != ")")
            {
                // Add the last element in the stack to output
				prefixTokens.push_back(opStack.back());
				// Remove the last element in the stack
				opStack.pop_back();
			}
			// Remove close parenthesis ')' from the opStack
			opStack.pop_back();
		}

		// if the token is an operator, add it to the stack
		else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^" || token == "%")
        {
			// If the stack is not empty, then check the precedence of the operator to push it in the stack
			// If the precedence of incoming operator > that of the last operator in the opStack
			if (!opStack.empty() && precedence(token) > precedence(opStack.back()))
			{
			    // Push the token into the back of opStack vector
				opStack.push_back(token);
			}
			// If the precedence of incoming operator = that of the last operator in the opStack
			else if (!opStack.empty() && precedence(token) == precedence(opStack.back()))
            {
                while (!opStack.empty() && opStack.back() == "^")
                {
                    // Push the last element if opStack to output
                    prefixTokens.push_back(opStack.back());
                    // Pop back the last element in opStack
                    opStack.pop_back();
                }
                // Push the token into the back of opStack vector
                opStack.push_back(token);
			}
            // If the precedence of incoming operator < that of the last operator in the opStack
			else if (!opStack.empty() && precedence(token) < precedence(opStack.back()))
            {
                // Pop the last operator in opStack until the condition is false
				while (!opStack.empty() && precedence(token) < precedence(opStack.back()))
				{
					prefixTokens.push_back(opStack.back());
					opStack.pop_back();
				}
				// Push the incoming operator into the back of opStack
				opStack.push_back(token);
			}
			else
            {
                opStack.push_back(token);
            }
		}
    }
		// Add the remain symbol in the stack to output
		while (!opStack.empty())
        {
            // Push the last element in opStack into the back of output
            prefixTokens.push_back(opStack.back());
            // Pop back the last element in opStack vector
            opStack.pop_back();
        }
        // Reverse the output to get prefix
        reverse(prefixTokens.begin(), prefixTokens.end());
}

// **************************************************
// Definition of function prefixCalculation         *
// This function evaluates the prefix expression    *
// **************************************************
double prefixCalculation(const vector<string> prefixTokens)
{
    vector<double> operands;
    double result;

    for (int i = prefixTokens.size() - 1; i >= 0; i--)
    {
        // Last element
        string lastElement = prefixTokens.at(i);

        // If the last element is an operand
        if (isOperand(lastElement) && !lastElement.empty())
        {
            int numbers = stoi(lastElement);        // Convert a string of numeric value into a number
            operands.push_back(numbers);
        }
        // If the last element is an operator
        else
        {
            // Take the last element in the operands vector
            double operand_1 = operands.back();
            operands.pop_back();

            // Take the next - last element in the operands vector
            double operand_2 = operands.back();
            operands.pop_back();

            if (lastElement == "+")
            {
                result = operand_1 + operand_2;
            }
            else if (lastElement == "-")
            {
                result = operand_1 - operand_2;
            }
            else if (lastElement == "*")
            {
                result = operand_1 * operand_2;
            }
            else if (lastElement == "/")
            {
                result = operand_1 / operand_2;
            }
            else if (lastElement == "^")
            {
                result = pow(operand_1, operand_2);
            }
            //store result of stack operation
            operands.push_back(result);
		}
    }
    return result;
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
// Definition of function precedence                *
// This function returns the precedence of the      *
// operator                                         *
// **************************************************
int precedence(string c)
{
    if (c == "+" || c == "-") return 1;
    else if (c == "*" || c == "/") return 2;
    else if (c == "^") return 3;
    return 0;
}

// **************************************************
// Definition of function areConsecutiveOperands    *
// This function determines if an infix containing  *
// two consecutive operands without any operator    *
// **************************************************
bool areConsecutiveOperands(string infix)
{
    bool isPreviousDigit = false,           // Flag to indicate the state of a digit
         isPreviousSpace = false;           // Flag to indicate the state of a space

    for (char character : infix)
    {
        // If the character is a whitespace, turn on its flag
        if (isspace(character))
        {
            isPreviousSpace = true;
        }
        // If the character is a digit
        else if (isdigit(character))
        {
            // If the previous characters are digit followed by a whitespace character
            if (isPreviousDigit && isPreviousSpace)
            {
                return true;
            }
            isPreviousDigit = true;         // Mark digit detected
            isPreviousSpace = false;        // Reset space flag
        }
        else
        {
            // Reset flags for non-digit, non-space characters
            isPreviousDigit = false;
            isPreviousSpace = false;
        }
    }

    // If no two consecutive operands is detected
    return false;
}

// **************************************************
// Definition of function isOpenParenthesisValid    *
// This function determines if an open parenthesis  *
// is valid or invalid                              *
// **************************************************
bool isOpenParenthesisValid(char currentChar, char &previousChar, int &numOfOpenParentheses)
{
    // If the previous character is a close parenthesis => ERROR )(
    if (previousChar == CLOSE_PARENTHESIS)
    {
        cout << "ERROR: Open parentheses after close parentheses without operators.\n";
        return false;
    }
    // IF the previous character is a number => ERROR 9(
    else if (previousChar == NUMBERS)
    {
        cout << "ERROR: Open parentheses after an operand without operators.\n";
        return false;
    }
    // Increase the number of open parenthesis by 1
    numOfOpenParentheses += 1;
    // Update the type of the previous character
    previousChar = OPEN_PARENTHESIS;
    // Return true if the current character is valid
    return true;
}

// **************************************************
// Definition of function isCloseParenthesisValid   *
// This function determines if a close parenthesis  *
// is valid or invalid                              *
// **************************************************
bool isCloseParenthesisValid(char currentChar, char &previousChar, int &numOfOpenParentheses)
{
    // If the the character is the first character => ERROR
    if (previousChar == '\0')
    {
        cout << "ERROR: Close parentheses at the beginning.\n";
        return false;
    }
    // If the previous character is an operator => *)
    else if (previousChar == OPERATOR)
    {
        cout << "ERROR: Close parentheses after operators.\n";
        return false;
    }
    // If the previous character is an open parenthesis => ()
    else if (previousChar == OPEN_PARENTHESIS)
    {
        cout << "ERROR: No operations between parentheses.\n";
        return false;
    }
    // If there is more close parenthesis more than open one => ERROR
    else if (numOfOpenParentheses == 0)
    {
        cout << "ERROR: Unmatched parentheses.\n";
        return false;
    }
    // Otherwise
    numOfOpenParentheses -= 1;
    // Update the previous character
    previousChar = CLOSE_PARENTHESIS;
    // Return true if the character is valid
    return true;
}

// **************************************************
// Definition of function isOperatorValid           *
// This function determines if an operator is valid *
// or invalid                                       *
// **************************************************
bool isOperatorValid(char currentChar, char &previousChar)
{
    // If the previous character is an operator => ERROR
    if (previousChar == OPERATOR)
    {
        cout << "ERROR: Two invalid consecutive operators.\n";
        return false;
    }
    // If the operator is at the beginning of the expression
    else if (previousChar == '\0')
    {
        // If the operator is not a minus or plus => ERROR
        if (currentChar != MINUS && currentChar != PLUS)
        {
            cout << "Invalid first operator.\n";
            return false;
        }
        else
        {
            previousChar = OPERATOR;
            return true;
        }
    }
    // If the previous character is an open parenthesis
    else if (previousChar == OPEN_PARENTHESIS)
    {
        // If a minus sign follows an open parenthesis
        if (currentChar != MINUS && currentChar != PLUS)
        {
            cout << "Invalid operator after open parentheses.\n";
            return false;
        }
        else
        {
            previousChar = OPERATOR;
            return true;
        }
    }
    // Otherwise the operator is valid, push it into the vector;
    // update the previous character and set result to true
    previousChar = OPERATOR;
    return true;
}

// **************************************************
// Definition of function isDigitValid              *
// This function determines if a digit is valid or  *
// invalid                                          *
// **************************************************
bool isDigitValid(char currentChar, char &previousChar)
{
    // If the previous character is a close parenthesis => ERROR
    if (previousChar == CLOSE_PARENTHESIS)
    {
        cout << "ERROR: Operands after close parenthesis.\n";
        return false;
    }
    previousChar = NUMBERS;
    return true;
}

// **************************************************
// Definition of function isOpeningParenthesis      *
// This function determines if a character is an    *
// opening parenthesis or not                       *
// **************************************************
bool isOpeningParenthesis(char character)
{
    if (character == OPEN_PARENTHESIS)
        return true;
    return false;
}

// **************************************************
// Definition of function isClosingParenthesis      *
// This function determines if a character is a     *
// closing parenthesis or not                       *
// **************************************************
bool isClosingParenthesis(char character)
{
    if (character == CLOSE_PARENTHESIS)
        return true;
    return false;
}

// **************************************************
// Definition of function isOperator                *
// This function determines if a character is an    *
// operator or not                                  *
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
// Definition of function choiceValidation          *
// This function validates the choice of user       *
// **************************************************
bool choiceValidation(string userChoice)
{
    int choiceLength = 0;

    // If the user's choice is a string or an empty string => ERROR
    for (char c : userChoice)
        {
            if (c == ' ' || c == '\t')
            {
                continue;
            }
            else
            {
                choiceLength += 1;
            }
        }

    if (userChoice.empty() || choiceLength >= 2 || choiceLength == 0)
    {
        return false;
    }

    return true;
}
