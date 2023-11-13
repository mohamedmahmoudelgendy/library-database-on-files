#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct PI_Author_ID_Header
{
    int no_of_records;

    PI_Author_ID_Header()
    {
    }

    PI_Author_ID_Header(int no)
    {
        no_of_records = no;
    }
};

struct PI_Author_ID
{
    char id[15];
    int index;

    PI_Author_ID()
    {
    }

    PI_Author_ID(char id[], int index)
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

struct SI_Author_ID_Header
{
    int no_of_records;

    SI_Author_ID_Header()
    {
    }

    SI_Author_ID_Header(int no)
    {
        no_of_records = no;
    }
};

struct SI_Author_ID
{
    char author_id[15];
    int index;

    SI_Author_ID()
    {
    }

    SI_Author_ID(char auther_id[], int index)
    {
        for (int i = 0; i < 15; i++)
        {
            this->author_id[i] = auther_id[i];
        }
        this->index = index;
    }

    bool operator==(SI_Author_ID si)
    {
        if ((string)this->author_id == (string)si.author_id && this->index == si.index)
        {
            return true;
        }
        return false;
    }

    bool operator<(SI_Author_ID si)
    {
        if ((string)this->author_id < (string)si.author_id)
        {
            return true;
        }
        return false;
    }

    bool operator>(SI_Author_ID si)
    {
        if ((string)this->author_id > (string)si.author_id)
        {
            return true;
        }
        return false;
    }

};

struct SI_Author_Name
{
};

struct AuthorHeader
{
    int no_of_records;
    int RNN ;

    AuthorHeader()
    {
        this->no_of_records = 0;
        this->RNN = -1;
    }

    AuthorHeader(int no , int rnn)
    {
        no_of_records = no;
        this->RNN = rnn;

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
list<PI_Author_ID> PI_Author_ID_List;
list<PI_ISBN> PI_ISBN_List;
list<SI_Author_ID> SI_Author_ID_List;
list<SI_Author_Name> SI_Author_name_List;

fstream books_file, authors_file, pi_author_id_file, pi_isbn_file, si_author_id_file, si_author_name_file;

template <class T1, class T2>
void fillList(fstream &file, list<T1> &List, T2& header)
{
    file.seekg(0, ios::beg);
    file.read((char *)&header, sizeof(header));
    for (int i = 0; i < header.no_of_records; i++)
    {
        T1 record;
        file.seekg(sizeof(header) + (i * sizeof(record)), ios::beg);
        file.read((char *)&record, sizeof(record));
        List.push_back(record);
    }
}

template <class T1, class T2>
void writeFile(fstream &file, list<T1> &List, T2 &header)
{
    file.seekg(0, ios::beg);
    file.read((char *)&header, sizeof(header));
    int counter = 0;
    for (auto record : List)
    {
        file.seekp(sizeof(header) + (counter * sizeof(record)), ios::beg);
        file.write((char *)&record, sizeof(record));
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

template <class T1, class T2>
void writeAtFile(fstream &file, T1& record, T2 &header , int position)
{
    file.seekg(0, ios::beg);
    file.read((char *)&header, sizeof(header));
    file.seekp(sizeof(header) + (position * sizeof(record)), ios::beg);
    file.write((char *)&record, sizeof(record));
}


void addBook()
{
    bookHeader book_h;
    PI_ISBN_Header pi_isbn_h;
    SI_Author_ID_Header si_author_id_h;
    fillList(books_file,bookList,book_h);
    fillList(pi_isbn_file, PI_ISBN_List, pi_isbn_h);
    fillList(si_author_id_file,SI_Author_ID_List,si_author_id_h);
    char isbn[15], title[30], auther_id[15];
    cout << "enter book's isbn: ";
    cin.getline(isbn,15);
    cout << "enter book's title: ";
    cin.getline(title,30);
    cout << "enter book's auther id: ";
    cin.getline(auther_id,15);
    Book book(isbn, title,auther_id);
    if (isRecordExist(bookList, book))
    {
        return;
    }
    if (isISBNExist(PI_ISBN_List, isbn))
    {
        cout << "Error ISBN is aready exist" << endl;
        return;
    }
    if(book_h.RNN == -1){
        int end = book_h.no_of_records ;
        PI_ISBN pi_isbn(isbn, end);
        SI_Author_ID si_author_id(auther_id,end);
        book_h.no_of_records++;
        pi_isbn_h.no_of_records++;
        si_author_id_h.no_of_records++;

        PI_ISBN_List.push_back(pi_isbn);
        SI_Author_ID_List.push_back(si_author_id);
        PI_ISBN_List.sort();
        SI_Author_ID_List.sort();

        addHeader(books_file, book_h);
        addHeader(pi_isbn_file, pi_isbn_h);
        addHeader(si_author_id_file,si_author_id_h);

        writeAtFile(books_file,book,book_h,end);
        writeFile(pi_isbn_file,PI_ISBN_List,pi_isbn_h);
        writeFile(si_author_id_file,SI_Author_ID_List,si_author_id_h);

    }
    else{

    }
    
    
    bookList.clear();
    PI_ISBN_List.clear();
    SI_Author_ID_List.clear();
}



int main()
{

    books_file.open("books.txt", ios::in | ios::out | ios::binary);
    authors_file.open("authors.txt", ios::in | ios::out | ios::binary);
    pi_author_id_file.open("PI_Author_ID.txt", ios::in | ios::out | ios::binary);
    pi_isbn_file.open("PI_ISBN.txt", ios::in | ios::out | ios::binary);
    si_author_id_file.open("SI_Author_ID.txt", ios::in | ios::out | ios::binary);
    si_author_name_file.open("SI_Author_Name.txt", ios::in | ios::out | ios::binary);

    bookHeader book_h(0, -1);
    PI_ISBN_Header pi_isbn_h(0);
    SI_Author_ID_Header si_author_id_h(0);
    AuthorHeader author_h(0,-1);

    addHeader(books_file, book_h);
    addHeader(pi_isbn_file, pi_isbn_h);
    addHeader(si_author_id_file,si_author_id_h);
    


    addBook();
    
    fillList(books_file,bookList,book_h);
    fillList(pi_isbn_file,PI_ISBN_List,pi_isbn_h);
    fillList(si_author_id_file,SI_Author_ID_List,si_author_id_h);

    for(Book i : bookList){
        cout << i.isbn << " " << i.title << " " << i.author_id << endl  ;
    }

    for(PI_ISBN i : PI_ISBN_List){
        cout << i.isbn << " " << i.index << endl  ;
    }

    for(SI_Author_ID i : SI_Author_ID_List){
        cout << i.author_id<< " " << i.index << endl  ;
    }

    bookList.clear();
    PI_ISBN_List.clear();
    SI_Author_ID_List.clear();

    addBook();

    fillList(books_file,bookList,book_h);
    fillList(pi_isbn_file,PI_ISBN_List,pi_isbn_h);
    fillList(si_author_id_file,SI_Author_ID_List,si_author_id_h);

    for(Book i : bookList){
        cout << i.isbn << " " << i.title << " " << i.author_id << endl  ;
    }

    for(PI_ISBN i : PI_ISBN_List){
        cout << i.isbn << " " << i.index << endl  ;
    }

    for(SI_Author_ID i : SI_Author_ID_List){
        cout << i.author_id<< " " << i.index << endl  ;
    }

    bookList.clear();
    PI_ISBN_List.clear();
    SI_Author_ID_List.clear();


}