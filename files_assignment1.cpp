#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

vector<string> PL;
vector<string> SL;
vector<string> SLL;

string getHeader(string fileName)
{
    fstream file;
    file.open(fileName += ".tXt", ios::in);
    char record[12];
    file.seekg(0, ios::beg);
    file.getline(record, 11);
    file.close();
    return record;
}

void putHeader(string fileName, string header)
{
    fstream file;
    file.open(fileName += ".tXt", ios::in | ios::out);
    file.seekp(0, ios::beg);
    int len = header.length();
    if (len < 10)
    {
        for (int i = 0; i < 10 - len; i++)
        {
            header += "$";
        }
    }
    file.write(header.substr(0, 10).c_str(), header.substr(0, 10).length());
    file.close();
}

int getNumOfRecord(string header)
{
    return stoi(header.substr(0, header.find('|')));
}

long getPyteOffset(string header)
{
    int firstDelimiterPosition = header.find('|');
    string subHeader = header.substr(firstDelimiterPosition + 1);
    return stol(subHeader.substr(0, subHeader.find('|')));
}

pair<string, string> getDeletedRecord(string fileName, long offset)
{
    fstream file;
    file.open(fileName + ".txt", ios::in);
    char delOffset[6], delSize[6];
    file.seekg(offset + 1, ios::beg);
    file.getline(delOffset, 6, '|');
    file.getline(delSize, 6, '|');
    file.close();
    pair<string, string> re;
    re.first = delOffset;
    re.second = delSize;
    return re;
}

pair<long, long> writeRecord(string fileName, string record, long offset = -1)
{
    fstream file;
    file.open(fileName + ".txt", ios::in | ios::out);
    pair<long, long> re;
    if (offset == -1)
    {
        file.seekp(0, ios::end);
        re.first = file.tellp();
    }
    else
    {
        file.seekp(offset, ios::beg);
        re.first = offset;
    }
    file.write(record.c_str(), record.length());
    re.second = file.tellp();
    file.close();
    return re;
}

string makeRecord(string str1, string str2, string str3, int len)
{

    str1 = str1.substr(0, 15);
    str2 = str2.substr(0, 30);
    str3 = str3.substr(0, len);

    string combinedString = "", l = "", re = "";
    combinedString += str1;
    combinedString += "|";
    combinedString += str2;
    combinedString += "|";
    combinedString += str3;
    combinedString += "|";

    int length = combinedString.length() + 1;
    l = to_string(length) += "|";
    re += l;
    re += combinedString;

    return re;
}

string makeIndexRecord(string str1, string str2, int len)
{

    str1 = str1.substr(0, len);
    str2 = str2.substr(0, 15);

    string combinedString = "'";
    combinedString += str1;
    combinedString += "' ";
    combinedString += str2;
    return combinedString;
}

void fillIndexList(string fileName, vector<string> &list)
{
    fstream file;
    file.open(fileName + ".txt", ios::in);
    file.seekg(0, ios::beg);
    while (!file.eof())
    {
        char record[50];
        file.getline(record, 50, '|');
        list.push_back(record);
    }
    list.pop_back();
    file.close();
}

void writeIndexFile(string fileName, vector<string> list)
{
    fstream file;
    file.open(fileName + ".txt", ios::out);
    file.seekp(0, ios::beg);
    for (string i : list)
    {
        file.write((i + "|").c_str(), i.length() + 1);
    }
    file.close();
}

vector<std::string> splitStringWithConsecutiveSpaces(string input)
{
    vector<std::string> result;
    regex re("'([^'\\\\]*(?:\\\\.[^'\\\\]*)*)?'|(?:\\S+)");
    sregex_iterator it(input.begin(), input.end(), re);
    sregex_iterator end;

    while (it != end)
    {
        std::string token = it->str();
        if (token.front() == '\'' && token.back() == '\'')
        {
            token = token.substr(1, token.length() - 2);
        }
        result.push_back(token);
        ++it;
    }

    return result;
}

