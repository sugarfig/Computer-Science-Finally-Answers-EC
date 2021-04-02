//Author: Erica Carballo
//Project name: Computer Science Finally Answers
//Description: Program that allows the user to ask questions to the computer science department
//and get their questions answered by the program.
//Last Changed: June 6, 2020

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct questionStruct
{
    int key;
    string questions;
};

struct answerMatches
{
    int key;
    int numberMatched;
};

void openFiles(ifstream& answerFile, ifstream& questionFile, ifstream& generalAnswerFile);
//Will open the input files.
//Output will be the open input files.

void fillVectorQuestions(vector<questionStruct>& questionsVector, ifstream& questionFile);
//Will fill in clean version of question vector.
//Input will be the input question file.
//Output will be the cleaned vector.

void fillVectorAnswers(vector<string>& answersVector, ifstream& answerFile);
//Will get the answer string.
//Input will be the input answer file.
//Output will be the question vector.

void fillVectorGeneralAnswers(vector<string>& generalAnswersVector, ifstream& generalAnswerFile);
//Will put all the answers from the general answer file
//into a string vector.
//Input will be the general anwer file.
//Output will be the general answer vector.

void getInput(string& userQuestion);
//Will ask the user for their question and clean it.
//Output will be the cleaned string of user.

int searchForKey(vector<questionStruct> questionsVector, string userQuestion);
//Will compare the user question to each question in the vector.
//Input will be the question vector and the user vector.
//Output will be key, else will be -1.

string findAnswer(vector<string> answersVector, int key);
//Will find the answer based on key.
//Input will be the answer vector and the kwy.
//Output will be the string answer.

void makeWordsMatchVector(const vector<questionStruct> questionsVector, vector<answerMatches>& keyWordMatchVector, const string userQuestion);
//Will put the key and number of words matched in a vector.
//Input will be the question vector.
//Output will be the keyWordMatchVector.

int numberWordsMatch(const string userQuestion, const string questionToMatch);
//Input will be user question and the questions vector.
//Will count the matches for each question and store the keys for the questions that have matched words.
//Output will be the number words matched if it is not zero and the key of the question.

void displayPossibleAnswers(vector<answerMatches> keyWordMatchVector, vector<string> answersVector, vector<string> generalAnswersVector, int& bestMatch);
//Will display the possible answers and
//ask user for best match.
//Input will be the answer vector, general answer
//vector, and the matches vector.
//Output wil be the best match number.

void proccessBestMatchChoice(int bestMatch, vector<answerMatches> keyWordMatchVector, string userQuestion, vector<string> answersVector,vector<string> generalAnswersVector);
//Will process the choice given by user.
//Input will be the best match, the user question,
//answer vector, general answer vector, and key word match vector.
//Output will be updating the files.

void openFilesAppend(ofstream& appendAnswers, ofstream& contactInfoFile, ofstream& appendQuestions);
//Will open the files to append them.
//Output will be the open files.

void updateQuestions(string userQuestion, int key, ofstream& appendQuestions);
//Will update the question file.
//Input will be the key, question file, and the user question.
//Output will be the key and the user question to the question file.

void updateAnswers(vector<string> answersVector, vector<string> generalAnswersVector, int bestMatch, int& lastKey, ofstream& appendAnswers);
//Wil update the answer file.
//Input is the answer file, the key, the best match,
//general answer vector, and answer vector.

void storeContactInfo(string userQuestion, ofstream& contactInfoFile);
//Will store the user's contact info with the question.
//Input will be the contact file, the user's question,
//and contact information.
//Output will be the user's question and contact info to
//the contact file.

vector<string> split(string target, string delimiter);
//Will split the string and put it into a vector.
//Input will be the sting and a delimiter.
//Output will be a vector.

string removePunct(const string& s);
//Will remove the punctuation from the string.
//Input will be original string.
//Output will be the cleaned string.

string makeLower(const string& s);
//Will make the string lower case.
//Input will be the original string.
//Output will be the lower case string.

string removeExtraSpaces(const string& s);
//Will make sure each word has only one space bewtween it.
//Input will be the original string.
//Output will be the string with only one space between words.

void sort(vector<answerMatches>& keyWordMatchVector);
//Will sort the vector.
//Input will be the unsorted vector.
//Output will be the sorted vector.

void swapValues(int& v1Key, int& v2Key, int& v1NumberMatched, int& v2NumberMatched);
//Will swap the values of the indexes.
//Input will be the keys and numbers matched of keyword match vecctor.
//Output will be the values swapped.

int indexOfLargest(vector<answerMatches> keyWordMatchVector, int startIndex);
//Will get the index of the largest match number.
//Input will be the keyWordMatchVector and the start index.
//Output will be the index of the largest.

int main()
{
    ifstream questionFile, answerFile, generalAnswerFile;
    vector<questionStruct> questionsVector;
    vector<string> answersVector;
    vector<answerMatches> keyWordMatchVector;
    vector<string> generalAnswersVector;
    string userQuestion, answerFound;
    int keyFound, bestMatch;

    openFiles(answerFile, questionFile, generalAnswerFile);
    fillVectorQuestions(questionsVector, questionFile);
    fillVectorAnswers(answersVector, answerFile);
    fillVectorGeneralAnswers(generalAnswersVector, generalAnswerFile);

    questionFile.close();
    answerFile.close();
    generalAnswerFile.close();

    getInput(userQuestion);
    keyFound = searchForKey(questionsVector, userQuestion);
    if (keyFound == -1)
    {
        makeWordsMatchVector(questionsVector, keyWordMatchVector,userQuestion);
        sort(keyWordMatchVector);

        //TEST TO SEE VALUES IN keyWordMatchVector
        /*for (int index = 0; index < keyWordMatchVector.size(); index++)
        {
          cout << keyWordMatchVector[index].key << " "
            << keyWordMatchVector[index].numberMatched << endl;
        }*/

        displayPossibleAnswers(keyWordMatchVector, answersVector, generalAnswersVector, bestMatch);

        proccessBestMatchChoice(bestMatch, keyWordMatchVector, userQuestion, answersVector, generalAnswersVector);

    }
    else
    {
        answerFound = findAnswer(answersVector, keyFound);
        cout << answerFound << endl << endl;
    }

    cout << endl << "This is the end of the program.\n";

    return 0;
}

void openFiles(ifstream& answerFile, ifstream& questionFile, ifstream& generalAnswerFile)
{
    questionFile.open("Questions.txt");
    if (questionFile.fail())
    {
        cout << "The Questions.txt file failed to open.\n";
        exit (1);
    }

    answerFile.open("Answers.txt");
    if(answerFile.fail())
    {
        cout << "The Answers.txt file failed to open.\n";
        exit(1);
    }

    generalAnswerFile.open("generalAnswers.txt");
    if(generalAnswerFile.fail())
    {
        cout << "The generalAnswers.txt file failed to open.\n";
        exit (1);
    }

}

void fillVectorQuestions(vector<questionStruct>& questionsVector, ifstream& questionFile)
{
    string temp;
    while (!questionFile.eof())
    {
        struct questionStruct q;

        questionFile >> q.key;
        getline(questionFile, q.questions);

        q.questions = removeExtraSpaces(q.questions);
        q.questions = removePunct(q.questions);
        q.questions = makeLower(q.questions);

        questionsVector.push_back(q);
    }
}

void fillVectorAnswers(vector<string>& answersVector, ifstream& answerFile)
{
    string temp;
    int tempKey;

    while(!answerFile.eof())
    {
        answerFile >> tempKey;
        getline(answerFile, temp);
        answersVector.push_back(temp);
    }
}

void fillVectorGeneralAnswers(vector<string>& generalAnswersVector, ifstream& generalAnswerFile)
{
    string temp;

    while (!generalAnswerFile.eof())
    {
        getline(generalAnswerFile, temp);
        generalAnswersVector.push_back(temp);
    }
}

void getInput(string& userQuestion)
{
    cout << "Please enter a question for the Computer\n"
         << "Science Department:\n";
    getline(cin, userQuestion);

    userQuestion = removePunct(userQuestion);
    userQuestion = makeLower(userQuestion);
    userQuestion = removeExtraSpaces(userQuestion);
}

int searchForKey(vector<questionStruct> questionsVector, string userQuestion)
{
    int key;
    string temp;
    key = -1;
    for (int index = 0; index < questionsVector.size(); index++)
    {
        temp = questionsVector[index].questions;

        if (userQuestion == temp)
        {
            key = questionsVector[index].key;
        }
    }

    return key;
}

