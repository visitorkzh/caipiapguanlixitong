#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<time.h>


extern int id;		//记录彩票的id；
extern int lucknum;//记录中奖号码；
//extern int balance;//记录彩民余额；

/*定义彩票信息的结构体*/
typedef struct piaoinfo
{
	int qihao;		//记录彩票期号
	int id;			//记录彩票id
	int price;		//记录彩票单价
	int num;			//记录购买号码
	char name[10];  //记录购买彩票的用户名
	int zhu;			//记录购买注数，单张不得超过5张
	char bingo[3]; 	//记录中奖状态0表不中，1表中
	int rmb;			//记录中奖金额
	struct piaoinfo *next;
}PI,*pi;

/*管理员发布的彩票*/
typedef struct faxing
{
	int qihao;		//期号
	int price;		//单价
	char bingo[3];	//开奖状态
	int lucknum;		//中奖号码
	int zhu;			//注数
	int sum;			//奖池金额
}FX,*pfx;
/*彩民注册帐号和密码的结构体*/
typedef struct user{
	char name[10];
	char passwd[10];
	int balance;
	struct user *next;
}SU,*psu;
int id=0;//记录彩票id
int lucknum=-1;//记录中奖号码
//int balance=0;//记录彩民余额；
void menu_1();
void enroll();
void adminlogin();
void adminmenu();
psu userlogin();
void faxing();
void randnum();
void goodman();
void randnum();
/*查询彩民信息*/
psu printinfo();
void print();
/*按彩民用户名排序*/
psu paixu();
/*保存彩票信息和彩民信息到文件中*/
void writetofile();
/*用户界面*/
void usermenu(psu H,char *usern);
/*保存退出*/
//void savetofile(psu H);
/*查看当前用户信息*/
void checkuserinfo(psu H,char *usern);
/*修改彩民密码*/
void changpasswd(psu H,char *usern);
/*彩民充值*/
void chongzhi(psu H,char *usern);
/*注销帐号*/
void deleteuser(psu H,char *usern);

