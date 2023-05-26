#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stack>
#include <conio.h>
#include"TrieTree.h"
using namespace std;

stack<string> previousTexts; // Stack to store previous texts
stack<string> deletedTexts;  // Stack to store deleted texts
int x = 0, y = 0;
string copiedText;

TrieTree t;


void insert1()
{
    string data = "", para;
    int temp = 0;
    cout << "Enter X position: ";
    cin >> x;
    cout << "Enter Y position: ";
    cin >> y;

    ofstream file;
    y--;
    y = 191 * y;
    y = y + x;
    bool check = false;

    while (true)
    {
        if (temp == -1)
        {
            break;
        }
        char keypress;
        file.open("test.txt", ios::app);
        cout << "Enter data: ";
        cin.ignore(); // Ignore the newline character in the input buffer
        getline(cin, data);


        string* suggested = t.suggest(data);

        for (int i = 0; suggested[i] != "NULL" && i < 10; i++) {
            cout << i << ":" << suggested[i] << endl;
        }
        int ch;
        if (suggested != NULL) {
            cout << "Enter Choice: or -1 to not choose ";
            cin >> ch;
            if (ch >= 0) {
                data = suggested[ch];
            }
        }


        if (check == false)
        {
            for (int i = 0; i < y; i++)
            {
                if ((i) % 191 == 0 && i != 0 && check == false)
                {
                    file << endl;
                }
                file << ' ';
            }
        }
        else
        {
            file << ' ';
        }



        check = true;
        file << data;
        previousTexts.push(data); // Store the entered data in the previousTexts stack
        cout << " (enter -1 to exit or press 1 to continue): ";
        cin >> temp;
        file.close();
    }
}

void undo()
{
    if (!previousTexts.empty())
    {
        string lastText = previousTexts.top();
        previousTexts.pop();
        deletedTexts.push(lastText); // Store the deleted text in the deletedTexts stack
        cout << "Undo successful." << endl;

        // Rewrite the file with remaining texts from previousTexts stack
        ofstream file("test.txt", ios::trunc); // Truncate the file
        bool check2 = false;
        while (!previousTexts.empty())
        {
            if (check2 == false)
            {
                for (int i = 0; i < y; i++)
                {
                    if ((i) % 191 == 0 && i != 0 && check2 == false)
                    {
                        file << endl;
                    }
                    file << ' ';
                }
            }
            else
            {
                file << ' ';
            }
            file << previousTexts.top() << " ";
            previousTexts.pop();
            check2 = true;
        }
        file.close();
    }
    else
    {
        cout << "No text to undo." << endl;
    }
}

void redo()
{
    if (!deletedTexts.empty())
    {
        string lastDeletedText = deletedTexts.top();
        deletedTexts.pop();
        previousTexts.push(lastDeletedText); // Restore the previously deleted text

        // Append the lastDeletedText to the file
        ofstream file("test.txt", ios::app);
        file << lastDeletedText << endl;
        file.close();

        cout << "Redo successful." << endl;
    }
    else
    {
        cout << "No text to redo." << endl;
    }
}

void count()
{
    ifstream file("test.txt");
    int words = -1;

    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    int characters = 0;
    bool check = false;

    while (!file.eof())
    {
        file >> line;
        words++;
        characters += line.size();
    }

    file.close();
    cout << "Total Words: " << words << endl;
    cout << "Total Characters: " << characters << endl;
}

void similarity()
{
    ifstream file1("file1.txt"); // Replace "file1.txt" with the path to the first file
    ifstream file2("file2.txt"); // Replace "file2.txt" with the path to the second file

    if (!file1 || !file2)
    {
        cerr << "Failed to open one or both files." << endl;
        return;
    }

    char char1, char2;
    int similarChars = 0;
    int totalChars = 0;

    while (file1.get(char1) && file2.get(char2))
    {
        totalChars++;
        if (char1 == char2)
        {
            similarChars++;
        }
    }

    file1.close();
    file2.close();

    float similarityPercentage = (static_cast<float>(similarChars) / totalChars) * 100;

    cout << "Similarity percentage: " << similarityPercentage << "%" << endl;
}

