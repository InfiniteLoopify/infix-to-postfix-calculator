#include "Parser.h"
#include "Stack.h"
#include <iostream>
#include <string>

Parser::Parser() : m_answer(0) {}
Parser::Parser(const std::string &str) : m_str(str), m_answer(0) { m_validExpression = isValid(); }
Parser::~Parser(){}

//infix to postfix converter
int Parser::convertPostfix()
{
    //if expression is invalid then end function
    if(!m_validExpression)
        return -1;

    char space = ' ';
    Stack <char, 50> op;

    //for every character in the string
    for(int i = 0; i < m_str.length(); ++i)
    {
        //if character is space then goto next iteration
        if(m_str[i] == space)
            continue;

        //if character is a number then push number until it ends
        if(isNumber(m_str[i]))
        {
            m_strPost.push_back(space);
            while(isNumber(m_str[i]))
            {
                m_strPost.push_back(m_str[i]);
                ++i;
            }
            --i;
        }
        //if character is open bracket '(' then push it
        else if(m_str[i] == '(')
        {
            int tempIndex = getPrevChar(i);
            //if previous character is number than add multiply '*' sign
            if(isNumber(m_str[tempIndex]))
            {
                if(getPriority(op.peek()) >= getPriority('*'))
                {
                    m_strPost.push_back(space);
                    m_strPost.push_back(op.pop());
                    op.push('*');
                }
                else
                    op.push('*');
            }
            op.push(m_str[i]);
        }
        //if character is close bracket ')' then pushback all popped operators to new string
        else if(m_str[i] == ')')
        {
            while(op.peek() != '(')
            {
                m_strPost.push_back(space);
                m_strPost.push_back(op.pop());
            }
            op.pop();
        }
        //if character is */+-
        else if(getPriority(m_str[i]) == 3 || getPriority(m_str[i]) == 2)
        {
            int tempIndex = getPrevChar(i);
            //if char is +- and previous is not a number or closed bracket ')' then push 0 before + or -
            if(getPriority(m_str[i]) == 2 && !isNumber(m_str[tempIndex]) && m_str[tempIndex] != ')')
            {
                char temp = m_str[i];
                m_strPost.push_back(space);
                m_strPost.push_back('0');
                m_strPost.push_back(space);
                ++i;
                while(isNumber(m_str[i]))
                {
                    m_strPost.push_back(m_str[i]);
                    ++i;
                }
                m_strPost.push_back(space);
                m_strPost.push_back(temp);
            }
            //if old char priority is >= new char then pushback old to string and push new to stack
            else if(getPriority(op.peek()) >= getPriority(m_str[i]))
            {
                m_strPost.push_back(space);
                m_strPost.push_back(op.pop());
                op.push(m_str[i]);
            }
            else
                op.push(m_str[i]);
        }
        else
            return -1;
    }
    //pushback remaining operators to the new string
    while(!op.isEmpty())
    {
        if(getPriority(op.peek()) == 1)
            op.pop();
        else
        {
            m_strPost.push_back(space);
            m_strPost.push_back(op.pop());
        }
    }

    return 0;
}

//ignore all spaces and go back to the previous character
int Parser::getPrevChar(int index)
{
    --index;
    while(m_str[index] == ' ')
        --index;
    return index;
}

//calculate the value of the postfix expression
double Parser::calculate()
{
    Stack <double, 50> st;
    //for every character in string
    for(int i = 0; i < m_strPost.length(); ++i)
    {
        //ignore spaces
        if(m_strPost[i] == ' ')
            continue;

        //if number then push number onto stack
        if(isNumber(m_strPost[i]))
        {
            int temp = i;
            while(isNumber(m_strPost[i]))
                ++i;
            st.push( std::stod( m_strPost.substr( temp, i - temp ) ) );
        }
        //else using operator, pop 2 values from stack and operate on them. then store new value in stack
        else
        {
            if(m_strPost[i] == '+')
                st.push(+ st.pop() + st.pop());
            else if(m_strPost[i] == '*')
                st.push(st.pop() * st.pop());
            else if(m_strPost[i] == '-')
            {
                double temp = st.pop();
                st.push(st.pop() - temp);
            }
            else if(m_strPost[i] == '/')
            {
                double temp = st.pop();
                st.push(st.pop() / temp);
            }
        }
    }
    m_answer = st.pop();
    return m_answer;
}

//check if expression is valid
bool Parser::isValid()
{
    m_isComputable = true;
    Stack <char, 50> st;
    for(int i = 0; i < m_str.length(); ++i)
    {
        //check if the characters are correct
        switch(m_str[i])
        {
        case '.':
        case '+': case '-': case '*': case '/': case ' ':
        case '(': case '{': case '[':
        case ')': case '}': case ']':
            break;
        default:
            if(m_str[i] >= '0' && m_str[i] <= '9')
                break;
            else if((m_str[i] >= 'a' && m_str[i] <= 'z') ||
                    (m_str[i] >= 'A' && m_str[i] <= 'Z') )
            {
                m_isComputable = false;
                break;
            }
            else
                return false;
        }

        //check delimiters, if bracket placement is correct
        if(m_str[i] == '[' || m_str[i] == '(' || m_str[i] == '{')
            st.push(m_str[i]);
        else if(m_str[i] == ']' || m_str[i] == ')' || m_str[i] == '}')
        {
            char ch = st.peek();
            if(ch == -1)
                return false;
            else if(ch == '[' && m_str[i] == ']')
                st.pop();
            else if(ch == '(' && m_str[i] == ')')
                st.pop();
            else if(ch == '{' && m_str[i] == '}')
                st.pop();
            else
                return false;
        }
    }
    if(st.isEmpty())
        return true;
    else
        return false;
}

//get priority for the characters
int Parser::getPriority(char ch)
{
    switch(ch)
    {
    case '/':
    case '*':
        return 3;
    case '+':
    case '-':
        return 2;
    case '(': case ')':
    case '{': case '}':
    case '[': case ']':
        return 1;
    default:
        return -1;
    }
}

//check if character is a number
bool Parser::isNumber(char ch) { return ((ch >= '0' && ch <= '9') || (ch == '.')) ||
                                        (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }
