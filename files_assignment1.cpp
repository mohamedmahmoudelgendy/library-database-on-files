#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct PI_Auther_ID_Header
{
    int no_of_records;

    PI_Auther_ID_Header()
    {
    }

    PI_Auther_ID_Header(int no)
    {
        no_of_records = no;
    }
};

struct PI_Auther_ID
{
    char id[15];
    int index;

    PI_Auther_ID()
    {
    }

    PI_Auther_ID(char id[], int index)
    {
        for (int i = 0; i < 15; i++)
        {
            this->id[i] = id[i];
        }
        this->index = index;
    }
};

struct PI_ISBN_Header
{
    int no_of_records;

    PI_ISBN_Header()
    {
    }

    PI_ISBN_Header(int no)
    {
        no_of_records = no;
    }
};

struct PI_ISBN
{
    char isbn[15];
    int index;

    PI_ISBN()
    {
    }

    PI_ISBN(char isbn[], int index)
    {
        for (int i = 0; i < 15; i++)
        {
            this->isbn[i] = isbn[i];
        }
        
        this->index = index;
    }

    bool operator==(PI_ISBN pi)
    {
        if ((string)this->isbn == (string)pi.isbn && this->index == pi.index)
        {
            return true;
        }
        return false;
    }

    bool operator<(PI_ISBN pi)
    {
        if ((string)this->isbn < (string)pi.isbn)
        {
            return true;
        }
        return false;
    }

    bool operator>(PI_ISBN pi)
    {
        if ((string)this->isbn > (string)pi.isbn)
        {
            return true;
        }
        return false;
    }
};

struct SI_Auther_ID_Header
{
    int no_of_records;

    SI_Auther_ID_Header()
    {
    }

    SI_Auther_ID_Header(int no)
    {
        no_of_records = no;
    }
};

struct SI_Auther_ID
{
    char id[15];
    int index;

    SI_Auther_ID()
    {
    }

    SI_Auther_ID(char id[], int index)
    {
        for (int i = 0; i < 15; i++)
        {
            this->id[i] = id[i];
        }
        this->index = index;
    }
};

struct SI_Auther_Name
{
};

struct AutherHeader
{
    int no_of_records;

    AutherHeader()
    {
    }

    AutherHeader(int no)
    {
        no_of_records = no;
    }
};

struct Author
{
    long length;
    char id[15];
    char name[30];
    char address[30];

    Author()
    {
        length = sizeof(*this);
    }

    Author(char id[], char name[], char address[])
    {
        for (int i = 0; i < 15; i++)
        {
            this->id[i] = id[i];
        }

        for (int i = 0; i < 30; i++)
        {
            this->name[i] = name[i];
        }

        for (int i = 0; i < 30; i++)
        {
            this->address[i] = address[i];
        }
    }
};

struct bookHeader
{
    int no_of_records;
    int RNN;

    bookHeader()
    {
        this->no_of_records = 0;
        this->RNN = -1;
    }

    bookHeader(int no, int rnn)
    {
        no_of_records = no;
        RNN = rnn;
    }
};

struct Book
{
    long length;
    char isbn[15];
    char title[30];
    char author_id[15];

    Book()
    {
        length = sizeof(*this);
    }

    Book(char isbn[], char title[], char author_id[])
    {

        for (int i = 0; i < 15; i++)
        {
            this->isbn[i] = isbn[i];
        }

        for (int i = 0; i < 30; i++)
        {
            this->title[i] = title[i];
        }

        for (int i = 0; i < 15; i++)
        {
            this->author_id[i] = author_id[i];
        }
    }

    bool operator==(Book b)
    {
        if ((string)this->isbn == (string)b.isbn && (string)this->title == (string)b.title && (string)this->author_id == (string)b.author_id)
        {
            return true;
        }
        return false;
    }

    bool operator<(Book b)
    {
        if ((string)this->isbn < (string)b.isbn)
        {
            return true;
        }
        return false;
    }

    bool operator>(Book b)
    {
        if ((string)this->isbn > (string)b.isbn)
        {
            return true;
        }
        return false;
    }
};

list<Book> bookList;
list<Author> authorList;
list<PI_Auther_ID> PI_Author_ID_List;
list<PI_ISBN> PI_ISBN_List;
list<SI_Auther_ID> SI_Author_ID_List;
list<SI_Auther_Name> SI_Author_name_List;

fstream books_file, authors_file, pi_author_id_file, pi_isbn_file, si_author_id_file, si_author_name_file;

template <class T1, class T2>
void fillList(fstream &file, list<T1> &List, T2& h)
{
    file.seekg(0, ios::beg);
    file.read((char *)&h, sizeof(h));
    for (int i = 0; i < h.no_of_records; i++)
    {
        T1 b;
        file.seekg(sizeof(h) + (i * sizeof(b)), ios::beg);
        file.read((char *)&b, sizeof(b));
        List.push_back(b);
    }
}