string findAnswer(vector<string> answersVector, int key)
{
    string result;

    if (key >= 0)
    {
        result = answersVector[key - 1];
    }

    return result;
}

void displayPossibleAnswers(vector<answerMatches> keyWordMatchVector, vector<string> answersVector, vector<string> generalAnswersVector, int& bestMatch)
{
    int index, vectorSize;
    vectorSize = keyWordMatchVector.size();

    cout << endl << "There was not an exact match for your\n"
         << "question. Please select the answer that is most\n"
         << "similar to your question:\n\n";

    if (keyWordMatchVector.empty())
    {
        for (index = 0; index < 4; index++)
        {
            cout << index + 1 << ". " << generalAnswersVector[index] << endl;
        }
    }
    else
    {
        switch (vectorSize)
        {
            case 1:
                for (index = 0; index < 1; index++)
                {
                    cout << index + 1 << "." << answersVector[keyWordMatchVector[index].key - 1] << endl;
                }
                while (index < 4)
                {
                    cout << index + 1 << ". " << generalAnswersVector[index] << endl;
                    index++;
                }
                break;
            case 2:
                for (index = 0; index < 2; index++)
                {
                    cout << index + 1 << "." << answersVector[keyWordMatchVector[index].key - 1] << endl;
                }

                while (index < 4)
                {
                    cout << index + 1 << ". " << generalAnswersVector[index] << endl;
                    index++;
                }
                break;
            default:
                for (index = 0; index < 3; index++)
                {
                    cout << index + 1 << "." << answersVector[keyWordMatchVector[index].key - 1] << endl;
                }

                while (index < 4)
                {
                    cout << index + 1 << ". " << generalAnswersVector[index] << endl;
                    index++;
                }
                break;
        }
    }

    index++;
    cout << index << ". None of these answer my question.\n\n"
         << "Please enter the number that answers your\n"
         << "question most: ";
    cin >> bestMatch;

    while ((bestMatch > 5) || (bestMatch <= 0))
    {
        cout << "That is not a valid option. Please enter a\n"
             << "number between 1 and 5: ";
        cin >> bestMatch;
    }
}

