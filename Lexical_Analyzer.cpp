#include<bits/stdc++.h>
#pragma GCC optimize(2)
struct Dfastate{
    bool isEnd;
    std::set<int> closure;
    std::map<char,int> g;
    Dfastate(){
        isEnd = false;
    }
};
std::vector<Dfastate> dfastates;
struct DFA{
    DFA(){}
    int startstate;
    std::set<int> endstates;
    std::set<char> terminator;
};
int find(const std::string& s, const DFA& dfa, int index){
    int pos = dfa.startstate;
    int last = -1;
    for(int i = index; i < s.size(); i++){
        if(dfastates[pos].g.count(s[i])){
            pos = dfastates[pos].g[s[i]];
        }
        else{
            break;
        }
        if(dfastates[pos].isEnd)
            last = i;
    }
    return last;
}
std::vector<DFA> v;
std::unordered_set<std::string> keyword;
std::string name[]{"Keyword","Number","Operator","Delimiter"};
int keyword_num = 0;
void init(){
    std::ifstream file("Keyword.txt");
    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            keyword.insert(word);
        }
        file.close();
    } else {
        std::cerr << "Failed to open file!" << std::endl;
    }
    std::ifstream file_DFA("DFA.out");
    int n;
    file_DFA >> n;
    for(int i = 0; i < n; i++){
        Dfastate dfastate;
        file_DFA >> dfastate.isEnd;
        int m, x, y; 
        file_DFA >> m;
        for(int j = 0; j < m; j++)
            file_DFA >> x, dfastate.closure.insert(x);
        file_DFA >> m;
        for(int j = 0; j < m; j++){
            file_DFA >> y >> x;
            dfastate.g[char(y)] = x;
        }
        dfastates.push_back(dfastate);
    }
    file_DFA >> n;
    for(int i = 0; i < n; i++){
        DFA dfa;
        file_DFA >> dfa.startstate;
        int m, x;
        file_DFA >> m;
        for(int j = 0; j < m; j++){
            file_DFA >> x;
            dfa.endstates.insert(x);
        }
        file_DFA >> m;
        for(int j = 0; j < m; j++){
            file_DFA >> x;
            dfa.terminator.insert(char(x));
        }
        v.push_back(dfa);
    }
}
bool cmp(std::pair<std::string,int> a,std::pair<std::string,int> b){
    return a.second<b.second;
}
int main(){
    auto time = clock();
    init();
    std::string op;
    std::string str;
    std::cin>>op;
    std::string path="sample_";
    path = path + op + ".in";
    std::ifstream file(path); 
    path = "output_" + op + ".out";
    std::ofstream file_out(path);
    int line = 0, idx = 0;
    std::unordered_map<std::string,int> table;
    std::vector<::std::string> number;
    std::ofstream file_word("word.out");
    while(std::getline(file,str)){
        line++;
        int pos = 0, last = -1;
        while(pos < str.size()){
            if(str[pos] == '\t'){
                pos++;
                continue;
            }
            if(str[pos] == ' '){
                pos++;
                continue;
            }
            if(str[pos]=='"'){
                int index = -1;
                std::string tmp;
                tmp.push_back('"');
                for(int j = pos + 1;j < str.size(); j++){
                    tmp.push_back(str[j]);
                    if(str[j]=='"'){
                        index = j;
                        break;
                    }
                }
                if(index != -1){
                    file_out << "<string," <<tmp<< ">" << std::endl;
                    pos = index + 1 ;
                    continue;
                }
            }
            int res = -1, id = -1;
            std::string tmp;
            if(pos + 1 < str.size()){
                if(str[pos] == '/' && str[pos + 1] == '/'){
                    for(int i = pos; i < str.size(); i++){
                        tmp += str[i];
                    }
                    file_out << "<Comment," << tmp << ">" << std::endl;
                    break;
                }
                if(str[pos] == '\\' && str[pos+1] == 'n'){
                    file_out<< "<escape character,\\n>"<<"\n";
                    pos+=2;
                    continue;
                }
            }
            for(int i = 0; i < v.size(); i++){
                int index = find(str, v[i], pos);
                if(index > res)
                    res = index, id = i;
            }
            if(id == -1){
                std::string s = str.substr(pos);
                file_out << "Error in Line " <<line<<" :"<< s << "\n";
                break;
            }
            for(int i = pos; i <= res; i++)
                tmp += str[i];
            if(id >= 4 ){
                file_out << "Error in Line " <<line<<" :"<< tmp << "\n";
                last = id;
                pos = res + 1;
                continue;
            }
            if(id < 4){
                file_word<<tmp<<" ";
                if(id == 0){
                    if(keyword.count(tmp))
                        file_out << "<" << name[id] << "," << tmp << ">" << std::endl;
                    else{
                        if(!table.count(tmp))
                            table[tmp]=++idx;
                        file_out << "<" << "identifier" << "," << tmp << ">" << std::endl;
                    }
                }
                else{
                    if(id==1){
                        number.push_back(tmp);
                    }
                    file_out << "<" << name[id] << "," << tmp << ">" << std::endl;
                }
            }
            last = id;
            pos = res + 1;
        }
        file_word<<"\n";
    }
    std::vector<std::pair<std::string,int>> temp;
    for(auto [i,j]:table){
        temp.push_back({i,j});
    }
    sort(temp.begin(),temp.end(),cmp);
    std::ofstream file_Identifier("Identifier.out");
    for(auto [i,j]:temp)
        file_Identifier<<i<<"\n";
    std::ofstream file_number("number.out");
    for(auto i:number)
        file_number<<i<<"\n";
    file.close(); 
    file_out.close(); 
    file_Identifier.close();
    //std::cerr << clock() - time << "ms\n";
    return 0;
}
