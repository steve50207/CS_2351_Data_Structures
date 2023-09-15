#include <iostream>
#define OJ
#define INF 1001;
using namespace std;

class CircularLinkedList;

class ListNode
// 宣告Linked List Node的class
{

public:

    int label_num;
    int macaron_num;
    ListNode *next;

    ListNode():label_num(0),macaron_num(0),next(NULL) {};
    ListNode(int label, int macaron):label_num(label),macaron_num(macaron),next(NULL) {};

    friend class CircularLinkedList;

};

class CircularLinkedList
// 宣告 Singly Circular Linked List的class
{

public:

    ListNode *first;
    ListNode *last;

    CircularLinkedList():first(NULL),last(NULL) {};

    void Push_front(int label, int macaron);
    void Push_back(int label, int macaron);
    void PrintList();
    void PrintResult(ListNode *start);

    ListNode *Delete(ListNode *current);
};

void CircularLinkedList::PrintList()
//印出Linked List所包含node的label與macaron數量,次序從first印到last
{
    if(first==NULL || last==NULL)
    {
        cout << "list is empty!\n";
        return;
    }

    ListNode *cur = first;
    while(cur!=last)
    {
        cout << cur->label_num << " / " << cur->macaron_num << "\n";
        cur = cur->next;
    }
    cout << cur->label_num << " / " << cur->macaron_num << "\n";
}

void CircularLinkedList::Push_front(int label, int macaron)
// 在Linked List的fisrt的前面新增node
{
    ListNode *newNode = new ListNode(label, macaron);

    if(first==NULL && last==NULL)
    {
        first = newNode;
        last = newNode;
        last->next = first;
    }
    else
    {
        newNode->next = first;
        last->next = newNode;
        first = newNode;

    }
}

void CircularLinkedList::Push_back(int label, int macaron)
// 在Linked List的last的後面新增node
{
    ListNode *newNode = new ListNode(label, macaron);

    if(first==NULL && last==NULL)
    {
        first = newNode;
        last = newNode;
        last->next = first;
    }
    else
    {
        last->next = newNode;
        newNode->next = first;
        last = newNode;
    }
}

ListNode *CircularLinkedList::Delete(ListNode * current)
// 將current的下一個node移出Linked List,並回傳移除node的記憶體位置
{
    ListNode *prev = current;
    ListNode *del = current->next;
    ListNode *prec = del->next;

    prev->next = prec;

    return del;
}

void CircularLinkedList::PrintResult(ListNode *start)
//印出Linked List所包含node的label與macaron數量,次序從start開始印到尾端
{
    ListNode *cur = start;

    while(cur->next!=start){
        cout << cur->label_num << " / " << cur->macaron_num << "\n";
        cur = cur->next;
    }
    cout << cur->label_num << " / " << cur->macaron_num;

}

CircularLinkedList Macaron_List;
// 設定Macaron List
CircularLinkedList Temporary_List;
// 設定Temporary List,用來放暫時被移出的node,並且按照reverse後次序放入
int current_round = 1;
// 當前round次數
int total_round = 100;
// 總共要執行round次數
ListNode *current = NULL;
// current node記憶體位置
ListNode *target = NULL;
// target node記憶體位置

void select_current_plate()
{
    if(current_round==1){
        current = Macaron_List.first;
        // 設定current為Macaron_List的first
    }else{
        int current_label = (target->label_num)%10+1;
        // 依照題目要求設定current_label為target_label%10+1

        while(current->label_num != current_label){
            current = current->next;
        }
        // 利用label_num更新current node

        ListNode *End = current->next;
        // 宣告end node

        while(End->next!=current)
        {
            End = End->next;
        }
        // 更新end node為current node的上一個
        Macaron_List.first = current;
        // 更新Macaron_List的first為current
        Macaron_List.last = End;
        // 更新Macaron_List的last為end
    }

}

