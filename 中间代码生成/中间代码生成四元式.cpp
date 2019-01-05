#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#define MAX 100
int m,sum;//sum���ڼ���������ĸ���
            //m���ڱ��������ʽ���ַ��ĸ���

char JG='A';
char str[MAX] = "";//���ڴ�������ʽ
int  token=0;//�����ŵı�־
void four();

struct grammar {
  int n, p, cnt;

  struct node {
    int line;
    string word;
    node() {}
    node(int _line, string _word){
      line = _line;
      word = _word;
    }
  } que[100005];

  node get(int p) {
    if (p == n) {
      return node(-1, "");
    }
    else return que[p];
  }

  void read() {
    freopen("file_out.txt", "r", stdin);
    int t;
    string s;
    while (cin >> t && cin >> s) {
      que[n++] = node(t, s);
    }
  }

  bool is_number(char c){
    return c >= '0' && c <= '9';
  }

  bool is_loweralpha(char c){
    return c >= 'a' && c <= 'z';
  }

  bool is_upperalpha(char c){
    return c >= 'A' && c <= 'Z';
  }

  bool is_alpha(char c) {
    return is_loweralpha(c) || is_upperalpha(c);
  }

  void print(int x, int y){

  }

  //����ǲ����޷�������
  bool check_number(){
    int tp = p;
    node t = get(p);
    p++;
    int l = t.word.length();
    for (int i = 0; i < l; ++i)
      if (!is_number(t.word[i])) {
        print(tp, p);
        return false;
      }
    print(tp, p);
    return true;
  }

  //����ǲ��Ǳ�ʶ��
  bool check_identifier() {
    int tp = p;
    node t = get(p++);
    int l = t.word.length();
    //�����һ���ַ�������ĸ�Ͳ��Ǳ�ʶ��
    if (!is_alpha(t.word[0])) {
      print(tp, p);
      return false;
    }
    //������ĸҲ�������־Ͳ��Ǳ�ʶ��
    for (int i = 0; i < l; ++i) {
      if (!is_alpha(t.word[i]) && !is_number(t.word[i])) {
        print(tp, p);
        return false;
      }
    }
    print(tp, p);
    return true;
  }

  //��鳣������
  bool check_const_definition() {
    int tp = p;
    if (!check_identifier()) {
      print(tp, p);
      return false;
    }
    if (get(p).word != string("=")) {
      print(tp, p + 1);
      return false;
    }
    else p++;
    if (!check_number()) {
      print(tp, p);
      return false;
    }
    print(tp, p);
    return true;
  }


  bool check_const() {
    int tp = p;
    p++;
    if (!check_const_definition()) {
      print(tp, p);
      return false;
    }
    //ѭ����鳣������
    while (true) {
      if (get(p).word == string(",")) {
        p++;
        if (!check_const_definition()) {
          print(tp, p);
          return false;
        }
      }
      else if (get(p).word == string(";")) {
        p++;
        break;
      }
      else {
        print(tp, p + 1);
        return false;
      }
    }
    print(tp, p);
    return true;
  }

  //����������
  bool check_var() {
    int tp = p;
    p++;
    if (!check_identifier()) {
      print(tp, p);
      return false;
    }
    while (true) {
      if (get(p).word == string(",")) {
        p++;
        if (!check_identifier()) {
          print(tp, p);
          return false;
        }
      }
      else if (get(p).word == string(";")) {
        p++;
        break;
      }
      else {
        print(tp, p);
        return false;
      }
    }
    print(tp, p);
    return true;
  }

  bool is_addop(string s) {
    return s == string("+") || s == string("-");
  }

  bool is_mulop(string s) {
    return s == string("*") || s == string("/");
  }

  //�ж��ǲ���factor
  bool check_factor() {
    int tp = p;
    node t = get(p);
    if (t.word == string("(")) {
      p++;
      if (!check_expression()) {
        print(tp, p);
        return false;
      }
      if (get(p).word != string(")")) {
        print(tp, p + 1);
        return false;
      }
      else p++;
    }
    else if (is_number(t.word[0])) {
      if (!check_number()) {
        print(tp, p);
        return false;
      }
    }
    else if (is_alpha(t.word[0])) {
      if (!check_identifier()) {
        print(tp, p);
        return false;
      }
    }
    else {
      print(tp, p);
      return false;
    }
    print(tp, p);
    return true;
  }