string getIndexKey(string record)
{
    vector<string> re = splitStringWithConsecutiveSpaces(record);
    return re[0];
}

string getIndexPtr(string record)
{
    vector<string> re = splitStringWithConsecutiveSpaces(record);
    return re[1];
}

string toLower(string str)
{
    string lowercaseStr = str;
    for (char &c : lowercaseStr)
    {
        c = tolower(c);
    }
    return lowercaseStr;
}

string binarySearchVector(vector<string> myvector, string target)
{
    if (myvector.empty())
    {
        return "NOT FOUND";
    }

    vector<string>::iterator low = myvector.begin();
    vector<string>::iterator high = myvector.end();

    while (low != high)
    {
        vector<string>::iterator mid = low;
        advance(mid, distance(low, high) / 2);
        if (toLower(getIndexKey(*mid)) == toLower(target))
        {
            return getIndexPtr(*mid);
        }
        else if (toLower(getIndexKey(*mid)) < toLower(target))
        {
            low = mid;
            low++;
        }
        else
        {
            high = mid;
        }
    }

    return "NOT FOUND";
}

int binarySearchVectorIndex(vector<string> myvector, string target)
{
    if (myvector.empty())
    {
        return -1;
    }

    vector<string>::iterator low = myvector.begin();
    vector<string>::iterator high = myvector.end();

    while (low != high)
    {
        vector<string>::iterator mid = low;
        advance(mid, distance(low, high) / 2);
        if (toLower(getIndexKey(*mid)) == toLower(target))
        {
            return distance(myvector.begin(), mid);
        }
        else if (toLower(getIndexKey(*mid)) < toLower(target))
        {
            low = mid;
            low++;
        }
        else
        {
            high = mid;
        }
    }

    return -1;
}

string GetRecord(string filename, long offset)
{
    fstream file;
    file.open(filename + ".txt", ios::in);
    file.seekg(offset, ios::beg);
    char size[10];
    file.getline(size, 10, '|');
    string re = size;
    char *record = new char[stoi(re)];
    file.getline(record, stoi(re) - 1);
    re = record;
    record = nullptr;
    delete[] record;
    return re;
}

vector<string> splitString(string input, char delimiter)
{
    vector<string> result;
    size_t start = 0;
    size_t end;

    while ((end = input.find(delimiter, start)) != string::npos)
    {
        if (end > start)
        {
            result.push_back(input.substr(start, end - start));
        }
        start = end + 1;
        while (input[start] == delimiter)
        {
            start++;
        }
    }
    if (end > start)
    {
        result.push_back(input.substr(start, end - start));
    }
    return result;
}

void checkHeaderExist(string fileName)
{
    fstream file;
    file.open(fileName + ".txt", ios::in | ios::out);
    file.seekp(0, ios::end);
    if (file.tellp() == 0)
    {
        string firstHeader = "0|-1|$$$$$";
        file.write(firstHeader.c_str(), firstHeader.length());
    }
}

bool findStringInvector(vector<string> v, string key)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (key == v[i])
        {
            return true;
        }
    }
    return false;
}

void shiftS(vector<string> &SLL, int index)
{
    for (int i = 0; i < SLL.size(); i++)
    {
        if (getIndexPtr(SLL[i]) != "-1" && stoi(getIndexPtr(SLL[i])) >= index)
        {
            SLL[i] = "'" + getIndexKey(SLL[i]) + "' " + to_string(stoi(getIndexPtr(SLL[i])) - 1);
        }
    }
}