void proccessBestMatchChoice(int bestMatch, vector<answerMatches> keyWordMatchVector, string userQuestion, vector<string> answersVector,vector<string> generalAnswersVector)
{
    ofstream contactInfoFile, appendAnswers, appendQuestions;
    openFilesAppend(appendAnswers, contactInfoFile, appendQuestions);

    int key, lastKey, vectorSize;
    vectorSize = keyWordMatchVector.size();

    if (keyWordMatchVector.empty())
    {
        switch(bestMatch)
        {
            case 1:
            case 2:
            case 3:
            case 4:
                updateAnswers(answersVector, generalAnswersVector, bestMatch, lastKey, appendAnswers);
                updateQuestions(userQuestion, lastKey, appendQuestions);
                cout << endl << "The answer to your question is:\n"
                     << generalAnswersVector[bestMatch - 1] << endl;
                break;
            case 5:
                cout << "I apologize for the inconvenience.\n";
                storeContactInfo(userQuestion, contactInfoFile);
                break;
            default:
                break;
        }
    }
    else //checks if there are only one or two questions with matches.
    {
        switch (vectorSize)
        {
            case 1:
                switch(bestMatch)
                {
                    case 1:
                        key = keyWordMatchVector[bestMatch - 1].key;
                        updateQuestions(userQuestion, key, appendQuestions);
                        cout << endl << "The answer to your question is:\n"
                             << answersVector[key - 1] << endl;
                        break;
                    case 2:
                    case 3:
                    case 4:
                        updateAnswers(answersVector, generalAnswersVector, bestMatch, lastKey, appendAnswers);
                        updateQuestions(userQuestion, lastKey, appendQuestions);
                        cout << endl << "The answer to your question is:\n"
                             << generalAnswersVector[bestMatch - 1] << endl;
                        break;
                    case 5:
                        cout << "I apologize for the inconvenience.\n";
                        storeContactInfo(userQuestion, contactInfoFile);
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                switch(bestMatch)
                {
                    case 1:
                    case 2:
                        key = keyWordMatchVector[bestMatch - 1].key;
                        updateQuestions(userQuestion, key, appendQuestions);
                        cout << endl << "The answer to your question is:\n"
                             << answersVector[key - 1] << endl;
                        break;
                    case 3:
                    case 4:
                        updateAnswers(answersVector, generalAnswersVector, bestMatch, lastKey, appendAnswers);
                        updateQuestions(userQuestion, lastKey, appendQuestions);
                        cout << endl << "The answer to your question is:\n"
                             << generalAnswersVector[bestMatch - 1] << endl;
                        break;
                    case 5:
                        cout << "I apologize for the inconvenience.\n";
                        storeContactInfo(userQuestion, contactInfoFile);
                        break;
                    default:
                        break;
                }
                break;
            default: //if there are 3 or more questions with matches.
                switch(bestMatch)
                {
                    case 1:
                    case 2:
                    case 3:
                        key = keyWordMatchVector[bestMatch - 1].key;
                        updateQuestions(userQuestion, key, appendQuestions);
                        cout << endl << "The answer to your question is:\n"
                             << answersVector[key - 1] << endl;
                        break;
                    case 4:
                        updateAnswers(answersVector, generalAnswersVector, bestMatch, lastKey, appendAnswers);
                        updateQuestions(userQuestion, lastKey, appendQuestions);
                        cout << endl << "The answer to your question is:\n"
                             << generalAnswersVector[bestMatch - 1] << endl;
                        break;
                    case 5:
                        cout << "I apologize for the inconvenience.\n";
                        storeContactInfo(userQuestion, contactInfoFile);
                        break;
                    default:
                        break;
                }
                break;
        }
    }

    contactInfoFile.close();
    appendAnswers.close();
    appendQuestions.close();
}

void openFilesAppend(ofstream& appendAnswers, ofstream& contactInfoFile, ofstream& appendQuestions)
{
    appendQuestions.open("Questions.txt", ios::app);
    if(appendQuestions.fail())
    {
        cout << "The Questions.txt file failed to open.\n";
        exit (1);
    }

    appendAnswers.open("Answers.txt", ios::app);
    if(appendAnswers.fail())
    {
        cout << "The Answers.txt file failed to open.\n";
        exit (1);
    }

    contactInfoFile.open("followUp.txt", ios::app);
    if (contactInfoFile.fail())
    {
        cout << "The followUp.txt file failed to open.\n";
        exit (1);
    }
}

void updateQuestions(string userQuestion, int key, ofstream& appendQuestions)
{
    string keyString;
    keyString = to_string(key);
    appendQuestions << keyString << " " << userQuestion << endl;
}

void updateAnswers(vector<string> answersVector, vector<string> generalAnswersVector, int bestMatch, int& lastKey, ofstream& appendAnswers)
{
    string lastKeyString;

    for (int index = 0; index < answersVector.size(); index++)
    {
        lastKey = index + 1;
    }

    lastKeyString = to_string(lastKey);
    appendAnswers << lastKeyString << " " << generalAnswersVector[bestMatch - 1] << endl;
}

void storeContactInfo(string userQuestion, ofstream& contactInfoFile)
{
    string contactInfo;

    cout << "We will get back to you. Please leave your name and email:\n";
    cin.ignore();
    getline(cin, contactInfo);

    contactInfoFile << contactInfo << " question: " << userQuestion << endl;

    cout << endl << "Thank you we will get back to you soon.\n";
}

void makeWordsMatchVector(const vector<questionStruct> questionsVector, vector<answerMatches>& keyWordMatchVector, const string userQuestion)
{
    int matched;
    struct answerMatches matches;

    for (int index = 0; index < questionsVector.size(); index++)
    {
        matched = numberWordsMatch(userQuestion, questionsVector[index].questions);

        if (matched > 0)
        {
            matches.key = questionsVector[index].key;
            matches.numberMatched = matched;keyWordMatchVector.push_back(matches);
        }

    }

}

int numberWordsMatch(string userQuestion, string questionToMatch)
{
    int count;
    vector<string> splitUserQuestion;
    vector<string> splitQuestionToMatch;
    count = 0;

    splitUserQuestion = split(userQuestion, " ");
    splitQuestionToMatch = split(questionToMatch, " ");

    for (int index = 0; index < splitUserQuestion.size(); index++)
    {
        for (int index2 = 0; index2 < splitQuestionToMatch.size(); index2++)
        {
            if (splitUserQuestion[index] == splitQuestionToMatch[index2])
            {
                count = count + 1;
            }
        }
    }

    return count;
}
//Author: Erica Carballo
vector<string> split(string target, string delimiter)
{
    int indexOfDelimiter, indexOfBeginString;
    vector<string> result;
    string subString;
    indexOfBeginString = 0;

    indexOfDelimiter = target.find(delimiter);
    while (indexOfDelimiter != string::npos)
    {
        subString = target.substr(indexOfBeginString, indexOfDelimiter - indexOfBeginString);indexOfBeginString = indexOfDelimiter + 1;
        result.push_back(subString);
        indexOfDelimiter = target.find(delimiter, indexOfBeginString);
    }
    subString = target.substr(indexOfBeginString, target.length() - indexOfBeginString);
    result.push_back(subString);

    return result;
}

string makeLower(const string& s)
{
    string temp;
    temp = s;

    for(int i = 0; i < s.length( ); i++)
    {
        temp[i] = towlower(s[i]);
    }

    return temp;
}

string removePunct(const string& s)
{
    string noPunct, aChar;
    int sLength, punctLength, location;
    sLength = s.length();
    const string punct = ",;:.?!()'\"/";
    punctLength = punct.length();
    bool found;
    found = false;

    for (int i = 0; i < sLength; i++)
    {

        aChar = s.substr(i, 1);
        location = punct.find(aChar, 0);

        if (location >= 0)
        {
            found = true;
        }
        else
        {
            found = false;
        }

        if (!found)
        {
            noPunct = noPunct + aChar;
        }
    }

    return noPunct;
}

string removeExtraSpaces(const string& s)
{
    char ch, previous;
    string noExtraSpaces;
    int index;
    previous = ' ';
    ch = ' ';
    index = 0;


    previous = s[index];
    if (previous == ' ')
    {
        index++;
        previous = s[index];
        ch = s[index];
    }
    else
    {
        previous = ' ';
        ch = s[index];
    }

    while (index < s.length())
    {
        while ((previous == ' ') && (ch == ' '))
        {
            previous = ch;
            index++;
            ch = s[index];
        }
        noExtraSpaces = noExtraSpaces + ch;
        previous = ch;
        index++;
        ch = s[index];
    }

    return noExtraSpaces;
}

void sort(vector<answerMatches>& keyWordMatchVector)
{
    int indexOfNextLargest;

    for (int index = 0; index < keyWordMatchVector.size(); index++)
    {
        indexOfNextLargest = indexOfLargest(keyWordMatchVector, index);
        swapValues(keyWordMatchVector[index].key, keyWordMatchVector[indexOfNextLargest].key, keyWordMatchVector[index].numberMatched, keyWordMatchVector[indexOfNextLargest].numberMatched);
    }
}

void swapValues(int& v1Key, int& v2Key, int& v1NumberMatched, int& v2NumberMatched)
{
    int temp, temp2;
    temp = v1Key;
    v1Key = v2Key;
    v2Key = temp;

    temp2 = v1NumberMatched;
    v1NumberMatched = v2NumberMatched;
    v2NumberMatched = temp2;
}

int indexOfLargest(vector<answerMatches> keyWordMatchVector, int startIndex)
{
    int largest, indexOfLargest;
    largest = keyWordMatchVector[startIndex].numberMatched;
    indexOfLargest = startIndex;

    for (int index = startIndex + 1; index < keyWordMatchVector.size(); index++)
    {
        if (keyWordMatchVector[index].numberMatched > largest)
        {
            largest = keyWordMatchVector[index].numberMatched;
            indexOfLargest = index;
        }
    }

    return indexOfLargest;
}

/*Please enter a question for the Computer
Science Department:
Why C++ called C++?

There was not an exact match for your
question. Please select the answer that is most
similar to your question:

1. The ++ in C++ means to increment by one, to indicate that the language is one step above C, adding object-oriented features.
2. C is the procedural programming language that does not support class, and objects, but C++ is the combination of both procedural and objects oriented programming language.
3. Base on my understanding in both language so far, in term of app delevopment, Java is more recommended since it is the foundation of Android development and is easy to use.
4. If you are a non-STEM major, you should begin with CS 001 at PCC. If you are a STEM major, you should begin with CS 002.
5. None of these answer my question.

Please enter the number that answers your
question most: 1

The answer to your question is:
 The ++ in C++ means to increment by one, to indicate that the language is one step above C, adding object-oriented features.

This is the end of the program.*/

