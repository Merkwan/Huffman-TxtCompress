#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include<string>
#include <algorithm>
#include <cstring>
using namespace std;
//����������㶨��
typedef struct
{
	int  weight;
	int parent,lchild,rchild;    //��¼��������±�
}HTNode,*HuffmanTree;

//�ı�Ƶ��ͳ�ƽṹ���� 
typedef struct
{
	char* data;
	int* num;
	int length;
	int* codelength;
}TNode;

//���������붨�� 
typedef struct
{
	char *data;
	char** HM;
}Code;
typedef char** HuffmanCode;

//���ܷ�ʽѡ��������/���ܣ� 
bool IfEncode()
{
	int choose;	
	cout<<"\t            1.����ѹ��     2.����ѹ��"<<endl;
	cout<<"������ѡ��"; 
	cin>>choose;
	if(choose==1)
		return true;
	else
		return false;
}
void Encode(vector<char>& v)       //������30λ���� 
{
	char ch[30];
	v.push_back('@');
	cout<<"������ѹ�����루30λ���ڣ�"<<endl; 
	cin>>ch;
	for(int i=0;ch[i]!='\0';i++)
		v.push_back(ch[i]);
	v.push_back('\0');
	v.push_back('@');
	cout<<"��ʼѹ����"<<endl; 
}

//��ʼ��Ƶ��ͳ�Ʊ�
void InitList(TNode& T)
{
	T.data=new char[256];
	T.num=new int[256];
	T.codelength=new int[256];
	if(!T.data||!T.num||!T.codelength)
		exit(1);
	T.length=0;
}

//���ļ� 
void ReadTxt(vector<char>& v)
{
	char ch;
	string f;
	cout<<"������Ŀ¼�µ�һ���ı��ļ���:";
	cin>>f;
	ifstream infile(f.c_str(),ios::in);
	if(!infile)
	{
		cerr<<"Open Error��"<<endl;
		exit(1);
	}
	if(IfEncode())
		Encode(v);
	while(infile.peek()!=EOF)
	{
		infile.get(ch);
		v.push_back(ch); 
	}	
	infile.close();	
}

//�ļ���ȡ�����ı�����vector�����У���ͳ���ַ�����Ƶ�� 
void CountFreq(vector<char> v)
{
	int i,dcharnum[50],charlength1=0,k=0,dataflag=0,charlength2=0,charnum[50];
	int j=0;
	float numsum;
	char m[1000][3];
	char singledata[100];
	for(i=0;i<v.size();i++)
	{
		if(v[i]&128)
		{
			if(dataflag==1)
			{
				dataflag=0;
				continue;
			} 
			
			for(j=0;j<charlength1;j++)
			{
			
				if((v[i]==m[j][0])&&(v[i+1]==m[j][1]))
				{
					(dcharnum[j])++;
					break;
				} 
			}	
			if((j==charlength1)||(charlength1==0))
			{
				m[charlength1][0]=v[i];
				m[charlength1][1]=v[i+1];
				m[charlength1][2]='\0';
				dcharnum[charlength1]=1;
				charlength1++;	
			}	
			dataflag=1;
		}
		else
		{
		 	if(dataflag==1)
			{
			  	dataflag=0;
				continue;
			}
			else
			{
			   for(k=0;k<charlength2;k++)
			   {
					if(singledata[k]==v[i])
					{
						(charnum[k])++;		
						break;			
				    }	   	
			   }
			   if((k==charlength2)||(charlength2==0))
			   {
				singledata[charlength2]=v[i];
			  	charnum[charlength2]=1;	
				charlength2++; 
			   }	
			}			
		}
	}
	for(i=0;i<charlength1;i++)
	{
		numsum=numsum+dcharnum[i];
	}
	for(i=0;i<charlength2;i++)
	{
		numsum=numsum+charnum[i];
	}
	for(i=0;i<charlength1;i++)
	{
		printf("%s�ĳ��ִ���Ϊ%d,Ƶ��Ϊ%f\n",m[i],dcharnum[i],dcharnum[i]/numsum);
	}
	for(i=0;i<charlength2;i++)
	{	
		printf("%c�ĳ��ִ���Ϊ%d��Ƶ��Ϊ%f\n",singledata[i],charnum[i],charnum[i]/numsum);
	}	
}

