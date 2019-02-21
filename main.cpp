#include <iostream>
#include <fstream>
#include <list>
#include <math.h>
#include <vector>
#include <stack>

using namespace std;

struct myString{
    string word="";
    long long int hash=-1;
    long long int hash2=-1;
    myString(string word,long long int hash,long long int hash2){
        this->word=word;
        this->hash=hash;
        this->hash2=hash2;
    }
};
int p=37,q=99881,p2=23,q2=99877;
string message;
int lengthOfMesaage=-1;
int exponential[1000][2]={-1};
long long int messageHash[1000][2];
list<myString> dictionary[1000];
int numOfSentence[1001];
bool isFound[1001]={false};
void hashMessage(){
    exponential[0][0]=1;
    exponential[0][1]=1;
    messageHash[0][0] = message[0] % q;
    messageHash[0][1] = message[0] % q2;
    for(int i=1;i<lengthOfMesaage;i++){
        exponential[i][0] = (exponential[i-1][0]*p)%q;
        messageHash[i][0]=(messageHash[i-1][0] + message[i]*exponential[i][0])%q;
        exponential[i][1] = (exponential[i-1][1]*p2)%q2;
        messageHash[i][1]=(messageHash[i-1][1] + message[i]*exponential[i][1])%q2;
    }
};
void hash(const string& word){// p üssünü aldığım q mod aldığım
    long long int hash=0;
    long long int hash2=0;
    int length = word.length();
    for(int i=0;i<length;i++){
        if(exponential[i][0]==-1){
            exponential[i][0]=exponential[i-1][0]*p%q;
        }
        hash = (word[i]*exponential[i][0] + hash)%q;
        if(exponential[i][1]==-1){
            exponential[i][1]=exponential[i-1][1]*p2%q2;
        }
        hash2 = (word[i]*exponential[i][1] + hash2)%q2;
    }
    dictionary[length-1].push_back(myString(word,hash,hash2));
};
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project5 [input_file] [output_file]" << endl;
        return 1;
    }
    ifstream input(argv[1]);
    ofstream output(argv[2]);
    int n;
    input >> message >> n;
    lengthOfMesaage = message.length();
    hashMessage();
    for(int i=0;i<n;i++){
        string s;
        input >> s;
        ::hash(s);
    }
    vector<int> breakPoints(1);
    numOfSentence[0]=1;
    for(int i=0;i<lengthOfMesaage;i++){
        int count = breakPoints.size();
        for(int j=0;j<count;j++){
            int startIndex = breakPoints[j];
            int length = i-startIndex;
            for(list<myString> :: iterator itr = dictionary[length].begin();itr!=dictionary[length].end();itr++){
                long long int messageH;
                long long int messageH2;
                if(startIndex==0){
                    messageH = messageHash[i][0];
                    messageH2 = messageHash[i][1];
                }
                else{
                    messageH = (messageHash[i][0] - messageHash[startIndex-1][0])%q;
                    if(messageH<0)
                        messageH+=q;
                    messageH2 = (messageHash[i][1] - messageHash[startIndex-1][1])%q2;
                    if(messageH2<0)
                        messageH2+=q2;
                }
                if((itr->hash*exponential[startIndex][0])%q==messageH&&(itr->hash2*exponential[startIndex][1])%q2==messageH2){
                    int num = (numOfSentence[startIndex] +numOfSentence[i+1])%1000000007;
                    numOfSentence[i+1]=num;
                    if(!isFound[i+1]){
                        breakPoints.push_back(i+1);
                        isFound[i+1]=true;
                    }
                    break;
                }
            }
        }
    }
    output << numOfSentence[lengthOfMesaage];
    return 0;
}