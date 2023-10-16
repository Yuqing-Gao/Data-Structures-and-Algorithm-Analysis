#include<fstream>
#include<cstring>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<map>
#include<bits/stdc++.h>
#define MAXN 9
using namespace std;
ifstream infile("C:\\Users\\F\\Desktop\\result2.txt");
map<string,int> order;//定义终结符在优先表中的序号
stack<string> s,save;//s为符号栈，save为临时存储符号的容器
vector<string> Terminal;//用来存放终结符，以此来进行对照，检查是不是终结符
vector<string>::iterator result;//一个迭代器来判断是不是终结符
string reduction[100];//定义本次需要进行规约的符号串
string str;//读入的字符串
string sym;//用来指示读入的符号
int flag=0;
int prioritytable[MAXN][MAXN]={{0,0,1,1,1,1,1,2,2},//算符优先分析的优先表，0代表=，1代表<,2代表>,3代表空
                          {0,0,1,1,1,1,1,2,2},
                          {2,2,0,0,1,1,1,2,2},
                          {2,2,0,0,1,1,1,2,2},
                          {2,2,2,2,0,3,3,2,2},
                          {2,2,2,2,3,0,3,2,2},
                          {1,1,1,1,1,1,1,0,2},
                          {2,2,2,2,3,3,0,2,2},
                          {1,1,1,1,1,1,1,1,0}};
void init(){//进行初始化，定义终结符的序号，以及简化终结符的查找通过vector的find函数
    order["plus"]=0;
    order["minus"]=1;
    order["star"]=2;
    order["slash"]=3;
    order["ident"]=4;
    order["number"]=5;
    order["l_paren"]=6;
    order["r_paren"]=7;
    order["#"]=8;
    for(int i=0;i<9;i++){
        Terminal.push_back("plus");
        Terminal.push_back("minus");
        Terminal.push_back("star");
        Terminal.push_back("slash");
        Terminal.push_back("ident");
        Terminal.push_back("number");
        Terminal.push_back("l_paren");
        Terminal.push_back("r_paren");
        Terminal.push_back("#");
    }
}
int judgePrior(string a,string b){//从优先关系表中判断两个符号的优先关系
    int row_prior,column_prior;//行列优先关系定位指针
    row_prior=order[a];
    column_prior=order[b];
    if(prioritytable[row_prior][column_prior]==0){//=
        return 0;
    }else if(prioritytable[row_prior][column_prior]==1){//<
        return 1;
    }else if(prioritytable[row_prior][column_prior]==2){//>
        return 2;
    }else return 3;//空
}
int advance(){//用来读入下一个符号
    int found;
    if(!getline(infile,str)){
        flag++;
        if(flag==1){
            sym="#";
            return 1;
        }
        else if(flag>1){
            return 0;
        }
    }
    found=str.find(',',0);
    sym=str.substr(1,found-1);
    return 1;
}
int SearchSyn(string c){//根据相应字符返回其对应序号
    return order[c];
}
int SearchTerminal(string c){//判断是不是终结符
    result=find(Terminal.begin(),Terminal.end(),c);
    if(result == Terminal.end()){
        return 0;
    }else{
        return 1;
    }
}

int Proc_Analysis(){//主控程序
    int k=1,j=0;
    string a,b,q;
    s.push("#");//符号栈S将#推入
    while(advance()){//把下一个输入符号读进a中；
        if(SearchTerminal(s.top())){
            j=k;
            q=s.top();
        }else{
            j=k-1;
            b=s.top();
            s.pop();
            save.push(b);
            q=s.top();
        }
        while(judgePrior(q,sym)==2){//判断优先关系While S[j] > a  do
            if(!SearchTerminal(s.top())){
               b=s.top();
               s.pop();
               save.push(b);
            }
            do{ //If S[k]   VT   then  j = k  else  j = k-1;如果不为终结符则将要跳到下一个字符
                b=s.top();
                s.pop();
                save.push(b);
                j--;
                if(SearchTerminal(s.top())){
                    q=s.top();
                }else{
                    save.push(s.top());
                    s.pop();
                    q=s.top();
                    j--;
                }
            }while(judgePrior(q,b)!=1);
            for(int i=0;i<k-j;i++){//这里我对需要规约的串进行存储，以方便规约
                b=save.top();
                save.pop();
                reduction[i]=b;
            }
            for(int i=0;i<k-j;i++){
                cout<<reduction[i]<<" ";
            }
            cout<<endl;
            //如下为进行规约
            if(reduction[0]=="term"){//<表达式> ::= <项>{<加法运算符> <项>}
                if(!((k-j)%2)){
                    printf("语法错误，表达式归约错误");
                    return 0;
                }
                for(int i=0;i<k-j;i++){
                    if(!(i%2)){
                        if(reduction[i]!="term"){
                            printf("语法错误，表达式归约错误");
                            return 0;
                        }
                    }else{
                        if(reduction[i]!="plus"||reduction[i]!="minus"){
                            printf("语法错误，表达式归约错误");
                            return 0;
                        }
                    }
                    k=j+1;
                    s.push("expression");
                }
            }else if(reduction[0]=="factor"){//<项> ::= <因子>{<乘法运算符> <因子>}
                if(!((k-j)%2)){
                    printf("语法错误，项归约错误");
                    return 0;
                }
                for(int i=0;i<k-j;i++){
                    if(i%2){
                        if(reduction[i]!="factor"){
                            printf("语法错误，项归约错误");
                            return 0;
                        }
                    }else{
                        if(reduction[i]!="star"||reduction[i]!="slash"){
                            printf("语法错误，项归约错误");
                            return 0;
                        }
                    }

                }
                k=j+1;
                s.push("term");
            }else if(reduction[0]=="l_paren"){//<因子> ::= <标识符>|<无符号整数>| ‘(’<表达式>‘)’
                if(reduction[0]=="l_paren"){
                    if((k-j)!=3){
                        printf("语法错误，因子归约错误");
                        return 0;
                    }
                    if(reduction[1]!="expression"||reduction[2]!="r_paren"){
                        printf("语法错误，因子归约错误");
                        return 0;
                    }
                }
                k=j+1;
                if(s.top()!="star"||s.top()!="slash"||sym!="star"||sym!="slash"){
                     s.push("term");
                }
                else s.push("factor");//这里我直接将因子直接化成项，但正确性还有待考虑
            }else if(reduction[0]=="number"||reduction[0]=="ident"){//<因子> ::= <标识符>|<无符号整数>| ‘(’<表达式>‘)’
                k=j+1;
                if(s.top()!="star"||s.top()!="slash"||sym!="star"||sym!="slash"){
                     s.push("term");
                }
                else s.push("factor");
            }else{
                printf("语法错误，字符错误");
                return 0;
            }
        }
        if(judgePrior(q,sym)==1||judgePrior(q,sym)==0){
            k++;
            s.push(sym);
        }
    }
    if(k==3){//如果最后为#表达式#则正确
        for(int i=0;i<3;i++){
            b=s.top();
            s.pop();
            if(i==0&&b!="#"){
                printf("语法错误，表达式错误");
                return 0;
            }else if(i==1&&b!="expression"){
                printf("语法错误，表达式错误");
                return 0;
            }else if(i==2&&b!="#"){
                printf("语法错误，表达式错误");
                return 0;
            }
        }
        return 1;
    }
    else{
       printf("语法错误，表达式错误");
       return 0;
    }
}
int main(){
    init();
    int right=Proc_Analysis();
    if(right){
        cout<<"语法正确"<<endl;
    }
    return 0;
}