//����Ƶ��ͳ�ƣ������ַ�������ִ���������
bool Find(TNode T,char ch)        //�ж��ַ��Ƿ��ڱ����Ѵ���
{
	int i;
	for(i=0;i<T.length;i++)
		if(ch==T.data[i])
			return true;
	return false;
}
void TCount(vector<char> v1,TNode &T)
{
	int i,j=0;
	char ch;
	int m=v1.size();
	for(i=0;i<m;i++)
	{
		ch=v1[i];
		if(!Find(T,ch))
		{
			T.data[j]=ch;
			T.num[j]=count(v1.begin(),v1.end(),ch);
			j++;
			T.length++;
		}		
	}		
}

//������������ 
void Select(HuffmanTree &HT,int m,int& s1,int& s2)       //�����ⲿ���
{
	int k,j,n,min=32767;
	for(k=1;k<=m;k++)
	{
		if(HT[k].parent==0)
			if(HT[k].weight<=min)
			{
				j=k;
				min=HT[k].weight;
			}			
	}
	s1=j;
	HT[j].parent=1;
	min=32767;
	for(k=1;k<=m;k++)
	{
		if(HT[k].parent==0)
			if(HT[k].weight<=min)
			{
				n=k;
				min=HT[k].weight;
			}			
	}
	s2=n;
}
void CreateHuffmanTree (HuffmanTree &HT,TNode T,int length)
{
	int m,i,s1,s2;
	if(length<=1)
		return;
	m=2*length-1;           //���������������Ҷ������Ϊlength�� 
	HT=new HTNode[m+1];
	for(i=1;i<=m;++i)
	{
		HT[i].parent=0;
		HT[i].lchild=0;
		HT[i].rchild=0;
	}
	for(i=1;i<=length;++i)
		HT[i].weight=T.num[i-1];	
	for(i=length+1;i<=m;i++)
	{
		Select(HT,i-1,s1,s2);
		HT[s1].parent=i;
		HT[s2].parent=i;
		HT[i].lchild=s1;
		HT[i].rchild=s2;
		HT[i].weight=HT[s1].weight+HT[s2].weight;
	}	
}

//���������� 
void CreatHuffmanCode (HuffmanTree HT,HuffmanCode &HC,int n,TNode&T)
{
	int i,f,c,start;
	HC=new char*[n+1];               //ָ���ַ���������� 
	char* cd=new char[n];
	cd[n-1]='\0';
	for(i=1;i<=n;i++)
	{
		start=n-1;
		c=i;
		f=HT[i].parent;
		while(f!=0)                //f==0��Ϊ���ڵ� 
		{
			--start;
			if(HT[f].lchild==c)
				cd[start]='0';
			else
				cd[start]='1';
			c=f;
			f=HT[f].parent;
		}
		T.codelength[i-1]=n-start-1;
		HC[i]=new char[n-start];
		strcpy(HC[i],&cd[start]);
	}	
	delete cd;
}

//�������ļ�����
void Zip1(HuffmanCode HC,vector<char> v,TNode T,float &ziplength)
{
	int i=0,j=0,k=0,num=0,m=0,s;
	unsigned char c;
	ofstream outclear("zip1.txt",ios::out);
	outclear.close();
	fstream outfile("zip1.txt", ofstream::app | ios::binary | ofstream::out);
	if (!outfile){
		cerr << "Open zip1.txt failure" << endl;
		return;
	}
	for(i=0;i<v.size();i++)
	{
		for(j=0;j<T.length;j++)
			if(T.data[j]==v[i])
				break;
		//cout<<HC[j+1]<<endl;
		for(k=0;HC[j+1][k]!='\0';k++)
		{
			m++;
		  	c=c<<1;
			if(HC[j+1][k]=='1') c=c|1;
			if(m==8) 
			{
				outfile.write((char*)&c,sizeof(c));
				//cout<<"���"<<int(c)<<endl;
				num++;
				m=0;	
			}			
		}
		if((i==v.size()-1)&&m!=0)
		{
			for(s=m+1;s<=8;s++)
			{
				c=c<<1;	
			}	
			outfile.write((char*)&c,sizeof(c));
			//cout<<"���"<<int(c)<<endl;
			num++;
		}			
	}
	outfile.seekg(0,ios::end);
	ziplength=outfile.tellg();
	outfile.close();
	outfile.clear();
	cout<<"����ѹ�� ��";Sleep(500);cout<<" ��";Sleep(500);cout<<" ��"<<endl;
	//cout<<num<<endl;
	cout<<"ѹ���ɹ����ɵ�zip.txt�в鿴ѹ�����ļ�"<<endl; 
} 

