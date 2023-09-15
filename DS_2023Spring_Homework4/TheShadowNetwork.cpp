#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include<list>
#include<string.h>
#include<cmath>

using namespace std;
#define TestCase
#define IDEA_OJ
//#define NTHU_OJ
#define INF 0x3f3f3f3f // INF是給shortest path distance用,如果開不夠大,spd是不會更新到正確的distance值,做relaxation會出錯
#define maxN (int) 101

class Node
{
public:
    int ID;
    string NodeType;
    string Name;
    Node(int id, string type, string name):ID(id),NodeType(type),Name(name) {}
};

int id;                             // node id
int n;                              // node total num
int start;                          // source(start point)
int destination;                    // Headquarter(destination point)
vector<Node> Node_List;             // list of Node

int mes_raw[maxN][maxN];            // row message between two nodes
double dis_raw[maxN][maxN];         // translate raw message to raw distance
int mes_allowed[maxN][maxN];        // message judged by node type to Allowed connections
double dis_allowed[maxN][maxN];     // translate allowed message to allowed distance

double shortest_path_dis[maxN];     // shortest path distance from source to destination
bool visited[maxN];                 // node is included in tree set or not

vector<int> pre[maxN];              // previous node list of this node, the list can do DFS backtracking the reversed shortest paths from destination to start
vector<int> tmp_path;               // temporary reversed path by the process of DFS 
vector<vector<int>> path_reverse;   // complete reversed path from destination to start
vector<vector<int>> path_order;     // use stack to reverse the reversed path to become ordered path from start to destination
vector<int> final_path;             // use judge rule to decide the final path from path order(several same shortest paths)


void Network_Initial(){
    id = 0; //  set id to 0
    
    Node_List.push_back(Node(0,"tmp_type","tmp_name")); // push a dummy node in the NodeList
    
    // initialize the adjacent matrix
    // mes_raw,dis_raw,mes_allowed,dis_allowed all have to be set
    for(int i=0; i<maxN; i++)
    {
        for(int j=0; j<maxN; j++)
        {
            if(i==j)                    // edge distances from node to itself is 0
            {
                mes_raw[i][j]=0;
                dis_raw[i][j]=0;
                mes_allowed[i][j]=0;
                dis_allowed[i][j]=0;
            }
            else                        // set every edge distances to INF
            {
                mes_raw[i][j]=INF;
                dis_raw[i][j]=INF;
                mes_allowed[i][j]=INF;
                dis_allowed[i][j]=INF;
            }
        }
    }

    for(int i=0; i<maxN; i++)
    {
        visited[i]=false;               // initial visited to false
        shortest_path_dis[i]=INF;       // initial shortest path distance to INF
    }

    for(int i=1; i<=n; i++)
    {
        pre[i].push_back(i);            // initial previous path to node itself
    }
    
}


// build the Node_List
void Node_Insertion(int id, string type, string name)
{
    Node_List.push_back(Node(id,type,name)); // construct the nodes and put the node in the Node list

    if(type=="SOURCE")
    {
        start = id;                         // start: soucrse id
    }
    else if(type=="HQ")
    {
        destination = id;                   // destination: headquarter id
    }
}

