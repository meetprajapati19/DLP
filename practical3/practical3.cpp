#include<iostream>
#include <fstream>
#include<string>
#include <cctype>
using namespace std;

int main()
{
string keywords[32]={"auto","break"	,"case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch",	"typedef"	,"union" , "unsigned","void","volatile","while"};

string operators[7]={"+","-","*","/","%","++","--"};

string punctuation [8]={";","{","}","[","]","(",")","," };
//  identifiers,string, laxical error

// symbol table entries

string temp;


ifstream MyReadFile("practical3.txt");

while (MyReadFile >> temp) {
//  int count=temp.find("//");
 if(temp.substr(0, 2) == "//"){
    // count=0;
    while(temp=='/n'){
continue;
    }
    
 }
 
  cout << temp <<endl;
}


MyReadFile.close();
return 0;
}


// give me code in c++ which take the textfile as input which has c program int it , give me output that howmany tokens in that and also write type of tokens like   identifiers,string,operators, punctuation,keywords and also give  