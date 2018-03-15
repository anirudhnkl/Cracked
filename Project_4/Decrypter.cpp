#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList wl;
    Translator translator;
    Tokenizer tokenizer;
    int getTranslateWord();
    vector<string> translations;
    vector<string> words;
    string cipherText;
    void crackHelper();
};

DecrypterImpl::DecrypterImpl(): tokenizer("0123456789 ,;:.!()[]{}-\"#$%^&")
{
}

bool DecrypterImpl::load(string filename)
{
    return wl.loadWordList(filename);
}

int DecrypterImpl::getTranslateWord()
{
//    int wPos = -1;
//    string w, temp;
//
//    for(int i = 0; i < words.size(); i++)
//    {
//        if(wPos == -1 && !chosenWords.find(i))
//            wPos = i;
//        else
//        {
//            w = translator.getTranslation(words[wPos]);
//            temp = translator.getTranslation(words[i]);
//            if(count(w.begin(), w.end(), '?') < count(temp.begin(), temp.end(), '?') && !chosenWords.find(i))
//                wPos = i;
//        }
//    }
//
//    chosenWords.associate(wPos, true);
//
//    return wPos;
    return 0;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    words = tokenizer.tokenize(ciphertext);
    translations.clear();
    
    cipherText = ciphertext;
    
    crackHelper();
    
    sort(translations.begin(), translations.end());
    
    return translations;
}

void DecrypterImpl::crackHelper()
{
    int wPos = -1;
    string wTemp, temp;
    MyHash<int, bool> chosenWords;
    
    for(int i = 0; i < words.size(); i++)
    {
        if(wPos == -1 && !chosenWords.find(i))
            wPos = i;
        else
        {
            wTemp = translator.getTranslation(words[wPos]);
            temp = translator.getTranslation(words[i]);
            if(count(wTemp.begin(), wTemp.end(), '?') < count(temp.begin(), temp.end(), '?') && !chosenWords.find(i))
                wPos = i;
        }
    }
    
    if(wPos == -1)
        return;
    
    chosenWords.associate(wPos, true);

    string w = words[wPos];
    string wTranslated = translator.getTranslation(w);
    string translatedText;
    vector<string> tempWords, candidates = wl.findCandidates(w, wTranslated);
    bool fullWord = false;
    bool fullyTranslated = true;
    bool inWordList = true;
    
    if(candidates.empty())
    {
        translator.popMapping();
        return;
    }
    
    for(int i = 0; i < candidates.size(); i++)
    {
        if(!translator.pushMapping(w, candidates[i]))
            continue;
        translatedText = translator.getTranslation(cipherText);
        tempWords = tokenizer.tokenize(translatedText);
        
        fullWord = false;
        fullyTranslated = true;
        inWordList = true;
        
        for(int j = 0; j < tempWords.size(); j++)
        {
            if(tempWords[j].find('?') != -1)
                fullyTranslated = false;
            
            if(tempWords[j].find('?') == -1)
            {
                fullWord = true;
                if(!wl.contains(tempWords[j]))
                    inWordList = false;
            }
        }
        
        if(!fullWord)
        {
            translator.popMapping();
            continue;
        }
        
        if(fullWord && !fullyTranslated && inWordList)
            crackHelper();

        
        if(fullWord && fullyTranslated && inWordList)
        {
            translations.push_back(translatedText);
            translator.popMapping();
        }
    }
    
    translator.popMapping();
    return;
}



//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
