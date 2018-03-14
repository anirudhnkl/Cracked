#include "provided.h"
#include <string>
#include <unordered_map>
#include <iostream>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    ~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    struct Node
    {
        Node * next;
        unordered_map<char, char> map;
    };
    
    Node * head;
    void toLower(string &str) const; //elsewhere??
};

TranslatorImpl::TranslatorImpl()
{
    head = new Node;
    head->next = nullptr;
}

TranslatorImpl::~TranslatorImpl()
{
    //free memory
}

void TranslatorImpl::toLower(string &str) const
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    toLower(ciphertext);
    toLower(plaintext);
    if(ciphertext.size() != plaintext.size() || ciphertext.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != -1 || plaintext.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != -1)
        return false;
    
    Node * mapping = new Node;
    unordered_map<char, char>::iterator iter;
    unordered_map<char, char>::iterator it;
    
    for(int i = 0; i < ciphertext.size(); i++)
    {
        iter = head->map.find(ciphertext[i]);
        
        if(iter == head->map.end())
            mapping->map[ciphertext[i]] = plaintext[i];
        
        else
        {
            if(iter->second != plaintext[i])
                return false;
            else
                mapping->map[ciphertext[i]] = plaintext[i];
        }
        
        for(it = head->map.begin(); it != head->map.end(); it++)
        {
            mapping->map[it->first] = it->second;
            if(it->second == plaintext[i])
                if(it->first != ciphertext[i])
                    return false;
        }
    }
    
    mapping->next = head;
    head = mapping;
    
    return true;
}

bool TranslatorImpl::popMapping()
{
    if(head->next == nullptr)
        return false;
    
    Node * temp = head;
    head = head->next;
    delete temp;
        
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string translatedWord;
    unordered_map<char, char>::iterator iter;
    
    for(int i = 0; i < ciphertext.size(); i++)
    {
        iter = head->map.find(tolower(ciphertext[i]));
        
        if(!isalpha(ciphertext[i]))
            translatedWord += ciphertext[i];
        
        else if(iter == head->map.end())
            translatedWord += "?";
        
        else if(isupper(ciphertext[i]))
            translatedWord += toupper(iter->second);
        
        else if(islower(ciphertext[i]))
            translatedWord += iter->second;
    }
    
    return translatedWord;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