void add(string FN, string FNP, string FNS, string FNSL, vector<string> &PL, vector<string> &SL, vector<string> &SLL, string inputP, string input2, string inputS, int lenR, int lenIN, bool type)
{
    string newRecord;

    PL.clear();
    fillIndexList(FNP, PL);

    if (binarySearchVector(PL, inputP) != "NOT FOUND")
    {
        cout << "this isbn is aready exist\n";
        return;
    }

    if (type)
    {
        newRecord = makeRecord(inputP, input2, inputS, lenR);
    }
    else
    {
        newRecord = makeRecord(inputP, inputS, input2, lenR);
    }

    long head = getPyteOffset(getHeader(FN));
    int numOfRecord = getNumOfRecord(getHeader(FN));

    PL.clear();
    SL.clear();
    SLL.clear();

    string PiRecord, newHeader;
    numOfRecord++;

    if ((head != -1 && stoi(getDeletedRecord(FN, head).second) < newRecord.length()) || (head == -1))
    {
        if (head == -1)
        {
            newHeader = to_string(numOfRecord) + "|-1|";
        }
        else
        {
            newHeader = to_string(numOfRecord) + "|" + to_string(head) + "|";
        }

        putHeader(FN, newHeader);
        long recordPyteOffset = writeRecord(FN, newRecord).first;

        PiRecord = makeIndexRecord(inputP, to_string(recordPyteOffset), 15);
        fillIndexList(FNP, PL);
        PL.push_back(PiRecord);
        sort(PL.begin(), PL.end());
        writeIndexFile(FNP, PL);
    }
    else
    {

        int remainingSize = stoi(getDeletedRecord(FN, head).second) - newRecord.length();

        newHeader = to_string(numOfRecord) + "|" + getDeletedRecord(FN, head).first + "|";
        long newRemainingRecordOffset = writeRecord(FN, newRecord, head).second;
        putHeader(FN, newHeader);

        if (remainingSize >= 8)
        {
            long newHead = getPyteOffset(getHeader(FN)), headBefore = 0;

            while (remainingSize < stoi(getDeletedRecord(FN, newHead).second) && stoi(getDeletedRecord(FN, newHead).first) != -1)
            {
                headBefore = newHead;
                newHead = stol(getDeletedRecord(FN, newHead).first);
            }

            if (headBefore == 0 && remainingSize >= stoi(getDeletedRecord(FN, newHead).second))
            {
                newHeader = to_string(numOfRecord) + "|" + to_string(newRemainingRecordOffset) + "|";
                putHeader(FN, newHeader);
                writeRecord(FN, "*" + to_string(newHead) + "|" + to_string(remainingSize) + "|", newRemainingRecordOffset);
            }
            else if (headBefore == 0 && remainingSize < stoi(getDeletedRecord(FN, newHead).second))
            {
                writeRecord(FN, "*" + to_string(newRemainingRecordOffset) + "|" + getDeletedRecord(FN, newHead).second + "|", newHead);
                writeRecord(FN, "*-1|" + to_string(remainingSize) + "|", newRemainingRecordOffset);
            }
            else if (headBefore != 0 && remainingSize < stoi(getDeletedRecord(FN, newHead).second) && stoi(getDeletedRecord(FN, newHead).first) == -1)
            {
                writeRecord(FN, "*" + to_string(newRemainingRecordOffset) + "|" + getDeletedRecord(FN, newHead).second + "|", newHead);
                writeRecord(FN, "*-1|" + to_string(remainingSize) + "|", newRemainingRecordOffset);
            }
            else
            {
                writeRecord(FN, "*" + to_string(newRemainingRecordOffset) + "|" + getDeletedRecord(FN, headBefore).second + "|", headBefore);
                writeRecord(FN, "*" + to_string(newHead) + "|" + to_string(remainingSize) + "|", newRemainingRecordOffset);
            }
        }

        PiRecord = makeIndexRecord(inputP, to_string(head), 15);
        fillIndexList(FNP, PL);
        PL.push_back(PiRecord);
        sort(PL.begin(), PL.end());
        writeIndexFile(FNP, PL);
    }

    string SiRecord, SiRecordForLL, ptr;

    fillIndexList(FNS, SL);
    fillIndexList(FNSL, SLL);

    ptr = binarySearchVector(SL, inputS);

    if (ptr == "NOT FOUND")
    {
        SiRecord = makeIndexRecord(inputS, to_string(SLL.size()), lenIN);
        SiRecordForLL = makeIndexRecord(inputP, "-1", 15);
        SL.push_back(SiRecord);
        SLL.push_back(SiRecordForLL);
        sort(SL.begin(), SL.end());
        writeIndexFile(FNS, SL);
        writeIndexFile(FNSL, SLL);
    }
    else
    {
        vector<string> vSLL = splitStringWithConsecutiveSpaces(SLL[stoi(ptr)]);
        int index = stoi(ptr);
        while (vSLL[1] != "-1")
        {
            index = stoi(vSLL[1]);
            vSLL = splitStringWithConsecutiveSpaces(SLL[stoi(vSLL[1])]);
        }
        SLL[index] = makeIndexRecord(getIndexKey(SLL[index]), to_string(SLL.size()), 15);
        SiRecordForLL = makeIndexRecord(inputP, "-1", 15);
        SLL.push_back(SiRecordForLL);
        writeIndexFile(FNSL, SLL);
    }

    cout << "Added Successfully\n";
}

