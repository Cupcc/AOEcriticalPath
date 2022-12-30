#include "widget.h"
#include "./ui_widget.h"

#include<QGraphicsRectItem>
#include<QGraphicsLineItem>
#include<QGraphicsEllipseItem>
#include<QGraphicsSimpleTextItem>
#include<math.h>
const int NODE_RADIUS = 40;
const int FONT_SIZE = 15;
using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->tableWidget->clearContents();//只清除工作区，不清除表头
    ui->tableWidget->setRowCount(4);//设置数据区行数

    ui->tableWidget->setVerticalHeaderItem(0, new QTableWidgetItem("Ee")); //设置垂直表头
    ui->tableWidget->setVerticalHeaderItem(1, new QTableWidgetItem("El"));
    ui->tableWidget->setVerticalHeaderItem(2, new QTableWidgetItem("Ae"));
    ui->tableWidget->setVerticalHeaderItem(3, new QTableWidgetItem("Al"));
    ui->tableWidget->setAlternatingRowColors(true); //设置交替行背景颜色

    QRectF rect(-100,0,width(),400);
    scene = new QGraphicsScene(rect);
    ui->graphicsView->setScene(scene);
    QGraphicsRectItem *item = new QGraphicsRectItem(rect);
    QPen pen;
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    item ->setPen(pen);
    scene->addItem(item);
    // 设置背景色
    scene->setBackgroundBrush(QBrush(qRgb(173,241,174)));
    ui->graphicsView->setScene(scene);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setTable(std::vector<int> &Ee, std::vector<int> &El, std::vector<int> &Ae, std::vector<int> &Al)
{
    ui->tableWidget->setColumnCount(Ee.size()>Ae.size()?Ee.size():Ae.size()); //设置列数

    for(int i=0 ; i<Ee.size(); ++i){
        QString str = QString::asprintf("%d",Ee[i]);
        QTableWidgetItem *item1;
        item1 = new  QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //文本对齐格式
        ui->tableWidget->setItem(0,i,item1); //为单元格设置Item
        QTableWidgetItem *item2= new QTableWidgetItem(QString::asprintf("%d",El[i]));
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(1,i,item2);
        ui->tableWidget->setColumnWidth(i,50);
    }


    for(int i=0 ; i<Ae.size(); ++i){
        QString str = QString::asprintf("%d",Ae[i]);
        QTableWidgetItem *item1;
        item1 = new  QTableWidgetItem(str);
        item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //文本对齐格式
        ui->tableWidget->setItem(2,i,item1); //为单元格设置Item
        QTableWidgetItem *item2= new QTableWidgetItem(QString::asprintf("%d",Al[i]));
        item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(3,i,item2);
        ui->tableWidget->setColumnWidth(i,50);
    }
}

void DrawLineWithArrow(QGraphicsScene * painter,QPen &pen,QBrush &brush, QPointF start, QPointF end)
{

    qreal arrowSize = 10;


    QLineF line(end, start);

    double angle = std::atan2(-line.dy(), line.dx());

    qreal length = line.length();
    QPointF edgeOffset((line.dx() * 20) / length, (line.dy() * 20) / length);
    end = line.p1() + edgeOffset;
    start = line.p2() - edgeOffset;
    line.setP1(end);
    line.setP2(start);

    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                          cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                          cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;

    painter->addLine(line,pen);
    painter->addPolygon(arrowHead,pen,brush);
}

void Widget::paintGraph(std::vector<std::vector<VertexNode> > &graph,std::vector<std::string> &indexName, std::vector<int> &Ae, std::vector<int> &Al)
{

    int r[]={3,1,4,6,3,6,1,3,2,5,1,3,4,6,1,4,6,3,6};
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);

    //绘制结点
    for(int i =0; i<indexName.size(); ++i){
        QPointF center((i)*(NODE_RADIUS+10), r[i] * NODE_RADIUS);
        QRectF rect(center.x()-NODE_RADIUS/2,center.y()-NODE_RADIUS/2,NODE_RADIUS,NODE_RADIUS);
        QGraphicsEllipseItem *node = new QGraphicsEllipseItem(rect);
        node->setBrush(QBrush(Qt::yellow));

//        node->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
        QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(QString::asprintf("%d",i));
        label->setPos(center.x()-label->boundingRect().width()/2, center.y()-label->boundingRect().height()/2);

        scene->addItem(node);
        scene->addItem(label);
    }

    // 绘制边
    for(int i =0;i<graph.size();i++){

        QPointF start((i) * (NODE_RADIUS+10),r[i] * NODE_RADIUS);

        for(int j=0; j< graph[i].size(); j++){

            int currentIndex = graph[i][j].index;
            QPointF end(currentIndex*(NODE_RADIUS+10), r[currentIndex] * NODE_RADIUS);

//            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(start,end));

//            line->setPen(pen);

            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            int no = graph[i][j].no;
            if(Ae[no]==Al[no]){
                pen.setColor(Qt::red);
                brush.setColor(Qt::red);
            }else{
                pen.setColor(Qt::black);
                brush.setColor(Qt::black);
            }
            DrawLineWithArrow(scene,pen,brush,start,end);
//            scene->addItem(line);
            QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(QString::asprintf("a%d=%d",no+1,graph[i][j].dur));
            label->setPos((start.x()+end.x())/2-20,(start.y()+end.y())/2);
            scene->addItem(label);
        }
    }




}



