#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>

const int MAX=10001;
using namespace std;

vector<int> e[3][MAX];
auto *Input = e[0];//for 1st input
auto *Intermediate = e[1];//for 2nd input
auto *Final = e[2];//to output BFS forest
int n, m;
int parent[2][MAX];//parent[0]: parents of each node in the 1st round; parent[1]: parents of each node used in the 2nd round;

void FirstBFS (int begin)
{
    queue<int> Q;
    parent[0][begin] = begin;
    Q.push(begin);
    while (Q.size())
    {
        auto current = Q.front(); 
        Q.pop();
        for (auto child:Input[current])
        {
            if(parent[0][child] != -1)
            {
                Intermediate[current].push_back(child);
            }
            else
            {
                parent[0][child] = current;
                Q.push(child);
                Final[current].push_back(child);
                Final[child].push_back(current);
            }
        }
        Input[current].clear(); //clean the unused edges
        Input[current].shrink_to_fit();
    }
}

void SecondBFS (int begin)
{
    queue<int> Q;
    parent[1][begin] = begin;
    Q.push(begin);
    while (Q.size())
    {
        auto current = Q.front(); 
        Q.pop();
        for (auto child:Intermediate[current])
        {
            if(!(parent[1][child] != -1 || (parent[0][child] == current || parent[0][current] == child)))
            {
                parent[1][child] = current;
                Q.push(child);
                Final[current].push_back(child);
                Final[child].push_back(current);
            }
        }
        Intermediate[current].clear(); //clean the unused edges
        Intermediate[current].shrink_to_fit();
    }
}

int visit[MAX];
int low[MAX];
int t = 0;
vector<int> CutVertices;

inline void DFS(int parent, int current)
{
    visit[current] = low[current] = ++t;
    int ChildCount = 0;
    bool cutVertex = false;
    for(auto next:Input[current])
    {
        if (visit[next]) // back edge
        {
            low[current] = min(low[current], visit[next]);
        }
        else // tree edge
        {
            ChildCount++;
            DFS(current, next);
            low[current] = min(low[current], low[next]);
            if (low[next] >= visit[current]) 
            {
                cutVertex = true;
            }
        }
    }
    if ((current == parent && ChildCount > 1) || (current != parent && cutVertex))
    {
        CutVertices.push_back(current);
    }
}
void BFSForest()
{
    //BFS forest construction
    fill(parent[0],parent[0]+n,-1);
    fill(parent[1],parent[1]+n,-1);
    for (int i=0; i<n; i++)
    {
        if(parent[0][i]==-1)
        {
            FirstBFS(i);
        }
    }
    for (int i=0; i<n; i++)
    {
        if(parent[1][i]==-1)
        {
            SecondBFS(i);
        }
    }
}
void build()
{
    while(m>0)
    {
        //Input Construction
        for(int i=0; i<2*n && m>0; i++, m--) 
        {
            int u, v;
            scanf("%d%d",&u,&v);
            u--;
            v--;
            if (u==v)
            {
                continue; //do not consider loop
            }
            Input[u].push_back(v);//Input at most 2n edges to construct a BFS forest!
            Input[v].push_back(u);
        }
        BFSForest();
        swap(Input,Final);
    }
    for(int i=0; i<n; i++)
    {
        if(!visit[i])
        {
            DFS(i, i);
        }
    }
}
int main()
{
    scanf("%d%d",&n,&m);
    build();
    for(auto x:CutVertices) 
    {
        printf("%d\n",x+1);
    }
    if(CutVertices.empty())
    {
        printf("None\n");
    }
}