void Delete(string FN, string FNP, string FNS, string FNSL, vector<string> &PL, vector<string> &SL, vector<string> &SLL, bool type, string key)
{
    PL.clear();
    SL.clear();
    SLL.clear();

    fillIndexList(FNP, PL);

    string binarySearchResult = binarySearchVector(PL, key);
    if (binarySearchResult == "NOT FOUND")
    {
        cout << "Error record dose not exist\n";
        return;
    }

    long delOffset = stol(binarySearchResult);
    vector<string> recordToDelete = splitString(GetRecord(FN, delOffset), '|');

    int delRecordLength = GetRecord(FN, delOffset).length() + 2;
    delRecordLength += to_string(delRecordLength).length();
    int numOfRecord = getNumOfRecord(getHeader(FN));
    numOfRecord--;

    long Head = getPyteOffset(getHeader(FN)), headBefore = 0;
    string newHeader = to_string(numOfRecord) + "|" + to_string(Head) + "|";
    putHeader(FN, newHeader);

    if (Head == -1)
    {
        newHeader = to_string(numOfRecord) + "|" + to_string(delOffset) + "|";
        putHeader(FN, newHeader);
        writeRecord(FN, "*-1|" + to_string(delRecordLength) + "|", delOffset);
    }
    else
    {
        while (delRecordLength < stoi(getDeletedRecord(FN, Head).second) && stoi(getDeletedRecord(FN, Head).first) != -1)
        {
            headBefore = Head;
            Head = stol(getDeletedRecord(FN, Head).first);
        }

        if (headBefore == 0 && delRecordLength >= stoi(getDeletedRecord(FN, Head).second))
        {
            newHeader = to_string(numOfRecord) + "|" + to_string(delOffset) + "|";
            putHeader(FN, newHeader);
            writeRecord(FN, "*" + to_string(Head) + "|" + to_string(delRecordLength) + "|", delOffset);
        }
        else if (headBefore == 0 && delRecordLength < stoi(getDeletedRecord(FN, Head).second))
        {
            writeRecord(FN, "*" + to_string(delOffset) + "|" + getDeletedRecord(FN, Head).second + "|", Head);
            writeRecord(FN, "*-1|" + to_string(delRecordLength) + "|", delOffset);
        }
        else if (headBefore != 0 && delRecordLength < stoi(getDeletedRecord(FN, Head).second) && stoi(getDeletedRecord(FN, Head).first) == -1)
        {
            writeRecord(FN, "*" + to_string(delOffset) + "|" + getDeletedRecord(FN, Head).second + "|", Head);
            writeRecord(FN, "*-1|" + to_string(delRecordLength) + "|", delOffset);
        }
        else
        {
            writeRecord(FN, "*" + to_string(delOffset) + "|" + getDeletedRecord(FN, headBefore).second + "|", headBefore);
            writeRecord(FN, "*" + to_string(Head) + "|" + to_string(delRecordLength) + "|", delOffset);
        }
    }

    int PindexToDelete = binarySearchVectorIndex(PL, key);
    PL.erase(PL.begin() + PindexToDelete);
    writeIndexFile(FNP, PL);

    fillIndexList(FNS, SL);
    string startLinked, nextLinked, BeforeLinked = "-1";

    if (type)
    {
        startLinked = binarySearchVector(SL, recordToDelete[2]);
    }
    else
    {
        startLinked = binarySearchVector(SL, recordToDelete[1]);
    }

    fillIndexList(FNSL, SLL);
    vector<string> vSLLr = splitStringWithConsecutiveSpaces(SLL[stoi(startLinked)]);
    nextLinked = vSLLr[1];
    while (nextLinked != "-1" && vSLLr[0] != key)
    {
        BeforeLinked = startLinked;
        startLinked = nextLinked;
        vSLLr = splitStringWithConsecutiveSpaces(SLL[stoi(startLinked)]);
        nextLinked = vSLLr[1];
    }

    if (nextLinked == "-1")
    {
        if (BeforeLinked == "-1")
        {
            int SindexToDelete;
            if (type)
            {
                SindexToDelete = binarySearchVectorIndex(SL, recordToDelete[2]);
            }
            else
            {
                SindexToDelete = binarySearchVectorIndex(SL, recordToDelete[1]);
            }
            SL.erase(SL.begin() + SindexToDelete);
        }
        else if (BeforeLinked != "-1")
        {
            SLL[stoi(BeforeLinked)] = "'" + getIndexKey(SLL[stoi(BeforeLinked)]) + "' -1";
        }
    }
    else if (nextLinked != "-1")
    {
        if (BeforeLinked == "-1")
        {
            int SindexToEdit;
            if (type)
            {
                SindexToEdit = binarySearchVectorIndex(SL, recordToDelete[2]);
            }
            else
            {
                SindexToEdit = binarySearchVectorIndex(SL, recordToDelete[1]);
            }
            SL[SindexToEdit] = "'" + getIndexKey(SL[SindexToEdit]) + "' " + nextLinked;
        }
        else if (BeforeLinked != "-1")
        {
            SLL[stoi(BeforeLinked)] = "'" + getIndexKey(SLL[stoi(BeforeLinked)]) + "' " + nextLinked;
        }
    }

    SLL.erase(SLL.begin() + stoi(startLinked));
    shiftS(SL, stoi(startLinked));
    shiftS(SLL, stoi(startLinked));
    writeIndexFile(FNS, SL);
    writeIndexFile(FNSL, SLL);

    cout << "Deleted Successfully\n";
}