// build the raw message and distance and judge the type and turn them into allowed message and allowd distance
void Edge_Insertion(int from, int to, int message)
{
    // build mes_raw and dis_raw, because indirect graph, two direction should be set
    mes_raw[from][to] = message;
    mes_raw[to][from] = message;
    // round to the nearest 2 decimal
    dis_raw[from][to] = round((1000.0/message)*100.0)/100.0;
    dis_raw[to][from] = round((1000.0/message)*100.0)/100.0;

    //build mes_allowd and dis_allow, judge by allowed connection type
    //turn id_1 & id_2 into corresponding type
    string from_type = Node_List[from].NodeType;
    string to_type = Node_List[to].NodeType;

    //use connection rule
    //if allow , set allowed to raw (allow)
    //if not allow, set allowed to INF (not allowed)
    if(from_type == "SOURCE" && to_type=="SPY" || from_type == "SPY" && to_type=="SOURCE" ||
       from_type == "SPY" && to_type=="SPY"    || 
       from_type == "CIV" && to_type=="HQ"     || from_type == "HQ" && to_type=="CIV")
    {
        mes_allowed[from][to] = INF;
        mes_allowed[to][from] = INF;
        dis_allowed[from][to] = INF;
        dis_allowed[to][from] = INF;
    }else
    {
        mes_allowed[from][to] = mes_raw[from][to];
        mes_allowed[to][from] = mes_raw[to][from];
        dis_allowed[from][to] = dis_raw[from][to];
        dis_allowed[to][from] = dis_raw[to][from];
    }

}

void Dijkstra(int s)
{
    n = id;     // set id to n (total node num)

    shortest_path_dis[s]=0;                                         // set source's shortest path distance is 0

    for(int i=1; i<=n; i++)                                         // collect n shortest distances, so do n rounds
    {
        int u = -1, min_dis = INF;                                  // u: the unvisited and smallest distance node id
                                                                    // min_dis: the smallest distance in this round
        for(int j=1; j<=n; j++)                                     // find the min_distance next to the visited node set
        {
            if(visited[j]==false && shortest_path_dis[j] < min_dis) // find the minimum distance and node, < is important
            {                                                       // check the node not in the tree set and the node's distance is smaller than min_dis
                u = j;                                              // uddate the minimum distance node id
                min_dis = shortest_path_dis[j];                     // update the minimum distance
            }
        }
        if(u==-1)
        {
            return;                                                 // can't find the next node,so terminate Dijkstra
        }
        visited[u]=true;                                            // put the node u into the tree set
        for(int v=1; v<=n; v++)                                     // check every unvisited and nearest node to the tree set to do the relaxation
        {
            if(visited[v]==false && dis_allowed[u][v]!=INF)         // if the edge of u and the unvisited node v is not INF (they are connected)  
            {
                if(shortest_path_dis[u] + dis_allowed[u][v] < shortest_path_dis[v])     // if we find a new path from u through edge(u,v) can go to v and shortest path distance is smaller than previous one
                {                                                                       
                    shortest_path_dis[v] = shortest_path_dis[u] + dis_allowed[u][v];    // do relaxation
                    pre[v].clear();                                                     // previous node list of v should be clear
                    pre[v].push_back(u);                                                // put the u into the v's previous node list because from u throgh edge(u,v) can get a smaller distance path to v
                }
                else if(shortest_path_dis[u] + dis_allowed[u][v] == shortest_path_dis[v]) // if we find a new path from u through edge(u,v) can go to v and shortest path distance is same as previous one
                {
                    pre[v].push_back(u);                                                  // put the u into the v's previous node list because from u throgh edge(u,v) can get a same distance path to v
                }
            }
        }
    }
}


void Shortest_Reversed_Paths(int cur)       // use DFS through the previous node list to do backtracking to build Shortest_Reversed_Paths
{
    if(cur==start)                          // recursion end condtion: when node is start
    {
        tmp_path.push_back(cur);            // put cur node into tmp_path
        path_reverse.push_back(tmp_path);   // we get a complete reversed shortest path from destination to start, so we record the path into path reverse              
        return;                             // end recursion
    }
    else                                    // recursion keep going condition: when node is not start
    {
        tmp_path.push_back(cur);                  // put cur node into tmp_path
        for(int i=0; i<(int)pre[cur].size(); i++) 
        {
            Shortest_Reversed_Paths(pre[cur][i]); // use DFS go to every previous node of cur
            tmp_path.pop_back();                  // when DFS return, we need to remove current previous node of cur from tmp_path
                                                  // change it to another previous node of cur
        }
    }
}

