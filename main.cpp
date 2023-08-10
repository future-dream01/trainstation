#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <termios.h>
using namespace std;
/*----------------------------------------------头文件引用------------------------------------------------------------------------*/
struct Timetable//定义Timetable结构体
{
    string number;//班次
    int hour;//发车时间 小时
    int minute;//发车时间  分钟
    char starting_station[10];//始发站
    char last_station[10];////终点站
    float time;//行车时长
    int fare;//票价
    int max_number;//最大载客量
    int sold_number;//已售票数
};
int main_window_select();
int admin_window_select();
int passenager_window_select() ;
void admin_mode(Timetable *timetables, char *filename); 
void passenager_mode(Timetable *timetables ,char *filename);
void signin();
int read_from_file(Timetable *timetables,int n,char *filename);
int write_to_file(Timetable *timetales,int n,char *filename);
int add_bus(Timetable *timetables,int n);
int delete_bus(Timetable *timetables,int n);
void show_timetables(Timetable *timetables,int n);
void show_bus_information(Timetable *timetables,int n,int i);
void query (Timetable *timetables,int n);
int  find(Timetable *timetables,int n,string j);//按班次查询
int  find(Timetable *timetables,int n,string j,int a[100]);//按终点站查询
void generate_log_filename(char  *log_filename);
int initialization_passagermode (Timetable *timetables, char *log_filename,char *filename);
void ticket_order(Timetable *timetables,int n);
void ticket_delete(Timetable *timetables,int n);
int stop_service1(Timetable *timetables,int n,string j,int &i);
int stop_service2(Timetable *timetables,int n,string j,int &i);
/*-----------------------------------------------------所有函数声明及结构体定义------------------------------------------------------*/
int main ()//程序主函数，显示主程序窗口，供用户选择对应选项
{
    Timetable timetables[100];//定义结构体数组，存放所有车次信息
   for(;;)
   {
    switch(main_window_select())//switch语句选择管理员模块功能
    {
        case 1:
        {
            cout<<"admin_mode……loading"<<endl;
            admin_mode(timetables,"bus.txt");//此处转到管理员功能模块
            system("pause");
            break;
        }
       case 2:
        {
            cout <<"passager_mode……loading"<<endl;
            passenager_mode(timetables ,"bus.txt");//此处转到顾客功能模块
            system("pause");
            break;
        }
        case 3:
        {
            cout <<"goodbye!"<<endl;
            system ("pause");
            exit(0);
        }
    }
   }
return 0;   
}

int main_window_select()//主函数窗口选择函数，负责具体显示管理员模块和顾客模块，让操作者做出选择
{
    string  m[5]={"1: 管理员登陆","2: 普通顾客登陆","3: 再会！ "};//定义字符串数组，存放功能选项的中文字条
    int i,choice ;//定义用户选择choice
    do
    {
        system("cls");//清屏函数
        for(i=0;i<=2;i++)
        {
            cout <<m[i]<<endl;

        }
        cout <<"请输入1～3 :"<<endl;
        cin >>choice;
        

    }while(choice<0||choice>3);//防止输入的数不在1～3范围内
    return (choice);//返回choice值给switch语句做判断
}

int admin_window_select()//管理员模块窗口选择函数
{
   int i,choice;
   string m[5]={"1: 增加车次信息","2: 浏览时刻表","3: 车辆信息查询 ","4: 注销车次","5: 返回上级菜单"};
   do
  {
    system("cls");
        for(i=0;i<=4;i++)
        {
            cout <<m[i]<<endl;

        }
        cout <<"请输入1～5 :"<<endl;
        cin >>choice;
  }
  while(choice<=0||choice>=6);
  return (choice);
}  

void signin()//管理员登陆选项函数
{
    string a;//定义字符串数组存放用户名
    int b;//定义整形存放密码
loop1://后续返回，goto到这边
    cout << "请输入用户名：" << endl;
    cin >> a;
    if (a == "admin")
    {
    loop2://后续返回，goto到这边
        cout << "请输入密码：" << endl;
        cin >> b;
        if (b == 123)
        {
            return;
        }
        else
        {
            cout << "密码错误" << endl;
            goto loop2;
        }
    }
    else
    {
        cout << "用户名错误" << endl;
        goto loop1;
    }
}