void Update(string FN, string FNP, string FNS, string FNSL, vector<string> &PL, vector<string> &SL, vector<string> &SLL, bool type, string key)
{
    PL.clear();
    fillIndexList(FNP, PL);
    string binarySearchResult = binarySearchVector(PL, key);
    if (binarySearchResult == "NOT FOUND")
    {
        cout << "Error record dose not exist\n";
        return;
    }
    vector<string> record = splitString(GetRecord(FN, stoi(binarySearchResult)), '|');
    string P, S, Non;
    if (type)
    {
        P = record[0];
        S = record[2];
        cout << "Enter new Title: ";
        getline(cin, Non);
        int oldTitleSize = record[1].length(), newTitleSize = Non.length();
        if (newTitleSize <= oldTitleSize)
        {
            for (int i = 0; i < oldTitleSize - newTitleSize; i++)
            {
                Non += " ";
            }
            string newRecord = makeRecord(P, Non, S, 15);
            writeRecord("books", newRecord, stoi(binarySearchResult));
        }
        else
        {
            Delete(FN, FNP, FNS, FNSL, PL, SL, SLL, type, P);
            add(FN, FNP, FNS, FNSL, PL, SL, SLL, P, Non, S, 15, 15, type);
        }
    }
    else
    {
        P = record[0];
        Non = record[2];
        cout << "Enter new Author name: ";
        getline(cin, S);
        Delete(FN, FNP, FNS, FNSL, PL, SL, SLL, type, P);
        add(FN, FNP, FNS, FNSL, PL, SL, SLL, P, Non, S, 30, 30, type);
    }

    cout << "Updated Successfully\n";
}

