#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100000
struct HTNode
{
    //int isleft;     //判断自身是否为左子树，便于得到霍夫曼编码
    int weight;     //该节点的权重
    int parent;
    int lchild;
    int rchild;
};
struct wcounter
{
    char c; //表示该字符的内容
    int cnt;//表示权重
};
/*struct codeexcel
{
    char c;
    char *str;
    int len;
};*/
void Select(struct HTNode* HT,int i,int *s1,int *s2)    //选出HT中1到i里面权值最小的序号s1和s2
{
    int temp1 = 0;
    int temp2 = 0;
    int j;
    for(int j = 1;j <= i;j++)
    {
        if((HT+j)->parent == 0) 
        {
            temp1 = (HT+j)->weight;
            break;
        }
    }
    for(j = 1;j <= i;j++)
    {
        if(((HT+j)->weight <= temp1)&&(HT+j)->parent == 0)
        {
            temp1 = (HT+j)->weight;
            *s1 = j;
        }
    }
    for(int k = 1;k <= i;k++)
    {
        if(((HT+k)->parent == 0)&&k != *s1)
        {
            temp2 = (HT+k)->weight;
            break;
        }
    }
    for(int m = 1;m <= i;m++)
    {
        if((((HT+m)->weight <= temp2)&&(HT+m)->parent == 0)&&m != *s1)
        {
            temp2 = (HT+m)->weight;
            *s2 = m;
        }
    }
}   //经验证测试该函数正确
void WeightCounter(struct wcounter **w,int *charkindnumber,char *fileloaction)     //统计字符权重的分布，构造HashTable
{
    (*charkindnumber) = 0;
    char temp;
    FILE *fp;
    fp = fopen(fileloaction,"rb");  //以二进制读
    while(1)
    {
        if(!fread(&temp,1,sizeof(char),fp)) break;
        if(*charkindnumber == 0)
        {
            w[0]->c = temp;
            (w[0]->cnt)++;
            (*charkindnumber)++;
        }else
        {
            int kmax = *charkindnumber;
            int k = 0;
            for(k = 0;k < kmax;k++)
            {
                if(w[k]->c == temp)
                {
                    (w[k]->cnt)++;
                    break;
                }
            /*    else if(w[k]->c == '\0')
                {
                    w[k]->c = temp;
                    (w[k]->cnt)++;
                    (*charkindnumber)++;
                    break;
                }*/
            }
            if(k == kmax)
            {
                w[kmax]->c = temp;
                (w[kmax]->cnt)++;
                (*charkindnumber)++;
            }
        }
    }
    fclose(fp);
}
void HuffmanEncoding(struct HTNode** HTree,char ***HC,struct wcounter **w,int *charkindnumber)          //构造霍夫曼树，得到压缩后个字符对应的编码表
{
    struct HTNode* p;
    int i;
    int *s1;
    int *s2;
    char *cd;
    int start;
    int l = 0;
    s1 = (int*)malloc(sizeof(int));
    s2 = (int*)malloc(sizeof(int));
    if(*charkindnumber <= 1) return;
    int m = 2*(*charkindnumber)-1;
    (*HTree) = (struct HTNode*)malloc((m+1)*sizeof(struct HTNode));     //0号单元未用
    for(p = (*HTree),i = 1;i <= (*charkindnumber);i++,p++)
    {
        p->weight = w[l]->cnt;
        p->lchild = 0;
        p->parent = 0;
        p->rchild = 0;
        l++;
    }
    for(;i <= m;i++,p++)
    {
        p->lchild = 0;
        p->parent = 0;
        p->rchild = 0;
        p->weight = 0;
    }
    for(i = (*charkindnumber)+1;i <= m;i++)  //建立霍夫曼树
    //在HTree[1,i-1]中选择parent为0且weight最小的两个结点，其序号分别为s1和s2
    {
        Select(*HTree,i-1,s1,s2);
        (*HTree)[*s1].parent = i;
        (*HTree)[*s2].parent = i;
        (*HTree)[i].lchild = *s1;
        (*HTree)[i].rchild = *s2;
        (*HTree)[i].weight = (*HTree)[*s1].weight+(*HTree)[*s2].weight;
    }
    //下面为从叶子到根逆向求每个字符的霍夫曼编码
    (*HC) = (char**)malloc(((*charkindnumber)+1)*sizeof(char*));
    cd = (char*)malloc((*charkindnumber)*sizeof(char));
    cd[(*charkindnumber)-1] = '\0';         //结束编码符
    for(int i = 1;i <= (*charkindnumber);i++)
    {
        int c = 0;
        int f = 0;
        start = (*charkindnumber)-1;
        for(c = i,f = (*HTree)[i].parent;f != 0;c = f,f = (*HTree)[f].parent)
        {
            if((*HTree)[f].lchild == c) cd[--start] = '0';
            else cd[--start] = '1';
        }
        (*HC)[i] = (char*)malloc(((*charkindnumber)-start)*sizeof(char));
        strcpy((*HC)[i],&cd[start]);
    }
    free(cd);
}
void PrintHuffmancode(struct wcounter **w,int *charkindnumber,char **huffmancode)
{
    for(int i = 0;i < *charkindnumber;i++)
    {
        printf("字符:%c",w[i]->c);
        printf("编码为:%s\n",*(huffmancode+i+1));
    }
}
/*void Createcodeexcel(char **huffmancode,struct wcounter **w,int *charkindnumber,struct codeexcel** huffmancodeexcel)
{
    *huffmancodeexcel = (struct codeexcel*)malloc(sizeof(struct codeexcel)*(*charkindnumber));   //为打表分配空间
    for(int i = 0;i < *charkindnumber;i++)
    {
        (*huffmancodeexcel)[i].c = w[i]->c;
        (*huffmancodeexcel)[i].len = strlen(*(huffmancode+i+1));
        (*huffmancodeexcel)[i].str = (char*)malloc(sizeof(char)*((*huffmancodeexcel)[i].len)+1);
        strcpy((*huffmancodeexcel)[i].str,*(huffmancode+i+1));
    }
}*/
/*void Printcodeexcel(struct codeexcel** huffmancodeexcel,int *charkindnumber)
{
    for(int i = 0;i < *charkindnumber;i++)
    {
        printf("%c %s %d\n",(*huffmancodeexcel)[i].c,(*huffmancodeexcel)[i].str,(*huffmancodeexcel)[i].len);
    }
}*/
void CreateZipFile(char **huffmancode,struct wcounter **w,char *filelocation,int *charkindnumber)    //第一个char存放结尾补0的个数，然后存放霍夫曼编码表，为字符(char)->霍夫曼编码(以char为单位存放，中间以','隔开结尾以\n为标记即可)
{
    FILE *fp1;  //用来写zip文件
    FILE *fp2;  //用来读原文件
    long filelengeh;
    long counter = 0;   //用来统计该文件会用多少位编码
    char zero;          //结尾补0的个数
    //先写结尾补0的个数
    if(!(fp2 = fopen(filelocation,"rb")))
    {
        printf("can not open file!\n");
        return;
    }
    fseek(fp2,0L,SEEK_END);
    filelengeh = ftell(fp2);
    rewind(fp2);
    for(int i = 0;i < *charkindnumber;i++)
    {
        counter += (w[i]->cnt)*strlen(*(huffmancode+i+1));
    }
    zero = 8-(counter%8);
    if(zero == 8) zero = 0; 
    char *zipname = "huffmanzip";
    fp1 = fopen(zipname,"w");
    fprintf(fp1,"%c",zero);
    fprintf(fp1,"%d",*charkindnumber);//写入要读出多少个字符打表。
    for(int i = 0;i < *charkindnumber;i++)
    {
        char temp;
        temp = w[i]->c;
        fprintf(fp1,"%c",temp);
        temp = ',';
        fprintf(fp1,"%c",temp);
        fprintf(fp1,"%s",*(huffmancode+i+1));
        fprintf(fp1,"%c",temp);
    }
    //至此将0数和霍夫曼表写进文件头的操作都完成，下面进行位操作写霍夫曼编码
    char buffer;
    int readcharnum = 0;
    int count = 0;  //判断计数是否达到八
    while(1)
    {
        char temp;
        int i = 0;
        int j;
        if(!fread(&temp,1,sizeof(char),fp2)) break;
        readcharnum++;
        for(i = 0;i < *charkindnumber;i++)
        {
            if(w[i]->c == temp) break;
        }
        char* code = *(huffmancode+i+1);
        for(j = 0;code[j] != '\0';j++)
        {
            if(*(code+j) == '0') 
            {
                buffer = buffer<<1; 
                count++;
                if(count == 8)
                {
                    fprintf(fp1,"%c",buffer);
                    count = 0;
                } 
            }
            else if(*(code+j) == '1')
            {
                buffer = buffer<<1;
                buffer = buffer|1;
                count++;
                if(count == 8)
                {
                    fprintf(fp1,"%c",buffer);                        
                    count= 0;
                }
            }
        }
        if(readcharnum == filelengeh) 
        {
            if(count != 8) 
            {
                for(;count != 8;count++)
                {
                    buffer = buffer<<1;
                }
                fprintf(fp1,"%c",buffer);
                count = 0;
            }
        }
    }
    fclose(fp2);
    fclose(fp1);
}
void Createback()
{
    FILE *fp1;
    char filebacklocation[30];
    scanf("%s",filebacklocation);
    fp1 = fopen(filebacklocation,"r");
    char zero;   //得到结尾0的个数
    int charkindnumber;
    struct wcounter w[256];
    int codecnt = 0;
    fscanf(fp1,"%c",&zero);
    fscanf(fp1,"%d",&charkindnumber);
    char huffmancode[charkindnumber+1][9];
    for(int i = 0;i <= charkindnumber;i++)
    {
        for(int j = 0;j < 9;j++)
        {
            huffmancode[i][j] = '\0';
        }
    }
    while(1)
    {
        if(codecnt == charkindnumber) break;
        char temp;
        fscanf(fp1,"%c",&w[codecnt].c);
        codecnt++;
        fscanf(fp1,"%c",&temp);
        int u = 0;
        do
        {
            fscanf(fp1,"%c",&temp);
            if(temp != ',') 
            {
                huffmancode[codecnt][u] = temp;
                u++;
            }else 
            {
                u = 0;
                break;
            }
        }while(1);
    }
 /*   for(int i = 0;i < charkindnumber;i++)
    {
        printf("%c",w[i].c);
        printf("%s\n",*(huffmancode+i+1));
    }
    printf("%d\n",zero);
    printf("%d\n",charkindnumber);*/
    //经过测试，得到了完整的霍夫曼编码表和zero和charkindnumber
    //接下来就是读文件内容
    FILE *fp2;
    fp2 = fopen("backfile","w");
    long loc;
    loc = ftell(fp1); 
    fclose(fp1);
    fp1 = fopen(filebacklocation,"rb");
    fseek(fp1,loc,SEEK_SET);
    char buffer;
    char findit[10] = "\0";
    int cnt = 0;
    unsigned char ismsbzero = 128;
    long lastloc;
    char prebuffer;
    while(1)
    {
        if(!fread(&buffer,1,sizeof(char),fp1)) 
        {
            //buffer为上一次的buffer
            /*char cntlast;
            buffer = prebuffer;
            fseek(fp2,lastloc,SEEK_SET);
            while(cntlast != 8-zero)
            {
                if((buffer&ismsbzero) == 0)     //最低位为0
                {
                    buffer = buffer << 1;
                    strcat(findit,"0");
                    cntlast++;
                } 
                else
                {
                    buffer = buffer << 1;
                    strcat(findit,"1");
                    cntlast++;
                }
                int i = 1;
                for(;i < charkindnumber+1;i++)
                {
                    if(!strcmp(findit,huffmancode[i])) break;
                }
                if(i != charkindnumber+1)
                {
                    fprintf(fp2,"%c",w[i-1].c);
                    for(int u = 0;u < 10;u++)
                    {
                        findit[u] = '\0';
                    }
                }
            }*/
            break;
        }
        lastloc = ftell(fp2);
        prebuffer = buffer;
        while(1)
        {
            if(cnt == 8)
            {
                cnt = 0;
                break;
            }
            if((buffer&ismsbzero) == 0)     //最低位为0
            {
                buffer = buffer << 1;
                strcat(findit,"0");
                cnt++;
            } 
            else
            {
                buffer = buffer << 1;
                strcat(findit,"1");
                cnt++;
            }
            int i = 1;
            for(;i < charkindnumber+1;i++)
            {
                if(!strcmp(findit,huffmancode[i])) break;
            }
            if(i != charkindnumber+1)
            {
                fprintf(fp2,"%c",w[i-1].c);
                for(int u = 0;u < 10;u++)
                {
                    findit[u] = '\0';
                }
            }
        }
    }
}
int main()
{
    struct wcounter *w[256];         //统计权重
    int *charkindnumber;    //统计出现了多少种字符 
    char filelocation[300] = "\0";
    char ***huffmancode;     //得到的编码表
    struct HTNode** HTree;
    struct codeexcel** huffmancodeexcel;
    huffmancode = (char***)malloc(sizeof(char**));
    HTree = (struct HTNode**)malloc(sizeof(struct HTNode*));
    scanf("%s",filelocation);     //读入文件地址
    //给指针分配指向的空间
    for(int i = 0;i<256;i++)
    {
        w[i] = (struct wcounter*)malloc(sizeof(struct wcounter));
    }
    charkindnumber = (int*)malloc(sizeof(int));
    for(int i = 0;i < 256;i++)    //初始化统计权重的数组
    {
        w[i]->c = '\0';
        w[i]->cnt = 0;
    }
    WeightCounter(w,charkindnumber,filelocation);    //统计权重
 //   (*charkindnumber)--;
    printf("%d",*charkindnumber);
    //在此调试WeightCounter(),没有出现问题
    HuffmanEncoding(HTree,huffmancode,w,charkindnumber);
    PrintHuffmancode(w,charkindnumber,*huffmancode);
    CreateZipFile(*huffmancode,w,filelocation,charkindnumber);
    Createback();
}