void admin_mode(Timetable *timetables,char *filename)//管理员模块函数
 {
    signin();//登陆函数，若用户名和密码正确则返回
     int n=read_from_file (timetables,0,filename);//n值为timetables内元素个数
     while(1)
     {
        int admin_choice=admin_window_select();//返回值给admin_choice
        switch(admin_choice)
        {
            case 1:
                { 
                  n=add_bus(timetables,n);
                  write_to_file(timetables,n,filename);//每次操作之后将改变后的timetables数组中数据给bus.txt文件
                  system("pause");
                  break;
                }
            case 2:
                {
                    show_timetables(timetables,n);
                    system("pause");
                    break;  
                }
            case 3:
                {
                    query(timetables,n);
                    system("pause");
                    break;
                }
            case 4:
                {
                    n=delete_bus(timetables,n);
                    system("pause");
                    write_to_file(timetables,n,"bus.txt");
                    break;
                }
            case 5:
                {
                    write_to_file(timetables,n,filename);
                    return ;

                }
        }
     }
 } 

int read_from_file(Timetable *timetables,int n, char *filename)//文件读取函数
 {
    cout <<"正在读取文件……"<<endl;

    int i=0;
    ifstream infile;//定义infile代替文件执行操作
    infile.open(filename);//打开目标文件
    
    if (!infile)//若infile文件无法正常打开会返回0
    {
        cout <<"读取时不能正常打开文件，请重试"<<endl;
        return (-1);
    }
    while(infile >> timetables[i].number>>timetables[i].hour>>timetables[i].minute
        >>timetables[i].starting_station>>timetables[i].last_station>>timetables[i].time
        >>timetables[i].fare>>timetables[i].max_number>>timetables[i].sold_number)//将infile中文件写入timetables数组
    {
        i++;
    }
   
    infile.close();//文件关闭
    cout <<"文件读取成功"<<endl;
    return (i);
 }

int write_to_file(Timetable *timetables,int n,char  *filename)//写入文件函数
 {
    int i=0;
    cout <<"正在写入文件……"<<endl;

    ofstream outfile;//定义outfile代替文件
    outfile.open (filename);//打开文件
     if (!outfile)
    {
        cout <<"写入时不能正常打开文件，请重试"<<endl;
        return (-1);
    }
   for(;i<=n-1;i++)
   {
    outfile << timetables[i].number<<"   "<<timetables[i].hour<<"   "<<timetables[i].minute
        <<"   "<<timetables[i].starting_station<<"   "<<timetables[i].last_station<<"   "<<timetables[i].time
        <<"   "<<timetables[i].fare<<"   "<<timetables[i].max_number<<"   "<<timetables[i].sold_number<<"\n";
   }//按一定格式将timetables数组中的数据写入文件
    outfile.close();//文件关闭
    cout <<"文件写入成功"<<endl;
    return (i);
 }

int add_bus(Timetable *timetables,int n)//增加车次信息函数
{
    string i;
    int j,k;
    loop3:
    cout <<"请输入要添加的车次号："<<endl;
    cin>>i;
    for (j=0;j<=n-1;j++)
    {
        if (timetables[j].number == i )//车次与输入的车次匹配
        {
            cout <<"该车次信息已经存在"<<endl;
            goto loop3;
        }
    }
    cout <<"该车辆信息可以继续添加"<<endl;
    timetables[n].number=i;
    cin >>timetables[n].hour>>timetables[n].minute
    >>timetables[n].starting_station
    >>timetables[n].last_station>>timetables[n].time>>timetables[n].fare
    >>timetables[n].max_number>>timetables[n].sold_number;
    n=n+1;//此时timetables数组中数据个数为n+1
    cout <<"您是否还需要添加车次信息？"<<endl;
    cout <<"如果是，请输入1 如果不，请输入0"<<endl;
    cin >>k;
    if(k==1)
    {
        goto loop3;
    }
    else
    return (n);//返回timetables数组中的数据个数
}

