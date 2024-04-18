#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <limits>
#include <queue>
#include <stack>
#include <fstream>
#define maxN 10002
using namespace std;
int n,m; // n la so sensor , m la so target
int parDis[maxN];
int cnt=0;
double rC ; // ban kinh truyen tin ;
double rS ; // ban kinh bao phu
int used[maxN];
int a[2001][2001];
double x[2001],y[2001];
int d[maxN], par[maxN];
bool visit[maxN];
void input()   // nhap vao cac toa do cua sensor va base voi qui uoc (x[0],y[0]) la toa do do cua base;
    {   
        cin >> n >> m  >> rC >> rS ;
        for(int i = 0 ; i <= n ; i++)  // nhap vap vi tri cua base station ( x[0],y[0]) va toa do cua n sensor;
                cin >> x[i] >> y[i];
        for(int i = n+1 ; i<= n+m ;i++) // nhap vao vi tri cua m target ;
                cin >> x[i] >> y[i];
    }
double distance (double x,double y,double z, double t )  // tinh khoang cach
    {
        double seg = sqrt((x-z)*(x-z)+(y-t)*(y-t));
        return seg;
    }
void graphModel() // dua ve mo hinh do thi
    {
        for(int i = 0 ; i <= n ; i++)   // noi cac canh giua cac sensor va base station;
            for( int j = 0 ; j <= n ;j ++)
                {
                    if(i==j)
                        a[i][j]=0;
                    else
                        {
                            if(distance(x[i],y[i],x[j],y[j]) <= rC )
                                a[i][j]=1;
                                a[j][i]=1;
                        }                        
                }
        for(int i = n+1 ; i <= n+m ; i++) // noi canh giua target va sensor neu khoang cach  <= rS
            for(int j = 1 ;j <=n ; j++)
                if(distance(x[i],y[i],x[j],y[j]) <= rS){
                    a[i][j]=1;
                    a[j][i]=1;
                }
    }
void fix(){
    for(int i = 1 ; i <=m+ n; i++){   // ham cap nhat lai ket qua sau khi de qui
        used[i]=0;
    }
}
void fixMeb(){
    for(int i = 1 ; i <=m+n ;i++){
        parDis[i]=0;
    }
}
void bfs(int s)  // xet s la dinh dang xet , o bai toan nay ta se bfs tu base station
 {
    fill_n(d, n + 1, 0);
    fill_n(par, n + 1, -1);
    fill_n(visit, n + 1, false); // ham kiem tra la da tham dinh hay chua 
    queue <int> q;
    q.push(s);
    visit[s] = true;
    while (!q.empty()) 
    {
        int u = q.front();
        q.pop();
        for (int v = 0 ;  v <= n ; v++) 
        {
            if(a[v][u]==1 && used [v]==0)
                if (!visit[v]) 
                {
                    d[v]     = d[u] + 1; // cap nhat do dai duong di ngan nhat
                    par[v]   = u; // truy vet duong di
                    visit[v] = true;
                    q.push(v);
                }
        }
    }
}
void delete_vertex(int s) // xoa mot dinh
    {
        for(int i = 0 ; i <= m+n; i++)
            {
                a[s][i]=0;
                a[i][s]=0;
            }
    }
void delete_edge(int s, int t) // xoa canh s->t
    {
        a[s][t]=0;
        a[t][s]=0;
    }
void add_edge(int s,int t)
    {
        a[s][t]=1;
        a[t][s]=1;
    }
int checkPath(int source){
    int sea=-1;
    int abc=INT_MAX;
    for(int i =1 ; i <= n; i++){
        if(a[source][i] == 1 && d[i] != 0 && used[i]==0){
            if(abc> d[i]){
                abc=d[i];
                sea=i;
            }
        }
    }
    return sea;
}
int naive(int source) // ham phu. tinh so duong di phan biet tu source den sink ( source o day chinh la target )
    {
            bfs(0);  // bfs tu di?nh root la 0 (chinh la base station)
            int temp=checkPath(source);
            if(temp==-1){
                return 0;
            }
            else{
                while(temp!=0){
                    used[temp]=1;   // danh dau nhung dinh bi bo
                    temp=par[temp];
                }
                return naive(source)+1;
        }                   
    }   
int MebDisjPath(int source)
    {  
        int search=-1,res=0;
        bfs(0); // tinh do dai duong di tu cac target, sensor toi base station;
        int cnt=INT_MAX; 
        search=checkPath(source); 
        if(search==-1){
            return 0;
        }     
        int temp1=0;
        int length=d[search]+1;
        for(int i = 0 ; i < length ; i++){    // vi ta goi de qui naive(source) nhieu lan nen ta can tao mang parent moi cho cac dinh thuoc duong di dang xet
            if(i==0){
                parDis[i]=search;
            }
            else{
                parDis[i]=par[parDis[i-1]];
            }
        }
        res=naive(source);
        fix();
        for(int i = 0 ; i < length ;i++)
            {
                if(i==0)
                    {
                        delete_edge(source,parDis[0]);
                        int ans=naive(source);
                        fix();
                        if(ans <= res )
                            add_edge(source,search);
                        else
                            res=ans ;// cap nhat dap an va khong them canh vua xoa ;
                    }
                else  // tuong tu truong hop i = 0;
                    {   
                        delete_edge(parDis[i-1],parDis[i]);
                        int ans=naive(source);
                        fix();
                        if(ans <= res )
                            add_edge(parDis[i-1],parDis[i]);
                        else
                            res=ans ;
                    }
            } 
        return res;
    } 
int main()
    {   
        
        freopen("nhap.inp","r",stdin);
        freopen("ra.out","w",stdout);
        input();
        graphModel();
        cnt=0;
        int example ;
        cin >> example;
        
        /*cout << "Ket qua cua bien search trong ham MeDisPath(3) la : ";
        MebDisjPath(3); */
        cout << naive(3) << endl;
        cout << used[1] << endl;
        fix();
        cout << naive(4) << endl;
        fix();
        cout << MebDisjPath(3) << endl;
        fix();
        cout << MebDisjPath(4) << endl;

    }