  //�ж��ǲ���term
  bool check_term() {
    int tp = p;
    if (!check_factor()) {
      print(tp, p);
      return false;
    }
    //term������*����/�ָ�
    while (true) {
      if (is_mulop(get(p).word)) {
        p++;
        if (!check_factor()) {
          print(tp, p);
          return false;
        }
      }
      else break;
    }
    print(tp, p);
    return true;
  }

  //�ж��ǲ��Ǳ��ʽ
  bool check_expression() {
    int tp = p;
    node t = get(p);
    //���ʽ���Ա��Ӽ����ŷָ�
    if(is_addop(t.word)) p++;
    if(!check_term()) {
      print(tp, p);
      return false;
    }

    while (true) {
      if (is_addop(get(p).word)) {
        p++;
        if (!check_term()) {
          print(tp, p);
          return false;
        }
      }
      else
        break;
    }
    print(tp, p);
    //printf("t%d=", ++cnt);
    //for (int i = tp; i < p; ++i)
     // cout << get(i).word;
    //cout << endl;
    return true;
  }

  //�ж��ǲ��Ǹ�ֵ���
  bool check_assign() {
    int tp = p;
    if (!check_identifier()) {
      print(tp, p);
      return false;
    }
    if (get(p).word != string("=")) {
      print(tp, p + 1);
      return false;
    }
    else p++;
    if(!check_expression()) {
      print(tp, p);
      return false;
    }
    if(get(p).word != string(";")) {
      print(tp, p);
      return false;
    }
    else p++;
    for(int i = tp; i < p; i++)
    {
        m++;
        strcat(str,(get(i).word).c_str());
        string tmp = get(i).word;
        if(tmp == string(" = ") || tmp == string("+") || tmp == string("-") || tmp == string("*") || tmp == string("/"))
            sum++;
    }
    four();
    memset(str,'\0',sizeof(str));
    print(tp, p);
    //cout << get(tp).word << "=t" << cnt << endl;
    return true;
  }

  //�жϸ������
  bool check_multiple() {
    int tp = p;
    if (get(p).word != string("begin")) {
      print(tp, p + 1);
      return false;
    }
    else p++;
    if (!check_sentence()) {
      print(tp, p);
      return false;
    }

    while (true) {
      if (p == n) {
        print(tp, p);
        return false;
      }
      else {
        if (get(p).word != string("end")) {
          if (!check_sentence()) {
            print(tp, p);
            return false;
          }
        }
        else {
          p++;
          break;
        }
      }
    }
    print(tp, p);

    return true;
  }

  bool is_relop(string s) {
    return s == string("==") || s == string("<=") || s == string("<") || s == string(">") || s == string (">=") || s == string("<>");
  }

  //�ж��������
  bool check_condition() {
    int tp = p;
    if (!check_expression()) {
      print(tp, p);

      return false;
    }
    if (!is_relop(get(p).word)) {
      print(tp, p + 1);
      return false;
    }
    else p++;
    if (!check_expression()) {
      print(tp, p);

      return false;
    }
    print(tp, p);

    return true;
  }

  bool check_if() {
    int tp = p;
    if (get(p).word != string("if")) {
      print(tp, p + 1);

      return false;
    }
    else p++;
    if (!check_condition()) {
      print(tp, p);

      return false;
    }
    if (get(p).word != string ("then")) {
      print(tp, p + 1);

      return false;
    }
    else p++;
    if (!check_sentence()) {
      print(tp, p);

      return false;
    }
    if (get(p).word == string("else")) {
      p++;
      if (!check_sentence()) {
        print(tp, p);

        return false;
      }
    }
    print(tp, p);

    return true;
  }

