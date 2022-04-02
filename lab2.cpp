#include <bits/stdc++.h>
using namespace std;

class bucket
{
    int depth;
    vector<long long> b;

    public:
    bucket(int dd){ depth=dd;}

    vector<long long> give(){ return b; }
    int d(){ return depth;}
    void deepen() { depth++; }
    void erase(long long val) { b.erase(find(b.begin(), b.end(), val)); }
    void add(long long val) { b.push_back(val); }
    void erase_all() { b.clear(); }
};

class file
{
    vector<bucket*> directory, bucket_list;
    int global_depth, bucket_capacity;
    public: 
    bucket* bucket_generate (int depth_assign)
    {
        bucket *p= new bucket(depth_assign);
        bucket_list.push_back(p);
        return p;
    }

    void double_directory()
    {
        for(long long i=0; i<(1LL<<global_depth); i++)
        {
            bucket* mirror_pointer= directory[i];
            directory.push_back(mirror_pointer);
        }
        global_depth++;
    }

    void doctor(int ind)
    {
        vector<long long> v= directory[ind]->give();
        int local_depth=directory[ind]->d();
        if(local_depth==global_depth)
        {
            double_directory();
            doctor(ind);
        }
        else
        {
            long long mirror= (ind ^ (1LL<<local_depth));
            vector<long long> mirrors;
            directory[ind]->deepen(); local_depth++;
            directory[mirror]=bucket_generate(local_depth);
            for(int i=0; i<(1LL<<global_depth); i++)
                if(i%(1LL<<local_depth)==mirror)
                    directory[i]=directory[mirror];
            vector<long long> segregate=v;
            directory[ind]->erase_all();
            for(auto x : segregate)
            {
                if(x%(1LL<<local_depth)==ind)
                    directory[ind]->add(x);
                else
                    directory[mirror]->add(x);
            }
            if(directory[ind]->give().size()>bucket_capacity) doctor(ind);
            else if (directory[mirror]->give().size()>bucket_capacity) doctor(mirror);
        }
    }

    bool search(long long val)
    {
        long long index=(val%(1LL<<global_depth));
        vector<long long> check= directory[index]->give();
        if(find(check.begin(), check.end(), val)==check.end()) return false;
        else return true;
    }

    bool erase (long long val)
    {
        if(!search(val))  return false;
        long long index=(val%(1LL<<global_depth));
        directory[index]->erase(val);
        return true;
    }

    void insert(long long val)
    {
        long long index=(val%(1LL<<global_depth));
        directory[index]->add(val);
        vector<long long> check= directory[index]->give();
        if(check.size()<=bucket_capacity)
            return;
        doctor(index);
    }

    void display()
    {
        cout<<global_depth<<endl<<bucket_list.size()<<endl;
        for(auto bb:bucket_list ) cout<<bb->give().size()<<" "<<bb->d()<<endl;
    }

    file(int gd, int bc)
    {
        bucket_capacity=bc; 
        global_depth=gd;
        for(long long i=0; i<(1LL<<global_depth); i++)
            directory.push_back(bucket_generate(global_depth));
    }
};

 
int main()
{
    ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int initial_global_depth, capacity;
    cin>>initial_global_depth>>capacity;
    file f(initial_global_depth, capacity);
    int input;
    do
    {
        cin>>input;
        if(input==2)
        {
            int x; cin>>x;
            f.insert(x);
        }
        else if(input==3)
        {
            int x; cin>>x;
            bool ok=f.search(x);
            //if(!ok) cout<<"Not "; cout<<"Found\n";
        }
        else if(input==4)
        {
            int x; cin>>x;
            bool ok=f.erase(x);
            //if(!ok) cout<<"Not "; cout<<"Deleted\n";
        }
        else if(input==5)
            f.display();

    } while(input!=6);
    
    return 0;
}