void remove_plates()
{
    ListNode *tmp;
    int remove_cnt = 3;
    // 要移出的node數量
    while(remove_cnt--)
    {
        tmp = Macaron_List.Delete(current);
        // 用tmp接收current下一個node的記憶體位置
        Temporary_List.Push_front(tmp->label_num,tmp->macaron_num);
        // 利用Push_front將tmp以reverse的方式插入Temporary_List
    }
}

void select_the_target_plate()
{
    ListNode *nodes[7]= {NULL};
    // nodes[7]:用array紀錄除了current之外的node記憶體位置
    int node_cnt = 1;
    // nodes array搭配的index
    int macaron_array[7][2]={0};
    // macaron_array[7][0]:紀錄除了current之外的node對應的marcaron數量
    // macaron_array[7][1]:標記對應最大marcaron數量的index為1
    int smaller_node_cnt = 0;
    // 紀錄比current macaron數量小的node有幾個
    int max_smaller_macaron = 0;
    // 紀錄比current macaron數量小的node之中,最大的macaron數量
    int max_bigger_macaron = 0;
    // 紀錄比current macaron數量大的node之中,最大的macaron數量
    int max_macaron_cnt = 0;
    // 紀錄有最大macaron數量的node有幾個
    int node_distance[7][4]={0};
    // node_distance[7][0]: :紀錄以順時針計算與current的距離
    // node_distance[7][1]: :紀錄以逆時針計算與current的距離
    // node_distance[7][2]: :紀錄以最小距離(順時針與逆時針距離之中最小)計算與current的距離
    // node_distance[7][3]: :標記對應最大marcaron數量且與current距離最小的index為1
    int min_distance = INF;
    // 紀錄有最大marcaron數量的node們,其中與current最近的距離

    ListNode *tmp = current->next;
    // tmp紀錄current下一個node的記憶體位置
    int current_macaron = current->macaron_num;
    // current_macaron紀錄current node的macaron數量

    while(tmp!=current)
    {
        int tmp_macaron = tmp->macaron_num;
        // tmp_macaron紀錄tmp node的macaron數量
        nodes[node_cnt]=tmp;
        // 把tmp放入nodes陣列
        macaron_array[node_cnt][0]=tmp_macaron;
        // 把tmp_macaron放入macaron_array[node_cnt][0]陣列

        if(tmp_macaron < current_macaron)
        // 當tmp node macaron數量比current macaron數量少
        {
            max_smaller_macaron = max(max_smaller_macaron,tmp_macaron);
            // 更新max_smaller_macaron
            smaller_node_cnt++;
            // 累計smaller_node_cnt
        }
        else
        // 當tmp node macaron數量比current macaron數量多
        {
            max_bigger_macaron = max(max_bigger_macaron,tmp_macaron);
            // 更新max_bigger_macaron
        }
        tmp = tmp->next;
        // 更新tmp,往一個node執行
        node_cnt++;
        // 更新node_cnt
    }

    if(smaller_node_cnt==0){
    // 當smaller_node_cnt數量為0,
    // 代表current node的macaron數量最少,
    // 要找macaron 陣列中所有node最大的marcaron數量
        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][0]==max_bigger_macaron){
            // 當macaron_array[i][0]為max_bigger_macaron
                macaron_array[i][1]=1;
                // 標記對應有最大macaron數量index為1,放入macaron_array[i][1]陣列
                max_macaron_cnt++;
                // 累計max_macaron_cnt
            }
        }
    }else{
    // 當smaller_node_cnt數量不為0,
    // 代表current node的macaron數量不是最少的,
    // 要找macaron陣列中比curren marcaron數量少的所有nodes,其中最大的marcaron數量
        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][0]==max_smaller_macaron){
            // 當macaron_array[i][0]為max_smaller_macaron
                macaron_array[i][1]=1;
                // 標記對應有最大macaron數量index為1,放入macaron_array[i][1]陣列
                max_macaron_cnt++;
                // 累計max_macaron_cnt
            }
        }
    }

    if(max_macaron_cnt==1){
    // 當max_macaron_cnt=1代表最大macaron數量的node只有一個,則此node為target
        for(int i=1;i<=node_cnt;i++){
            if(macaron_array[i][1]==1){
            // 利用macaron_array[i][1]的標記找出target index
                target = nodes[i];
                // nodes[i]設定為target
                break;
            }
        }
    }else{
    // 當max_macaron_cnt=1代表最大macaron數量的node有多個,
    // 則需要再進一步計算這些node與current之間的距離
        for(int i=1;i<node_cnt;i++){
            node_distance[i][0]=i;
            // 建立對應index node以順時針與current距離陣列,用node_distance[i][0]記錄
            node_distance[i][1]=abs(i-7);
            // 建立對應index node以逆時針與current距離陣列,用node_distance[i][1]記錄
            node_distance[i][2]=min(node_distance[i][0],node_distance[i][1]);
            // 建立對應index node與current最小距離陣列,用node_distance[i][2]記錄
        }

        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][1]==1){
            // 利用macaron_array[i][1]的標記,挑出最大macaron數量的index
                min_distance = min(min_distance,node_distance[i][2]);
                // 更新這些最大macaron數量的index與current的最小距離,用min_distance記錄
            }
        }

        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][1]==1 && node_distance[i][2]==min_distance){
            // 當此node為最大macaron數量且與current距離為最短
                node_distance[i][3]=1;
                // 標記此node為最大macaron數量且與current距離為最短的index為1,放入node_distance[i][3]陣列
            }
        }

        for(int i=1;i<node_cnt;i++){
            if(node_distance[i][3]==1){
            // 當最大macaron數量且與current距離為最短時
            // 當出現這種node的數量為1,則直接選則此node為target
            // 當出現這種node的數量>1,則按照規則選順時針最靠近的node為target
            // 兩種情況處理方式都是for迴圈找到第一個符合node_distance[i][3]==1的node當作target
                target = nodes[i];
                // nodes[i]設定為target
                break;
            }
        }
    }
}