int delete_bus(Timetable *timetables,int n)//删除车次信息函数
{
    string j;
    int i,k;
    loop6:
    cout <<"请输入想要注销的班次"<<endl;
    cin >>j;
    for(i=0;i<=n-1;i++)
    {
        if (timetables[i].number==j)
        {
            show_bus_information(timetables,n,i);
            cout <<"您确定要删除该车辆信息吗？"<<endl;
            cout <<"如果是，请输入1，如果不是，请输入0"<<endl;
            cin >>k;
            if (k==1)
            {
                for (;i<=n-1;i++)
                {
                    timetables[i]=timetables[i+1];//删除这一班次的列车信息，通过后面覆盖前面方式
                }
                cout<<"删除成功"<<endl;
                n--;
                cout <<"您想再次删除吗？"<<endl;
                cout <<"如果想冲再次删除，请输入1，如果想退出，请输入0"<<endl;
                cin >>k;
                if(k==1)
                {
                    goto loop6;
                }
                else
                {
                    return (n);

                }
            }
            else
            {
                cout <<"您想重新删除吗？"<<endl;
                cout <<"如果想重新删除，请输入1，如果想退出，请输入0"<<endl;
                cin >>k;
                if (k==1)
                {
                    goto loop6;
                }
                else
                {
                    return (n);

                }
            }
        }
    }
    cout <<"没有查询到您输入的车辆信息"<<endl;
    cout <<"您想要重新输入吗？是请输入1，不是请输入0"<<endl;
    cin >>k;
    if (k==1)
    {
        goto loop6;
    }

    else
    {
        return (n);
    }
}

void show_timetables(Timetable *timetables,int n)//显示所有车次信息函数
{
    int j,i;
    cout <<"以下是目前汽车站所有的车辆信息："<<endl;
    for (j=0;j<=1440;j++)
    {
        for(i=0;i<=n-1;i++)
        {
            if (((timetables[i].hour)*60+(timetables[i].minute))==j)//将所有列车的发车时间换算成分钟，让i从0开始到1140，达到从早到晚排序的目的
            {
                 show_bus_information(timetables,n,i);
            }
        }
    }
    return;
}

void show_bus_information(Timetable *timetables,int n,int i)//显示目标车次信息函数
{
    cout <<"班次为："<<timetables[i].number<<"的车辆信息是："<<endl;
    cout <<"发车时间（小时）："<<timetables[i].hour<<"   "<<"发车时间（分钟）："
    <<timetables[i].minute<<"   "<<"始发站："<< timetables[i].starting_station
    << "   "<<"终点站："<<timetables[i].last_station<<"   "<<"行车时长："<<timetables[i].time
    <<"   "<<"票价:"<<timetables[i].fare<<"   "<<"最大载客量："<<timetables[i].max_number
    <<"   "<<"已售票数："<<timetables[i].sold_number<<endl;
    return ;
}

void query (Timetable *timetables,int n)//查询车辆信息函数
{
    string j;//字符串用来输入目标站
    int k,a[100];
    cout <<"您想查询的是班次信息还是终点站信息？"<<endl;
    cout <<"如果是班次信息，请输入1，如果是终点站信息，请输入2"<<endl;
    cin >>k;
    if (k==1)
    {
        find(timetables,n,j);//班次查询函数调用
    }
    else
    {
        find(timetables,n, j,a);//终点站查询函数调用
    }
    return ;
}

int  find(Timetable *timetables,int n,string j)//按班次查询
{
    int i,k,l=0;//j用来存放用户输入的班次
    loop5:
    cout <<"请输入您想查询的班次："<<endl;
    cin >>j;
    for (i=0;i<=n-1;i++)
    {
        if(j==timetables[i].number)
        {
            cout <<"以下是班次信息为："<<j<<"的车辆信息"<<endl;
            show_bus_information(timetables,n,i);
            l++;
            break;
        }
    }
    if(l==1)
    {
        loop4:
        cout <<"还要继续查询吗？"<<endl;
        cout <<"如果继续查询，请输入1，如果要退出或继续买票退票，请输入0"<<endl;
        cin >>k;
        if (k==1)
        {
            goto loop5;
        }
        else 
        return (i) ;
    }
    else
    {
        cout <<"没有找到该班次信息"<<endl;
        goto loop4;
    }
}

