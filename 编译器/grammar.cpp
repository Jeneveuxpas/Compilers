#include<bits/stdc++.h>
#define State std::pair<std::vector<std::string>, int>
const int N = 1e5+5;
std::set<State> start;
std::pair<std::vector<std::string>,int> ACC;
std::map<std::string,std::vector<std::vector<std::string>> > grammar;
std::set<std::pair<std::vector<std::string>,int>> state;
std::set<std::string> Terminals;
std::map<std::string,std::set<std::string> > First,Follow;
void find_first(std::string s){
    //std::cout<<s<<" "<<Terminals.count(s)<<"\n";
    if(Terminals.count(s)){
        First[s].insert(s);
        return;
    }
    int ok = 0;
    for(auto i:grammar[s]){
        int flag = 0;
        for(auto j:i){
            if(j==s){
                flag = 1;
                break;
            }
            //  cout<<s<<" "<<j<<"\n";
            if(First.count(j) == 0)
                find_first(j);
            for(auto t:First[j])
                if(t!="epsilon")
                    First[s].insert(t);//,std::cout<<s<<"<-"<<t<<"\n";
            if(First[j].count("epsilon") == 0){
                flag = 1;
                break;
            }
        }
        //std::cout<<s<<" "<<flag<<"\n";
        ok|=flag;
        if(!flag)First[s].insert("epsilon"); 
    }
    if(!ok&&grammar[s].size())
        First[s].insert("epsilon");
}
std::map<std::string,std::vector<std::string> > g;
std::map<std::string,int> dfn,low,vis,st;
std::vector<std::set<std::string> > first;
int tim = 0, num = 0;
std::stack<std::string> s;
void trajan(std::string x){
	dfn[x]=low[x]=++tim;
	s.push(x);
	vis[x]=1;
	for(auto v : g[x]){
		if(!dfn.count(v)){
			trajan(v);
			low[x]=std::min(low[x],low[v]);
		} 
		else if(vis.count(v)){
			low[x]=std::min(low[x],dfn[v]);
		}
	}
    std::set<std::string> tmp;
	if(dfn[x]==low[x]){
		std::string y;
		num++;
        tmp.clear();
		while(x!=y){
			y=s.top();
			s.pop();
			vis.erase(y);
			st[y]=num;
            for(auto k:Follow[y]){
                    tmp.insert(k);
                //std::cout<<k<<"+"<<y<<"\n";
            }
		}
        first.push_back(tmp);
	}
}
std::vector<std::pair<std::string,std::string>> edge;
std::vector<int> G[N];
int in[N];
void topo(){
	std::queue<int> q;
	for(int i=1;i<=num;i++)
	    if(!in[i])
	        q.push(i);
	while(!q.empty()){
		int x=q.front();q.pop();
		for(auto v:G[x]){
            for(auto t:first[x])
                first[v].insert(t);
			in[v]--;
			if(!in[v]){
				q.push(v);
			}
		}
	}
}
void follow(){
    std::set<std::string> tmp;
    for(auto [s,j]:grammar){
        tmp.insert(s);
        for(auto k:j){
            for(int i = k.size() - 1; i > 0; i--){
                for(int j = i-1;j >= 0; j--){
                //std::cout<<k[i]<<" "<<k[i-1]<<"\n";
                    if(Terminals.count(k[i]))
                        Follow[k[j]].insert(k[i]);
                    for(auto t:First[k[i]]){
                        if(!Terminals.count(k[j]))
                            Follow[k[j]].insert(t);
                        //std::cout<<k[i]<<" "<<k[i-1]<<"\n";
                    }
                    if(!First[k[j]].count("epsilon"))
                        break;
                }
            }
            //std::cout<<"\n";
            for(int i = k.size() - 1; i >= 0; i--){
                if(s!=k[i]&&!Terminals.count(s)&&!Terminals.count(k[i]))
                    g[s].push_back(k[i]),tmp.insert(k[i]),edge.push_back({s,k[i]});//,std::cout<<s<<"->"<<k[i]<<"\n";
                //std::cout<<k[i]<<" "<<First[k[i]].count("epsilon")<<"\n";
                if(First[k[i]].count("epsilon") == 0)
                    break;
            }
        }
    }  
    // for(auto [i,j]:Follow){
    //     std::cout<<i<<":";
    //     for(auto k:j)
    //         std::cout<<k<<" ";
    //     std::cout<<"\n";
    // }
    // std::cout<<"------------------\n";
    std::set<std::string>temp;
    first.push_back(temp);
    for(auto s:tmp){
        if(!dfn.count(s))
            trajan(s);
    }
    for(auto [i,j]:edge){
        //std::cout<<i<<"->"<<j<<"\n";
        int x = st[i], y = st[j];
        if(x!=y)
            G[x].push_back(y),in[y]++;//,std::cout<<x<<"->"<<y<<"\n";
    }
    topo();
    // for(int i=0;i<first.size();i++){
    //     for(auto j:first[i])
    //         std::cout<<j<<"-";
    //     std::cout<<"\n";
    // }
    for(auto s:tmp){
        //std::cout<<s<<" ";
        for(auto x:first[st[s]]){
            Follow[s].insert(x);
        }
    }
    std::ofstream file_Follow("follow.out");
    file_Follow<<"Follow:\n";
    for(auto &[i,j]:Follow){
        if(j.count("epsilon"))
            j.erase("epsilon");
        file_Follow<<i<<":";
        for(auto k:j)
            file_Follow<<k<<" ";
        file_Follow<<"\n";
    }
}
int sum = 0;
std::set<std::set<State>>  states;
std::map<std::set<State>,int> id;
std::map<std::pair<std::set<State>, std::string>, std::pair<std::set<State>, std::string> > Action;
std::set<State> epcloure(std::set<State> u){
    std::queue<State> q;
    std::set<State> tmp = u;
    for(auto i:u)
        q.push(i);
    while(q.size()){
        auto [x,y] = q.front(); q.pop();
        while(y<x.size()){
            if(x[y]=="epsilon")
                y++;
            else break;
        }
        if(y<x.size()){
            std::string now = x[y];
            for(auto i:grammar[now]){
                std::vector<std::string> temp;
                temp.push_back(now);
                for(auto j:i)
                    temp.push_back(j);
                std::pair<std::vector<std::string>,int> v = {temp,1};
                while(v.second<v.first.size()){
                    if(v.first[v.second]=="epsilon")
                        v.second++;
                    else break;
                }
                if(!tmp.count(v)){
                    q.push(v);
                    tmp.insert(v);
                }
            }
        }
    }
    // for(auto [i,j]:tmp){
    //     int cnt = j;
    //     for(auto k:i){
    //         std::cout<<k<<" ";
    //         cnt--;
    //         if(cnt==0)std::cout<<".";
    //     }
    //     std::cout<<"\n";
    // }
    // std::cout<<"++++++++++\n";
    return tmp;
}
void next(std::set<State> u){
    if(states.count(u))return;
    states.insert(u);
    std::map<std::pair<std::set<State> ,std::pair<std::string,std::string> >, std::set<State> > tmp;
    for(auto p:u){
        auto &[x,y] = p;
        while(y<x.size()){
            if(x[y]=="epsilon"){
                y++;
            }
            else
                break;
        }
        if(y==x.size()){
            for(auto i:Follow[x[0]]){
                if(i=="epsilon"){
                    //std::cout<<x[0]<<"\n";
                    continue;
                }
                if(p == ACC){
                    tmp[{u,{i,"acc"}}].insert({x,y});
                    // std::cerr<<i<<" ";
                    // for(auto o:u){
                    //     for(auto p:o.first)
                    //         std::cerr<<p<<" ";
                    //     std::cerr<<o.second<<" ";
                    //     std::cerr<<"\n";
                    // }
                    }
                else 
                    tmp[{u,{i,"r"}}].insert({x,y});
            }
            //std::cout<<"\n";
        }    
        else{
            auto s = x[y];
            if(Terminals.count(s)){
                auto t = p;
                t.second++;
                if(t == ACC)
                    tmp[{u,{s,"acc"}}].insert({x,y+1});
                else
                    tmp[{u,{s,"s"}}].insert({x,y+1});            
            }
            else 
                tmp[{u,{s,"goto"}}].insert({x,y+1});
        }
    }
    std::map<std::pair<std::set<State> ,std::pair<std::string,std::string> >, std::set<State> > temp;
    for(auto [i,j]:tmp){
        std::set<State> now = epcloure(j);
        for(auto k:now)
            temp[i].insert(k);
    }
    for(auto [i,j]:temp){
        Action[{i.first,i.second.first}] = {j,i.second.second};
        if(i.second.second=="goto"||i.second.second=="s"){
            next(j);
        }
    }
}
void solve(std::vector<std::pair<int, std::string> > str,std::vector<std::string> all){
    std::ofstream file_output("output.txt");
    std::queue<std::pair<int, std::string> >q;
    for(auto i:str)
        q.push(i);
    q.push({0,"$"});
    std::vector<std::string> st_string;
    st_string.push_back("$");
    std::stack<std::set<State>> st_state;
    st_state.push(start);
    while(true){
        if(st_state.empty()){
            file_output<<"state stack is empty\n";
            return;
        }
        if(q.empty()){
            file_output<<"Queue is empty\n";
            return;
        }
        auto now = st_state.top();
        auto symbol = q.front().second;
        // for(auto i : now){
        //     auto [x,y] = i;
        //     int cnt = y;
        //     for(auto i:x){
        //         file_output<<i<<" ";
        //         cnt--;
        //         if(cnt==0)file_output<<".";
        //     }
        //     file_output<<"\n";
        // }
        if(Action.count({now,symbol}) == 0){
            file_output<<"Error in Line : "<< q.front().first<<"\n";
            file_output<<"invalid syntax in expression\n";
            int cnt = 0;
            bool flag = 0, ok = 0;
            for(int i=0;i<(int)all[q.front().first-1].size();i++){
                std::string ns;
                for(int j = i; j <(int) i + symbol.size();j++)
                    ns+=all[q.front().first-1][j];
                if(ns==symbol){
                    if(ok)ok = 0;
                    else ok = 1;
                    break;
                }
                if(all[q.front().first-1][i]!='\t'&&all[q.front().first-1][i]!=' '){
                    ok = 1;
                }
            }
            //std::cerr<<ok<<"\n";
            if(ok&&q.front().first!=1)q.front().first--;
            file_output<<all[q.front().first-1]<<"\n";
            if(ok){
                for(int i = 0;i<all[q.front().first-1].size();i++)
                    file_output<<" ";
                file_output<<"^^^\n";
                if(Action.count({now,";"})){
                     file_output<<"need: ;\n";
                     return;
                }
                for(auto nex:Terminals){
                    if(Action.count({now,nex})){
                        file_output<<"need: "<<nex<<"\n";
                        return;
                    }
                }
                return;
            }
            for(int i=0;i<(int)all[q.front().first-1].size();i++){
                std::string ns;
                for(int j = i; j <(int) i + symbol.size()-1;j++)
                    ns+=all[q.front().first-1][j];
                if(ns==symbol){
                    file_output<<"^^^ \n";
                    if(Action.count({now,";"})){
                        file_output<<"need: ;\n";
                        return;
                    }
                    file_output<<"need: ";
                    for(auto nex:Terminals){
                        if(Action.count({now,nex})){
                            file_output<<"need: "<<nex<<"\n";
                            return;
                        }
                    }

                    return;
                }
                file_output<<" ";
            }
            return;
        }
        auto action = Action[{now,symbol}];
        for(auto i:st_string)
            file_output<<i<<" ";
        file_output<<"\t";  
        if(action.second=="s"){
            st_string.push_back(symbol);
            st_state.push(action.first);
            file_output<<"移入\n";
            q.pop();
        }
        else if(action.second=="r"){
            auto s = (*action.first.begin());
            auto S = s.first[0];
            for(int i = s.first.size() - 1; i > 0; i--){
                if(st_string.size()&&st_state.size()&&s.first[i]==st_string.back()){
                    st_string.pop_back();
                    st_state.pop();
                }
                else if(s.first[i]=="epsilon"){
                    continue;
                }
                else{
                    file_output<<"Error\n";
                    return;
                }
            }
            file_output<<"归约/";
            file_output<<S<<"->";
            for(int i = 1; i < s.first.size(); i++)
                file_output<<s.first[i]<<" ";
            file_output<<"\n";
            st_string.push_back(S);
            if(Action.count({st_state.top(),st_string.back()})&&Action[{st_state.top(),st_string.back()}].second=="goto")
                st_state.push(Action[{st_state.top(),st_string.back()}].first);
            else{
                file_output<<"Can't find Goto table\n";
                return;
            }
        }
        else if(action.second=="acc"){
            file_output<<"接受!\n";
            std::cerr<<"successful\n";
            return;
        }
        else{
            file_output<<"Error on goto\n";
            return;
        }
    }
}

