#include "PostfixFormProcessor.h"

PostfixFormProcessor::TokenType PostfixFormProcessor::checkToken(const char token)
{
	if ((token == ' ') || (token == '\n') || (token == '\t') || (token == '\b'))
		return TokenType::space;
	if (token == ')')
		return TokenType::closingBrace;
	if (((token >= 'a') && (token <= 'z')) || ((token >= 'A') && (token <= 'Z')))
		return TokenType::operand;
	if ((token == '+') || (token == '-') || (token == '*') || (token == '/') || (token == '('))
		return TokenType::operation;
	return TokenType::unknown;
}

size_t PostfixFormProcessor::countPostfixFormLength(const std::string& expression)
{
	size_t count = 0;
	for (std::string::const_iterator token = expression.begin(); token != expression.end(); token++)
	{
		TokenType type = checkToken(*token);
		if (type == TokenType::unknown)
			throw "olala";
		else if ((type == TokenType::operand) || ((type == TokenType::operation) && *token != '('))
			count++;
		else
			continue;
	}
	return count;
}

size_t PostfixFormProcessor::countOperations(const std::string& expression)
{
	size_t count = 0;
	for (std::string::const_iterator token = expression.begin(); token != expression.end(); token++)
	{
		TokenType type = checkToken(*token);
		if (type == TokenType::unknown)
			throw "olala";
		else if (type == TokenType::operation)
			count++;
		else
			continue;
	}
	return count;
}

PostfixFormProcessor::Priority PostfixFormProcessor::checkPriority(const char first, const char second)
{
	if ((checkToken(first) != TokenType::operation) || (checkToken(second) != TokenType::operation))
		return Priority::unknown;
	int numericPriorityFirst  = (first == '*') || (first == '/') ? 3 : (first == '+') || (first == '-') ? 2 : 1;
	int numericPrioritySecond = (second == '*') || (second == '/') ? 3 : (second == '+') || (second == '-') ? 2 : 1;
	if (numericPriorityFirst > numericPrioritySecond)
		return Priority::higher;
	if (numericPriorityFirst < numericPrioritySecond)
		return Priority::lower;
	return Priority::equal;
}

PostfixFormProcessor::Variable PostfixFormProcessor::findVariableByName(const Variables variables, const char name)
{
	for (size_t i = 0; i < variables.count; i++)
	{
		const Variable& variable = variables.variables[i];
		if (variable.name == name)
			return Variable(variable);
	}
	return Variable();
}

std::string PostfixFormProcessor::findVariables(const std::string& expression)
{
	if (!checkExpression(expression))
		throw "olala"; // cannot found in bad exp
	std::string variablesNames;
	for (std::string::const_iterator token = expression.begin(); token != expression.end(); token++)
	{
		TokenType type = checkToken(*token);
		if (type == TokenType::operand)
		{
			bool isFirstTime = true;
			for (std::string::const_iterator prevToken = expression.begin(); prevToken != token; prevToken++)
				if (*token == *prevToken)
					isFirstTime = false;
			if (isFirstTime)
				variablesNames += *token;
		}
	}
	return variablesNames;
}

bool PostfixFormProcessor::checkExpression(const std::string& expression)
{
	try
	{
		if (countPostfixFormLength(expression))
			return true;
		return false;
	}
	catch (...)
	{
		return false;
	}
	return true;
}

std::string PostfixFormProcessor::parse(const std::string& expression)
{
	size_t postfixFormLength = countPostfixFormLength(expression);
	size_t operationsCount = countOperations(expression);
	TStack<char> postfixForm(postfixFormLength), operations(operationsCount);
	for (std::string::const_iterator token = expression.begin(); token != expression.end(); token++)
	{
		TokenType type = checkToken(*token);
		if (type == TokenType::operand)
			postfixForm.push(*token);
		else if (*token == '(')
			operations.push(*token);
		else if (type == TokenType::closingBrace)
		{
			if (operations.height() > 0)
				while (checkPriority(*token, operations.top()) == Priority::lower)
					postfixForm.push(operations.pop());
			while (operations.top() != '(')
				postfixForm.push(operations.pop());
			operations.pop(); // remove '('
		}
		else if (type == TokenType::operation)
		{
			if(operations.height() > 0)
				while (checkPriority(*token, operations.top()) == Priority::lower)
					postfixForm.push(operations.pop());
			operations.push(*token);
		}
		else
			throw "olala"; // something unknown
	}
	while (!operations.empty())
		postfixForm.push(operations.pop());
	std::string result;
	result.resize(postfixForm.height(), 0);
	for (std::string::reverse_iterator i = result.rbegin(); !postfixForm.empty(); i++)
		*i = postfixForm.pop();
	return result;
}

double PostfixFormProcessor::calculate(const std::string& postfixForm, const Variables variables)
{
	TStack<double> calculated(postfixForm.size());
	for (std::string::const_iterator i = postfixForm.begin(); i != postfixForm.end(); i++)
	{
		TokenType type = checkToken(*i);
		if (type == TokenType::operand)
		{
			Variable variable = findVariableByName(variables, *i);
			if (!variable.name)
				throw "olala"; // variable value not found
			calculated.push(variable.value);
		}
		else if (type == TokenType::operation)
		{
			double second = calculated.pop(), first = calculated.pop();
			if (*i == '+')
				calculated.push(first + second);
			else if (*i == '-')
				calculated.push(first - second);
			else if (*i == '*')
				calculated.push(first * second);
			else if (*i == '/')
			{
				if (second == 0.0)
					throw "olala"; // division by 0
				calculated.push(first / second);
			}
			else
				throw "olala"; // something wrong with operation
		}
		else
			throw "olala"; // something unawaitable like '='
	}
	double result = 0.0;
	try
	{
		result = calculated.pop();
	}
	catch(...)
	{
		throw "olala"; // no result => something went wrong
	}
	return result;
}