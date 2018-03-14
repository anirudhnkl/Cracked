#include "provided.h"
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const string& s) const;
private:
    //string separators;
    unordered_set<char> separatorSet;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    //this->separators = separators;
    for(int i = 0; i < separators.size(); i++)
        separatorSet.insert(separators[i]);
}

vector<string> TokenizerImpl::tokenize(const string& s) const
{
    vector<string> words;
    string word = "";
    bool addChar = true;
    
    unordered_set<char>::const_iterator it;
    
    for(int i = 0; i < s.size(); i++)
    {
        it = separatorSet.find(s[i]);   //O(P) implementation
        if(it != separatorSet.end())
            addChar = false;
        
        /*for(int j = 0; j < separators.size(); j++)   //O(SP) implementation
        {
            if(s[i] == separators[j])
            {
                addChar = false;
                break;
            }
        }*/
        
        if(addChar)
            word += s[i];
        
        if(!word.empty() && (!addChar || i == s.size() - 1))
        {
            words.push_back(word);
            word = "";
        }
        
        addChar = true;
    }
    
    return words;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const string& s) const
{
    return m_impl->tokenize(s);
}
