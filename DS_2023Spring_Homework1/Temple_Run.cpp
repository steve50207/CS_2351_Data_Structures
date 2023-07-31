#include <iostream>
#include <string>
//#define OJ
using namespace std;

string cmd_map[100];
string cmd_coin[100];
int cmd_map_idx = 0;
int cmd_coin_idx = 0;

template<class T>
class StackArray{

private:
    int top;
    int capacity;
    T *Stack;
    void DoubleCapacity();
public:
    StackArray():top(-1),capacity(10){
        Stack = new T[capacity];
    }
    void Push(T x);
    void Pop();
    bool IsEmpty();
    T Top();
};

template<class T>
void StackArray<T>::DoubleCapacity(){

    capacity *= 2;
    T *newStack = new T[capacity];

    for (int i = 0 ; i < capacity/2; i++) {
        newStack[i] = Stack[i];
    }

    delete [] Stack;
    Stack = newStack;
}

template<class T>
void StackArray<T>::Push(T x){

    if (top == capacity - 1) {
        DoubleCapacity();
    }
    Stack[++top] = x;
}

template<class T>
void StackArray<T>::Pop(){

    top--;
}

template<class T>
bool StackArray<T>::IsEmpty(){

    return (top == -1);
}

template<class T>
T StackArray<T>::Top(){

    return Stack[top];
}

template<class T>
class QueueArray{

private:
    int capacity, Front, Back;
    T *Queue;
    void DoubleCapacity();

public:
    QueueArray():capacity(10),Front(0),Back(0)
    {
        Queue = new T[capacity];
    }
    void Push(T x);
    void Pop();
    bool IsEmpty();
    bool IsFull();
    T getFront();
    int getSize();
};

template<class T>
void QueueArray<T>::DoubleCapacity()
{
    T *newQueue = new T[capacity*2];
    int j = Front, Size = getSize();
    for(int i=1;i<=Size;i++)
    {
        newQueue[i] = Queue[++j % capacity];
    }

    Back = Size;
    Front = 0;
    capacity *= 2;

    delete [] Queue;
    Queue = newQueue;
}

template<class T>
void QueueArray<T>::Push(T x)
{
    if(IsFull())
    {
        DoubleCapacity();
    }

    Back = (Back+1)%capacity;
    Queue[Back] = x;
}

template<class T>
void QueueArray<T>::Pop()
{

    Front = (Front+1)%capacity;
}

template<class T>
bool QueueArray<T>::IsEmpty()
{
    return (Front==Back);
}

template<class T>
bool QueueArray<T>::IsFull()
{
    return ( (Back+1)%capacity == Front);
}

template<class T>
T QueueArray<T>::getFront()
{
    return Queue[Front+1];
}

template<class T>
int QueueArray<T>::getSize()
{
    return (Back-Front+capacity)%capacity;
}

void compute_route(){

    StackArray<string> route_city_to_temple;
    string route_result[400]={"\0"};
    int route_result_idx = 0;

    for(int i=0;i<cmd_map_idx;i++){
        route_city_to_temple.Push(cmd_map[i]);
    }

    while(!route_city_to_temple.IsEmpty()){
        string tmp = route_city_to_temple.Top();
        route_city_to_temple.Pop();
        if(tmp=="TL"){
            route_result[route_result_idx++] = "TR";
        }else if(tmp=="TR"){
            route_result[route_result_idx++] = "TL";
        }
    }

    for(int i=0;i<route_result_idx;i++){
        cout << route_result[i] << ((i==route_result_idx-1) ? "\n" : "->");
    }
}


void compute_coin_collected(){

    QueueArray<string> coin_collect;
    int gold_count = 0;
    int silver_count = 0;
    int group_count = 0;
    int penalty_count = 0;
    int collect_value = 0;

    for(int i=0;i<cmd_coin_idx;i++){
        coin_collect.Push(cmd_coin[i]);
    }

    //cout << "coin_collect\n";
    while(!coin_collect.IsEmpty()){
        string tmp = coin_collect.getFront();
        coin_collect.Pop();

        if(tmp=="MISS"){
            penalty_count=4;
        }else if(tmp=="GOLD" || tmp=="SILVER"){
            if(penalty_count!=0){
                penalty_count--;
            }else if(penalty_count==0){
                if(tmp=="GOLD"){
                    gold_count++;
                }else if(tmp=="SILVER"){
                    silver_count++;
                }
            }
        }
        group_count = gold_count+silver_count;
        if(group_count==3){
            if(gold_count==3){
                collect_value+=500;
            }else if(gold_count==2){
                collect_value+=300;
            }else if(gold_count==1){
                collect_value+=150;
            }else if(gold_count==0){
                collect_value+=50;
            }
            gold_count=0;
            silver_count=0;
            group_count=0;
        }
    }
    cout << collect_value <<  "\n";

}


void compute_coin_total(){

    QueueArray<string> coin_total;
    int gold_count = 0;
    int silver_count = 0;
    int group_count = 0;
    int total_value = 0;

    for(int i=0;i<cmd_coin_idx;i++){
        coin_total.Push(cmd_coin[i]);
    }

    //cout << "coin_total\n";
    while(!coin_total.IsEmpty()){
        string tmp = coin_total.getFront();
        coin_total.Pop();
        if(tmp=="GOLD"){
            gold_count++;
        }else if(tmp=="SILVER"){
            silver_count++;
        }
        group_count = gold_count+silver_count;
        if(group_count==3){
            if(gold_count==3){
                total_value+=500;
            }else if(gold_count==2){
                total_value+=300;
            }else if(gold_count==1){
                total_value+=150;
            }else if(gold_count==0){
                total_value+=50;
            }
            gold_count=0;
            silver_count=0;
            group_count=0;
        }
    }
    cout << total_value <<  "\n";
}


int main() {

    #ifndef OJ
        freopen("testdata.txt", "r", stdin);
    #endif // OJ

    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    string cmd;

    while(cin >> cmd && !cin.eof()){
        if(cmd =="TL" || cmd == "TR"){
            cmd_map[cmd_map_idx++] = cmd;
        }else if(cmd == "GOLD" || cmd == "SILVER" || cmd == "MISS"){
            cmd_coin[cmd_coin_idx++] = cmd;
        }
    }
    /*
    cout << "cmd_map\n";
    for(int i=0;i<cmd_map_idx;i++){
        cout << cmd_map[i] << "\n";
    }

    cout << "cmd_coin\n";
    for(int i=0;i<cmd_coin_idx;i++){
        cout << cmd_coin[i] << "\n";
    }
    */
    compute_route();
    compute_coin_collected();
    compute_coin_total();

    return 0;
}
