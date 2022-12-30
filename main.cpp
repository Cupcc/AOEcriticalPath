#include "widget.h"

#include <QApplication>


using namespace std;
//typedef struct Vertex
//{
//    string name; // 结点名
//    int index;   // 索引
//    int dur;     // 活动持续时间
//    int no;      // 活动序号
//    Vertex(string inputName, int inputIndex, int inputDur, int inputNo) : name(inputName), index(inputIndex), dur(inputDur), no(inputNo){};
//} VertexNode;

std::unordered_map<std::string, int> graphMap; // 存储图 名字->索引
std::vector<std::string> indexName;            // 反向映射 索引->名字

std::vector<std::vector<VertexNode>> adjGraph;        // 邻接表
std::vector<std::vector<VertexNode>> reverseAdjGraph; // 逆邻接表
// 声明------------------------------------------------
std::vector<int> id;    // 入度
std::vector<int> invId; // 出度
std::vector<int> Ee;    // 事件最早开始时间
std::vector<int> El;    // 事件最晚开始时间
std::vector<int> Ae;    // 活动最早开始时间
std::vector<int> Al;    // 活动最晚开始时间

int actNum = 0; // 活动序号
int graphSize;
// 声明END------------------------------------------


// graph邻接表，revGraph逆邻接表 ，id入度，invId出度，topSordId拓扑排序索引序列
bool criticalPath(const vector<vector<VertexNode>> &graph, const vector<vector<VertexNode>> &revGraph,
                  vector<int> &id, vector<int> &invId, vector<int> &Ee, vector<int> &El, vector<int> &Ae, vector<int> &Al)
{

    vector<int> topSordId; // 拓扑排序
    topSordId.reserve(graph.size());

    // 1. 拓扑排序  Ee事件最早开始时间
    stack<int> st;
    for (int i = 0; i < graph.size(); ++i)
    {
        if (id[i] == 0)
        {
            st.push(i);
        }
    }

    for (int i = 0; i < graph.size(); i++)
    {
        if (st.empty())
        {
            cerr << "不存在入度为0的结点!" << endl;
            return false;
        }
        int currentIndex = st.top(); // 栈顶元素
        st.pop();
        topSordId.push_back(currentIndex);
        for (int k = 0; k < graph[currentIndex].size(); k++)
        {
            int linkIndex = graph[currentIndex][k].index;
            int linkDur = graph[currentIndex][k].dur;
            if (--id[linkIndex] == 0)
            {
                st.push(linkIndex);
            }
            // 事件最早发生时间
            Ee[linkIndex] = max(Ee[currentIndex] + linkDur, Ee[linkIndex]);
        }
    }

    // 2. 逆拓扑排序  El时间最晚开始时间
    stack<int> st2; // 存放出度为0的
    for (int i = 0; i < El.size(); i++)
    {
        El[i] = Ee[Ee.size() - 1]; // 结点赋值为最大值
        if (invId[i] == 0)
        {
            st2.push(i);
        }
    }
    for (int i = 0; i < revGraph.size(); i++)
    {
        int currentIndex = st2.top();
        st2.pop();
        for (int k = 0; k < revGraph[currentIndex].size(); k++)
        {
            int linkIndex = revGraph[currentIndex][k].index;
            int linkDur = revGraph[currentIndex][k].dur;
            if (--invId[linkIndex] == 0)
            {
                st2.push(linkIndex);
            }
            El[linkIndex] = min(El[currentIndex] - linkDur, El[linkIndex]);
        }
    }

    // 3. 判断是否为关键活动
    for (int i = 0; i < graph.size(); i++)
    {
        for (int j = 0; j < graph[i].size(); j++)
        {

            int linkIndex = graph[i][j].index;
            int linkDur = graph[i][j].dur;
            int no = graph[i][j].no;
            Ae[no] = Ee[i];                   // 活动最早开始时间 = 初始结点的Ve值(事件最早开始时间)
            Al[no] = El[linkIndex] - linkDur; // 活动最晚开始时间= 终结点最晚开始时间-活动时间
            if (Ae[no] == Al[no])
            {
                cout << "a" << no + 1 << ":" << i << "->" << linkIndex;
                cout << " 持续时间:" << graph[i][j].dur << endl;
            }
        }
    }

    return true;
}

