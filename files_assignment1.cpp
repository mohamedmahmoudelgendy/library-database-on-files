#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct PI_Auther_ID_Header{
    int no_of_records ;

    PI_Auther_ID_Header(){

    }

    PI_Auther_ID_Header(int no){
        no_of_records = no ;
    }
};

struct PI_Auther_ID
{
    char id[15];
    int index ;

    PI_Auther_ID(){

    }

    PI_Auther_ID(char id[],int index){
        for(int i = 0 ; i < 15 ; i++){
            this->id[i] = id[i] ;
        }
        this->index = index ;
    }

};

struct PI_ISBN_Header{
    int no_of_records ;

    PI_ISBN_Header(){

    }

    PI_ISBN_Header(int no){
        no_of_records = no ;
    }
};

struct PI_ISBN
{
    char ISBN[15];
    int index ;

    PI_ISBN(){

    }

    PI_ISBN(char ISBN[],int index){
        for(int i = 0 ; i < 15 ; i++){
            this->ISBN[i] = ISBN[i] ;
        }
        this->index = index ;
    }
};

struct SI_Auther_ID_Header{
    int no_of_records ;

    SI_Auther_ID_Header(){

    }

    SI_Auther_ID_Header(int no){
        no_of_records = no ;
    }
};

struct SI_Auther_ID
{
    char id[15];
    int index ;

    SI_Auther_ID(){

    }

    SI_Auther_ID(char id[],int index){
        for(int i = 0 ; i < 15 ; i++){
            this->id[i] = id[i] ;
        }
        this->index = index ;
    }

};

struct SI_Auther_name
{

};

struct AutherHeader{
    int no_of_records ;

    AutherHeader(){

    }

    AutherHeader(int no){
        no_of_records = no ;
    }
};

struct Author
{   
    long length ;
    char id[15];
    char name[30];
    char address[30];

    Author(){
        length = sizeof(this);
    }

    Author(char id[],char name[],char address[]){
        for(int i = 0 ; i < 15 ; i++){
            this->id[i] = id[i] ;
        }
        
        for(int i = 0 ; i < 30 ; i++){
            this->name[i] = name[i] ;
        }

        for(int i = 0 ; i < 30 ; i++){
            this->address[i] = address[i] ;
        }
        
    }
};

struct bookHeader{
    int no_of_records ;
    int RNN ;

    bookHeader(){
        this->no_of_records = 0 ;
        this->RNN = -1 ;
    }

    bookHeader(int no){
        no_of_records = no ;
    }
};

struct Book
{   
    long length ;
    char isbn[15];
    char title[30];
    char auther_id[15];

    Book(){
        length = sizeof(this);
    }

    Book(char isbn[],char title[],char auther_id[]){
        
        for(int i = 0 ; i < 15 ; i++){
            this->isbn[i] = isbn[i] ;
        }

        for(int i = 0 ; i < 30 ; i++){
            this->title[i] = title[i] ;
        }
        
        for(int i = 0 ; i < 15 ; i++){
            this->auther_id[i] = auther_id[i] ;
        }
    }

    bool operator==(Book b){
        if((string)this->isbn == (string)b.isbn && (string)this->title == (string)b.title && (string)this->auther_id == (string)b.auther_id ){
            return true ;
        }
        return false ;
    }

    bool operator<(Book b){
        if((string)this->isbn < (string)b.auther_id){
            return true ;
        }
        return false ;
    }

    bool operator>(Book b){
        if((string)this->isbn > (string)b.auther_id){
            return true ;
        }
        return false ;
    }
    
};



list<Book> bookList ;
list<Author> authorList ;
list<PI_Auther_ID> PI_Auther_ID_List ;
list<PI_ISBN> PI_ISBN_List ;
list<SI_Auther_ID> SI_Auther_ID_List ;
list<SI_Auther_name> SI_Auther_name_List ;

fstream myfile ;

template <class T1 , class T2> 
void fillList(fstream& file,list<T1>& List , T2& h){
    file.seekg(0,ios::beg);
    file.read((char *)&h,sizeof(h));
    for(int i = 0 ; i < h.no_of_records ;i++){
        T1 b ;
        file.seekg(sizeof(h)+(i*sizeof(b)),ios::beg);
        file.read((char *)&b,sizeof(b));
        List.push_back(b);
    }
}

template<class T>
bool isRecordExist(list<T> list , T record){
    for(T i: list){
        if(i == record){
            return true;
        }
    }
    return false ;
}

template<class T>
void getHeader(fstream& file ,T& h){
    file.seekg(0,ios::beg);
    file.read((char *)h ,sizeof(h));
    file.seekg(0,ios::beg);
}

template<class T>
void addHeader(fstream& file,T h){
    file.seekp(0,ios::beg);
    file.write((char *)h ,sizeof(h));
    file.seekp(0,ios::end);
}

bool isISBNExist(list<Book> list ,string isbn ){
    for(Book i: list){
        if((string)i.isbn == isbn){
            return true;
        }
    }
    return false ;
}




void addBook(fstream& file,list<Book>& list){
    string isbn , title ,auther_id ;
    cout<< "enter book's isbn: ";
    getline(cin,isbn);
    cout<< "enter book's title: ";
    getline(cin,title);
    cout<< "enter book's auther id: ";
    getline(cin,auther_id);
    Book b((char *)&isbn,(char *)&title,(char *)&auther_id) ;
    if(isRecordExist(list,b)){
        return ;
    }
    if(isISBNExist(list,isbn)){
        cout<< "Error ISBN is aready exist" << endl ;
        return ;
    }
    PI_ISBN_Header index_h ;
    getHeader(file , index_h);
    PI_ISBN index((char *)&isbn,index_h.no_of_records) ;
    


}

void add(fstream& file ,Author auther){
    file.write((char *)&auther,sizeof(auther));
}

int main(){

myfile.open("new.txt",ios::in|ios::out|ios::binary);

bookHeader h ;
Book b((char *)"kkk",(char *)"aaa",(char *)"aaa"), b2((char *)"aaa",(char *)"aaa",(char *)"aaa");
fillList(myfile,bookList,h);
bookList.sort();


for(Book i : bookList){
    cout << i.auther_id ;
}
// cout << isRecordExist(bookList,b) ;

}