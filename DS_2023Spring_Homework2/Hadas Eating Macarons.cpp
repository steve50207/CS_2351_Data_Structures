#include <iostream>
#define OJ
#define INF 1001;
using namespace std;

class CircularLinkedList;

class ListNode
// �ŧiLinked List Node��class
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
// �ŧi Singly Circular Linked List��class
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
//�L�XLinked List�ҥ]�tnode��label�Pmacaron�ƶq,���Ǳqfirst�L��last
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
// �bLinked List��fisrt���e���s�Wnode
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
// �bLinked List��last���᭱�s�Wnode
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
// �Ncurrent���U�@��node���XLinked List,�æ^�ǲ���node���O�����m
{
    ListNode *prev = current;
    ListNode *del = current->next;
    ListNode *prec = del->next;

    prev->next = prec;

    return del;
}

void CircularLinkedList::PrintResult(ListNode *start)
//�L�XLinked List�ҥ]�tnode��label�Pmacaron�ƶq,���Ǳqstart�}�l�L�����
{
    ListNode *cur = start;

    while(cur->next!=start){
        cout << cur->label_num << " / " << cur->macaron_num << "\n";
        cur = cur->next;
    }
    cout << cur->label_num << " / " << cur->macaron_num;

}

CircularLinkedList Macaron_List;
// �]�wMacaron List
CircularLinkedList Temporary_List;
// �]�wTemporary List,�Ψө�ȮɳQ���X��node,�åB����reverse�ᦸ�ǩ�J
int current_round = 1;
// ��eround����
int total_round = 100;
// �`�@�n����round����
ListNode *current = NULL;
// current node�O�����m
ListNode *target = NULL;
// target node�O�����m

void select_current_plate()
{
    if(current_round==1){
        current = Macaron_List.first;
        // �]�wcurrent��Macaron_List��first
    }else{
        int current_label = (target->label_num)%10+1;
        // �̷��D�حn�D�]�wcurrent_label��target_label%10+1

        while(current->label_num != current_label){
            current = current->next;
        }
        // �Q��label_num��scurrent node

        ListNode *End = current->next;
        // �ŧiend node

        while(End->next!=current)
        {
            End = End->next;
        }
        // ��send node��current node���W�@��
        Macaron_List.first = current;
        // ��sMacaron_List��first��current
        Macaron_List.last = End;
        // ��sMacaron_List��last��end
    }

}

void remove_plates()
{
    ListNode *tmp;
    int remove_cnt = 3;
    // �n���X��node�ƶq
    while(remove_cnt--)
    {
        tmp = Macaron_List.Delete(current);
        // ��tmp����current�U�@��node���O�����m
        Temporary_List.Push_front(tmp->label_num,tmp->macaron_num);
        // �Q��Push_front�Ntmp�Hreverse���覡���JTemporary_List
    }
}