int  find(Timetable *timetables,int n,string j,int a[100])//按终点站查询
{
    int i,k,l=0,m=0,u;
    loop5:
    cout <<"请输入您想查询的终点站："<<endl;
    cin >>j;
    for (i=0;i<=n-1;i++)
    {
        if(j==timetables[i].last_station)//将所有符合条件的车次在结构体中的下标存放进a[]数组
        {
            a[m]=i;
            l++;
            m++;  
        }
    }
    if(l>0)
    {
        cout<<"以下是终点站为"<<j<<"的车次信息"<<endl;
        for(n=0;n<=1440;n++)
        {
            for(u=0;u<=m-1;u++)
            {
                if((timetables[a[u]].hour)*60+(timetables[a[u]].minute)==n)//时间顺序
                {
                    show_bus_information(timetables,n,a[u]);
                }
            }
        }
        loop4:
        cout <<"还要继续查询吗？"<<endl;
        cout <<"如果继续查询，请输入1，如果要退出，请输入0"<<endl;
        cin >>k;
        if (k==1)
        {
            goto loop5;
        }
        else 
        return 1 ;
    }
    else
    {
        cout <<"没有找到该终点站信息"<<endl;
        goto loop4;
    }
}

/*----------------------------------------------------------------------------------*/
void passenager_mode(Timetable *timetables ,char *filename)//顾客模块函数
{
    int n;
    char  log_filename[200];//定义字符串数组村反复用户日志文件名
    generate_log_filename(log_filename);//日志文件名生成函数
    n= initialization_passagermode(timetables,log_filename,filename);//n的值为timetables数组中元素个数
    int passager_choice;
    while(1)
    {
        switch(passager_choice=passenager_window_select())
        {
            case 1:
                {
                    query(timetables,n);
                    system("pause");
                    break;
                }
            case 2:
                {
                    ticket_order(timetables,n);
                    write_to_file(timetables,n,log_filename);
                    system("pause");
                    break;
                }
            case 3:
                {
                    ticket_delete(timetables,n);
                    write_to_file(timetables,n,log_filename);
                    system("pause");
                    break;
                }
            case 4:
                 {
                    write_to_file(timetables,n,log_filename);
                    system("pause");
                    return;

                }
        }
    }
}

void generate_log_filename(char *log_filename)//日志名生成函数
{
    char b[5]=".log";

    cout <<"请输入当前的日期"<<endl;
    cin >>log_filename;
    strcat(log_filename ,b);//字符串链接函数，拼接后的log_filename是日期加.log后缀的形式
    return ;

}
int initialization_passagermode (Timetable *timetables, char*log_filename,char *filename)//日志文件写入函数
{
    int i=0,n;
    ifstream infile2;
    infile2.open(log_filename);//先打开目标文件
    if (!infile2)
    {
        cout <<""<<endl;
        ofstream file(log_filename);//创建日志文件
        file.close();
        cout <<"日志文件创建成功"<<endl;
        n=read_from_file(timetables,0,filename);
        cout <<"bus.txt文件写入timetables成功"<<endl;
        for (i=0;i<=n-1;i++)
        {
            timetables[i].sold_number=0;//将timetables数组中已售票数都改为0
        }
        ofstream outfile;
        outfile.open(log_filename);
        if (!outfile)
        {
            cout <<"写入时不能正常打开文件，请重试"<<endl;
            return(-1);
        }
        for(;i<=n-1;i++)
        {
        outfile <<" "<< timetables[i].number<<" "<<timetables[i].hour<<" "<<timetables[i].minute
        <<" "<<timetables[i].starting_station<<" "<<timetables[i].last_station<<" "<<timetables[i].time
        <<" "<<timetables[i].fare<<" "<<timetables[i].max_number<<" "<<timetables[i].sold_number;
        }
        outfile.close();
        cout <<"timetables数据写入log_filename文件成功"<<endl;
        return (n);//返回timetables数组中元素个数
    }
   else
   {
    cout <<"检测到您不今天不是第一次登陆"<<endl;
    ifstream infile;
    infile.open(log_filename);
    if (!infile)
    {
        cout <<"读取时不能正常打开文件，请重试"<<endl;
        return (-1);
    }
    while(infile >> timetables[i].number>>timetables[i].hour>>timetables[i].minute
        >>timetables[i].starting_station>>timetables[i].last_station>>timetables[i].time
        >>timetables[i].fare>>timetables[i].max_number>>timetables[i].sold_number)
    {
        i++;
    }
   
    infile.close();
    cout <<"log_filename文件读取到timetables数组成功"<<endl;
    return (i+1);
   }
}

