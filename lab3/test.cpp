#include <iostream>
using namespace std;

void func(string& str){
    str += ("a" + string("ab"));
}
int main(){
    string str = "c";
    func(str);
    cout<<str;
    return 0;
}