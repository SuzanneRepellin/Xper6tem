#include "xper6tem.h"

using namespace std;

void    print_bin_tree(t_node* tree)
{
    t_node* base = tree;
    while (tree != NULL)
    {
        cout << tree->data << endl;
        tree = tree->left;
    }
}

string  strtrim(string str)
{
    string ret;
    vector<char> vec(str.begin(), str.end());
    for (char c : vec)
    {
        if (c == '\t' || c == ' ' || c == '\n')
            continue;
        else
            ret.append(1, c);
    }
    return ret;
}

size_t charpos(vector<char> vec, char c)
{
    size_t  i = 0;
    for (char x : vec)
    {
        if (x == c)
            return i;
        i++;
    }
    return i;
}

size_t charpos_nopar(vector<char> vec, char c)
{
    size_t  i = 0;
    size_t  par_op = 0;
    size_t  par_cl = 0;
    for (char x : vec)
    {
        if (x == '(')
            par_op++;
        else if (x == ')')
            par_cl++;
        else if (x == c && par_op == par_cl)
            return i;
        i++;
    }
    return i;
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        if (item.length() > 0)
            elems.push_back(item);  
    }
    return elems;
}

vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

int     countchar(const string s, char c)
{
    int ret = 0;

    for (char x : s)
    {
        if (x == c)
            ret++;
    }
    return ret;    
}

bool    find_elem_in_tree(t_node *tree, char elem)
{
   t_node  *start = tree;
   bool ret = false;
   if (tree && tree->type != Entity)
   {
       ret |= find_elem_in_tree(tree->left, elem);
       ret |= find_elem_in_tree(tree->right, elem);
   }
   if (tree && tree->data == elem)
        return true;
   return ret;
}

string  remove_doubles(const string src)
{
    string dest = "";
    for (char c : src)
    {
        if (dest.find(c) == string::npos)
        {
            dest += c;
        }
    }
    return dest;
}