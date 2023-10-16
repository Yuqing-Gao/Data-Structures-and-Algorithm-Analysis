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
map<string,int> order;//�����ս�������ȱ��е����
stack<string> s,save;//sΪ����ջ��saveΪ��ʱ�洢���ŵ�����
vector<string> Terminal;//��������ս�����Դ������ж��գ�����ǲ����ս��
vector<string>::iterator result;//һ�����������ж��ǲ����ս��
string reduction[100];//���屾����Ҫ���й�Լ�ķ��Ŵ�
string str;//������ַ���
string sym;//����ָʾ����ķ���
int flag=0;
int prioritytable[MAXN][MAXN]={{0,0,1,1,1,1,1,2,2},//������ȷ��������ȱ�0����=��1����<,2����>,3�����
                          {0,0,1,1,1,1,1,2,2},
                          {2,2,0,0,1,1,1,2,2},
                          {2,2,0,0,1,1,1,2,2},
                          {2,2,2,2,0,3,3,2,2},
                          {2,2,2,2,3,0,3,2,2},
                          {1,1,1,1,1,1,1,0,2},
                          {2,2,2,2,3,3,0,2,2},
                          {1,1,1,1,1,1,1,1,0}};
void init(){//���г�ʼ���������ս������ţ��Լ����ս���Ĳ���ͨ��vector��find����
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
int judgePrior(string a,string b){//�����ȹ�ϵ�����ж��������ŵ����ȹ�ϵ
    int row_prior,column_prior;//�������ȹ�ϵ��λָ��
    row_prior=order[a];
    column_prior=order[b];
    if(prioritytable[row_prior][column_prior]==0){//=
        return 0;
    }else if(prioritytable[row_prior][column_prior]==1){//<
        return 1;
    }else if(prioritytable[row_prior][column_prior]==2){//>
        return 2;
    }else return 3;//��
}
int advance(){//����������һ������
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
int SearchSyn(string c){//������Ӧ�ַ��������Ӧ���
    return order[c];
}
int SearchTerminal(string c){//�ж��ǲ����ս��
    result=find(Terminal.begin(),Terminal.end(),c);
    if(result == Terminal.end()){
        return 0;
    }else{
        return 1;
    }
}

int Proc_Analysis(){//���س���
    int k=1,j=0;
    string a,b,q;
    s.push("#");//����ջS��#����
    while(advance()){//����һ��������Ŷ���a�У�
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
        while(judgePrior(q,sym)==2){//�ж����ȹ�ϵWhile S[j] > a  do
            if(!SearchTerminal(s.top())){
               b=s.top();
               s.pop();
               save.push(b);
            }
            do{ //If S[k]   VT   then  j = k  else  j = k-1;�����Ϊ�ս����Ҫ������һ���ַ�
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
            for(int i=0;i<k-j;i++){//�����Ҷ���Ҫ��Լ�Ĵ����д洢���Է����Լ
                b=save.top();
                save.pop();
                reduction[i]=b;
            }
            for(int i=0;i<k-j;i++){
                cout<<reduction[i]<<" ";
            }
            cout<<endl;
            //����Ϊ���й�Լ
            if(reduction[0]=="term"){//<���ʽ> ::= <��>{<�ӷ������> <��>}
                if(!((k-j)%2)){
                    printf("�﷨���󣬱��ʽ��Լ����");
                    return 0;
                }
                for(int i=0;i<k-j;i++){
                    if(!(i%2)){
                        if(reduction[i]!="term"){
                            printf("�﷨���󣬱��ʽ��Լ����");
                            return 0;
                        }
                    }else{
                        if(reduction[i]!="plus"||reduction[i]!="minus"){
                            printf("�﷨���󣬱��ʽ��Լ����");
                            return 0;
                        }
                    }
                    k=j+1;
                    s.push("expression");
                }
            }else if(reduction[0]=="factor"){//<��> ::= <����>{<�˷������> <����>}
                if(!((k-j)%2)){
                    printf("�﷨�������Լ����");
                    return 0;
                }
                for(int i=0;i<k-j;i++){
                    if(i%2){
                        if(reduction[i]!="factor"){
                            printf("�﷨�������Լ����");
                            return 0;
                        }
                    }else{
                        if(reduction[i]!="star"||reduction[i]!="slash"){
                            printf("�﷨�������Լ����");
                            return 0;
                        }
                    }

                }
                k=j+1;
                s.push("term");
            }else if(reduction[0]=="l_paren"){//<����> ::= <��ʶ��>|<�޷�������>| ��(��<���ʽ>��)��
                if(reduction[0]=="l_paren"){
                    if((k-j)!=3){
                        printf("�﷨�������ӹ�Լ����");
                        return 0;
                    }
                    if(reduction[1]!="expression"||reduction[2]!="r_paren"){
                        printf("�﷨�������ӹ�Լ����");
                        return 0;
                    }
                }
                k=j+1;
                if(s.top()!="star"||s.top()!="slash"||sym!="star"||sym!="slash"){
                     s.push("term");
                }
                else s.push("factor");//������ֱ�ӽ�����ֱ�ӻ��������ȷ�Ի��д�����
            }else if(reduction[0]=="number"||reduction[0]=="ident"){//<����> ::= <��ʶ��>|<�޷�������>| ��(��<���ʽ>��)��
                k=j+1;
                if(s.top()!="star"||s.top()!="slash"||sym!="star"||sym!="slash"){
                     s.push("term");
                }
                else s.push("factor");
            }else{
                printf("�﷨�����ַ�����");
                return 0;
            }
        }
        if(judgePrior(q,sym)==1||judgePrior(q,sym)==0){
            k++;
            s.push(sym);
        }
    }
    if(k==3){//������Ϊ#���ʽ#����ȷ
        for(int i=0;i<3;i++){
            b=s.top();
            s.pop();
            if(i==0&&b!="#"){
                printf("�﷨���󣬱��ʽ����");
                return 0;
            }else if(i==1&&b!="expression"){
                printf("�﷨���󣬱��ʽ����");
                return 0;
            }else if(i==2&&b!="#"){
                printf("�﷨���󣬱��ʽ����");
                return 0;
            }
        }
        return 1;
    }
    else{
       printf("�﷨���󣬱��ʽ����");
       return 0;
    }
}
int main(){
    init();
    int right=Proc_Analysis();
    if(right){
        cout<<"�﷨��ȷ"<<endl;
    }
    return 0;
}


