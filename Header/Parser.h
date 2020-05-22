#include <string>

#ifndef PARSER_H
#define PARSER_H


class Parser
{
    public:
        Parser();
        Parser(const std::string &str);
        ~Parser();
        int convertPostfix();
        double calculate();

        const std::string& getInfix() const { return m_str; }
        const std::string& getPostfix() const { return m_strPost; }
        double getAnswer() const { return m_answer; }
        void setString(const std::string &str) { m_str = str; m_validExpression = isValid(); }
        bool isValidExpression() { return m_validExpression; }
        bool isComputable() { return m_isComputable; }

    private:
        bool isValid();
        int getPriority(char ch);
        bool isNumber(char ch);
        int getPrevChar(int index);

    private:
        std::string m_str;
        std::string m_strPost;
        double m_answer;
        bool m_validExpression;
        bool m_isComputable;
};

#endif // PARSER_H
