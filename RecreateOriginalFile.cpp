// map<string, char> address2;
map<string, char> address3;

void readCompressedFile(ifstream &file, ofstream &newFile)
{
    string t = "";
    char *n = new char[3778];
    char k;
    int count = 0;
    while (file.good())
    {
        file.read(n, 3778);
        for (int i = 0; i < 3778; i++)
        {
            char ch = n[i];
            count++;
            // cout<<ch;
            file.get(ch);
            for (int i = 7; i > -1; i--)
            {

                k = ((ch >> i) & 1) ? '1' : '0';

                t += k;

                if (address3.count(t) > 0)
                {
                    newFile << address3[t];
                    t = "";
                }
            }
        }
    }
    // cout<<"Count is "<<count<<endl;
}

void recreateHashmap(ifstream &file)
{
    string k;
    char r;
    file.read(&r, 1);

    char read_char;
    while (true)
    {
        file.read(&read_char, 1);

        if (read_char == r)
        {
            if (k.length() == 0)
            {
                break;
            }
            else
            {
                address3[k.substr(0, k.length() - 1)] = k[k.length() - 1];
                k = "";
            }
        }
        else
        {
            k += read_char;
        }
    }

    for (map<string, char>::iterator i = address3.begin(); i != address3.end(); i++)
    {
        cout << i->first << ' ' << i->second << endl;
    }
}