void print(string filename, string indexfilename, vector<string> &mylist, bool type, string key)
{

    mylist.clear();
    fillIndexList(indexfilename, mylist);
    string binarySearchResult = binarySearchVector(mylist, key);
    if (binarySearchResult == "NOT FOUND")
    {
        cout << "NOT FOUND" << endl;
    }
    else
    {
        vector<string> Splitted_Record = splitString(GetRecord(filename, stol(binarySearchResult)), '|');
        if (type == true)
        {
            cout << "Book's ISBN: " + Splitted_Record[0] << endl;
            cout << "Book's Title: " + Splitted_Record[1] << endl;
            cout << "Author ID: " + Splitted_Record[2] << endl;
        }
        else
        {
            cout << "Author's ID: " + Splitted_Record[0] << endl;
            cout << "Author's Name: " + Splitted_Record[1] << endl;
            cout << "Author's Address: " + Splitted_Record[2] << endl;
        }
    }
}

void queryProcessing(string query)
{
    query = toLower(query);

    vector<string> vQuery = splitStringWithConsecutiveSpaces(query), afterSelectWords = {"author id", "author name", "address", "isbn", "title", "all"},
                   afterFromWords = {"books", "authors"}, afterWhereWords = {"author id", "author name", "isbn"}, authorWords = {"author id", "author name", "address", "all"},
                   bookWords = {"author id", "isbn", "title", "all"};
    string afterSelect, afterFrom, afterWhere, afterEqual;

    if (vQuery[2] == "from" && (vQuery.size() == 9 || vQuery.size() == 10))
    {
        if (vQuery[0] != "select" || vQuery[2] != "from" || vQuery[4] != "where" || (vQuery[6] != "=" && vQuery[7] != "="))
        {
            cout << "Error Incorrect Syntax\n";
            return;
        }

        afterSelect = vQuery[1];
        afterFrom = vQuery[3];

        if (vQuery.size() == 9)
        {
            afterWhere = vQuery[5];
            afterEqual = vQuery[7];
        }
        else
        {
            afterWhere = vQuery[5] + " " + vQuery[6];
            afterEqual = vQuery[8];
        }
    }
    else if (vQuery[3] == "from" && (vQuery.size() == 10 || vQuery.size() == 11))
    {
        if (vQuery[0] != "select" || vQuery[3] != "from" || vQuery[5] != "where" || (vQuery[7] != "=" && vQuery[8] != "="))
        {
            cout << "Error Incorrect Syntax\n";
            return;
        }

        afterSelect = vQuery[1] + " " + vQuery[2];
        afterFrom = vQuery[4];

        if (vQuery.size() == 10)
        {
            afterWhere = vQuery[6];
            afterEqual = vQuery[8];
        }
        else
        {
            afterWhere = vQuery[6] + " " + vQuery[7];
            afterEqual = vQuery[9];
        }
    }
    else
    {
        cout << "Error Incorrect Syntax\n";
        return;
    }

    if (!findStringInvector(afterSelectWords, afterSelect))
    {
        cout << "Error on Select attribute\n";
        return;
    }

    if (!findStringInvector(afterFromWords, afterFrom))
    {
        cout << "Error on table name\n";
        return;
    }

    if (!findStringInvector(afterWhereWords, afterWhere))
    {
        cout << "Error on where attribute\n";
        return;
    }

    if (afterFrom == "books" && (!findStringInvector(bookWords, afterSelect) || (afterWhere != "isbn" && afterWhere != "author id")))
    {
        cout << "Error this attributes not in " + afterFrom + " table or there is no index on it\n";
        return;
    }

    if (afterFrom == "authors" && (!findStringInvector(authorWords, afterSelect) || (afterWhere != "author id" && afterWhere != "author name")))
    {
        cout << "Error this attributes not in " + afterFrom + " table or there is no index on it\n";
        return;
    }

    PL.clear();
    SL.clear();
    SLL.clear();

    if (afterFrom == "books")
    {
        if (afterWhere == "isbn")
        {
            if (afterSelect == "all")
            {
                print("books", "PI_ISBN", PL, true, afterEqual);
            }
            else
            {
                fillIndexList("PI_ISBN", PL);
                string binarySearchResult = binarySearchVector(PL, afterEqual);
                if (binarySearchResult == "NOT FOUND")
                {
                    cout << "NOT FOUND" << endl;
                }
                else
                {
                    vector<string> Splitted_Record = splitString(GetRecord(afterFrom, stol(binarySearchResult)), '|');
                    if (afterSelect == "title")
                    {
                        cout << "Book's Title: " + Splitted_Record[1] + "\n";
                    }
                    else if (afterSelect == "author id")
                    {
                        cout << "Author's ID: " + Splitted_Record[2] + "\n";
                    }
                }
            }
        }
        else if (afterWhere == "author id")
        {
            fillIndexList("SI_Author_ID", SL);
            string binarySearchResult = binarySearchVector(SL, afterEqual);
            if (binarySearchResult == "NOT FOUND")
            {
                cout << "NOT FOUND" << endl;
            }
            else
            {
                fillIndexList("SI_Author_ID_LL", SLL);
                vector<string> vISBNs;
                vISBNs.push_back(getIndexKey(SLL[stoi(binarySearchResult)]));
                string index = getIndexPtr(SLL[stoi(binarySearchResult)]);
                while (index != "-1")
                {
                    vISBNs.push_back(getIndexKey(SLL[stoi(index)]));
                    index = getIndexPtr(SLL[stoi(index)]);
                }
                if (afterSelect == "all")
                {
                    for (int i = 0; i < vISBNs.size(); i++)
                    {
                        print(afterFrom, "PI_ISBN", PL, true, vISBNs[i]);
                    }
                }
                else if (afterSelect == "isbn")
                {
                    for (int i = 0; i < vISBNs.size(); i++)
                    {
                        cout << "Book's ISBN: " + vISBNs[i] + "\n";
                    }
                }
                else if (afterSelect == "title")
                {
                    fillIndexList("PI_ISBN", PL);
                    for (int i = 0; i < vISBNs.size(); i++)
                    {
                        vector<string> Splitted = splitString(GetRecord(afterFrom, stol(binarySearchVector(PL, vISBNs[i]))), '|');
                        cout << "Book's Title: " + Splitted[1] + "\n";
                    }
                }
            }
        }
    }
    else if (afterFrom == "authors")
    {
        if (afterWhere == "author id")
        {
            if (afterSelect == "all")
            {
                print("authors", "PI_Author_ID", PL, false, afterEqual);
            }
            else
            {
                fillIndexList("PI_Author_ID", PL);
                string binarySearchResult = binarySearchVector(PL, afterEqual);
                if (binarySearchResult == "NOT FOUND")
                {
                    cout << "NOT FOUND" << endl;
                }
                else
                {
                    vector<string> Splitted_Record = splitString(GetRecord(afterFrom, stol(binarySearchResult)), '|');
                    if (afterSelect == "author name")
                    {
                        cout << "Author's Name: " + Splitted_Record[1] + "\n";
                    }
                    else if (afterSelect == "address")
                    {
                        cout << "Author's Address: " + Splitted_Record[2] + "\n";
                    }
                }
            }
        }
        else if (afterWhere == "author name")
        {
            fillIndexList("SI_Author_Name", SL);
            string binarySearchResult = binarySearchVector(SL, afterEqual);
            if (binarySearchResult == "NOT FOUND")
            {
                cout << "NOT FOUND" << endl;
            }
            else
            {
                fillIndexList("SI_Author_Name_LL", SLL);
                vector<string> vNames;
                vNames.push_back(getIndexKey(SLL[stoi(binarySearchResult)]));
                string index = getIndexPtr(SLL[stoi(binarySearchResult)]);
                while (index != "-1")
                {
                    vNames.push_back(getIndexKey(SLL[stoi(index)]));
                    index = getIndexPtr(SLL[stoi(index)]);
                }
                if (afterSelect == "all")
                {
                    for (int i = 0; i < vNames.size(); i++)
                    {
                        print(afterFrom, "PI_Author_ID", PL, false, vNames[i]);
                    }
                }
                else if (afterSelect == "author id")
                {
                    for (int i = 0; i < vNames.size(); i++)
                    {
                        cout << "Author's ID: " + vNames[i] + "\n";
                    }
                }
                else if (afterSelect == "address")
                {
                    fillIndexList("PI_Author_ID", PL);
                    for (int i = 0; i < vNames.size(); i++)
                    {
                        vector<string> Splitted = splitString(GetRecord(afterFrom, stol(binarySearchVector(PL, vNames[i]))), '|');
                        cout << "Author's Address: " + Splitted[2] + "\n";
                    }
                }
            }
        }
    }
}

