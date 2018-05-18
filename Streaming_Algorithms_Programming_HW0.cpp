#ifdef STREAMING_JUDGE
#include "io.h"
#define next_token io.next_token
#else
#include<string>
#include<iostream>
using namespace std; 
string next_token()
{
	string s;
	cin >> s;
	return s;
}
#endif

#include<cstdio>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<iostream>
#include<math.h>
#include<numeric>
using namespace std;

int main()
{
	srand(time(NULL));
	//1st pass: randomly choose sqrt(n) numbers from the given stream of numbers
    int n = atoi(next_token().c_str());
    int observed = 0;
    int sampled = 0;
    int p = (int)ceil((n-observed)/((int)ceil(sqrt(n))-sampled));//sample each observation with probability (sqrt(n) - n_sampled) / (n - n_observed)
    vector<int> a;
    a.reserve(p);
    for(int i=0; i<n; i++)
	{
		int s=atoi(next_token().c_str());
		if( rand()%p == 0 && (int)a.size() < (int)ceil(sqrt(n)) )
		{
            a.push_back(s);
            sampled++;
		}
        observed++;
	}
	sort(a.begin(), a.end());
    //2nd pass: find the k such that the median lies in a[k] and a[k+1], and find the rank of the median between a[k] and a[k+1]
    next_token();
    vector<int> rank(a.size(), 0);
	for( int i = 0; i < n; i++ )
	{
		int s=atoi(next_token().c_str());
		// Find index of interval in O(log n) time
        int idx = std::upper_bound(a.begin(), a.end(), s) - a.begin();
        // Increase the rank of only that interval
        ++rank[idx];
	}
    partial_sum(rank.begin(), rank.end(), rank.begin());
    for( int j = 0; j < (int)rank.size(); j++ )
    {
        rank[j]++;
    }
    int median = 0;
    int middle = (n+1)/2;
    int k;
    if( (int)a.size() == 1 && rank.front() == middle )
    {
        median=a.front();
        cout << median << endl;
        return 0;
    }
    for( int j = 0; j < (int)rank.size(); j++ )
    {
        if( rank[j] == middle )
        {
            cout << a[j] << endl;
            return 0;
        }
        else if( rank[j] < middle && rank[j+1] > middle )
        {
            k = j;
            break;
        }
    }
	//3rd pass: sort the numbers in (a[k], a[k+1])
    next_token();
    vector<int> FinalRun;
    if( rank.front() > middle )
    {
        for( int i = 0; i < n; i++ )
        {
		    int s = atoi(next_token().c_str());
            if( s < a.front() )  FinalRun.push_back(s);
        }
        sort( FinalRun.begin(), FinalRun.end() );
        cout << FinalRun[middle-1] << endl;
        return 0;
    }
    else if ( rank.back() < middle )
    {
        for( int i = 0; i < n; i++ )
        {
		    int s = atoi(next_token().c_str());
            if( s > a.back() )  FinalRun.push_back(s);
        }
        sort( FinalRun.begin(), FinalRun.end() );
        cout << FinalRun[middle-rank.back()-1] << endl;
        return 0;
    }
    else
    {
        for( int i = 0; i < n; i++ )
        {
            int s = atoi(next_token().c_str());
            if( s > a[k] && s < a[k+1] )  FinalRun.push_back(s);
        }
        sort( FinalRun.begin(), FinalRun.end() );
        cout << FinalRun[middle-rank[k]-1] << endl;
        return 0;
    }
}