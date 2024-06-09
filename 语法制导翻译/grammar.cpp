#include<bits/stdc++.h>
#define State std::pair<std::vector<std::string>, int>
const int N = 1e5+5;
std::set<State> start;
std::pair<std::vector<std::string>,int> ACC;
std::map<std::string,std::vector<std::vector<std::string>> > grammar;
std::set<std::pair<std::vector<std::string>,int>> state;
std::set<std::string> Terminals;
std::map<std::string,std::set<std::string> > First,Follow;
std::map<std::vector<std::string>, std::vector<std::vector<std::string> > > sdt;
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
std::vector<std::pair<std::string,std::string> > symtab;
struct node{
    std::string type;
    std::string addr;
    std::string lexeme;
    std::vector<int> truelist, falselist, nextlist;
    int instr, nextstr;
    std::string type_width;
};
std::vector<std::vector<std::string> > answer;
int nums = 0, nextinstr = 0;
void solve(std::vector<std::pair<int, std::string> > str,std::vector<std::string> all){
    std::ofstream file_output("output.txt");
    std::ofstream file_answer("answer.txt");
    std::queue<std::pair<int, std::string> >q;
    for(auto i:str)
        q.push(i);
    q.push({0,"$"});
    std::vector<node> st;
    std::vector<std::string> st_string;
    st_string.push_back("$");
    st.push_back(node());
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
            //file_output<<symbol<<"\n";

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
            std::ofstream error_file("error.txt");
            error_file<<"Error in Line : "<< q.front().first<<"\n";
            error_file<<"invalid syntax in expression\n";
            error_file<<all[q.front().first-1]<<"\n";
            file_output<<"Error in Line : "<< q.front().first<<"\n";
            file_output<<"invalid syntax in expression\n";
            file_output<<all[q.front().first-1]<<"\n";
            if(ok){
                for(int i = 0;i<all[q.front().first-1].size();i++)
                    file_output<<" ",error_file<<" ";
                file_output<<"^^^\n";error_file<<"^^^\n";
                if(Action.count({now,";"})){
                     file_output<<"need: ;\n";
                     error_file<<"need: ;\n";
                     return;
                }
                for(auto nex:Terminals){
                    if(Action.count({now,nex})){
                        file_output<<"need: "<<nex<<"\n";
                        error_file<<"need: "<<nex<<"\n";
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
                    error_file<<"^^^\n";
                    if(Action.count({now,";"})){
                        file_output<<"need: ;\n";
                        error_file<<"need: ;\n";
                        return;
                    }error_file<<"need: ";
                    file_output<<"need: ";
                    for(auto nex:Terminals){
                        if(Action.count({now,nex})){
                            error_file<<"need: "<<nex<<"\n";
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
            st.push_back(node());
            file_output<<"移入\n";
            q.pop();
        }
        else if(action.second=="r"){
            auto s = (*action.first.begin());
            auto S = s.first[0];
            file_output<<"归约/";
            file_output<<S<<"->";
            for(int i = 1; i < s.first.size(); i++)
                file_output<<s.first[i]<<" ";
            node now;
            if(sdt[s.first].size()){
                // for(auto x:sdt[s.first]){
                //     for(auto y:x){
                //         file_output<<y<<" ";
                //     }
                // }
                for(auto x:sdt[s.first]){
                    if(x[0]=="symtab.put"){
                        //file_output<<st[st.size()-2].lexeme<<" "<<st[st.size()-3].type<<"\n";
                        symtab.push_back({st[st.size()-2].lexeme,st[st.size()-3].type});
                    }
                    else if(x[0]=="type"){
                        now.type = x[2];
                    }
                    else if(x[0]== "gen"){
                        // std::cout<<(int)x.size()<<":";
                        // for(auto p:x)std::cout<<p<<" ";
                        // std::cout<<"\n";
                        if((int)x.size()==14){
                            std::vector<std::string> tmp;
                            tmp.push_back(now.addr);tmp.push_back("=");tmp.push_back(st[st.size()-3].addr);tmp.push_back(x[8]);tmp.push_back(st[st.size()-1].addr);
                            answer.push_back(tmp);
                            nextinstr++;
                            // file_answer<<"\t"<<now.addr<<" = "<< st[st.size()-3].addr << " " <<  x[8] << " " <<st[st.size()-1].addr<<";\n";
                        }
                        else if((int)x.size()==10){
                            std::vector<std::string> tmp;
                            tmp.push_back(st[st.size()-4].addr);tmp.push_back("=");tmp.push_back(st[st.size()-2].addr);
                            answer.push_back(tmp);
                            nextinstr++;
                            // file_answer<<"\t"<<st[st.size()-4].addr <<" = " << st[st.size()-2].addr <<"\n"; 
                        }
                        else if((int)x.size()==8){
                            std::vector<std::string> tmp;
                            if(x[6]=="L.type.width"){
                                tmp.push_back(now.addr);tmp.push_back("=");
                                tmp.push_back(st[st.size()-2].addr);tmp.push_back("* 4");
                            }
                            else{
                                tmp.push_back("if");tmp.push_back(st[st.size()-3].addr);tmp.push_back(st[st.size()-2].addr);
                                tmp.push_back(st[st.size()-1].addr);tmp.push_back("goto");
                            }
                            answer.push_back(tmp);
                            nextinstr++;
                        } 
                        else if((int)x.size()==9){
                            std::vector<std::string> tmp;
                            tmp.push_back(now.addr);tmp.push_back("=");tmp.push_back(st.back().lexeme);
                            tmp.push_back("[");tmp.push_back(st.back().addr);tmp.push_back("]");
                            answer.push_back(tmp);
                            nextinstr++;
                        }
                        else if((int)x.size()==4){
                            std::vector<std::string> tmp;
                            tmp.push_back("goto");
                            answer.push_back(tmp);
                            nextinstr++;
                        }
                        else if((int)x.size()==5){
                            std::vector<std::string> tmp;
                            tmp.push_back("goto");tmp.push_back(std::to_string(st[st.size()-6].instr));
                            answer.push_back(tmp);
                            nextinstr++;
                        }
                    }
                    else if(x[2]=="newTemp()"){
                        now.addr = "t" + std::to_string(++nums);
                    }
                    else if(x[0]=="addr"){
                        if(x[2]=="symtab.get"){
                            now.addr = st.back().lexeme;
                        }
                        else if(x[2]=="num.value"){
                            now.addr = st.back().addr;
                        }//{addr = value XX}
                        else if(x[2]=="expr.addr"){
                            now.addr = st[st.size()-2].addr;
                        }
                        else if(x[2]=="value"){
                            now.addr = x[3];
                        }
                        else if(x[2]=="id.lexem"){
                            now.lexeme = st[st.size()-4].addr;
                            // std::cerr<<now.lexeme<<"\n";
                        }
                        else{
                            now.addr = st[st.size()-1].addr;
                        }
                        //file_output<<now.addr<<" ";
                    }
                    else if(x[0]=="lexeme"){
                        now.addr = x[3];
                    }
                    else if(x[0]=="rel.op"){
                        now.addr = x[2];
                    }
                    else if(x[0]=="bool.truelist"){
                        
                        if(x[2]=="nextinstr")
                            now.truelist.push_back(nextinstr);
                        else if(x[2]=="merge(bool1.truelist,bool2.truelist)"){
                            //std::cout<<"-----------------\n";
                            for(auto p:st[st.size()-1].truelist)
                                now.truelist.push_back(p);
                            for(auto p:st[st.size()-4].truelist)
                                now.truelist.push_back(p);
                        }
                        else if(x[2]=="bool2.truelist"){
                            for(auto p:st[st.size()-1].truelist)
                                now.truelist.push_back(p);
                        }
                        else if(x[2]=="bool1.falselist"){
                            for(auto p:st[st.size()-1].falselist)
                                now.truelist.push_back(p);
                        }
                        else if(x[2]=="bool3.truelist"){
                            for(auto p:st[st.size()-2].truelist)
                                now.truelist.push_back(p);
                        }
                    }
                    else if(x[0]=="bool.falselist"){
                        if(x[2]=="nextinstr+1")
                            now.falselist.push_back(nextinstr + 1);
                        else if(x[2]=="bool2.falselist"){
                            for(auto p:st[st.size()-1].falselist)
                                now.falselist.push_back(p);
                        }
                        else if(x[2]=="merge(bool1.falselist,bool2.falselist)"){
                            for(auto p:st[st.size()-1].falselist)
                                now.falselist.push_back(p);
                            for(auto p:st[st.size()-4].falselist)
                                now.falselist.push_back(p);
                        }
                        else if(x[2]=="bool1.truelist"){
                            for(auto p:st[st.size()-1].truelist)
                                now.falselist.push_back(p);
                        }
                        else if(x[2]=="bool3.falselist"){
                            for(auto p:st[st.size()-2].falselist)
                                now.falselist.push_back(p);  
                        }
                    }
                    else if(x[0]=="stmt.nextlist"){
                        // std::cout<<(int)x.size()<<":";
                        // for(auto p:x)std::cout<<p<<" ";
                        // std::cout<<"\n";
                        if(x[2]=="bool.falselist"){
                            now.nextlist = st[st.size()-3].falselist;
                        }
                        else if(x[2]=="stmt2.nextlist")
                            now.nextlist = st[st.size()-1].nextlist;
                        else if(x[2]=="merge(bool.falselist,stmt1.nextlist)"){
                            for(auto p:st[st.size()-4].falselist)
                                now.nextlist.push_back(p);
                            for(auto p:st[st.size()-1].nextlist)
                                now.nextlist.push_back(p);
                        }
                    }
                    else if(x[0]=="stmts.nextlist"){
                        now.nextlist = st[st.size()-1].nextlist;
                    }
                    else if(x[0]=="stmts'.nextlist"){
                        now.nextlist = st[st.size()-1].nextlist;
                    }
                    else if(x[0]=="backpatch"){
                        if(x[2]=="bool.truelist"){
                            if(x[4]=="M.instr"){
                                for(auto tmp_index:st[st.size()-4].truelist){
                                    //std::cerr<<tmp_index<<"\n";
                                    answer[tmp_index].push_back(std::to_string(st[st.size()-2].instr));
                                }
                            }
                            else if(x[4]=="M3.instr"){
                                for(auto tmp_index:st[st.size()-3].truelist){
                                    answer[tmp_index].push_back(std::to_string(st[st.size()-8].instr));
                                }
                            }
                            else if(x[4]=="M1.instr"){
                                for(auto tmp_index:st[st.size()-8].truelist){
                                    //std::cerr<<tmp_index<<"\n";
                                    answer[tmp_index].push_back(std::to_string(st[st.size()-6].instr));
                                }
                            }
                            else if(x[4]=="M2.instr"){
                                for(auto tmp_index:st[st.size()-4].truelist){
                                    answer[tmp_index].push_back(std::to_string(st[st.size()-2].instr));
                                }
                            }
                        }
                        else if(x[2]=="bool.falselist"){
                            for(auto tmp_index:st[st.size()-8].falselist)
                                answer[tmp_index].push_back(std::to_string(st[st.size()-2].instr));
                        }
                        else if(x[2]=="bool1.falselist"){
                            for(auto tmp_index:st[st.size()-4].falselist)
                                answer[tmp_index].push_back(std::to_string(st[st.size()-2].instr));
                        }
                        else if(x[2]=="bool1.truelist"){
                            for(auto tmp_index:st[st.size()-4].truelist)
                                answer[tmp_index].push_back(std::to_string(st[st.size()-2].instr));
                        }
                        else if(x[2]=="stmt.nextlist"){
                            for(auto tmp_index:st[st.size()-3].nextlist)
                                answer[tmp_index].push_back(std::to_string(st[st.size()-2].instr));
                        }
                        else if(x[2]=="N.nextlist"){
                            for(auto tmp_index:st[st.size()-4].nextlist)
                                answer[tmp_index].push_back(std::to_string(st[st.size()-1].nextstr));
                        }
                        else if(x[2]=="stmt1.truelist"){
                            if(x[4]=="M2.instr"){
                                for(auto tmp_index:st[st.size()-7].truelist)
                                    answer[tmp_index].push_back(std::to_string(st[st.size()-5].instr)); 
                            }
                            else{
                                for(auto tmp_index:st[st.size()-1].truelist)
                                    answer[tmp_index].push_back(std::to_string(st[st.size()-6].instr)); 
                            }
                        }
                    }
                    else if(x[0]=="N.nextlist"){
                        now.nextlist.push_back(nextinstr);
                    }
                    else if(x[0]=="M.instr"){
                        now.instr = nextinstr;
                    }
                    else if(x[0]=="stmt'.nextlist"){
                        now.nextlist = st[st.size()-4].falselist;
                    }
                }
            }
            now.nextstr = nextinstr;
            file_output<<"\n";
            for(int i = s.first.size() - 1; i > 0; i--){
                if(st_string.size()&&st_state.size()&&s.first[i]==st_string.back()){
                    st_string.pop_back();
                    st_state.pop();
                    st.pop_back();
                }
                else if(s.first[i]=="epsilon"){
                    continue;
                }
                else{
                    file_output<<"Error\n";
                    return;
                }
            }
            st_string.push_back(S);
            if(Action.count({st_state.top(),st_string.back()})&&Action[{st_state.top(),st_string.back()}].second=="goto")
                st_state.push(Action[{st_state.top(),st_string.back()}].first),st.push_back(now);
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
std::vector<std::vector<std::string>> check(std::string s){
    std::vector<std::vector<std::string>> t;
    std::string tmp;
    std::vector<std::string> temp;
    for(int i = 0; i < s.size(); i++){
        if(s[i]==' '){
            if(tmp.size())
                temp.push_back(tmp);
            tmp.clear();
        }
        else if(s[i]=='\\'&&s[i+1]==';'){
            if(temp.size())
                t.push_back(temp);
            temp.clear();
            i++;
        }
        else{
            tmp.push_back(s[i]);
        }
    }
    return t;
}
void sdt_show(){
    for(auto [i,j]:sdt){
        for(auto k:i)
            std::cerr<<k<<" ";
            std::cerr<<" | ";
        for(auto k:j){
            for(auto l:k)
                std::cerr<<l<<" ";
        }
        std::cerr<<"\n";
    }
}
int main(){
    std::ifstream file("sdt.txt");
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
        std::vector<std::vector<std::string> > tt; 
        for(int i = index + 2; i < str.size(); i++){
            if(str[i]=='\t')continue;
            if(str[i]=='\\'&&str[i+1]=='|'){
                if(t.size())
                    tmp.push_back(t),t.clear();
                if(tmp.size()){
                    grammar[s].push_back(tmp);
                    std::vector<std::string> now;
                    now.push_back(s);
                    for(auto k:tmp)now.push_back(k);
                    // for(auto k:now)std::cerr<<k<<" ";
                    // std::cerr<<"-----";
                    // for(auto x:tt){
                    //     for(auto y:x){
                    //         std::cerr<<y<<" ";
                    //     }
                    // }
                    // std::cerr<<"\n";
                    sdt[now] = tt;
                }
                tt.clear();
                tmp.clear();
                i++;
            }
            else if(str[i]=='\\'&&str[i+1]=='{'){
                int j;
                std::string tmp;
                for(j = i+2 ;j<str.size(); j++){
                    if(str[j]=='\\'&&str[j+1]=='}'){
                        j--;
                        break;
                    }
                    tmp.push_back(str[j]);
                }
                tt = check(tmp);
                i=j+2;
            }
            else if(str[i]==' '){
                if(t.size())
                    tmp.push_back(t),t.clear();
            }
            else{
                t+=str[i];
            }
        }
        if(t.size())
            tmp.push_back(t),t.clear();
        if(tmp.size()){
            grammar[s].push_back(tmp);
            std::vector<std::string> now;
            now.push_back(s);
            for(auto k:tmp)now.push_back(k);
            //for(auto k:now)std::cerr<<k<<" ";
            // std::cerr<<"-----";
            // for(auto x:tt){
            //     for(auto y:x){
            //         std::cerr<<y<<" ";
            //     }
            // }
            // std::cerr<<"\n";
            sdt[now] = tt;
        }
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
    //     std::cout<<i<<"->";
    //     for(auto k:j){
    //         for(auto l:k){
    //             std::cout<<l<<"  ";
    //         }
    //         std::cout<<" | ";
    //     }
    //     std::cout<<"\n";
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
        tmp.clear();
        tmp.push_back("num");
        tmp.push_back(str);
        std::vector<std::vector<std::string> > tmp_num;
        std::vector<std::string> temp_num;
        temp_num.push_back("addr");
        temp_num.push_back("=");
        temp_num.push_back("value");
        temp_num.push_back(str);
        tmp_num.push_back(temp_num);
        sdt[tmp] = tmp_num;
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
        tmp.clear();
        tmp.push_back("id");
        tmp.push_back(str);
        std::vector<std::vector<std::string> > tmp_num;
        std::vector<std::string> temp_num;
        temp_num.push_back("lexeme");
        temp_num.push_back("=");
        temp_num.push_back("value");
        temp_num.push_back(str);
        tmp_num.push_back(temp_num);
        sdt[tmp] = tmp_num;
    }
    for(auto [i,j]:grammar){
        //std::cout<<i<<"\n";
        if(First.count(i))continue;
        find_first(i);
    }
    //sdt_show();
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
    std::ofstream file_answer("answer.txt");
    int tmp_index = 0;
    for(auto i:answer){
        file_answer<<tmp_index++<<"\t";
        for(auto j:i){
            file_answer<<j<<" ";
        }
        file_answer<<"\n";
    }
    file_answer<<tmp_index<<"\n";
    return 0*0;
}