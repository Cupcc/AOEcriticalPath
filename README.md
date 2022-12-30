# 项目介绍

AOE网络求关键路径，C/C++配合QT实现，跨平台图形界面。

## 问题描述

【问题描述】

以某一工程为蓝本，采用图的结构表示实际的工程计划的时间。调查以分析和预测这个工程计划各个阶段的时间。用调查的结果(使用图7.29)建立AOE网（Activity On Edge Network），即边表示活动的网络，并用图的形式表示，用图来存储这些信息。

【基本要求】

（1）要求输入使用教材图7.29，将数据存放在origin.txt，直接读出；

（2）将项目中的各项活动视为有一个时间属性的结点，用有方向的线段标出各结点的紧前活动和紧后活动的关系，使之成为一个有方向的网络图，在界面中输出；

（3）用正推法和逆推法计算出各个活动的最早开始时间，最晚开始时间，最早完工时间和最迟完工时间，并计算出各个活动的时差，分别输出顶点表和活动表，参图7.31。 

（4）找出所有时差为零的活动所组成的路线，即为关键路径，并在图中描红；

（5）采用图形界面。

<img width="852" alt="image" src="https://user-images.githubusercontent.com/53716543/210078918-746a0d5b-5725-482b-b56e-6ef63235e116.png">
## 环境配置

## 展示
运行展示

<img width="824" alt="image" src="https://user-images.githubusercontent.com/53716543/210078525-c7b7e452-d2fa-46e9-939f-a5f18e98f294.png">

## 环境配置
### 下载qt>=5.10以上版本
### 使用qt creator创建项目
1. 选择QWidgets Application
<img width="1116" alt="image" src="https://user-images.githubusercontent.com/53716543/210078664-a891df0f-c52b-4c0c-b8c5-e92c3e8aa2e9.png">
2. 填写项目名称和存放路径
<img width="788" alt="image" src="https://user-images.githubusercontent.com/53716543/210078708-87c25ebf-8fe2-4f25-8b8c-464a0179fc7e.png">
3. 选择cmake或qmake都可以
<img width="792" alt="image" src="https://user-images.githubusercontent.com/53716543/210079026-a429c14a-45ac-4870-88c8-28ae98bad725.png">
4. 选择Qwidget
<img width="796" alt="image" src="https://user-images.githubusercontent.com/53716543/210079101-9c3bb703-3880-4b7d-b909-296baf431517.png">
5. 语言：无
6. 选择一个编译工具，Macos是clang，windows系统是mingw
<img width="795" alt="image" src="https://user-images.githubusercontent.com/53716543/210079142-c48edce2-b88c-479a-a4ff-91f5b83f8e20.png">


7. 无， 点完成

8. 将本仓库代码粘贴进项目并替换

下列代码复制，粘贴进新建的项目，其他文件不用动。
- origin.txt 
- widget.cpp
- widget.h
- main.h
- widget.ui

9. 打开Qt creator 左下角点击绿色按钮构建，若提示是否保存文件，点全部保存。