int passenager_window_select()//顾客模块选择窗口函数
{
    int i,choice;
   string m[5]={"1: 车辆信息查询","2: 购买车票","3: 退回车票 ","4: 返回上级菜单"};
        do
  {
    system("cls");
        for(i=0;i<=3;i++)
        {
            cout <<m[i]<<endl;

        }
        cout <<"请输入1～4 :"<<endl;
        cin >>choice;
  }
  while(choice<=0||choice>=5);
  return (choice);
}

void ticket_order(Timetable *timetables,int n)//购票函数
{
    int i,l=0,w,p;
    string j;
    loop7:
    if(stop_service1 (timetables,n,j,i))
    {
        cout<<"不可买票！"<<endl;
        cout <<"您想重新输入车次吗？是请输入1，否请输入0"<<endl;
        cin >>w;
        if(w)
        {
            goto loop7;
        }
        else
        {
            return ;
        }
    }
    else
    {
        loop8:
        cout <<"请输入您想要购买的票数："<<endl;
        cin>>w;
        if ((timetables[i].max_number-timetables[i].sold_number)>=w)
        {
            cout <<"剩余票数大于要购买的车票数目，可以购票"<<endl;
            cout <<"您确定要购买"<<w<<"张票吗？"<<endl;
            cout <<"确定请输入1，返回请输入0"<<endl;
            cin >>p;
            if (p)
            {
                timetables[i].sold_number=timetables[i].sold_number+w;//已售票数改变
                cout <<"购买成功！"<<endl;
                cout <<"您还要继续购买吗？继续购买请输入1，退出请输入0"<<endl;
                cin >>p;
                if (p)
                {
                    goto loop7;
                }
                else
                {
                    return ;
                }
            }
            else
            {
                return;
            }
        }
        else
            {
                cout <<"剩余票数不足，购买失败"<<endl;
                cout <<"您要重新购买吗？"<<endl;
                cout <<"若想修改购买票数，请输入1，若想修改班次，请输入2，若想退出，请输入0"<<endl;
                cin >>w;
                if(w==1)
                {
                    goto loop8;
                }
                if(w==2)
                {
                    goto loop7;
                }
                if(w==0)
                {
                    return;
                }
            }
    }
}

void ticket_delete(Timetable *timetables, int n)//退票函数
{
    int i,w,k;
    string  j;
    loop9:
    if(stop_service2 (timetables,n,j,i)==1)
    {
        cout <<"距离"<<j<<"次车辆出发时间不足10分钟，或系统提示该班次暂无售票记录"<<endl;
        cout <<"不可退票"<<endl;
        cout <<"如果想重新选择班次，请输入1，如果想退出，请选择0"<<endl;
        cin >>w;
        if(w)
        {
            goto loop9;
        }
        else 
        {
            return ;
        }
    }
    else
    {
        cout <<"请输入想要退的票数"<<endl;
        cin >>k;
        cout << "即将为您退票"<<endl;
        timetables[i].sold_number=timetables[i].sold_number-k;//已售票数改变
        cout <<"退票成功"<<endl;
        return ;
    }
}

int stop_service1(Timetable *timetables,int n,string j,int &i)//购票终止函数
{
    struct tm *local;
    time_t t;
    t=time(NULL);
    local=localtime(&t);
    i= find(timetables,n,j);//获取目标元素下标
    if((local->tm_hour*60+local->tm_min)+10<(timetables[i].hour*60+timetables[i].minute)&&(timetables[i].max_number>timetables[i].sold_number))
    {
        return 0;
    }
    return 1;
}

int stop_service2(Timetable *timetables,int n,string j,int &i)//退票终止函数
{
    struct tm *local;
    time_t t;
    t=time(NULL);
    local=localtime(&t);
     i= find(timetables,n,j);
    if((local->tm_hour*60+local->tm_min)+10<(timetables[i].hour*60+timetables[i].minute)&&(timetables[i].sold_number!=0))
    {
        return 0;
    }
    return 1;
}