void menu ()
{
	system("clear");
	printf("\t\t\t**************************\n");
	printf("\t\t\t**       欢迎使用       **\n");
	printf("\t\t\t**    彩票管理系统1.0   **\n");
	printf("\t\t\t**************************\n");
	printf("\t\t\t**       1.登录         **\n");
	printf("\t\t\t**       2.注册         **\n");
	printf("\t\t\t**       3.退出         **\n");
	printf("\t\t\t**************************\n");
	int num;
	printf("请选择：");
	scanf("%d",&num);
	switch(num)
	{
		case 1:menu_1();
			break;
		case 2:enroll();
			break;
		case 3:exit(1);
			exit(1);
		default:printf("选择无效！\n");
			break;
	}
}
void menu_1()
{
	system("clear");
	printf("\t\t\t**************************\n");
	printf("\t\t\t**    1.管理员登录      **\n");
	printf("\t\t\t**    2.普通彩民登录    **\n");
	printf("\t\t\t**    3.公证人登录      **\n");
	printf("\t\t\t**    4.退出            **\n");
	printf("\t\t\t**************************\n");
	printf("请选择：");
	int num;
	scanf("%d",&num);
	switch(num)
	{
		case 1:adminlogin();//调用管理员登录函数
			break;
		case 2:userlogin();//调用普通用户登录函数
			break;
		case 3:goodman();	  //调用公证人登录函数
			break;
		case 4:menu();	  //返回主函数
			break;
		default:printf("选择无效！\n");
			break;
	}
}
void enroll()
{
	psu L,p,r;
	char username[10]={'\0'};
	char passwd1[10]={'\0'};
	char passwd2[10]={'\0'};
	L = (psu)malloc(sizeof(SU));
	L->next = NULL;
	p=L;
	FILE *fp;
	fp = fopen("username.txt","a+");
	if(NULL == fp)
	{
		printf("open  username.txt  defeated!!\n");
		return;
	}
	while( !feof(fp))
	{
		psu new = (psu)malloc(sizeof(SU));
		if (fscanf(fp,"%s%s%d",new->name,new->passwd,&new->balance) == 3)
		{
			p->next = new;
			p = new;
		}
		new ->next = NULL;
	}
	p->next = NULL;
	while( 1 )
	{
		printf("请输入用户名：");
		scanf("%s",username);
		printf("请输入密码：");
		scanf("%s",passwd1);
		printf("请再输入一次密码：");
		scanf("%s",passwd2);
		if ( strcmp(passwd1,passwd2) != 0 )
		{
			printf("两次输入的密码不一致，请重新输入：\n");
			continue;
		}
		else
			break;
	}
	r=L;
	r=r->next;
	while(r)
	{
		if(strcmp(r->name,username) == 0)
		{
			printf("对不起，该帐户已存在，请重新选择！\n");
			return;
		}
		else
			r=r->next;
	}
	psu q;
	q = (psu)malloc(sizeof(SU));
	q->next = NULL;
	q = L->next;
	strcpy(q->name,username);
	strcpy(q->passwd,passwd1);
	q->balance = 0;
	while(q)
	{
		fprintf(fp,"%s\t\t%s\t%d\n",q->name,q->passwd,q->balance);
		q=q->next;
	}
	fclose(fp);
	printf("恭喜！！注册成功！！\n");
	menu();
}
void adminlogin ()
{
	char adminame[7]={'\0'},passwd[7]={'\0'};
	while(1)
	{
		printf("请输入管理员帐号：");
		scanf("%s",adminame);
		printf("请输入管理员密码：");
		scanf("%s",passwd);
		if ( strcmp(adminame,"admin") !=0 || strcmp(passwd, "admin") != 0)
		{
			printf("输入的帐号或密码不正确，请重新输入！\n");
			continue;
		}
		else if( strcmp(adminame,"admin") == 0 && strcmp(passwd,"admin") == 0)
			break;
		else
			continue;
	}
	adminmenu();
}
void adminmenu()
{
	while(1)
	{
		system("clear");
		printf("\t\t\t***************************\n");
		printf("\t\t\t** 1.发行彩票            **\n");
		printf("\t\t\t** 2.查看彩民信息        **\n");
		printf("\t\t\t** 3.根据彩民帐号排序    **\n");
		printf("\t\t\t** 4.保存彩票和彩民信息  **\n");
		printf("\t\t\t** 5.退出              **\n");
		printf("\t\t\t***************************\n");
		printf("请选择：");
		int num;
		scanf("%d",&num);
		switch(num)
		{
			case 1:if(lucknum == -1)
					{
						printf("对不起，上一期彩票还未开奖，您无权发行彩票！！\n");
						continue;
					}
					faxing();//发行彩票
					break;
			case 2:print();//查看彩民信息
					break;
			case 3:paixu();//排序；
					break;
			case 4:writetofile();
					break;
			case 5:menu();
					break;
			default:printf("输入了无效的选择！！\n");
	//				adminmenu();
					break;
		}
	}
}
/*查看彩票信息*/
void faxing()
{
	int qihao=0,price=5,sum,zhu;
	sum=zhu*5;
	char bingo[3]={'\0'};
	if (lucknum != -1)
		strcpy(bingo,"开");
	else
		strcpy(bingo,"无");
	printf("本次彩票期号：%d\n",qihao);
	printf("    彩票单价：%d元/注\n",price);
	printf("    开奖状态：%s\n",bingo);
	printf("    中奖号码：%d\n",lucknum);
//	printf("本期售出注数：%d\n",zhu);
//	printf("本期奖池总额：%d\n",sum);
}
/*查看彩民信息，并返回查看后链表*/
psu printinfo()
{
	FILE *fp=fopen("username.txt","r");
	if (NULL == fp)    
	return 0 ;
	psu L = (psu)malloc(sizeof(SU));
	L->next=NULL;
	psu p=L;
	SU us;
	while( !feof(fp))
	{
		if(fscanf(fp,"%s%s%d",us.name,us.passwd,&us.balance)==3)
		{
			psu new= (psu)malloc(sizeof(SU));
			strcpy(new->name,us.name);
			strcpy(new->passwd,us.passwd);
			new->balance = us.balance;
			p->next=new;
			p=new;
		}
		p->next=NULL;
	}
	fclose(fp);
	return L;
}
void print()
{
	psu L=printinfo();
	psu q=L->next;
	while(q)
	{
		printf("用户名：%s\n余额：%d\n",q->name,q->balance);
		q=q->next;
	}
}

/*按彩民用户名排序*/
psu paixu()
{
	psu L=printinfo();
	psu p=L,x,y;
	psu tmax=NULL;
	if(L->next == NULL)
	{
		printf("无用户信息！！\n");
		return 0;
	} 
	else if(L->next->next == NULL)
	{
		printf("只有一个用户，无需排序！！\n");
		return 0;
	}
	else
	{
		 while( tmax !=L->next->next)
		 {
			 for(p=L;p->next->next != tmax;p=p->next)
			 {
			 	if(strcmp(p->next->name,p->next->next->name)> 0)
			 	{
			 		x = p->next;	y=p->next->next;
			 		p->next = y;	x->next = y->next;
			 		y->next = x;
			 	}
			 }
			 tmax=p->next;
		}
	}
	return L;
}

