#include<bits/stdc++.h>
#pragma GCC optimize(3)
int id = 0;
std::unordered_map<char,int> pri;
struct node{
    char c;
    int to;
    std::set<int> g;
};
std::vector<node> NfaStates;
struct NFA{
    int from,to;
    NFA(){}
    NFA(int from,int to):from(from),to(to){}
};
NFA Creat_NFA(){
    node a,b;
    int x = id, y = id + 1;
    id+=2;
    NfaStates.push_back(a);
    NfaStates.push_back(b);
    NFA nfa(x,y);
    return nfa;
}
bool check(std::string s){
    if(s.size()>1&&s[s.size()-2]!='\\'&&s.back()!='\\')
        return true;
    else if(s.size()==1&&s.back()!='\\')
        return true;
    else 
        return false;
}
void add(int from,int to,char c){
    NfaStates[from].c = c;
    NfaStates[from].to = to;
    //std::cout<<from<<"->"<<to<<" "<<c<<"\n";
}
void add(int from,int to){
    NfaStates[from].g.insert(to);
    //std::cout<<from<<"->"<<to<<"\n";
}
std::string preprecess(std::string s){
    std::string str;
    for(int i = 0;i < s.size(); i++){
        //std::cout<<str<<" "<<s[i]<<"\n";
        if(str.size()&&(check(str)||str.back()=='*'||str.back()==')'||str.back()=='+'||str.back()=='?')){
            if((s[i]!='\\')||(s[i]=='\\'&&s[i+1]=='(')){
                str.push_back('\\');
                str.push_back('&');
            }
        }
        str.push_back(s[i]);
    }
    // std::cout<<str<<"\n";
    return str;
}
std::string infixToSuffix(std::string s){
    s = preprecess(s);
    std::string str;
    std::stack<char> st;
    for(int i = 0;i < s.size(); i++){
        if(s[i]!='\\'){
            str+=s[i];
        }
        else{
            i++;
            if(s[i]=='('){
                st.push(s[i]);
            }
            else if(s[i]==')'){
                char c = st.top();
                while(c != '('){
                    str+='\\';
                    str+=c;
                    st.pop();
                    c=st.top();
                }
                st.pop();
            }
            else{
                if(st.size()){
                    char c = st.top();
                    while(pri[c] >= pri[s[i]]){
                        str+='\\';
                        str+=c;
                        st.pop();
                        if(st.empty())
                            break;
                        else c = st.top();
                    }
                    st.push(s[i]);
                }
                else{
                    st.push(s[i]);
                }
            }
        }
    }
    while(!st.empty()){
		char c = st.top();
		st.pop();
        str +='\\';
		str += c;
	}
    return str;
}
NFA strtoNFA(std::string s){
    std::stack<NFA> NfaStack;		/*定义一个NFA栈*/ 
	for(int i = 0; i < s.size(); i++)		/*读取后缀表达式，每次读一个字符*/ 
	{

		if(s[i]!='\\')		                    /*遇到操作数*/ 
		{
			
			NFA n = Creat_NFA();		        /*新建一个NFA*/ 
			add(n.from, n.to, s[i]);			/*NFA的头指向尾，弧上的值为s[i]*/
			NfaStack.push(n);					/*将该NFA入栈*/
		}
        else{
            i++;
            if(s[i] == '*')		            /*遇到闭包运算符*/
            {
                
                NFA n1 = Creat_NFA();			    /*新建一个NFA*/
                NFA n2 = NfaStack.top();			/*从栈中弹出一个NFA*/
                NfaStack.pop();
                add(n2.to, n2.from);				/*n2的尾通过ε指向n2的头*/
                add(n2.to, n1.to);				    /*n2的尾通过ε指向n1的尾*/
                add(n1.from, n2.from);				/*n1的头通过ε指向n2的头*/
                add(n1.from, n1.to);				/*n1的头通过ε指向n1的尾*/
                
                NfaStack.push(n1);					/*最后将新生成的NFA入栈*/
            }
            else if(s[i] == '+'){
                NFA n1 = Creat_NFA();			    /*新建一个NFA*/
                NFA n2 = NfaStack.top();			/*从栈中弹出一个NFA*/
                NfaStack.pop();
                //std::cout<<n2.from<<"---"<<n2.to<<"\n";
                add(n2.to, n2.from);				/*n2的尾通过ε指向n2的头*/
                add(n2.to, n1.to);				    /*n2的尾通过ε指向n1的尾*/
                add(n1.from, n2.from);				/*n1的头通过ε指向n2的头*/
                
                NfaStack.push(n1);					/*最后将新生成的NFA入栈*/
            }
            else if(s[i] == '?'){
                NFA n = NfaStack.top();
                NfaStack.pop();
                add(n.from,n.to);
                NfaStack.push(n);
            }
            else if(s[i] == '|')   	                /*遇到或运算符*/
            {	
                NFA n1, n2;							/*从栈中弹出两个NFA，栈顶为n2，次栈顶为n1*/
                n2 = NfaStack.top();
                NfaStack.pop();
                
                n1 = NfaStack.top();
                NfaStack.pop();
                
                NFA n = Creat_NFA();		        /*新建一个NFA*/

                add(n.from, n1.from);				/*n的头通过ε指向n1的头*/
                add(n.from, n2.from);				/*n的头通过ε指向n2的头*/	
                add(n1.to, n.to);				    /*n1的尾通过ε指向n的尾*/
                add(n2.to, n.to);				    /*n2的尾通过ε指向n的尾*/
                
                NfaStack.push(n);					/*最后将新生成的NFA入栈*/
            }
            else if(s[i] == '&')		            /*遇到连接运算符*/
            {
                
                NFA n1, n2, n;				        /*定义一个新的NFA n*/
                n2 = NfaStack.top();				/*从栈中弹出两个NFA，栈顶为n2，次栈顶为n1*/
                NfaStack.pop();
                n1 = NfaStack.top();
                NfaStack.pop();
                add(n1.to, n2.from);				/*n1的尾通过ε指向n2的尾*/
                n.from = n1.from;					/*n的头为n1的头*/
                n.to = n2.to;					    /*n的尾为n2的尾*/
                NfaStack.push(n);					/*最后将新生成的NFA入栈*/
            }
        }
	}
	
	return NfaStack.top();		/*最后的栈顶元素即为生成好的NFA*/
}
struct Dfastate{
    bool isEnd;
    std::set<int> closure;
    std::map<char,int> g;
    Dfastate(){
        isEnd = false;
    }
};
int dfanum = 0;
std::vector<Dfastate> dfastates;
struct DFA{
    DFA(){}
    int startstate;
    std::set<int> endstates;
    std::set<char> terminator;
};
std::set<int> epcloure(std::set<int> s)
{
	
	std::stack<int> epStack;		
	
	for(auto i:s)
		epStack.push(i);			/*将该状态集中的每一个元素都压入栈中*/
	
	while(!epStack.empty())			/*只要栈不为空*/
	{
		
		int x = epStack.top();		/*从栈中弹出一个元素*/
		epStack.pop();
		
		for(auto i:NfaStates[x].g)
		{
			if(!s.count(i))				/*遍历它通过ε能转换到的状态集*/
			{								/*如果当前元素没有在集合中出现*/
				s.insert(i);			/*则把它加入集合中*/
				epStack.push(i);		/*同时压入栈中*/
			}
		}
	}
	return s;		/*最后的s即为ε-cloure*/
}
std::set<int> moveEpCloure(std::set<int> s, char ch)
{
	
	std::set<int> temp;
	for(auto i : s)		/*遍历当前集合s中的每个元素*/
	{
		if(NfaStates[i].c == ch)				/*如果对应转换弧上的值为ch*/
		{
			temp.insert(NfaStates[i].to);		/*则把该弧通过ch转换到的状态加入到集合temp中*/
		}
	}
	
	temp = epcloure(temp);			/*最后求temp的ε闭包*/
	return temp;
}
bool isEnd(NFA n,std::set<int> s){
    for(auto i : s){
        if(i==n.to){
            return true;
        }
    }
    return false;
}
int Creat_DFA(NFA n,std::set<int> s){
    Dfastate dfastate;
    int index = dfanum;
    dfastate.closure = epcloure(s);
    dfastate.isEnd = isEnd(n,dfastate.closure);
    dfastates.push_back(dfastate);
    dfanum++;
    return index;
}
DFA NfaToDfa(NFA n,std::string s){
    std::map<std::set<int>,int> states;
    DFA d;
    std::set<char> terminator;
    for(int i = 0;i < s.size(); i++){
        if(s[i]=='\\')i++;
        else terminator.insert(s[i]);
    }
    std::set<int> tmp;
    tmp.insert(n.from);
    int index = Creat_DFA(n, tmp);
    d.startstate = index;
    d.terminator = terminator;
    states[dfastates[index].closure] = index;
    // for(auto i:dfastates[index].closure){
    //     std::cout<<i<<" ";
    // }
    // std::cout<<"\n";
    std::queue<int> q;
    q.push(index);
    while(q.size()){
        int x = q.front();q.pop();
        for(auto c : terminator){
            tmp = moveEpCloure(dfastates[x].closure,c);
            if(tmp.empty())continue;
            if(states.count(tmp)){
                index = states[tmp];
                dfastates[x].g[c] = index;
            }
            else{
                index = Creat_DFA(n,tmp);
                states[tmp] = index;
                dfastates[x].g[c] = index;
                q.push(index);
            }
        }
    }
    for(auto k:states){
        auto j = k.second;
        if(dfastates[j].isEnd == true)
            d.endstates.insert(j);
        // for(auto [x,y]:dfastates[j].g){
        //     std::cout<<j<<" "<<y<<" "<<x<<"\n";
        // }
    }
    // std::cout<<"start:"<<d.startstate<<"\n";
    // for(auto i:d.endstates){
    //     std::cout<<i<<" ";
    // }
    // std::cout<<"\n";
    // std::cout<<"----------------------------------------\n";
    return d;
}
int find(std::string s,DFA dfa,int index){
    int pos = dfa.startstate;
    //std::cout<<dfa.startstate<<"\n";
    int last = -1;
    for(int i=index;i<s.size();i++){
        //std::cout<<s[i]<<" "<<dfastates[pos].g[s[i]]<<"\n";
        if(dfastates[pos].g.count(s[i])){
            pos = dfastates[pos].g[s[i]];
        }
        else{
            break;
        }
        if(dfastates[pos].isEnd == true)
            last = i;
    }
    return last;
}
DFA cal(std::string str){
    std::string s = infixToSuffix(str);
    NFA nfa = strtoNFA(s);
    DFA dfa = NfaToDfa(nfa,s);
    return dfa;
}
std::vector<DFA> v;
std::string Keyword(){
    std::string str;
    for(int k = 0; k < 2; k++){
        str+="\\(";
        for(int i = 0;i < 26; i++){
            str+=char('a'+i);
            str+="\\|";
        }
        for(int i = 0;i < 26; i++){
            str+=char('A'+i);
            str+="\\|";
        }
        if(k==1){
            for(int i = 0;i <= 9; i++){
                str+=char('0'+i);
                str+="\\|";
            }
        }
        str+="_";
        str+="\\)";
    }
    str+="\\*";
    return str;
}
std::string Number(){
//  digit+ (.digit+)?(E(+|–)?digit+)?
    std::string str;
    str+="\\(";
    for(int i = 0;i < 9; i++){
        str+=char('0'+i);
        str+="\\|";
    }
    str+="9\\)\\+";
    str+="\\(.";
    str+="\\(";
    for(int i = 0;i < 9; i++){
        str+=char('0'+i);
        str+="\\|";
    }
    str+="9\\)\\+";
    str+="\\)\\?";
    str+="\\(E\\(+\\|-\\)\\?";
    str+="\\(";
    for(int i = 0;i < 9; i++){
        str+=char('0'+i);
        str+="\\|";
    }
    str+="9\\)\\+\\)\\?";
    return str;
}
std::string Operator(){
    /*
    算术运算符（+, -, *, /, %）
    关系运算符（==, !=, <=, >=, <, >）
    逻辑运算符（&&, ||, !）
    位运算符（&, |, ^, ~, <<, >>)
    赋值运算符（=, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=）
    其他运算符（?, :, ++, --, ->, .）
    */
    std::string str;
    str="+\\|-\\|*\\|/\\|%\\|==\\|!=\\|<=\\|>=\\|<\\|>\\|&&\\|||\\|!\\|&\\||\\|^\\|<<\\|>>\\|=\\|+=";
    str+="\\|-=\\|*=\\|/=\\|%=\\|&=\\||=\\|^=\\|<<=\\|>>=";
    str+="\\|?\\|:\\|++\\|--\\|->\\|.";
    return str;
}
std::string Delimiter(){
    std::string str;
    str="(\\|)\\|,\\|[\\|]\\|;\\|{\\|}\\|\"\\|:\\|'\\|#";
    return str;
}