int main(){
    std::ifstream file("grammar.txt");
    std::string str;
    bool ok = 0;
    while(std::getline(file,str)){
        std::string s,t;
        int index;
        for(int i = 0;i < str.size(); i++){
            if(str[i]=='\\'&&str[i+1]=='-'){
                index = i + 1;
                break;
            }
            if(str[i]!=' ')
                s+=str[i];
        }
        std::vector<std::string> tmp;
        for(int i = index + 2; i < str.size(); i++){
            if(str[i]=='\t')continue;
            if(str[i]=='\\'&&str[i+1]=='|'){
                if(t.size())
                    tmp.push_back(t),t.clear();
                if(tmp.size())
                    grammar[s].push_back(tmp);
                tmp.clear();
                i++;
            }
            else if(str[i]==' '){
                if(t.size())
                    tmp.push_back(t),t.clear();
            }
            else{
                t+=str[i];
            }
        }
        if(t.size())tmp.push_back(t);
        if(tmp.size())grammar[s].push_back(tmp);
        if(!ok){
            Follow[s].insert("$");
            std::vector<std::string> now;
            now.push_back(s);
            for(auto i:tmp)
                now.push_back(i);
            start.insert({now,1});
            ACC = {now,now.size()};
            // std::cerr<<"ACC:";
            // for(auto i:now)
            //     std::cerr<<i<<" ";
            // std::cerr<<"\n";
            ok = 1;
        }
    }
    // for(auto [i,j]:grammar){
    //     std::cout<<i<<":";
    //     for(auto k:j){
    //         for(auto l:k){
    //             std::cout<<l<<":";
    //         }
    //         std::cout<<"\n";
    //     }
    // }
    std::ifstream file_word("Terminals.txt");
    while(file_word>>str){
        Terminals.insert(str);
        // std::cout<<str<<"\n";
    }
    std::ifstream file_number("number.out");
    while(file_number>>str){
        Terminals.insert(str);
        //std::cout<<str<<"\n";
        std::vector<std::string> tmp;
        tmp.push_back(str);
        grammar["num"].push_back(tmp);
    }
    std::ifstream file_string("string.out");
    while(getline(file_string,str)){
        Terminals.insert(str);
        std::vector<std::string> tmp;
        tmp.push_back(str);
        grammar["string'"].push_back(tmp);
 
    }
    std::ifstream file_Identifier("Identifier.out");
    while(file_Identifier>>str){
        Terminals.insert(str);
        //std::cout<<str<<"\n";
        std::vector<std::string> tmp;
        tmp.push_back(str);
        grammar["id"].push_back(tmp);
    }
    for(auto [i,j]:grammar){
        //std::cout<<i<<"\n";
        if(First.count(i))continue;
        find_first(i);
    }
    std::ofstream file_First("First.out");
    file_First<<"First:\n";
    for(auto [i,j]:First){
        file_First<<i<<":";
        for(auto k:j)
            file_First<<k<<" ";
        file_First<<"\n";
    }
    // std::cout<<"--------------------\n";
    follow();
    start = epcloure(start);
    next(start);
    std::ofstream file_action("action.out");
    std::ofstream file_goto("goto.out");
    std::ofstream file_s("s.out");
    for(auto [i,j]:Action){
        if(!id.count(i.first))
            id[i.first] = id.size() + 1;
        if(!id.count(j.first))
            id[j.first] = id.size() + 1;
    }
    std::set<std::string> goto_table,s_table;
    for(auto [i,j]:Action){
        if(j.second=="goto")
            goto_table.insert(i.second);
        else{
            s_table.insert(i.second);
        }
        file_action<<id[i.first]<<"  "<<i.second<<"  "<<j.second<<"  "<<id[j.first]<<"\n";
    }
    for(auto i : goto_table){
        file_goto<<i<<"\n";
    }
    for(auto i : s_table){
        file_s<<i<<"\n";
    }
    std::string t;
    std::ifstream file_grammar("word.out");    
    std::vector<std::pair<int,std::string> >  temp;
    std::vector<std::string> all;
    std::ifstream file_sample("sample.txt");
    while(std::getline(file_sample,t)){
        std::cerr<<t<<"\n";
        if(t[0]!='/')
            all.push_back(t);
    } 
    while(std::getline(file_grammar,t)){
        std::string s;
        int line = 0, index = 0;
        for(int i = 0; i< t.size() ; i++){
            if(t[i]==' '){
                index = i;
                break;
            }
            line = line * 10 +t[i] - '0';
        }
        for(int i = index + 1;i<t.size();i++)
            s+=t[i];
        temp.push_back({line,s});
    }
    solve(temp,all);
    return 0*0;
}