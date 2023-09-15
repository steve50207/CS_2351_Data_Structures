#include <iostream>
#include <string>
#include <vector>
#include <functional>
using namespace std;
//#define TestCase

/*
 * Hyper Learning Framework 1.0
 *  note to self: this is not DEEP learning
 *  note to self2: shroom is not bought on Guangfu Rd. Section 1
 *  it looks a lot like something I have learnt in CS 235100...?
 */
struct Fruit {
    string      name;
    vector<int> hyperFeatures;
    Fruit(string name):name(name){}
};

struct GradBinHyperWeight {
    int                featureIdx;
    double             gradSlider;
    int                binThreshold;
    GradBinHyperWeight *backwardContinuation = nullptr;
    GradBinHyperWeight *forwardContinuation = nullptr;
    
    GradBinHyperWeight(int featureIdx, double gradSlider, int binThreshold):featureIdx(featureIdx),gradSlider(gradSlider),binThreshold(binThreshold){}

    bool shouldBackward(Fruit& f) const
    {
        return f.hyperFeatures[featureIdx] < binThreshold;
    }
    void applyGradSlider(Fruit& f) const
    {
        f.hyperFeatures[featureIdx] = (int) ((double) f.hyperFeatures[featureIdx] * gradSlider);
    }
};

int Number_of_Fruit;
int Number_of_Hyperfeature;
int Number_of_GradBinHyperWeight;
vector<Fruit> Fruit_List;
vector<GradBinHyperWeight> GradBinHyperWeight_List;

// recursive to solve output
void Hyper_Learning_Architecture(vector<Fruit> Fruit_List,int tree_idx){

    /*
    cout << "tree_idx=" << tree_idx << "\n";
    cout << "Fruit_List\n";
    for(auto i:Fruit_List){
        cout << i.name << "\n";
        for(auto j:i.hyperFeatures){
            cout << j << " ";
        }
        cout << "\n";
    }
    */

    // calculate Fruit_List length
    int len = Fruit_List.size();

    // len==0 don't do anything
    if(len==0){
        return;
    }else if(len==1){
    // len==1 output the Fruit of Fruit_List and use GradBinHyperWeight_List and tree index to get the value of last used hyperfeature
        cout << Fruit_List[0].name << " ";
        if(tree_idx%2==0){ // from left child
            cout << Fruit_List[0].hyperFeatures[GradBinHyperWeight_List[tree_idx/2].featureIdx] <<"\n";
        }else if(tree_idx%2==1){ // from right child
            cout << Fruit_List[0].hyperFeatures[GradBinHyperWeight_List[(tree_idx-1)/2].featureIdx] <<"\n";
        }
        return;
    }else{
    // len > 1 
        bool flag = false;
        vector<Fruit> Left_Fruit_List;
        vector<Fruit> Right_Fruit_List;
        
        // use applyGradSlider and shouldBackward function to build the Left_Fruit_List and Right_Fruit_List
        for(int i=0;i<len;i++){
            GradBinHyperWeight_List[tree_idx].applyGradSlider(Fruit_List[i]);
            flag = GradBinHyperWeight_List[tree_idx].shouldBackward(Fruit_List[i]);
            if(flag==true){
                Left_Fruit_List.push_back(Fruit_List[i]);
            }else if(flag==false){
                Right_Fruit_List.push_back(Fruit_List[i]);
            }
        }
        // recursive call to pass the Fruit_List and tree index 
        Hyper_Learning_Architecture(Left_Fruit_List, tree_idx*2);
        Hyper_Learning_Architecture(Right_Fruit_List, tree_idx*2+1);
    }
}

int main(void){

#ifdef TestCase
    freopen("testdata.txt","r",stdin);
#endif

    // GET INPUT Number_of_Fruit
    cin >> Number_of_Fruit;
    // GET Number_of_Hyperfeature
    cin >> Number_of_Hyperfeature;
    // Build newFruit and Fruit_List
    for(int i=1;i<=Number_of_Fruit;i++){
        string Name;
        cin >> Name;
        Fruit newFruit = Fruit(Name);
        for(int i=1;i<=Number_of_Hyperfeature;i++){
            int Num;
            cin >> Num;
            newFruit.hyperFeatures.push_back(Num);
        }
        Fruit_List.push_back(newFruit);
    }

    // GET INPUT Number_of_GradBinHyperWeight
    cin >> Number_of_GradBinHyperWeight;

    // Push temp node in GradBinHyperWeight_List because the tree index start from 1
    GradBinHyperWeight_List.push_back(GradBinHyperWeight(0,0,0));

    // Build GradBinHyperWeight_List
    for(int i=1;i<=Number_of_GradBinHyperWeight;i++){
        int featureIdx;
        double gradSlider;
        int binThreshold;
        cin >> featureIdx >> gradSlider >> binThreshold;
        GradBinHyperWeight_List.push_back(GradBinHyperWeight(featureIdx,gradSlider,binThreshold));
    }

    /*
    for(auto i:Fruit_List){
        cout << i.name << "\n";
        for(auto j:i.hyperFeatures){
            cout << j << " ";
        }
        cout << "\n";
    }

    for(auto i:GradBinHyperWeight_List){
        cout << "featureIdx: " << i.featureIdx << " ,gradSlider: " << i.gradSlider << " ,binThreshold: " << i.binThreshold << "\n";
    }
    */

    // Recursive Call and start from tree index=1
    Hyper_Learning_Architecture(Fruit_List,1);

    return 0;
}