#include <iostream>
#include <map>
#include <queue>
#include <fstream>
#include <math.h>
using namespace std;

#include "createCompressedFile.cpp"
#include "RecreateOriginalFile.cpp"

string fileName(string& filename, string add){
    int i;
    for( i = 0; i < filename.length(); i++){
        if(filename[i] == '.'){
            break;
        }
    }

    return filename.substr(0, i) + add + filename.substr(i, filename.length()-i);
}

int main()
{
    cout<<"Welcome to File Compressor, Yeah I got the best naming skills"<<endl;
    cout<<"Please enter your filename"<<endl;
    cout<<"Tip: Enter Filename only (with extension) if it is in same directory as this current file or you might have to enter it's whole path or relative path to this file"<<endl;
    string filename;
    cin>>filename;
    //Creating Compressed File


    //Creating Object Of Huffman Tree class
    HuffmanTree hft;
    ifstream file;

    //Creating Huffman Tree
    file.open("sample.txt");
    hft.readFile(file);
    hft.createTree();
    file.close();

    ofstream comp;
    ifstream og;
    og.open("sample.txt");
    string compressedFile = fileName(filename, "-compressed");
    comp.open(compressedFile.c_str(), ios :: binary | ios :: out | ios :: trunc);
    hft.createCompressedFile(og, comp);
    //Creating compressed file


    //hft.levelwisetraversal();
    // This function can print huffman Tree levelwise

    og.close();
    comp.close();


    //Saving Hashmap
    ofstream h;
    string hashmapFile = fileName(filename, "-hashmap");
    h.open(hashmapFile.c_str(), ios :: binary | ios :: trunc);
    hft.saveHashmap(h);
    h.close();




    //Recreating original file
    ifstream hread;
    ifstream domp;
    ofstream newFile;

    hread.open(hashmapFile.c_str(), ios :: binary | ios :: in);
    recreateHashmap(hread); //Recreating hashmap



    string recreatedFile = fileName(filename, "-recreated");
    newFile.open(recreatedFile.c_str(), ios::out | ios::trunc);

    domp.open(compressedFile.c_str(), ios :: binary | ios :: in);
    domp.seekg(0, ios::end);
    int s = domp.tellg(); // Finding size of compressed file to traverse
    domp.seekg(0, ios::beg);
    readCompressedFile(domp, newFile, s); // Recreating original file

    hread.close();
    newFile.close();
    domp.close();


    return 0;

}