  bool check_while() {
    int tp = p;
    if (get(p).word != "while") {
      print(tp, p + 1);

      return false;
    }
    else p++;
    if (!check_condition()) {
      print(tp, p);

      return false;
    }
    if (get(p).word != "do") {
      print(tp, p + 1);

      return false;
    }
    else p++;
    if (!check_sentence()) {
      print(tp, p);

      return false;
    }
    print(tp, p);

    return true;
  }

  bool check_sentence() {
    int tp = p;
    node t = get(p);
    if (t.word == string("begin")) {
      if (!check_multiple()) {
        print(tp, p);

        return false;
      }
    }
    else if (t.word == string ("if")) {
      if (!check_if()) {
        print(tp, p);

        return false;
      }
    }
    else if (t.word == string ("while")) {
      if (!check_while()) {
        print(tp, p);

        return false;
      }
    }
    else {
      if (!check_assign()) {
        print(tp, p);

        return false;
      }
    }
    print(tp, p);

    return true;
  }

  bool grammar_analysis() {
    node t = get(p);

    if (t.word == string("Const")) {
      if (!check_const()) return false;
    }
    t = get(p);
    if (t.word == string("Var")) {
      if (!check_var()) return false;
    }

    while (p != n) {
      if (!check_sentence()) return false;
    }
    return true;
  }
}gram;


void change(int e)
{
    int f = e + 2;
    char ch=str[f];
    if(ch >= 'A' && ch <= 'Z')
    {
        for(int l = 0;l< (m + 10);l ++)
        {
            if(str[l] == ch)
                str[l] = JG;
        }
    }

    if(str[e] >= 'A' && str[e] <= 'Z')
    {
        for(int i = 0;i < m;i ++)
        {
            if(str[i] == str[e])
                    str[i] = JG;
        }
    }
}

void mulOperation(int i,int m)
{
    i++;
    for( ;i<=m-1;i++)//����˳�����
    {
        if(str[i]=='*'||str[i]=='/')
        {

            cout<<"("<<str[i]<<" "<<str[i-1]<<"  "<<str[i+1]<<"  "<<JG<<")"<<endl;
            change(i-1);
            str[i-1]=str[i]=str[i+1]=JG;
            sum--;
            JG=JG+1;
        }
    }
}

void addOperation(int j,int m)
{
    j++;
    for( ;j<=m-1;j++)//����Ӽ�����
    {
        if(str[j]=='+'||str[j]=='-')
        {
            cout<<"("<<str[j]<<" "<<str[j-1]<<"  "<<str[j+1]<<"  "<<JG<<")"<<endl;
            change(j-1);
            str[j-1]=str[j]=str[j+1]=JG;
            sum--;
            JG=JG+1;
        }
    }
}

void four()
{
    for(int i=0;i<=m-1;i++)//����˳�����
    {
        if(str[i]=='*'||str[i]=='/')
        {
            cout<<"("<< str[i] <<"  "<< str[i-1] <<"  "<< str[i+1] <<"  "<< JG << ")" <<endl;
            change(i-1);
            str[i-1] = str[i] = str[i+1] = JG;
            sum--;
            JG = JG + 1;
        }
    }

    for(int j=0;j<=m-1;j++)//����Ӽ�����
    {
        if(str[j]=='+'||str[j]=='-')
        {
            cout<<"("<<str[j]<<"  "<<str[j-1]<<"  "<<str[j+1]<<"  "<<JG<<")"<<endl;
            change(j-1);
            str[j-1] = str[j] = str[j+1] = JG;
            sum--;
            JG=JG+1;
        }
    }

    for(int k = 0;k <= m-1;k ++)//����ֵ����
    {
        if(str[k] == '=')
        {
            JG= JG - 1;
            cout<<"("<<str[k]<<"  "<<str[k+1]<<"  "<<"  "<<" "<<str[k-1]<<")"<<endl;
            sum--;
            change(k+1);
            str[k-1] = JG;
        }
    }
}

int main() {
  //freopen("out.txt","w",stdout);
  
  gram.p = 0;
  gram.cnt = 0;
  gram.read();
  gram.grammar_analysis();

  cout << str << endl;

  return 0;
}