//���������ļ����� 
int in_HM(Code &c,int &alllength)
{
	c.data=new char[256];
	c.HM=new char* [256];
	int length=0,i;
	ifstream infile("code.dat",ios::in);
	if(!infile)
	{
		cerr<<"Open Error��"<<endl;
		exit(1);
	}
	infile>>length;
	infile>>alllength;
	for(i=0;i<length;i++)
	{
		infile>>c.data[i];
		c.HM[i]=new char[length+1];
		infile>>c.HM[i];
	}
	infile.close(); 
	return length;	
}
void out_HM(HuffmanCode HC,TNode T)
{
	int i,dataflag=0,alllength=0;	
	ofstream outfile("code.dat",ios::out);
	if(!outfile)
	{
		cerr<<"Open Error��"<<endl;
		exit(1);
	}
	for(i=0;i<T.length;i++)
	{
		alllength=alllength+T.num[i];
	}
	outfile<<T.length<<endl;
	outfile<<alllength<<endl;
	for(i=0;i<T.length;i++)
	{
		outfile<<T.data[i]<<endl;
		outfile<<HC[i+1]<<endl;
	}
	outfile.close();
}

/*void outputfile()
{
	fstream file;
	file.open("zip1.txt", ifstream::in | ios::binary);
	if (!file){
		cerr << "Open fir.txt for read failure!" << endl;
	}
	unsigned char i = 0;
	int k=0;
	//���ζ�ȡ�������ļ��е�unsigned charֵ���뵽����i,�����
	while (file.read((char *)&i, sizeof(i)))
	{
		k++;
		cout << (int)i << "\t";
	}
	cout <<endl<<k<<endl;
	file.close();
	file.clear();
}*/

//��ѹ����ȷ�� 
bool REncode(char ch[])
{
	int i=0;
	char code[30];
	cout<<"�������ѹ����:"<<endl;
	cin>>code;
	if(strcmp(code,ch)==0)
		return true;
	else
		return false;
}

 //�����ļ������
void RZip1()
{
	char ch[8];
	unsigned char cc,ccc;
	char ch2[30];
	Code c;
	int i,j,alllength,readflag=0,mm=0,k,length,flag,flag2=0,m=0,lengthflag=0;
	length=in_HM(c,alllength);
	ofstream outfile("rzip1.txt",ios::out);
	fstream infile;
	infile.open("zip1.txt", ifstream::in | ios::binary);
	if (!infile){
		cerr << "Open zip1.txt for read failure!" << endl;
		return ;
	}
	if(!outfile)
	{
		cerr<<"open error"<<endl;
		exit(1);
	}
	while(1)
	{	
		flag=0;
		char* cd=new char[length];
		for(i=0;;i++)
		{
			if(mm==8||mm==0)
			{
				if(!(infile.read((char*)&cc,sizeof(cc)))) 
				{
					readflag=1;
					break;
				}
				//cout<<(int)cc<<"niaho"<<endl;
				//infile>>ch;
				for(k=0;k<8;k++)
				{
					if(cc&128) ch[k]='1';
					else ch[k]='0';
					cc=cc<<1; 
				}
				mm=0;	
			}
			//cout<<ccc<<endl;
			//cout<<ch[mm]<<endl;
			cd[i]=ch[mm];
			mm++;
			cd[i+1]='\0';
			//cout<<cc<<endl;
			for(j=0;j<length;j++)
				if(strcmp(c.HM[j],cd)==0)
				{
					//cout<<"ok1"<<endl;
					if(c.data[j]=='@')
					{	
						if(flag2==0)
						{
							flag=1;
							flag2=1;
							delete cd;
							break;
						}
						if(flag2==1)
						{
							while(1)
							{
								if(REncode(ch2))
								{
									cout<<"������ȷ�����ڽ�ѹ�� "<<endl;
									flag2=0;
									break;
								}						
								else
									cout<<"�����������������"<<endl; 
							} 
							flag=1;
							delete cd;
							break;
						}	
					}
					//cout<<"ok2"<<i<<endl; 
					if(flag2==1)
					{
						ch2[m]=c.data[j];
						flag=1;
						m++;
						delete cd;
						break;	
					}
					if(flag2==0)
					{
						outfile<<c.data[j];
						//cout<<c.data[j]<<endl; 
						flag=1;
						delete cd;
						break;
					}
				}
			if(flag==1)
				break;
		}
		if(readflag==1) break;
		lengthflag++;
		if(lengthflag>=alllength) break;
	}
	//cout<<"����"<<alllength<<endl;
	outfile.close();
	outfile.clear();
	infile.close();
	infile.clear();
	cout<<"���ڽ�ѹ ��";Sleep(500);cout<<" ��";Sleep(500);cout<<" ��"<<endl;	
	cout<<"��ѹ�ɹ�!�뵽rzip.txt�в鿴��ѹ���ļ�" <<endl; 
}

