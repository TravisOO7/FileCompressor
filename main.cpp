#include <iostream>
#include <map>
#include <queue>
#include <fstream>
#include <math.h>
using namespace std;

class Node{
    public:
    char data;
    Node* left;
    Node* right;
    int frequency;

    Node(){
    left = NULL;
    right = NULL;
    data = 'X';
    frequency = -1;
    }
};

class HuffmanTree{
    public:
    Node* root;
    map<char, int> hmp;

    map<char, string> address;
    map<string, char> address2;
    map<string, char> address3;

    HuffmanTree(){
    root = NULL;

    }

    void readFile(fstream &file){


        string a;
        while(file.good()){
            getline(file, a);
            for(int i = 0; i < a.length(); i++){
                if(hmp.count(a[i]) > 0){
                    hmp[a[i]]++;
                }else{
                    hmp[a[i]] = 1;
                }
            }

            if(hmp.count('\n') == 0){
                hmp['\n'] = 0;
            }
            hmp['\n']++;

        }
    }

    void createCompressedFile(ifstream &ogFile, ofstream &file){


        string og, s;
        while(ogFile.good()){
            getline(ogFile, og);
            //cout<<"og is "<<og<<endl;

            for(int i = 0; i < og.length(); i++){
                if(address.count(og[i]) > 0){
                    s += address[og[i]];
                }else{
                    //cout<<og[i]<<" was not present in hashmap"<<endl;
                }
            }

            s += address['\n'];
        }

        //cout<<endl<<"String is: "<<s<<endl<<endl;

        while(s.length()%8 != 0){
            s += '0';
        }

        cout<<s.length();

        int power[8] = {1, 2, 4, 8, 16, 32, 64, 128};

        int i = 0;
        while(i < s.length()){
            int a = 8;
            char t;
            int num = 0;
            while(a > 0){
                if(s[i] == '1'){
                    num += power[a-1];
                }
                i++;
                a--;
            }
            t = num;
            file<<t;
        }
    }

    void readCompressedFile(ifstream &file, ofstream &newFile){
        string t = "";
        char* n = new char[3778];
        char k;
        int count = 0;
        while(file.good()){
        file.read(n, 3778);
        for(int i = 0; i < 3778; i++){
            char ch = n[i];
            count++;
            //cout<<ch;
            file.get(ch);
            for(int i = 7; i > -1; i--){

                k = ((ch >> i) & 1) ? '1' : '0';

                t += k;

                if(address2.count(t)>0){
                    newFile<<address2[t];
                    t = "";
                }
            }
        }
        }
        //cout<<"Count is "<<count<<endl;
    }

    void createTree(){
        root = insertNodes();
        findLoc(root, "");
    }

    Node* insertNodes(){
        priority_queue<pair<int, Node*>, vector<pair<int, Node*> >, greater<pair<int, Node*> > > pq;
        map<char, int>::iterator iter;

        for(iter = hmp.begin(); iter != hmp.end(); iter++){

            Node* newNode = new Node();
            newNode->data = iter->first;
            newNode->frequency = hmp[iter->first];
            pq.push({iter->second, newNode});

        }




        while(pq.size() > 1){
            pair<int, Node*> first = pq.top();
            pq.pop();

            pair<int, Node*> second = pq.top();
            pq.pop();

            Node* newNode = new Node();
            newNode->frequency = first.first + second.first;

            newNode->left = first.second;
            newNode->right = second.second;
            pq.push({newNode->frequency, newNode});
        }
        return pq.top().second;

    }

    void findLoc(Node* node, string s){
        if(node->left == NULL && node->right == NULL){
            if(node->data != 'X'){
                address[node->data] = s;
                address2[s] = node->data;
            }
            return;
        }

        findLoc(node->left, s+'0');
        findLoc(node->right, s+'1');
    }

    void levelwisetraversal(){
        Node* temp = root;
        queue<Node*> q1;
        queue<Node*> q2;

        q1.push(temp);
        while(!q1.empty()){
            temp = q1.front();
            q1.pop();
            cout<<temp->data<<" "<<temp->frequency<<" ";
            if(temp->data != 'X'){
                cout<<address[temp->data];
            }
            cout<<endl;

            if(temp->left != NULL){
                q2.push(temp->left);
            }

            if(temp->right != NULL){
               q2.push(temp->right);
            }



            if(q1.empty()){
                cout<<"***************************************************"<<endl;
                while(!q2.empty()){
                    temp = q2.front();
                    q1.push(temp);
                    q2.pop();
                }
            }
        }

    }

    void saveHashmap(ofstream &file){
        char j = 222;
        file << j;
        for(map<string, char> :: iterator i = address2.begin(); i != address2.end(); i++){
            file<<i->first;
            file<<i->second;
            file<<j;
        }
        file<<j;
    }

    void recreateHashmap(ifstream &file){
        string k;
        char r;
        file.read(&r, 1);

        char read_char;
        while(true){
            file.read(&read_char,1);

            if(read_char == r){
                if(k.length() == 0){
                    break;
                }else{
                    address3[k.substr(0, k.length() - 1)] = k[k.length()-1];
                    k = "";
                }
            }else{
                k += read_char;
            }
        }

        for(map<string, char> :: iterator i = address3.begin(); i != address3.end(); i++){
            cout<<i->first<<' '<<i->second<<endl;
        }
    }


};

int main()
{


    HuffmanTree hft;
    fstream file;
    file.open("sample.txt");
    hft.readFile(file);
    hft.createTree();

    file.close();

    ofstream comp;

    ifstream og;

    og.open("sample.txt");


    comp.open("sample-compressed.txt", ios :: binary | ios :: out | ios :: trunc);

    hft.createCompressedFile(og, comp);



    hft.levelwisetraversal();

    og.close();

    comp.close();

    ifstream domp;
    domp.open("sample-compressed.txt", ios :: binary | ios :: in);

    ofstream newFile;
    newFile.open("sample-recreated.txt", ios::out | ios::trunc);
    hft.readCompressedFile(domp, newFile);
    newFile.close();

    domp.close();

    ofstream h;
    h.open("sample-hashmap.txt", ios :: binary | ios :: trunc);

    hft.saveHashmap(h);

    h.close();

    ifstream hread;
    hread.open("sample-hashmap.txt", ios :: binary | ios :: in);

    hft.recreateHashmap(hread);

    hread.close();



    return 0;

}