int main()
{
    checkHeaderExist("books");
    checkHeaderExist("authors");

    while (true)
    {
        string answer;
        cout << "1.Add New Author\n2.Add New Book\n3.Update Author Name (Author ID)\n4.Update Book Title (ISBN)\n5.Delete Book (ISBN)\n6.Delete Author (Author ID)\n7.Print Author (Author ID)\n8.Print Book (ISBN)\n9.Write Query\n10.Exit\nEnter your choice: ";
        getline(cin, answer);
        if (answer == "1")
        {
            string authorID, authorName, address;
            cout << "Enter Auther ID: ";
            getline(cin, authorID);
            cout << "Enter Auther's Name: ";
            getline(cin, authorName);
            cout << "Enter Auther's Address: ";
            getline(cin, address);
            add("authors", "PI_Author_ID", "SI_Author_Name", "SI_Author_Name_LL", PL, SL, SLL, authorID, address, authorName, 30, 30, false);
        }
        else if (answer == "2")
        {
            string isbn, title, authorID;
            cout << "Enter book's ISBN: ";
            getline(cin, isbn);
            cout << "Enter book's title: ";
            getline(cin, title);
            cout << "Enter Auther's ID: ";
            getline(cin, authorID);
            add("books", "PI_ISBN", "SI_Author_ID", "SI_Author_ID_LL", PL, SL, SLL, isbn, title, authorID, 15, 15, true);
        }
        else if (answer == "3")
        {
            string authorID;
            cout << "Enter Auther ID: ";
            getline(cin, authorID);
            Update("authors", "PI_Author_ID", "SI_Author_Name", "SI_Author_Name_LL", PL, SL, SLL, false, authorID);
        }
        else if (answer == "4")
        {
            string isbn;
            cout << "Enter book's ISBN: ";
            getline(cin, isbn);
            Update("books", "PI_ISBN", "SI_Author_ID", "SI_Author_ID_LL", PL, SL, SLL, true, isbn);
        }
        else if (answer == "5")
        {
            string isbn;
            cout << "Enter Book's ISBN: ";
            getline(cin, isbn);
            Delete("books", "PI_ISBN", "SI_Author_ID", "SI_Author_ID_LL", PL, SL, SLL, true, isbn);
        }
        else if (answer == "6")
        {
            string authorID;
            cout << "Enter Author's ID: ";
            getline(cin, authorID);
            Delete("authors", "PI_Author_ID", "SI_Author_Name", "SI_Author_Name_LL", PL, SL, SLL, false, authorID);
        }
        else if (answer == "7")
        {
            cout << "Enter Author's ID: ";
            string key;
            getline(cin, key);
            print("authors", "PI_Author_ID", PL, false, key);
        }
        else if (answer == "8")
        {
            cout << "Enter Book's ISBN: ";
            string key;
            getline(cin, key);
            print("books", "PI_ISBN", PL, true, key);
        }
        else if (answer == "9")
        {
            cout << "Note:please separate between word using space included '='\n"
                 << "Enter Your Query: ";
            string query;
            getline(cin, query);
            queryProcessing(query);
        }
        else if (answer == "10")
        {
            break;
        }
        else
        {
            cout << "wrong input\n";
        }
    }

    return 0;
}