void init(){
    pri['*']=3;
    pri['+']=3;
    pri['?']=3;
    pri['&']=2;
    pri['|']=1;
    pri['(']=0;
    v.push_back(cal(Keyword()));
    v.push_back(cal(Number()));
    v.push_back(cal(Operator()));
    v.push_back(cal(Delimiter()));
    v.push_back(cal(" \\*"));
}
int main(){
    auto time = clock();
   // freopen("Graph.txt","w",stdout);
    init();
    std::ofstream file_out("DFA.out");
    file_out<<dfastates.size()<<"\n";
    for(auto i:dfastates){
        auto [isEnd,closure,g] = i;
        file_out<<isEnd<<"\n";
        file_out<<closure.size()<<"\n";
        for(auto j:closure)
            file_out<<j<<" ";
        file_out<<"\n";
        file_out<<g.size()<<"\n";
        for(auto j:g)
            file_out<<static_cast<int>(j.first)<<" "<<j.second<<" ";
        file_out<<"\n";
    }
    file_out<<v.size()<<"\n";
    for(auto i:v){
        auto [startstate, endstates, terminator] = i;
        file_out<<startstate<<"\n";
        file_out<<endstates.size()<<"\n";
        for(auto j:endstates)
            file_out<<j<<" ";
        file_out<<"\n"<<terminator.size()<<"\n";
        for(auto j:terminator)
            file_out<<static_cast<int>(j)<<" ";
        file_out<<"\n";
    }
    std::cerr<<clock()-time<<"ms\n";
    return 0;
}