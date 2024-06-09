#include<bits/stdc++.h>
using namespace std;
map<pair<string,string>,pair<string,string> > mp;
int main(){
    ifstream file_goto("goto.out");
    ifstream file_s("s.out");
    vector<string> st;
    string str;
    while(getline(file_s,str)){
        st.push_back(str);
    }
    while(getline(file_goto,str)){
        st.push_back(str);
    }
    ifstream file_action("action.out");
    int maxn = 0;
    string s1,s2,s3,s4;
    while(file_action>>s1>>s2>>s3>>s4){
        mp[{s1,s2}]={s3,s4};
    }
    ofstream output("table.out");
    output<<"state\t";
    for(auto i:st)
        output<<i<<"\t";
    output<<"\n";
    for(int i = 0;i <= 200; i++){
        //cerr<<i<<"\n";
        string s = to_string(i);
        bool flag = 0;
        for(auto j:st){
            if(mp.count({s,j})){
                flag = 1;
            }
        }
        if(flag){
            output<<i<<"\t";
            for(auto j:st){
                if(mp.count({s,j})){
                    if(mp[{s,j}].first=="goto"){
                        output<<mp[{s,j}].second<<"\t";
                    }
                    else{
                        output<<mp[{s,j}].first<<mp[{s,j}].second<<"\t";
                    }
                }
                else{
                    output<<"error\t";
                }
            }
            output<<"\n";
        }
    }
}
