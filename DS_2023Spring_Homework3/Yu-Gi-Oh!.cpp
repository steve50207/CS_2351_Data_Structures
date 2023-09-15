#include <iostream>
#include <string.h>
//#define OJ
using namespace std;

int summon_cnt = 0; // 總共召喚的monster數量
int summon_num = 0; // 目前已召喚的monster數量,輸出判斷用

class BinaryTree;
class TreeNode{

private:
    TreeNode *left;
    TreeNode *right;
    int stars;

    TreeNode(int monster_stars):left(nullptr),right(nullptr),stars(monster_stars){};

    friend class BinaryTree;
};

class BinaryTree{

public:

    TreeNode *root;
    int total_damage;

    BinaryTree():total_damage(0){
        root = new TreeNode(0);                 // 預設root的monster stars = 0
    };

    void Summon_Monster(int monster_stars);     // 傳入Monster的星數,依照召喚規則修改Binary Tree
    void Attack_Wall();                         // 計算目前real world monster對wall的傷害,並累計到對wall的總傷害量
    void Print_Result();                        // 印出指令執行結果
    void Preorder_Traversal(TreeNode *current); // 印出目前Binary Tree的Preorder Traversal (中左右次序)
    void Print_Total_Damage();                  // 印出對wall的總傷害量

};

void BinaryTree::Summon_Monster(int monster_stars){ // 傳入Monster的星數,依照召喚規則修改Binary Tree

    int sacrifice_num;  // 召喚需要犧牲的怪物數量

    if(monster_stars <= 4){ 
        sacrifice_num = 0;  // 如果要召喚monster_stars <= 4, 則犧牲的怪物數量為0
    }else if(monster_stars >= 5 && monster_stars <= 6){
        sacrifice_num = 1;  // 如果要召喚 5 <= monster_stars <= 6, 則犧牲的怪物數量為1
    }else if(monster_stars >= 7 && monster_stars <= 8){
        sacrifice_num = 2;  // 如果要召喚 7 <= monster_stars <= 8, 則犧牲的怪物數量為2
    }

    if(sacrifice_num==0){                                   // 犧牲的怪物數量為0
        if(!root->left && !root->right){                    // root的左右皆為null
            root->left = new TreeNode(monster_stars);       // 在root的左召喚monster
        }else if(root->left && !root->right){               // root的左非null且右為null
            root->right = new TreeNode(monster_stars);      // 在root的右召喚monster
        }
    }else if(sacrifice_num==1){                                     // 犧牲的怪物數量為1
        if(root->left && !root->right){                             // root的左非null且右為null
            TreeNode *leftchild = root->left;                       // 紀錄目前root的左為leftchild
            TreeNode *newNode = new TreeNode(monster_stars);        // 建立新的monster
            root->left = newNode;                                   // root的左接新的monster
            newNode->left = leftchild;                              // 新的monster的左接leftchild
        }else if(root->left && root->right){                        // root的左右皆非null
            int left_stars = root->left->stars;                     // 紀錄目前root left的星數為left stars
            int right_stars = root->right->stars;                   // 紀錄目前root right的星數為right stars
            if(left_stars <= right_stars){                          // 若left stars < right stars 或left stars = right stars
                TreeNode *leftchild = root->left;                   // 紀錄目前root的左為leftchild
                TreeNode *newNode = new TreeNode(monster_stars);    // 建立新的monster
                root->left = newNode;                               // root的左接新的monster
                newNode->left = leftchild;                          // 新的monster的左接leftchild   
             }else{                                                 // 若left stars > right stars
                TreeNode *rightchild = root->right;                 // 紀錄目前root right為rightchild
                TreeNode *newNode = new TreeNode(monster_stars);    // 建立新的monster
                root->right = newNode;                              // root的右接新的monster
                newNode->right = rightchild;                        // 新的monster的右接leftchild   
             }
        }
    }else if(sacrifice_num==2){                                     // 犧牲的怪物數量為2
        if(root->left && root->right){                              // root的左右皆非null
            root->stars = monster_stars;                            // 修改root的stars為monster_stars
            TreeNode *newNode = new TreeNode(0);                    // 建立新的node(預設star為0)
            newNode->left = root;                                   // 將新的node的左接root(當作新的monster)
            root = newNode;                                         // 把新的node更新為root
        }
    }
}

void BinaryTree::Attack_Wall(){     // 計算目前real world monster對wall的傷害,並累計到對wall的總傷害量

    int sum = 0;                    // sum紀錄目前real world monster對wall的傷害
    if(root->left){                 // 如果real world左邊有monster
        sum+=root->left->stars;     // 累計左邊monster星數為對wall傷害
    }
    if(root->right){                // 如果real world右邊有monster
        sum+=root->right->stars;    // 累計左邊monster星數為對wall傷害
    }
    total_damage += sum;            // total damage累計加總對wall傷害
}

void BinaryTree::Preorder_Traversal(TreeNode *current){     // 印出目前Binary Tree的Preorder Traversal (中左右次序)

    if(current){                                            // 如果當前node current存在
        if(current->stars != 0){                            // 若當前node不為root
            summon_num++;                                   // 目前已召喚的monster數量+1
            if(summon_num<summon_cnt){                      // 若目前已召喚的monster數量 < 總共召喚的monster數量
                cout << current->stars << "* ";             // 代表要輸出"* "
            }else if(summon_num==summon_cnt){               // 若目前已召喚的monster數量 = 總共召喚的monster數量
                cout << current->stars << "*";              // 代表要輸出"*" (最後一隻monster)
            }
        }
        Preorder_Traversal(current->left);                  // 遞迴呼叫current的left
        Preorder_Traversal(current->right);                 // 遞迴呼叫current的right
    }
}

void BinaryTree::Print_Total_Damage(){                      // 印出對wall的總傷害量
    cout << total_damage << "\n";                           // 印出total_damage
}

void BinaryTree::Print_Result(){                            // 印出指令執行結果

    Preorder_Traversal(root);                               // 第一行印出目前Binary Tree的Preorder Traversal (中左右次序)
    cout << "\n";                                           // 換行
    Print_Total_Damage();                                   // 第二行印出對wall的總傷害量
}

int main()
{

#ifndef OJ
    freopen("testdata.txt", "r", stdin);
#endif // OJ

    BinaryTree T;
    char cmd[20]={"\0"};            // 用cmd字串紀錄輸入指令

    while (!cin.eof())              // 吃輸入直到eof
    {
        memset(cmd,0,sizeof(cmd));  // 清空cmd字串
        cin >> cmd;                 
        if(!strcmp(cmd,"Summon")){  // 若cmd為"Summon"
            char tmp[20]={"\0"};                                // 用tmp紀錄"->"吃掉
            cin >> tmp;                                         
            char str_monster_stars[20]={"\0"};                  // 用str_monster_stars紀錄star&"*"符號
            cin >> str_monster_stars;
            int int_monstars_stars = str_monster_stars[0]-'0';  // 轉換string to int,紀錄monster star
            summon_cnt++;                                       // 總共召喚的monster數量+1
            T.Summon_Monster(int_monstars_stars);               // 傳入Monster的星數,依照召喚規則修改Binary Tree
        }else if(!strcmp(cmd,"Attack")){                        // 若cmd為"Attack"
            T.Attack_Wall();                                    // 計算目前real world monster對wall的傷害,並累計到對wall的總傷害量
        }else if(!strcmp(cmd,"TheEnd")){                        // 若cmd為"TheEnd"
            T.Print_Result();                                   // 印出指令執行結果
        }
    }

    return 0;
}
