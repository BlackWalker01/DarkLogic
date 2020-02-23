#include "utils.h"
#include <sstream>

bool isCharInVar(char c)
{
    return (c>='0' && c<='9')||(c>='a' && c<='z')||(c>='A' && c<='Z');
}

bool isSyntaxChar(char c)
{
    return c=='('||c==')'||c=='{'||c=='}'||c==',';
}

std::string uintToString(unsigned int x)
{
    std::string ret="";
    std::stringstream ss;
    ss<<x;
    ss>>ret;
    return ret;
}

std::string sizeToString(const size_t& x)
{
    std::string ret="";
    std::stringstream ss;
    ss<<x;
    ss>>ret;
    return ret;
}

std::string getVarName(const std::string &content, size_t& i)
{
    std::string crtName="";
    char c=content[i];
    //arithmetic case
    if(c>='0' && c<='9')
    {
        crtName+=c;
        size_t k=i+1;
        bool hasDot=false;
        for(;k<content.size();k++)
        {
            c=content[k];
            if(c<'0'||c>'9')
            {
                if(c=='.'&&!hasDot)
                {
                    hasDot=true;
                }
                else if((c>='a' && c<='z')||(c>='A' && c<='Z'))
                {
                    std::string ch="";
                    ch+=c;
                    throw std::runtime_error("Syntax error at index "+sizeToString(i)+", a variable name cannot start with a number");
                }
                else
                {
                    break;
                }
            }
            crtName+=c;
        }
        i=k;

    }
    //other case
    else if((c>='a' && c<='z')||(c>='A' && c<='Z'))
    {
        crtName+=c;
        size_t k=i+1;
        for(;k<content.size();k++)
        {
            c=content[k];
            if(!isCharInVar(c))
            {
                break;
            }
            crtName+=c;
        }
        i=k;
    }

    return crtName;
}