void updatesugesstions() {
    string searchWord, replaceWord;
    cout << "Enter the word to search: ";
    cin >> searchWord;
    cout << "Enter the word to replace with: ";
    cin >> replaceWord;

    ifstream file("dictionary.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    bool found = false;

    while (getline(file, line))
    {
        size_t pos = line.find(searchWord);
        while (pos != string::npos)
        {
            found = true;
            line.replace(pos, searchWord.length(), replaceWord);
            pos = line.find(searchWord, pos + replaceWord.length());
        }

        fileContents.push_back(line);
    }

    file.close();

    if (found)
    {
        ofstream outFile("dictionary.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        cout << "Word replaced successfully." << endl;
    }
    else
    {
        cout << "Word not found." << endl;
    }
}

void copyText()
{
    int startX, startY, endX, endY;
    cout << "Enter starting position (X, Y): ";
    cin >> startX >> startY;
    cout << "Enter ending position (X, Y): ";
    cin >> endX >> endY;

    ifstream file("test.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    string copiedString;
    int lineNumber = 0;
    bool copyInProgress = false;

    while (getline(file, line))
    {
        lineNumber++;

        if (lineNumber >= startY && lineNumber <= endY)
        {
            if (lineNumber == startY)
            {
                // Copy from the starting position
                int startPos = startX - 1;
                int endPos = (lineNumber == endY) ? (endX - 1) : line.size();
                copiedString += line.substr(startPos, endPos - startPos + 1);
            }
            else if (lineNumber == endY)
            {
                // Copy until the ending position
                int endPos = endX - 1;
                copiedString += line.substr(0, endPos + 1);
            }
            else
            {
                // Copy the whole line
                copiedString += line;
            }

            copyInProgress = true;
        }
        else if (copyInProgress)
        {
            // Copying has been completed
            break;
        }
    }

    file.close();
    copiedText = copiedString;

    cout << "Text copied successfully." << endl;
}

void pasteText()
{
    int pasteX, pasteY;
    cout << "Enter paste position (X, Y): ";
    cin >> pasteX >> pasteY;

    ifstream file("test.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    int lineNumber = 0;
    bool pasteSuccessful = true;

    while (getline(file, line))
    {
        lineNumber++;

        if (lineNumber == pasteY)
        {
            int pastePos = pasteX - 1;
            if (pastePos >= 0 && pastePos <= line.size())
            {
                if (line[pastePos] != ' ')
                {
                    // Text already found at the paste position
                    char choice;
                    cout << "Text already found. Do you still want to paste the text? (y/n): ";
                    cin >> choice;
                    if (choice != 'y' && choice != 'Y')
                    {
                        pasteSuccessful = false;
                        break;
                    }
                }
                line.replace(pastePos, copiedText.size(), copiedText);
            }
        }

        fileContents.push_back(line);
    }

    file.close();

    if (pasteSuccessful)
    {
        ofstream outFile("test.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        cout << "Text pasted successfully." << endl;
    }
}

void deleteText()
{
    string deleteString;
    cout << "Enter the string to delete: ";
    cin.ignore();
    getline(cin, deleteString);

    ifstream file("test.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    bool deleteSuccessful = false;

    while (getline(file, line))
    {
        if (line.find(deleteString) != string::npos)
        {
            // Match found, delete the line
            deleteSuccessful = true;
        }
        else
        {
            fileContents.push_back(line);
        }
    }

    file.close();

    if (deleteSuccessful)
    {
        ofstream outFile("test.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        cout << "Text deleted successfully." << endl;
    }
    else
    {
        cout << "No matching text found." << endl;
    }
}

void searchWords()
{
    string searchString;
    cout << "Enter the string to search: ";
    cin.ignore();
    getline(cin, searchString);

    ifstream file("test.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    bool found = false;

    while (getline(file, line))
    {
        lineNumber++;

        size_t pos = line.find(searchString);
        while (pos != string::npos)
        {
            found = true;
            cout << "String found at position (" << pos + 1 << ", " << lineNumber << ")" << endl;
            pos = line.find(searchString, pos + 1);
        }
    }

    file.close();

    if (!found)
    {
        cout << "String not found." << endl;
    }
}

void replaceWord()
{
    string searchWord, replaceWord;
    cout << "Enter the word to search: ";
    cin >> searchWord;
    cout << "Enter the word to replace with: ";
    cin >> replaceWord;

    ifstream file("test.txt");
    if (!file)
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    string line;
    vector<string> fileContents;
    bool found = false;

    while (getline(file, line))
    {
        size_t pos = line.find(searchWord);
        while (pos != string::npos)
        {
            found = true;
            line.replace(pos, searchWord.length(), replaceWord);
            pos = line.find(searchWord, pos + replaceWord.length());
        }

        fileContents.push_back(line);
    }

    file.close();

    if (found)
    {
        ofstream outFile("test.txt");
        if (!outFile)
        {
            cerr << "Failed to open the file for writing." << endl;
            return;
        }

        for (const auto& line : fileContents)
        {
            outFile << line << endl;
        }

        outFile.close();
        cout << "Word replaced successfully." << endl;
    }
    else
    {
        cout << "Word not found." << endl;
    }
}

int main()
{

    fstream dictionary;
    dictionary.open("dictionary.txt", ios::in);
    int choice = -1;
    if (!dictionary) {
        cout << "File not found";
    }
    string s;
    // loading a dictionary words from file
    while (!dictionary.eof())
    {
        dictionary >> s;
        t.LoadData(s);
    }
    dictionary.close();




    system("Color B5");
    system("color F1"); // for console color
    // gotoxy(17, 5);
    cout << "--------------------------------------------------" << endl;
    // gotoxy(17, 7);
    cout << "||            Advance Notepad System                ||" << endl;
    // gotoxy(17, 9);
    cout << "--------------------------------------------------" << endl;
    // gotoxy(17, 11);
    cout << "|Subject Teacher ->> Sir Hannan Farooq <<-- |\n"
        << endl;
    // gotoxy(17, 13);
    cout << ">>>----Project Implemented By-----<<<" << endl;
    // gotoxy(22, 15);
    cout << "Saad Rehman (21F-9640), Ali Shaikh (21F-9631)" << endl
        << endl;
    // system("sleep");
    system("PAUSE");
    system("cls");
    system("color Fc");
    while (choice != 0)
    {

        cout << "1-Insert" << endl;
        cout << "2-Count words" << endl;
        cout << "3-Similarity checker" << endl;
        cout << "4-Undo" << endl;
        cout << "5-Redo" << endl;
        cout << "6-Copy Text" << endl;
        cout << "7-Paste Text" << endl;
        cout << "8-Delete Text" << endl;
        cout << "9-Search Words" << endl;
        cout << "10-Find a Word and Replace It" << endl;
        cout << "11 Update suggestion.txt file: " << endl;

        cout << "0-Exit" << endl;
        cout << "Enter numbers from 1-10 to go to the menu functions, else press 0 to exit\n";
        cin >> choice;

        switch (choice)
        {
        case 1:
            insert1();
            break;

        case 2:
            count();
            break;

        case 3:
            similarity();
            break;

        case 4:
            undo();
            break;

        case 5:
            redo();
            break;

        case 6:
            copyText();
            break;

        case 7:
            pasteText();
            break;

        case 8:
            deleteText();
            break;

        case 9:
            searchWords();
            break;

        case 10:
            replaceWord();
            break;
        case 11:
            updatesugesstions();
            break;
        default:
            break;
        }

        system("pause");
        system("cls");
    }

    return 0;
}