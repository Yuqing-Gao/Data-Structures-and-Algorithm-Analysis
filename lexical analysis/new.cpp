#include<fstream>
#include<cstring>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<map>
#include<bits/stdc++.h>
#include <iostream>  
#include <fstream>  
#include <stdlib.h>
using namespace std;
map<string,string> word;//应用map数据结构形成一个string->string的对应
std::map<string,string>::iterator it;//用来遍历整个对应关系的迭代器
void map_init(){//对应关系进行初始化
    word["["]="l_square";
    word["]"]="r_square";
    word["("]="l_paren";
    word[")"]="r_paren";
    word["{"]="l_brace";
    word["}"]="r_brace";
    word["."]="period";
    word["..."]="ellipsis";
    word["&"]="amp";
    word["&&"]="ampamp";
    word["&="]="ampequal";
    word["*"]="star";
    word["*="]="starequal";
    word["+"]="plus";
    word["++"]="plusplus";
    word["+="]="plusequal";
    word["-"]="minus";
    word["->"]="arrow";
    word["--"]="minusminus";
    word["-="]="minusequal";
    word["~"]="tilde";
    word["!"]="exclaim";
    word["!="]="exclaimequal";
    word["/"]="slash";
    word["/="]="slashequal";
    word["%"]="percent";
    word["%="]="percentequal";
    word["<"]="less";
    word["<="]="lessequal";
    word[">"]="greater";
    word[">="]="greaterequal";
    word["^"]="caret";
    word["^="]="caretequal";
    word["|"]="pipe";
    word["||"]="pipepipe";
    word["|="]="pipeequal";
    word["?"]="question";
    word[":"]="colon";
    word[";"]="semi";
    word["="]="equal";
    word["=="]="equalequal";
    word[","]="comma";
    word["#"]="hash";
    word["##"]="hashhash";
    word["#@"]="hashat";
    word[".*"]="periodstar";
    word["->*"]="arrowstar";
    word["::"]="coloncolon";
    word["int"]="int";
    word["void"]="void";
    word["if"]="if";
    word["else"]="else";
    word["for"]="for";
    word["return"]="return";
    word["main"]="identifier";
    word["scanf"]="identifier";
}
struct node{
	string id;
	string s;
}aa[10000];
int main(){
    map_init();//初始化
    char ch;
    char a;
    int len = 0;
    string word1;//string变量识别单词
    string str;//string变量进行字符识别
    ifstream infile("D:\\VScodeproject\\cppproject\\lexicalanalysis\\test.txt");//文件输入流，文件路径
    ofstream outfile("D:\\VScodeproject\\cppproject\\lexicalanalysis\\result.txt");//文件输出流
    ostringstream buf;
    while(buf&&infile.get(ch)) buf.put(ch);//将文件中的字符读出来
    str= buf.str();//将得到的字符储存到string类型变量中
    int csize=str.length();
    for(int i=0;i<csize;i++){//对整个字符串进行遍历
        while(str[i]==' '||str[i]=='\n') i++;//若最开始为空格或换行符，则将指针的位置往后移
        if(str[i]=='"'){//对字符串进行识别
            word1=str[i++];
            while(str[i]!='"'){
                word1+=str[i++];
            }
            word1+=str[i++];
            cout<<"("<<"string_literal"<<","<<word1<<")"<<endl;
            aa[len].id  =  "string_literal";
            aa[len++].s = word1;
        }
        if(isalpha(str[i])){//对标识符和基本字进行识别,调用库函数isalpha()
            word1=str[i++];
            while(isalpha(str[i])||isdigit(str[i])){
                word1+=str[i++];
            }
            it=word.find(word1);
            if(it!=word.end()){//判断是不是基本字，若为基本字则进行输出
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id =word[word1];
                aa[len++ ].s = word1;
            }
            else{//否则直接输出
                cout<<"(char_constant"<<","<<word1<<")"<<endl;
                aa[len].id  =  "char_constant";
                aa[len++].s = word1;
            }
            i--;
        }
        else if(isdigit(str[i])){//判断是不是常数，调用库函数isdigit()
            word1=str[i++];
            while(isdigit(str[i])){
                word1+=str[i++];
            }
            if(isalpha(str[i])){
                cout<<"unknown"<<endl;
                break;
            }
            else{
                cout<<"(numeric_constant"<<","<<word1<<")"<<endl;
                aa[len].id  = "numeric_constant";
                aa[len++].s = word1;
            }
            i--;
        }else if(str[i]=='<'){//对<,<=分别进行判断
            word1=str[i++];
            if(str[i]=='>'){
                word1+=str[i];
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else if(str[i]=='='){
                word1+=str[i];
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else if(str[i]!=' '||!isdigit(str[i])||!isalpha(str[i])){
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else{
                cout<<"unknown"<<endl;
                break;
            }
            i--;          
        }else if(str[i]=='>'){//对>,>=分别进行判断
            word1=str[i++];
            if(str[i]=='='){
                word1+=str[i];
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else if(str[i]!=' '||!isdigit(str[i])||!isalpha(str[i])){
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else{
                cout<<"unknown"<<endl;
                break;
            }
            i--;
        }else if(str[i]==':'){//对:=进行判断
            word1=str[i++];
            if(str[i]=='='){
                word1+=str[i];
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else if(str[i]!=' '||!isdigit(str[i])||!isalpha(str[i])){
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else{
                cout<<"unknown"<<endl;
                break;
            }
            i--;
        }else{//对其他的基本字依次进行判断
            word1=str[i];
            it=word.find(word1);
            if(it!=word.end()){
                cout<<"("<<word[word1]<<","<<word1<<")"<<endl;
                aa[len].id  = word[word1];
                aa[len++].s = word1;
            }else{
                cout<<"unknown"<<endl;
                break;
            }
        }
    }
    //输出流
    if (outfile.is_open())   
    {  
        for(int i = 0 ;i < len ;i ++){
        	outfile <<"("<<aa[i].id<<","<<aa[i].s<<")"<<'\n';  
		} 
		 outfile.close(); 
    }  
    infile.close();
    return 0;
}