void Shortest_Oredered_Paths(){                   // use stack to change reversed paths to ordered path 
    for(auto i:path_reverse)
    {
        stack<int> path_stack;
        vector<int> tmp_vec;
        for(auto j:i)
        {
            path_stack.push(j);
        }
        while(!path_stack.empty())
        {
            int path_node = path_stack.top();
            path_stack.pop();
            tmp_vec.push_back(path_node);
        }
        path_order.push_back(tmp_vec);
    }
}


void Optimal_Shortest_Path_Judge()      // use the judge rule to decide which ordered path should be the optimal path
{   
    int cnt = (int)path_order.size();   // cnt: the total num of same distance shortest path
    bool valid_path[cnt];               // valid_path: mark the valid candidate path
    for(int i=0; i<cnt; i++)
    {
        valid_path[i]=true;             // intialize valid path to be true(all valid in the beginning) 
    }
    int pos = 0;                        // pos: the current checking col of ordered path
    int num = cnt;                      // num: the current num of valid path
                                
    while(num > 1) // only when we have multiple same length shortest paths,we need to compare paths to decide the optimal one
    {
        priority_queue <int, vector<int>,greater<int> > pq; // put all ordered path's the same checking col id into the priority queue 
        for(int i=0; i<cnt; i++)
        {
            if(valid_path[i]==true)                         // only check the valid path
            {
                pq.push(path_order[i][pos]);
            }
        }
        int min_id = pq.top();                              // min_id: get the smallest id in all col of ordered path

        bool flag_same_value = true;                        // flag_same_value: record the all id in same col is same or not
        for(int i=0; i<cnt; i++)
        {
            if(valid_path[i]==true && path_order[i][pos]!=min_id) // if valid path and corresponding id is not min_id
            {
                flag_same_value = false;                          // all id in same col of ordered path are not the same
            }
        }
        if(flag_same_value==false)
        {
            for(int i=0; i<cnt; i++)
            {
                if(valid_path[i]==true && path_order[i][pos]!=min_id)   // if the ordered path's same col id is bigger than min_id
                {
                    valid_path[i]=false;                                // the correspoding path is not valid anymore
                    num--;                                              // valid path num -1 
                }
            }
        }
        pos++;    // go to next col
    }
    for(int i=0; i<cnt; i++)
    {
        if(valid_path[i]==true)                 // when num is one, we get the optimal path
        {
            final_path = path_order[i];         // choose the only valid path to become final_path
        }
    }
}

void PrintResult()                              // print result
{
    int length = final_path.size();
    for(int i=0; i<length; i++)
    {
        cout << Node_List[final_path[i]].Name << ( (i==length-1) ? "\n" : " -> "); // change final path's id into name
    }

    int total_message = 0;
    for(int i=0; i<length-1; i++)
    {
        int from = final_path[i];
        int to = final_path[i+1];
        total_message += mes_allowed[from][to];             // total_message: sum the edge through final path
    }
    cout << total_message << "\n";

    double total_distance = shortest_path_dis[destination]; // total_distance: the shortest path distance from start to destination
#ifdef IDEA_OJ
    cout << total_distance;
#endif

#ifdef NTHU_OJ
    cout << total_distance << "\n";
#endif

}


void Analyze()
{
    Dijkstra(start);
    Shortest_Reversed_Paths(destination);
    Shortest_Oredered_Paths();
    Optimal_Shortest_Path_Judge();
    PrintResult();
}

int main(void)
{

#ifdef TestCase
    freopen("testdata.txt","r",stdin);
#endif

    Network_Initial();
    
    while(!cin.eof())
    {
        string cmd;
        cin >> cmd ;
        if(cmd=="INSERT")
        {
            string type, name;
            cin >> type >> name;
            Node_Insertion(++id,type,name);
        }
        else if(cmd=="INSERT_EDGE")
        {
            int from, to, message;
            cin >> from >> to >> message;
            Edge_Insertion(from,to,message);
        }
        else if(cmd=="ANALYZE")
        {
            Analyze();
            break;
        }
    }
    return 0;
}
