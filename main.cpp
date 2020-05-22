#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//stack of type T and length of array N
template <class T, int N>
class Stack
{
//---------------------------------------------------------------------------------------------------------------------
    public:
//---------------------------------------------------------------------------------------------------------------------
        Stack() : m_size(-1){}
        ~Stack(){}
//---------------------------------------------------------------------------------------------------------------------
        Stack(const Stack& other) : m_maxSize(50)
		{
		    m_size = other.m_size;
		    for(int i = 0; i <= m_size; ++i)
		        m_array[i] = other.m_array[i];
		}
//---------------------------------------------------------------------------------------------------------------------
        T push(T num)
		{
		    //if stack is not full then push data and return data
		    if( !isFull() )
		    {
		        ++m_size;
		        m_array[m_size] = num;
		        return num;
		    }
		    else
		        return -1;
		}
//---------------------------------------------------------------------------------------------------------------------
		T pop()
		{
		    //if stack is not empty then pop data and return popped value
		    if( !isEmpty() )
		    {
		        --m_size;
		        return m_array[m_size + 1];
		    }
		    else
		        return -1;
		}
//---------------------------------------------------------------------------------------------------------------------
        T peek() { return m_array[m_size]; }
        bool isEmpty() { return (m_size == -1); }
        bool isFull() { return (m_size == m_maxSize - 1); }
//---------------------------------------------------------------------------------------------------------------------
        void display()
		{
		    for(int i = 0; i <= m_size; ++i)
		        std::cout << m_array[i] << " ";
		    std::cout << "\n";
		}
//---------------------------------------------------------------------------------------------------------------------
    private:
//---------------------------------------------------------------------------------------------------------------------
        const int m_maxSize = N;
        T m_array[50];
        int m_size;
//---------------------------------------------------------------------------------------------------------------------
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Parser
{
//---------------------------------------------------------------------------------------------------------------------
    public:
//---------------------------------------------------------------------------------------------------------------------
        Parser() : m_answer(0) {}
        Parser(const std::string &str) : m_str(str), m_answer(0) { m_validExpression = isValid(); }
        ~Parser(){}
//---------------------------------------------------------------------------------------------------------------------        
		//infix to postfix converter
		int convertPostfix()
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
		        // else if(getPriority(m_str[i]) == 3 || getPriority(m_str[i]) == 2)
				else if(getPriority(m_str[i]) >= 2)
		        {
					char tempThan;
					tempThan = ' ';
		            int tempIndex = getPrevChar(i);
					
					if((getPriority(m_str[i]) == 4 || getPriority(m_str[i]) == 5) && m_str[i+1] == '=')
					{
						if(m_str[i] == '<')
							tempThan = ',';
						else if(m_str[i] == '>')
							tempThan = '?';
						else if(m_str[i] == '=')
							tempThan = '=';
						else if(m_str[i] == '!')
							tempThan = '!';
						++i;
					}
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
						if(tempThan == ' ')
		                	op.push(m_str[i]);
						else
							op.push(tempThan);	
		            }
		            else
					{
						if(tempThan == ' ')
		                	op.push(m_str[i]);
						else
							op.push(tempThan);
					}
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
//---------------------------------------------------------------------------------------------------------------------		
        //calculate the value of the postfix expression
		double calculate()
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

					//new edits
		            else if(m_strPost[i] == '=')
		                st.push(st.pop() == st.pop());
		            else if(m_strPost[i] == '!')
		                st.push(st.pop() != st.pop());
		            else if(m_strPost[i] == '<')
		                st.push(st.pop() > st.pop());
		            else if(m_strPost[i] == '>')
		                st.push(st.pop() < st.pop());
		            else if(m_strPost[i] == ',')
		                st.push(st.pop() >= st.pop());
		            else if(m_strPost[i] == '?')
		                st.push(st.pop() <= st.pop());
		            else if(m_strPost[i] == '|')
		                st.push(st.pop() || st.pop());
		            else if(m_strPost[i] == '&')
		                st.push(st.pop() && st.pop());
		        }
		    }
		    m_answer = st.pop();
		    return m_answer;
		}
//---------------------------------------------------------------------------------------------------------------------
		//convert integer value to boolean
		bool convertToBool(int num)
		{
			return (num == 0) ? false : true;
		}
//---------------------------------------------------------------------------------------------------------------------
		//print infix without spaces
		void printInfix()
		{
			for(int i = 0; i < m_str.length(); ++i)
				if(m_str[i] != ' ')
					std::cout << m_str[i];
		}
//---------------------------------------------------------------------------------------------------------------------
        const std::string& getInfix() const { return m_str; }
        const std::string& getPostfix() const { return m_strPost; }
        double getAnswer() const { return m_answer; }
        void setString(const std::string &str) { m_str = str; m_validExpression = isValid(); }
        bool isValidExpression() { return m_validExpression; }
        bool isComputable() { return m_isComputable; }

//---------------------------------------------------------------------------------------------------------------------
    private:
//---------------------------------------------------------------------------------------------------------------------
        //check if expression is valid
		bool isValid()
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
				case '=': case '!': 
				case '<': case '>':
				case '|': case '&':
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
//---------------------------------------------------------------------------------------------------------------------        
        //get priority for the characters
		int getPriority(char ch)
		{
		    switch(ch)
		    {
			case '|':
		        return 7;
			case '&':
		        return 6;
			case '=':
			case '!':
		        return 5;
			case ',':
		    case '?':
		        return 4;
			case '<':
		    case '>':
		        return 4;
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
//---------------------------------------------------------------------------------------------------------------------		
		//check if character is a number
		bool isNumber(char ch) { return ((ch >= '0' && ch <= '9') || (ch == '.')) ||
		                                        (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }

//---------------------------------------------------------------------------------------------------------------------       
        //ignore all spaces and go back to the previous character
        int getPrevChar(int index)
		{
		    --index;
		    while(m_str[index] == ' ')
		        --index;
		    return index;
		}

//---------------------------------------------------------------------------------------------------------------------
    private:
//---------------------------------------------------------------------------------------------------------------------
        std::string m_str;
        std::string m_strPost;
        double m_answer;
        bool m_validExpression;
        bool m_isComputable;
//---------------------------------------------------------------------------------------------------------------------
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printRules()
{
	std::cout << "(+)  Addition\n"
				 "(-)  Subtraction\n"
				 "(*)  Multiplication\n"
				 "(/)  Division\n"
				 "(==) Equal to\n"
				 "(!=) Not Equal to\n"
				 "(<)  Less Than\n"
				 "(>)  Greater Than\n"
				 "(<=) Less Than Equal to\n"
				 "(>=) Greater Than Equal to\n"
				 "(&)  And\n"
				 "(|)  Or\n"
				 "\n";
}
//---------------------------------------------------------------------------------------------------------------------
int main()
{
    Parser par("51+ 0 <>=");
    par.convertPostfix();
    if(par.isComputable())
        par.calculate();
        
    printRules();

    //if the expression is valid then display answer, else only display expression
    if(par.isValidExpression())
    {
        std::cout << "Expression:\n";
        par.printInfix();
        std::cout << "\n\n";
        //std::cout << "Postfix:\n" << par.getPostfix() << "\n\n";
        if(par.isComputable())
        {
        	std::string strAns = par.convertToBool(par.getAnswer()) ? "True" : "False";
            std::cout << "Answer:\n" << strAns << "\n\n";
        }
        	
    }
    else
        std::cout << "Invalid Expression:\n" << par.getInfix() << "\n\n";
    
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