/*保存彩票信息和彩民信息到文件中*/
void writetofile()
{
	psu L=paixu();
	psu q=L->next;
	FILE *fp=fopen("lotanduser.txt","w");
	if (NULL == fp)   return;
	while(q)
	{
		fprintf(fp,"%s\t\t%s\t\%d\n",q->name,q->passwd,q->balance);
		q=q->next;
	}
}
psu userlogin()
{
	char usern[10]={'\0'};
	char passw[10]={'\0'};
	printf("请输入帐号：");
	scanf("%s",usern);
	printf("请输入密码：");
	scanf("%s",passw);
	psu H=(psu)malloc(sizeof(SU));
	H->next=NULL;
	psu p = H;
	FILE *fp=fopen("username.txt","r");
	if(NULL == fp)	
	return 0;
	while( !feof(fp))
	{
		psu new = (psu)malloc(sizeof(SU));
		new ->next = NULL;
		if (fscanf(fp,"%s%s%d",new->name,new->passwd,&new->balance) == 3)
		{
			p->next = new;
			p = new;
		}
	} p->next = NULL;
	fclose(fp);
	psu q = H;
	q = q->next; 
	while(q)
	{
		if (strcmp(q->name,usern) == 0 && strcmp(q->passwd,passw) == 0)
		{
			usermenu(H,usern);
			;
		}
		else
			q = q->next;;	
	}
	printf("对不起，登录失败！！\n");
	printf("是否前去注册？( y / n)");
	char ch;
	getchar();
	scanf("%c",&ch);
	if ( ch == 'y' || ch =='Y')
		enroll();
	else
		menu_1();
}

/*user   menu*/
void usermenu(psu H,char *usern)
{
	system("clear");
	printf("\t\t\t**************************\n");
	printf("\t\t\t**  1.查看个人信息      **\n");
	printf("\t\t\t**  2.修改密码          **\n");
	printf("\t\t\t**  3.帐户充值          **\n");
	printf("\t\t\t**  4.购买彩票          **\n");
	printf("\t\t\t**  5.查看未开奖彩票    **\n");
	printf("\t\t\t**  6.查看历史记录      **\n");
	printf("\t\t\t**  7.保存注销登录      **\n");
	printf("\t\t\t**  8.注销帐号          **\n");
	printf("\t\t\t**************************\n");
	int num;
	static int balance;
	printf("请选择：");
	scanf("%d",&num);
	switch(num)
	{
		case 1:checkuserinfo(H,usern);		//查看个人信息
				break;
		case 2:changpasswd(H,usern);		   //修改密码
				break;
		case 3:chongzhi(H,usern);			   //帐户充值
				break;
		case 4:		//购买彩票
				break;
		case 5:		//待开奖彩票
				break;
		case 6:		//历史购买记录
				break;
		case 7:	//保存信息，注销登录
				menu();
				break;
		case 8:menu();
				break;
		default:printf("输入了无效选择！！\n请重新选择：\n");
	//			usermenu();
				break;
	}
}

/*查看当前用户信息*/
void checkuserinfo(psu H,char *usern)
{
	psu p,q;
	p=H;
	p = p->next;	
	while(p)
	{
		if(strcmp(p->name,usern) == 0)
		{
			printf("用户名：%s\n帐户余额：%d\n",p->name,p->balance);
			break;
		}
		else
			p=p->next;
	}
	printf("\t\t\t**************************\n");
	printf("\t\t\t**  1.返回上一菜单      **\n");
	printf("\t\t\t**  2.返回主菜单        **\n");
	printf("\t\t\t**************************\n");
	printf("\n\n**请选择：");
	int num;
	while(1)
	{
		scanf("%d",&num);
		switch(num)
		{
			case 1:usermenu(H,usern);
				break;
			case 2:menu();
				break;
			default:printf("***选择无效！！\n***请重新选择：");
				continue;
		}	
	}
}