void eat_macarons()
{
    if(target->macaron_num < target->label_num)
    // 當target的macaron數量比target的label小
    {
        target->macaron_num += 50;
        // target的macaron數量增加50
    }
    target->macaron_num -=  target->label_num;
    // target的macaron數量扣除target的label
}

void put_back_the_removed_plates()
{
    ListNode *prec = target->next;
    // 紀錄target的下一個node為prec
    target->next = Temporary_List.first;
    // 將target的next設定為Temporary_List的開頭
    // 代表Temporary_List串接在target後面
    Temporary_List.last->next = prec;
    // 將Temporary_List的last設定為prec
    // 代表Temporary_List串接在prec前面
    // 將Temporary_List以reverse方式插入回到Macaron_List
    Temporary_List.first = NULL;
    // 將Temporary_List的first設定為null
    Temporary_List.last = NULL;
    // 將Temporary_List的last設定為null
    // 將Temporary_List恢復為空的Linked List
}

int main()
{

#ifndef OJ
    freopen("testdata.txt", "r", stdin);
#endif // OJ

    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int label = 1;
    // 初始化label變數
    int macaron;
    // 紀錄marcaron數量

    while(cin >> macaron && !cin.eof())
    {
        Macaron_List.Push_back(label++, macaron);
        // 吃輸入並以Push back方式插入到Macaron List內,依照label從小到大的次序
    }


    while(current_round <= total_round)
    // 迴圈執行total round次以下指令
    {
        select_current_plate();
        remove_plates();
        select_the_target_plate();
        eat_macarons();
        put_back_the_removed_plates();
        current_round++;
    }

    Macaron_List.PrintResult(target);
    // 印出執行完total round之後的Macaron List,從target開始印到結尾

    //cout << "\n";

    return 0;
}
