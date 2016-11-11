#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <ctime>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class AntiFraud{
public:
    AntiFraud(const char* batchfile){
        preprocessing(batchfile); // Constructor invokes preprocessing function to initialize the friends network
    }
    
    void process(const char* streamfile, const char* outputfile, int feature){ // Read each payment record in "stream_payment.txt", determine whether it is trusted according to the given feature (1, 2, or 3), and then write the result ("trusted" or "unverified") into the corresponding output file
        cout << "Processing stream payment with Feature " << feature << " ..." << endl;
        ifstream input(streamfile);
        ofstream output(outputfile);
        int dist;
        if(feature==1)
            dist = 1;
        else if(feature==2)
            dist = 2;
        else
            dist = 4;
        string line;
        getline(input, line);
        time_t begin = time(NULL);
        int count = 0;
        while(getline(input, line)){
            istringstream ss(line);
            string token;
            getline(ss, token, ',');
            getline(ss, token, ',');
            int id1 = stoi(token);
            getline(ss, token, ',');
            int id2 = stoi(token);
            if(query(id1, id2, dist)) // Invoke the query function defined below to decide whether id1 and id2 are considered "friends" according to the given feature
                output << "trusted" << endl;
            else
                output << "unverified" << endl;
            ++count;
        }
        time_t end = time(NULL);
        cout << "Processed " << count << " payments with Feature " << feature << endl;
        cout << "Average processing time per payment with Feature " << feature << ": " << (end-begin)/(count*1.0) << "s" << endl;
        input.close();
        output.close();
    }
private:
    // The preprocessing function initializes the friends network specified by "batch_payment.txt"
    void preprocessing(const char* batchfile){
        cout << "Building graph, please wait..." << endl;
        ifstream input(batchfile);
        string line;
        getline(input, line);
        int count = 0;
        while(getline(input, line)){
            istringstream ss(line);
            string token;
            getline(ss, token, ',');
            getline(ss, token, ',');
            int id1 = stoi(token);
            getline(ss, token, ',');
            int id2 = stoi(token);
            ++count;
            map[id1].insert(id2);
            map[id2].insert(id1);
        }
        cout << "Graph building finished" << endl;
        cout << "The graph " << map.size() << " nodes with average " << (2*count)/(map.size()*1.0) << " neighbors per node" << endl;
        input.close();
    }
    
    // The query function checks whether id2 is within dist-th friends network of id1, which is implemented through BFS
    bool query(int id1, int id2, int dist){
        if(map.find(id1)==map.end() || map.find(id2)==map.end())
            return false;
        if(map[id1].find(id2)!=map[id1].end())
            return true;
        if(dist==1) return false;
        
        // BFS that determines whether id1 and id2 are connected through at most dist-degree of separations
        // Let n be the average number of neighbors per node. The naive BFS will traversal the graph starting from id1 and stops when one of the following conditions are satisfied:
        // 1) id2 is reached
        // 2) dist levels are traversed
        // Obviously, the worst-case time complexity of the naive BFS is O(n^(dist))
        // Instead, the following code performs a smart BFS, which launches two branches of traversal, one starting from id1, and the other starting from id2. In this way, the BFS will go through at most dist/2 levels, and may end earlier if the two branches intersect. Therefore, it has a worst-case time complexity O(n^(dist/2)), which is much faster than the naive BFS when n is large
        queue<int> q;
        q.push(id1);
        q.push(id2);
        unordered_set<int> set1, set2;
        set1.insert(id1);
        set2.insert(id2);
        int level = 0;
        while(!q.empty() && 2*level+1<=dist){
            ++level;
            int size = q.size();
            for(int i = 1; i<=size; ++i){
                int cur = q.front(); q.pop();
                if(set1.count(cur)){
                    for(int nb: map[cur]){
                        if(!set1.count(nb)){
                            if(set2.count(nb)){
                                return 2*level-1<=dist;
                            }else{
                                set1.insert(nb);
                                q.push(nb);
                            }
                        }
                    }
                }else{
                    for(int nb: map[cur]){
                        if(!set2.count(nb)){
                            if(set1.count(nb)){
                                return 2*level<=dist;
                            }else{
                                set2.insert(nb);
                                q.push(nb);
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
    
    unordered_map<int, unordered_set<int> > map; //The undirected graph that represents the friends network specified by "batch_payment.txt"
};

int main(int argc, const char * argv[]) {
    if(argc != 6){
        cout << "Invalid Input\n";
    }else{
        AntiFraud af(argv[1]);
        for(int f = 1; f<=3; ++f){
            af.process(argv[2], argv[2+f], f);
        }
    }
    return 0;
}