bool readFile()
{
    ifstream read("./origin.txt");

    if (!read.good())
    {
        cerr << "文件不存在!" << endl;
        return false;
    }
    string line;
    int index = 0;
    // 构造图信息
    while (getline(read, line))
    {

        istringstream iss(line);
        string tmp1, tmp2; // tmp1存储结点名 tmp2存储活动时长
        if (iss >> tmp1)
        {
            if (graphMap.find(tmp1) == graphMap.end())
            {
                // 没找到就插入
                graphMap.insert(make_pair(tmp1, index++));
            }
        }
        while (iss >> tmp1 >> tmp2)
        {
            auto itr = graphMap.find(tmp1) == graphMap.end();

            if (graphMap.find(tmp1) == graphMap.end())
            {
                graphMap.insert(make_pair(tmp1, index++));
            }
        }
    }
    graphSize = graphMap.size(); // 获取结点数量

    // 反向映射
    indexName.resize(graphSize); // 开辟空间
    for (auto &itr : graphMap)
    {
        indexName[itr.second] = itr.first;
        int second = itr.second;
    }

    // 重新读取
    read.clear();
    read.seekg(0, ios::beg);           // 重置到开头
    adjGraph.resize(graphSize);        // 邻接矩阵开辟空间
    reverseAdjGraph.resize(graphSize); // 逆邻接表开辟空间
    int currentIndex = 0;              // 当前结点的索引值

    id.resize(graphSize);    // 入度
    invId.resize(graphSize); // 出度
    Ee.resize(graphSize);    // 事件最早开始时间
    El.resize(graphSize);    // 事件最晚开始时间

    while (getline(read, line))
    {
        istringstream iss(line);
        int inputDur;
        string name; // 存储结点名字

        if (iss >> name)
        {

            currentIndex = graphMap[name]; // 根据名字获取结点索引
        }
        else
        {
            cerr << "read error!" << endl;
        }

        while (iss >> name >> inputDur)
        {

            adjGraph[currentIndex].emplace_back(VertexNode(name,
                                                           graphMap[name],
                                                           inputDur,
                                                           actNum++));
            id[graphMap[name]]++;  // 终点 入度+1
            invId[currentIndex]++; // 始点 出度+1
            // 构造逆邻接表
            reverseAdjGraph[graphMap[name]].emplace_back(VertexNode(
                indexName[currentIndex],
                currentIndex,
                inputDur, 0));
        }
    }
    Ae.resize(actNum); // 活动最早开始时间
    Al.resize(actNum); // 活动最晚开始时间
    return true;
}
int runAoe(){
    if (readFile())
        {
            if (criticalPath(adjGraph, reverseAdjGraph, id, invId, Ee, El, Ae, Al))
            {
                cout << "关键路径测试成功！" << endl;
            }
            else
            {
                cerr << "网络中存在闭环！" << endl;
            }
        }else{
            cerr << "运行错误！" <<endl;
            return -1;
        }
    return 1;
}


int main(int argc, char *argv[])
{
    if (readFile())
        {
            if (criticalPath(adjGraph, reverseAdjGraph, id, invId, Ee, El, Ae, Al))
            {
                cout << "关键路径测试成功！" << endl;
            }
            else
            {
                cerr << "网络中存在闭环！" << endl;
            }
        }else{
            cerr << "运行错误！" <<endl;
            return -1;
        }
    QApplication a(argc, argv);
    Widget w;
    w.setTable(Ee,El,Ae,Al);
    w.paintGraph(adjGraph,indexName,Ae,Al);
    w.show();
    return a.exec();
}