void about()
{
	system("cls");
	cout<<"�X==============================================================�["<<endl;
	cout<<"�U                 ��ӭʹ�ù���������/�������                  �U"<<endl;
	cout<<"�U  ���ڳ���                                                  �U"<<endl;
	cout<<"�U      ���룺                                                  �U"<<endl;	
	cout<<"�U        �ɱ����������Ŀ¼�µ�txt�ļ������ɱ�����zip.txt    �U"<<endl;
	cout<<"�U                                                              �U"<<endl;
	cout<<"�U      ���룺                                                  �U"<<endl;
	cout<<"�U        �ɽ�������Ŀ¼�µ�zip.txt�����ɽ�����rzip.txt       �U"<<endl;
	cout<<"�U                                                              �U"<<endl;
	cout<<"�U      ����֧��ѹ����Ӣ���ļ���                                �U"<<endl;
	cout<<"�U                                                              �U"<<endl;
	cout<<"�U                                         ������  ��С��       �U"<<endl;
	cout<<"�U                                         ������  Τɭҫ       �U"<<endl;
	cout<<"�U                                                              �U"<<endl;
	cout<<"�U                                              ��������������  �U"<<endl; 
	cout<<"�^==============================================================�a"<<endl;
}
//����ѡ��˵� 
void menu()
{
	cout<<"�X==============================================================�["<<endl;
	cout<<"�U                                                              �U"<<endl;
	cout<<"�U                ��ӭʹ�ù���������/�������                   �U"<<endl;
	cout<<"�U                                                              �U"<<endl;
	cout<<"�U                     1. �����ļ�                              �U"<<endl;
	cout<<"�U                     2. �����ļ�                              �U"<<endl;
	cout<<"�U                     3. ���ڳ���                              �U"<<endl;
	cout<<"�U                     4. �˳�����                              �U"<<endl;
	cout<<"�U                                                              �U"<<endl;
	cout<<"�^==============================================================�a"<<endl;
} 
int main()
{
	vector<char> v;   //ѹ�������� 
	int n,choose;
	float ziplength=0;
	TNode T;
	InitList(T);
	HuffmanTree HT;	
	HuffmanCode HC;	// ���������� 
	while(1)
	{
		system("cls"); 
		menu();
		cout<<"��������ѡ������:";
		cin>>choose;
		switch(choose)
		{
			case 1:
			   //��ȡ�ļ� 
			   	ReadTxt(v); 
				TCount(v,T);	//ͳ���ַ��������Ӧ��������Ȩֵ 
				n=T.length;
				CreateHuffmanTree (HT,T,n);  //�����ַ���Ȩֵ������������ 
				CreatHuffmanCode(HT,HC,n,T);		//�������������� 
				Zip1(HC,v,T,ziplength);
				out_HM(HC,T);
				cout<<"����ѹ��Ƶ��Ϊ��"<<endl; 
				CountFreq(v);
				cout<<"�ļ�ѹ����Ϊ��"<<v.size()/ziplength<<endl;
				system("pause");
				break;			
			case 2: 
				RZip1();
				system("pause");
				break;
			case 3:
				about();
				system("pause");
				break;
			case 4:
				return 0;
			default:
				cout<<"�����������������"<<endl;
				system("pause");
				break; 		
		}
	}
	return 0; 
}