void select_the_target_plate()
{
    ListNode *nodes[7]= {NULL};
    // nodes[7]:��array�������Fcurrent���~��node�O�����m
    int node_cnt = 1;
    // nodes array�f�t��index
    int macaron_array[7][2]={0};
    // macaron_array[7][0]:�������Fcurrent���~��node������marcaron�ƶq
    // macaron_array[7][1]:�аO�����̤jmarcaron�ƶq��index��1
    int smaller_node_cnt = 0;
    // ������current macaron�ƶq�p��node���X��
    int max_smaller_macaron = 0;
    // ������current macaron�ƶq�p��node����,�̤j��macaron�ƶq
    int max_bigger_macaron = 0;
    // ������current macaron�ƶq�j��node����,�̤j��macaron�ƶq
    int max_macaron_cnt = 0;
    // �������̤jmacaron�ƶq��node���X��
    int node_distance[7][4]={0};
    // node_distance[7][0]: :�����H���ɰw�p��Pcurrent���Z��
    // node_distance[7][1]: :�����H�f�ɰw�p��Pcurrent���Z��
    // node_distance[7][2]: :�����H�̤p�Z��(���ɰw�P�f�ɰw�Z�������̤p)�p��Pcurrent���Z��
    // node_distance[7][3]: :�аO�����̤jmarcaron�ƶq�B�Pcurrent�Z���̤p��index��1
    int min_distance = INF;
    // �������̤jmarcaron�ƶq��node��,�䤤�Pcurrent�̪񪺶Z��

    ListNode *tmp = current->next;
    // tmp����current�U�@��node���O�����m
    int current_macaron = current->macaron_num;
    // current_macaron����current node��macaron�ƶq

    while(tmp!=current)
    {
        int tmp_macaron = tmp->macaron_num;
        // tmp_macaron����tmp node��macaron�ƶq
        nodes[node_cnt]=tmp;
        // ��tmp��Jnodes�}�C
        macaron_array[node_cnt][0]=tmp_macaron;
        // ��tmp_macaron��Jmacaron_array[node_cnt][0]�}�C

        if(tmp_macaron < current_macaron)
        // ��tmp node macaron�ƶq��current macaron�ƶq��
        {
            max_smaller_macaron = max(max_smaller_macaron,tmp_macaron);
            // ��smax_smaller_macaron
            smaller_node_cnt++;
            // �֭psmaller_node_cnt
        }
        else
        // ��tmp node macaron�ƶq��current macaron�ƶq�h
        {
            max_bigger_macaron = max(max_bigger_macaron,tmp_macaron);
            // ��smax_bigger_macaron
        }
        tmp = tmp->next;
        // ��stmp,���@��node����
        node_cnt++;
        // ��snode_cnt
    }

    if(smaller_node_cnt==0){
    // ��smaller_node_cnt�ƶq��0,
    // �N��current node��macaron�ƶq�̤�,
    // �n��macaron �}�C���Ҧ�node�̤j��marcaron�ƶq
        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][0]==max_bigger_macaron){
            // ��macaron_array[i][0]��max_bigger_macaron
                macaron_array[i][1]=1;
                // �аO�������̤jmacaron�ƶqindex��1,��Jmacaron_array[i][1]�}�C
                max_macaron_cnt++;
                // �֭pmax_macaron_cnt
            }
        }
    }else{
    // ��smaller_node_cnt�ƶq����0,
    // �N��current node��macaron�ƶq���O�̤֪�,
    // �n��macaron�}�C����curren marcaron�ƶq�֪��Ҧ�nodes,�䤤�̤j��marcaron�ƶq
        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][0]==max_smaller_macaron){
            // ��macaron_array[i][0]��max_smaller_macaron
                macaron_array[i][1]=1;
                // �аO�������̤jmacaron�ƶqindex��1,��Jmacaron_array[i][1]�}�C
                max_macaron_cnt++;
                // �֭pmax_macaron_cnt
            }
        }
    }

    if(max_macaron_cnt==1){
    // ��max_macaron_cnt=1�N��̤jmacaron�ƶq��node�u���@��,�h��node��target
        for(int i=1;i<=node_cnt;i++){
            if(macaron_array[i][1]==1){
            // �Q��macaron_array[i][1]���аO��Xtarget index
                target = nodes[i];
                // nodes[i]�]�w��target
                break;
            }
        }
    }else{
    // ��max_macaron_cnt=1�N��̤jmacaron�ƶq��node���h��,
    // �h�ݭn�A�i�@�B�p��o��node�Pcurrent�������Z��
        for(int i=1;i<node_cnt;i++){
            node_distance[i][0]=i;
            // �إ߹���index node�H���ɰw�Pcurrent�Z���}�C,��node_distance[i][0]�O��
            node_distance[i][1]=abs(i-7);
            // �إ߹���index node�H�f�ɰw�Pcurrent�Z���}�C,��node_distance[i][1]�O��
            node_distance[i][2]=min(node_distance[i][0],node_distance[i][1]);
            // �إ߹���index node�Pcurrent�̤p�Z���}�C,��node_distance[i][2]�O��
        }

        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][1]==1){
            // �Q��macaron_array[i][1]���аO,�D�X�̤jmacaron�ƶq��index
                min_distance = min(min_distance,node_distance[i][2]);
                // ��s�o�ǳ̤jmacaron�ƶq��index�Pcurrent���̤p�Z��,��min_distance�O��
            }
        }

        for(int i=1;i<node_cnt;i++){
            if(macaron_array[i][1]==1 && node_distance[i][2]==min_distance){
            // ��node���̤jmacaron�ƶq�B�Pcurrent�Z�����̵u
                node_distance[i][3]=1;
                // �аO��node���̤jmacaron�ƶq�B�Pcurrent�Z�����̵u��index��1,��Jnode_distance[i][3]�}�C
            }
        }

        for(int i=1;i<node_cnt;i++){
            if(node_distance[i][3]==1){
            // ��̤jmacaron�ƶq�B�Pcurrent�Z�����̵u��
            // ��X�{�o��node���ƶq��1,�h������h��node��target
            // ��X�{�o��node���ƶq>1,�h���ӳW�h�ﶶ�ɰw�̾a��node��target
            // ��ر��p�B�z�覡���Ofor�j����Ĥ@�ӲŦXnode_distance[i][3]==1��node��@target
                target = nodes[i];
                // nodes[i]�]�w��target
                break;
            }
        }
    }
}

void eat_macarons()
{
    if(target->macaron_num < target->label_num)
    // ��target��macaron�ƶq��target��label�p
    {
        target->macaron_num += 50;
        // target��macaron�ƶq�W�[50
    }
    target->macaron_num -=  target->label_num;
    // target��macaron�ƶq����target��label
}

void put_back_the_removed_plates()
{
    ListNode *prec = target->next;
    // ����target���U�@��node��prec
    target->next = Temporary_List.first;
    // �Ntarget��next�]�w��Temporary_List���}�Y
    // �N��Temporary_List�걵�btarget�᭱
    Temporary_List.last->next = prec;
    // �NTemporary_List��last�]�w��prec
    // �N��Temporary_List�걵�bprec�e��
    // �NTemporary_List�Hreverse�覡���J�^��Macaron_List
    Temporary_List.first = NULL;
    // �NTemporary_List��first�]�w��null
    Temporary_List.last = NULL;
    // �NTemporary_List��last�]�w��null
    // �NTemporary_List��_���Ū�Linked List
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
    // ��l��label�ܼ�
    int macaron;
    // ����marcaron�ƶq

    while(cin >> macaron && !cin.eof())
    {
        Macaron_List.Push_back(label++, macaron);
        // �Y��J�åHPush back�覡���J��Macaron List��,�̷�label�q�p��j������
    }


    while(current_round <= total_round)
    // �j�����total round���H�U���O
    {
        select_current_plate();
        remove_plates();
        select_the_target_plate();
        eat_macarons();
        put_back_the_removed_plates();
        current_round++;
    }

    Macaron_List.PrintResult(target);
    // �L�X���槹total round���᪺Macaron List,�qtarget�}�l�L�쵲��

    //cout << "\n";

    return 0;
}
