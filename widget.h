#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<vector>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stack>
typedef struct Vertex
{
    std::string name; // 结点名
    int index;   // 索引
    int dur;     // 活动持续时间
    int no;      // 活动序号
    Vertex(std::string inputName, int inputIndex, int inputDur, int inputNo) : name(inputName), index(inputIndex), dur(inputDur), no(inputNo){};
} VertexNode;



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    QGraphicsScene *scene;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void setTable(std::vector<int> &Ve,std::vector<int> &Vl,std::vector<int> &Ae,std::vector<int> &Al );

    void paintGraph(std::vector<std::vector<VertexNode>> &graph,std::vector<std::string> & indexName,
                    std::vector<int> &Ae,std::vector<int> &Al );
private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
