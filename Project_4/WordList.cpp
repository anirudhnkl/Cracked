#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
using namespace std;

#include <fstream>
#include <iostream>

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, bool> allWords;
    MyHash<string, vector<string>> indexedWords;
    string getPattern(string word) const;
    void clear();
    void toLower(string &str) const;
};

void WordListImpl::clear()
{
    allWords.reset();
    indexedWords.reset();
}

void WordListImpl::toLower(string &str) const
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

string WordListImpl::getPattern(string word) const
{
    MyHash<char, char> letters;
    char c = ' ', patternLetter = 'a';
    char * associated;
    string pattern = "";
    
    for(int i = 0; i < word.size(); i++)
    {
        c = word[i];
        associated = letters.find(c);
        if(associated != nullptr)
            pattern += *associated;
        else
        {
            letters.associate(c, patternLetter);
            pattern += patternLetter;
            patternLetter++;
        }
    }
    
    return pattern;
}

bool WordListImpl::loadWordList(string filename)
{
    clear();
    
    ifstream infile(filename);
    
    if(!infile)
        return false;
    
    string s, sPattern;
    vector<string> * words;
    while(getline(infile, s))
    {
        toLower(s);
        if(s.find_first_not_of("abcdefghijklmnopqrstuvwxyz'") == -1)
        {
            allWords.associate(s, true);
            sPattern = getPattern(s);
            words = indexedWords.find(sPattern);
            if(words == nullptr)
            {
                vector<string> tWords;
                tWords.push_back(s);
                indexedWords.associate(sPattern, tWords);
            }
            else
                words->push_back(s);
        }
    }
    
    return true;
}

bool WordListImpl::contains(string word) const
{
    toLower(word);
    return (allWords.find(word) != nullptr);
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> words;
    toLower(cipherWord);
    toLower(currTranslation);
    if(indexedWords.find(getPattern(cipherWord)) == nullptr)
        return words;
    
    words = *indexedWords.find(getPattern(cipherWord));
    string word;
    bool increment = true;
    
    for(vector<string>::iterator iter = words.begin(); iter != words.end(); )
    {
        word = *iter;
        for(int i = 0 ; i < currTranslation.size(); i++)
        {
            increment = true;
            if(isalpha(currTranslation[i]))
            {
                if(!isalpha(cipherWord[i]))
                   return *(new vector<string>);
                if(currTranslation[i] != word[i])
                {
                    increment = false;
                    iter = words.erase(iter);
                    break;
                }
            }
            
            else if(currTranslation[i] == '?')
            {
                if(!isalpha(cipherWord[i]))
                    return *(new vector<string>);
                if(!isalpha(word[i]))
                {
                    increment = false;
                    iter = words.erase(iter);
                    break;
                }
            }
            
            else if(currTranslation[i] == '\'')
            {
                if(cipherWord[i] != '\'')
                    return *(new vector<string>);
                if(word[i] != '\'')
                {
                    increment = false;
                    iter = words.erase(iter);
                    break;
                }
            }
        }
        
        if(increment)
            iter++;
    }
    
    return words;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