template <class T1, class T2>
void writeFile(fstream &file, list<T1> &List, T2 &h)
{
    file.seekg(0, ios::beg);
    file.read((char *)&h, sizeof(h));
    int counter = 0;
    for (auto i : List)
    {
        file.seekp(sizeof(h) + (counter * sizeof(i)), ios::beg);
        file.write((char *)&i, sizeof(i));
        counter++;
    }
}

template <class T>
bool isRecordExist(list<T> list, T record)
{
    for (T i : list)
    {
        if (i == record)
        {
            return true;
        }
    }
    return false;
}

template <class T>
void getHeader(fstream &file, T &header)
{
    file.seekg(0, ios::beg);
    file.read((char *)&header, sizeof(header));
    file.seekg(0, ios::beg);
}

template <class T>
void addHeader(fstream &file, T header)
{
    file.seekp(0, ios::beg);
    file.write((char *)&header, sizeof(header));
    file.seekp(0, ios::end);
}

bool isISBNExist(list<PI_ISBN> list, string isbn)
{
    for (PI_ISBN i : list)
    {
        if ((string)i.isbn == isbn)
        {
            return true;
        }
    }
    return false;
}

void addBook()
{
    bookHeader bh;
    PI_ISBN_Header pi_isbn_h;
    fillList(books_file, bookList, bh);
    fillList(pi_isbn_file, PI_ISBN_List, pi_isbn_h);
    char isbn[15], title[30], auther_id[15];
    cout << "enter book's isbn: ";
    cin.getline(isbn,15);
    cout << "enter book's title: ";
    cin.getline(title,30);
    cout << "enter book's auther id: ";
    cin.getline(auther_id,15);
    Book b(isbn, title,auther_id);
    if (isRecordExist(bookList, b))
    {
        return;
    }
    if (isISBNExist(PI_ISBN_List, isbn))
    {
        cout << "Error ISBN is aready exist" << endl;
        return;
    }
    PI_ISBN pi_isbn(isbn, bh.no_of_records);
    bh.no_of_records++;
    pi_isbn_h.no_of_records++;
    bookList.push_back(b);
    PI_ISBN_List.push_back(pi_isbn);
    PI_ISBN_List.sort();
    addHeader(books_file, bh);
    addHeader(pi_isbn_file, pi_isbn_h);
    writeFile(books_file, bookList, bh);
    writeFile(pi_isbn_file, PI_ISBN_List, pi_isbn_h);
    bookList.clear();
    PI_ISBN_List.clear();
}

void add(fstream &file, Author auther)
{
    file.write((char *)&auther, sizeof(auther));
}

void fillList(fstream &file, list<Book> &List)
{
    bookHeader h ;
    file.seekg(0, ios::beg);
    file.read((char *)&h, sizeof(h));
    for (int i = 0; i < h.no_of_records; i++)
    {
        Book b;
        file.seekg(sizeof(h) + (i * sizeof(b)), ios::beg);
        file.read((char *)&b, sizeof(b));
        List.push_back(b);
    }
}

int main()
{

    books_file.open("books.txt", ios::in | ios::out | ios::binary);
    authors_file.open("authers.txt", ios::in | ios::out | ios::binary);
    pi_author_id_file.open("PI_Auther_ID.txt", ios::in | ios::out | ios::binary);
    pi_isbn_file.open("PI_ISBN.txt", ios::in | ios::out | ios::binary);
    si_author_id_file.open("SI_Auther_ID.txt", ios::in | ios::out | ios::binary);
    si_author_name_file.open("SI_Auther_Name.txt", ios::in | ios::out | ios::binary);

    bookHeader h(0, -1);
    PI_ISBN_Header pi_isbn_h(0);
    addHeader(books_file, h);
    addHeader(pi_isbn_file, pi_isbn_h);
    PI_ISBN pi((char *)"1234",1);

    pi_isbn_file.seekp(sizeof(pi_isbn_h),ios::beg);
    pi_isbn_file.write((char *)&pi,sizeof(pi));

    addBook();
    
    fillList(books_file,bookList,h);
    fillList(pi_isbn_file,PI_ISBN_List,pi_isbn_h);

    for(Book i : bookList){
        cout << i.isbn << " " << i.title << " " << i.author_id << endl  ;
    }

    for(PI_ISBN i : PI_ISBN_List){
        cout << i.isbn << " " << i.index << endl  ;
    }

    bookList.clear();
    PI_ISBN_List.clear();

    addBook();

    fillList(books_file,bookList,h);
    fillList(pi_isbn_file,PI_ISBN_List,pi_isbn_h);

    for(Book i : bookList){
        cout << i.isbn << " " << i.title << " " << i.author_id << endl  ;
    }

    for(PI_ISBN i : PI_ISBN_List){
        cout << i.isbn << " " << i.index << endl  ;
    }

    bookList.clear();
    PI_ISBN_List.clear();


}