/*修改密码*/
void changpasswd(psu H,char *usern)
{
	psu p,q;
	p=H;
	p=p->next;
	char passwd[10],passwd1[10],passwd2[10];
	/*
	while(p)
	{
		if (strcmp(p->name,usern) == 0)
			break;
		else
			p=p->next;
	}
	*/
	while(1)
	{	
		printf("请输入旧密码：");
		scanf("%s",passwd);
		printf("请输入新密码：");
		scanf("%s",passwd1);
		printf("请再输入一次新密码：");
		scanf("%s",passwd2);	
		if (strcmp(passwd1,passwd2) != 0)
		{
			printf("对不起，旧密码输入有误，请重新输入");
				continue;
		}
		else if (strcmp(passwd1,passwd2) == 0)
			break;
	}
	printf("是否确认修改？（y/n）");
	getchar();
	char ch;
	scanf("%c",&ch);
	if ( ch == 'Y' || ch == 'y' )
	{
		while(p)
		{
			if (strcmp(p->name,usern) == 0)
			{
				strcpy(p->passwd,passwd1);
				break;
			}
			else
				p=p->next;
		}
		FILE *fp=fopen("username.txt","w");
		if ( NULL == fp)   return;
		q=H;
		q=q->next;
		while(q)
		{
			fprintf(fp,"%s\t\t%s\t\t%d\n",q->name,p->passwd,p->balance);
			q=q->next;
		}
		fclose(fp);
		printf("密码修改成功！！\n");
	}
	else if (ch == 'n' || ch == 'N')
	{
		printf("密码无修改！！\n");
	}
	printf("\t\t\t**************************\n");
	printf("\t\t\t**  1.返回上一菜单      **\n");
	printf("\t\t\t**  2.返回主菜单        **\n");
	printf("\t\t\t**************************\n");
	printf("\n\n**请选择：");
	int num;
	while(1)
	{
		scanf("%d",&num);
		switch(num)
		{
			case 1:usermenu(H,usern);
				break;
			case 2:menu();
				break;
			default:printf("***选择无效！！\n***请重新选择：");
				continue;
		}	
	}	
}

/*彩民充值*/
void chongzhi(psu H,char *usern)
{
	psu p=H,q=H;
	p=p->next;
	int num;
	while(p)
	{
		if ( strcmp(p->name,usern) == 0)
			break;
		else
			p=p->next;
	}
	while(1)
	{
		printf("请输入充值金额(不能高于9999)：");
		scanf("%d",&num);
		if ( num >9999 || num < 0)
		{
			printf("对不起,无效充值！\n");
			continue;
		}
		else
		{
			p->balance += num;
			printf("充值成功！！\n");
			break;
		}
	}
	psu s=H;
	s=s->next;
	FILE *fp=fopen("./etc/","w+");
	if(NULL == fp)	return ;
	while(s)
	{
		fprintf(fp,"%s\t\t%s\t\t%d\n",s->name,s->passwd,s->balance);
		s=s->next;
	}
	fclose(fp);
//	usermenu(H,usern);
}
/*保存退出*/
void savetofile(psu H)
{
	
}

/*购买彩票*/
pi buylot()
{
	printf("\n");

}

/*注销帐号*/
void deleteuser(psu H,char *usern)
{
	psu p,q;
	p=H;
//	p=p->next;
	while(p->next)
	{
		q=p->next;
		if ( strcmp(q->name,usern) == 0 )
		{
			printf("确认注销帐号？(y/n)");
			getchar();
			char ch;
			scanf("%ch",&ch);
			if( ch == 'y' || ch == 'Y')
			{
				p=q->next;
				free(q);
				break;
			}
			else
			{
				printf("帐号无删除！\n");
				break;
			}
		}
		else
			p=p->next;
	}		
}
void goodman()
{
	system("clear");
	printf("\t\t\t**************************\n");
	printf("\t\t\t**      1.开奖          **\n");
	printf("\t\t\t**      2.查看彩票信息  **\n");
	printf("\t\t\t**      3.退出登录      **\n");
	printf("\t\t\t**************************\n");
	printf("请选择：");
	int num;
	scanf("%d",&num);
	switch(num)
	{
		case 1:randnum();//调用开奖函数，产生本期中奖号码
				break;
		case 2:          //查看彩票信息
				break;
		case 3:menu();   //退出公证人帐号，返回主界面
				break;
		default:printf("选择无效！！\n");
				break;
	}
}

/*开奖函数,获得随即数*/
void randnum()
{
	srand((unsigned)time(0));
	lucknum= rand()%10;
	system("clear");
	printf("\t\t\t**************************\n");
	printf("\t\t\t**  本期中奖号码是：%d   **\n",lucknum);
	printf("\t\t\t**  恭喜各位中奖者！！  **\n");
	while(1)
	{
		printf("\t\t\t**************************\n");
		printf("\t\t\t**  1.返回上一菜单      **\n");
		printf("\t\t\t**  2.返回主菜单        **\n");
		printf("\t\t\t**************************\n");
		printf("**请选择：");
		int num;
		if (scanf("%d",&num) != 1)
		{
			getchar();
			printf("输入无效！！\n请重新输入：\n");
			continue;
		}
		else 
		{
			switch(num)
			{
				case 1:goodman();
					break;
				case 2:menu();
					break;
				default:printf("***选择无效！！\n***请重新选择：\n");
					continue;
			}	
		}
	}
}



int main(int argc,char **argv)
{
	menu();
	return 0;
}