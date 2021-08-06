#include<iostream>
#include<stdlib.h>
#include<algorithm>
#include<windows.h>
#include<stdio.h> 
#include<math.h>
#include<string.h>
#include<vector>
using namespace std;
const int BORDER = 50;

class Trie{
    public:
        Trie* children[26];
        int count;
        char* word;
        bool found;
        Trie();
};

Trie::Trie(){
    count = 0;
    word = NULL;
    found = false;
    int i;
    for(i = 0; i < 26; i++)
        {
            children[i] = NULL;
        }
}
Trie* dict;
int duplicates = 0;
int words = 0;
int * board;
int board_size;
int puzzle_size;
int cols;
int * children;
vector<char*> found;

char* Color(int color=7, char* Message = ""){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    return Message;
}

char* readF( char fname[] ){
    FILE * dict;
    long int fSize;
    char * ptr;
    long int result;
    dict = fopen ( fname , "r" );
    if (dict==NULL) {fputs ("File error",stderr); exit (1);}
    fseek (dict , 0 , SEEK_END);
    fSize = ftell (dict);
    rewind (dict);
    ptr = (char*) malloc (sizeof(char)*fSize);
    if (ptr == NULL) {fputs ("Memory error",stderr); exit (2);}
    result = fread (ptr,1,fSize,dict);
    if (result != fSize) {fputs ("Reading error",stderr); exit (3);}
    fclose (dict);
    return ptr;
}

void getFoundWords(Trie* p)
{
    int i;

    if( p )
        {
            if( p->found )
                found.push_back( p->word );

            for(i = 0; i < 26; i++)
                {
                    getFoundWords( p->children[i] );
                }
        }
}

void * buildBoard()
{
    int n;
    cout<<"Enter the order of boggle: ";
    cin>>n;
    char* s;
    s = (char*) malloc (sizeof(char)*n);
    cout<<"Input the Boggle Puzzle: ";
    cin>>s;
    int len = strlen(s);
    if(len==(n*n))
    {
    cols = n + 2;
    children = new int[8] {-1-cols, -cols, 1-cols, -1, 1, cols-1, cols, cols+1};
    board_size = cols * cols;
    puzzle_size = (cols-2) * (cols-2);
    board = new int[board_size];
    int j = 0;
    for(int i = 0; i < board_size; i++)
        {
            if( (i < cols) ||((i+1) % cols == 0) || (i > cols * (cols -1)) || (i % cols == 0) )
                {
                    board[i] = BORDER;
                }
            else
                {
                    board[i] = s[j] - 'a';
                    j++;
                }
        }
    }
    else
        {
            cout<<"Invalid Input"<<endl;
            exit(0);
        }
}

void insertWord( char word[], const int len ){
    Trie* p = dict;
    int i;

    for(i = 0; word[i]; i++)
        {
        int letter = word[i] - 'a';
        p->count++;
        if( !p->children[letter] )
            p->children[letter] = new Trie;

        p = p->children[letter];
        }

    p->word = word;
}

void buildTrie( char dictFile[] )
{
    char * ptr = readF( dictFile );
    char * word;
    int len, i;
    dict = new Trie;
    word = strtok(ptr,"\n\t");
    while (word != NULL)
        {
            len = strlen( word );
            if( len >= 3 )
            {
                insertWord( word, len );
                words++;
            }
            word = strtok (NULL, "\n\t");
        }
}

inline Trie* lookup(const int i, Trie* p){
    p = p->children[i];
    if( p && p->word )
        {
        if( p->found )
        {
            duplicates++;
        }
        else
        {
            p->found = true;
        }
    }

    return p;
}

inline bool next(int cube, Trie* p, vector<bool> done)
{

    p = lookup( board[cube], p);

    if( p && p->count ){

        done[cube] = true;

        for(int i = 0; i <8; i++){
            int child = cube + children[i];
            if((board[child] != BORDER) && !done[child])
                next(child, p, done);
        }
    }
}

void traverseBoard(){
    Trie* p = dict;
    vector<bool> done;
    int i=0, j = 0;
    for(int i = 0; i < board_size; i++) done.push_back(false);

    for(int i = 0; i < board_size; i++){
        if(board[i] != BORDER){
            next(i, p, done);
        }
    }
}

void output()
{
    cout<<"\n";
    sort(found.begin(), found.end());
    for(vector<char*>::iterator it = found.begin(); it != found.end(); ++it) {
            cout<<*it;
            cout<<"\n";
    }
}

int main(){
    char dictFile[100] = "mydictionary.txt";
    cout << Color(11,"\n\t\tBOGGLE SOLVER")<<endl;
    Color(9);
    cout<< "================================================="<<endl;
    buildTrie( dictFile );
    buildBoard();
    Color(14);
    cout<<endl<<words <<" words in " <<dictFile<<endl;
    cout<< puzzle_size <<" cubes on the board"<< endl;

    cout << ".................................................";
    cout<<endl;
    traverseBoard();
    getFoundWords( dict );
    char c;
    for(int i = 0; i < board_size; i++)
        {
            c = (char) (board[i] + 'a');

    if( BORDER == board[i] )
        {
            cout<<"#\t";
        }
    else{
            cout<<c;
            cout<<"\t";
        }
        if( (i+1) % cols == 0 ) cout<<"\n";
    }
    Color(12);
    cout<<endl<<found.size() << " words found"<<endl<<duplicates<< " duplicate words found" << endl<<endl;
    Color(14);
    cout<<"Words found on the board are: ";
    output();
    Color(9);
    cout    << "=================================================" <<endl;
    Color(11);
    cout<<"\t\tTHANK YOU"<< endl;
    Color(8);
}
