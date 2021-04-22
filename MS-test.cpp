#include<iostream>
#include<vector>
#include<map>
#include<unordered_map>
#include<queue>
#include<stack>

using namespace std;

class task_2{
public:
    task_2(){
    }

    int solution(int N, vector<int> &A, vector<int> &B){
        unordered_map<int, int> log;
        priority_queue<int> edge;

        for(auto const &a: A){
            if(log.find(a) != log.end()){
                log[a]++;
            }
            else{
                log[a]=1;
            }
        }

        for(auto const &b: B){
            if(log.find(b) != log.end()){
                log[b]++;
            }
            else{
                log[b]=1;
            }
        }

        for(auto const &p: log){
            edge.emplace(p.second);
        }

        int rst=0, val=N;
        while(!edge.empty()){
            rst+=edge.top() * (val--);
            edge.pop();
        }

        return rst;
    }

    ~task_2(){}
};

class task_3{
public:
    task_3(){
    }

    std::string solution(string &S){
        vector<char> rst_str;

        // 0 for empty, 1 for A at top, 2 for B at top, 3 for C at top, 4 for D at top, 5 for abnormal conditon
        int state=0;

        for(auto const &ch: S){
            if(state == 0){
                rst_str.push_back(ch);
                switch(ch){
                    case 'A':state=1;break;
                    case 'B':state=2;break;
                    case 'C':state=3;break;
                    case 'D':state=4;break;
                    default:state=5;break;
                }

            }
            else{
                if(ch == 'A'){
                    if(state == 2){
                        rst_str.pop_back();

                        if(rst_str.empty()){
                            state=0;
                            continue;
                        }
                        else{
                            switch(rst_str[rst_str.size()-1]){
                                case 'A':state=1;break;
                                case 'B':state=2;break;
                                case 'C':state=3;break;
                                case 'D':state=4;break;
                                default:state=5;break;
                            }
                        }
                    }
                    else{
                        rst_str.push_back(ch);
                        state=1;
                    }
                }
                else if(ch == 'B'){
                    if(state == 1){
                        rst_str.pop_back();

                        if(rst_str.empty()){
                            state=0;
                            continue;
                        }
                        else{
                            switch(rst_str[rst_str.size()-1]){
                                case 'A':state=1;break;
                                case 'B':state=2;break;
                                case 'C':state=3;break;
                                case 'D':state=4;break;
                                default:state=5;break;
                            }
                        }
                    }
                    else{
                        rst_str.push_back(ch);
                        state=2;
                    }
                }
                else if(ch == 'C'){
                    if(state == 4){
                        rst_str.pop_back();

                        if(rst_str.empty()){
                            state=0;
                            continue;
                        }
                        else{
                            switch(rst_str[rst_str.size()-1]){
                                case 'A':state=1;break;
                                case 'B':state=2;break;
                                case 'C':state=3;break;
                                case 'D':state=4;break;
                                default:state=5;break;
                            }
                        }
                    }
                    else{
                        rst_str.push_back(ch);
                        state=3;
                    }
                }
                else if(ch == 'D'){
                    if(state == 3){
                        rst_str.pop_back();

                        if(rst_str.empty()){
                            state=0;
                            continue;
                        }
                        else{
                            switch(rst_str[rst_str.size()-1]){
                                case 'A':state=1;break;
                                case 'B':state=2;break;
                                case 'C':state=3;break;
                                case 'D':state=4;break;
                                default:state=5;break;
                            }
                        }
                    }
                    else{
                        rst_str.push_back(ch);
                        state=4;
                    }
                }
                else{
                    rst_str.push_back(ch);
                    state=5;
                }
            }
        }

        std::string res;
        res.insert(res.begin(), rst_str.begin(), rst_str.end());
        return res;
    }

    ~task_3(){}
};

int main(int argc, char* argv){
    // task_2* t = new task_2;
    // int a[] = {1, 3};
    // int b[] = {2, 4};

    // vector<int> A(a, a+2);
    // vector<int> B(b, b+2);
    // int rst = t->solution(4, A, B);

    // cout<<rst<<endl;
    // delete t;
    task_3* t = new task_3;
    string ss = "";

    string r = t->solution(ss);
    cout<<r<<endl;
    return 0;
}