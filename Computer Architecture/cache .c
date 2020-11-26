#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct Block {
   char index[15];
   char tag[19];
   char valid;

   int test_index;
   int test_tag;
}Block;
//// block 구조체
typedef struct Cache {
    int acesss;
    int hits;
    int misses;
    int cache_size;
    int block_size;
    int block_num;
    Block* blocks;
}Cache;
//// Cache 구조체
//16진수 2진수 변환함수 정의.
void Convert16to2(char *Hex_array);
int Convert2to10();
//// 문자열을 n 진수로 변환하기 위한 시도 ..

void initBlock(Block *block){
    int i;
    for(i=0; i<15; i++)
        block->index[i] ='0';
    for(i=0; i<19; i++)
        block->tag[i] ='0';
    block->valid='N';
    //// 문자열을 n 진수로 변환하기 위한 시도 ..

    block->test_index=0;
    block->test_tag=0;
}

//// Instruction cache 를 초기화 하는 함수
void initICache(Cache *cache){
    int size;
    printf("Instruction cache size (KB) : ");
    scanf("%d",&size);
    cache->acesss=0;
    cache->hits=0;
    cache->misses=0;
    cache->cache_size=size * 1024;
    //32 byte 로 Block size 고정
    printf("Instruction cache block size (B) : ");
    scanf("%d",&size);
    cache->block_size=size;

    //// Block 의 개수는 cache size / block size
    cache->block_num=cache->cache_size/cache->block_size;
    cache->blocks = (Block *)malloc(sizeof(Block)*cache->block_num);
    for(int i=0; i<(cache->cache_size / cache->block_size); i++){
        initBlock(&cache->blocks[i]);
    }
}


///// Data Cache 를 초기화하는 함수
void initDCache(Cache *cache){
    int size;
    printf("Data cache size (KB) : ");
    scanf("%d",&size);
    cache->acesss=0;
    cache->hits=0;
    cache->misses=0;
    cache->cache_size=size * 1024;
    //32 byte 로 Block size 고정
    printf("Data cache block size (B) : ");
    scanf("%d",&size);
    cache->block_size=size;

    //// Block 의 개수는 cache size / block size
    cache->block_num=cache->cache_size/cache->block_size;
    cache->blocks = (Block *)malloc(sizeof(Block)*cache->block_num);
    for(int i=0; i<(cache->cache_size / cache->block_size); i++){
        initBlock(&cache->blocks[i]);
    }
}


FILE *fp;
char *Hex_array; // 파일서 읽어온 16진수를 문자열로 읽기 위한 포인터 변수
unsigned int fetched_inst; //명령어의 16진수를 정수형으로 읽기 위한 변수
char main_tag[19];
char main_index[15];
////////////////////////////////////////////////////////////////
int operand; // (0일때 read data, 1 write data)  <- data cache , 2 instruction fetch <- instruction cache
char binary[33];//16 -> 2진수로 변환 저장 공간
int two_to_ten; //// 문자열을 n 진수로 변환하기 위한 시도 ..
///////////////////////////////////////////////////////////////

int main()
{
    int i=0,j=0;
    unsigned int int_address; //// 16진수를 8자리, 32bit 를 정수로 저장하기 위하여 unsigned integer 를 사용함.
    int int_command;
    int int_index,int_tag; ///// 그것을 index 와 tag 로 구분할 수 있다.
    int data_index, data_tag; //// data cache 처리를 하기 위한 변
    char str_file[20] = { 0 };

    // Trace file 입력 받기
    printf("File : ");
    scanf("%s", str_file);

    //  트레이스 파일 읽기
    if((fp=fopen(str_file, "r")) == NULL) {
        printf("file open error");
        exit(1);
    }

    Cache *cache = (Cache *)malloc(sizeof(Cache));
    Cache *Icache = (Cache *)malloc(sizeof(Cache));
    initICache(Icache);
    initDCache(cache);


    Hex_array = (char *)malloc(sizeof(char) * 11);

    while(feof(fp)==NULL)		//파일 끝까지 문자열 불러오기.
    {

        ////fscanf(fp, "%d", &operand);
        ////fscanf(fp, "%s", Hex_array); //명령어를 문자열로 읽음
        fscanf(fp, "%d %x\n", &int_command, &int_address); //din파일에서 한줄 한줄 읽어온다.
        /* Trace file 명령어를 정수형으로 읽기 - 명령어를 문자열로 읽는 함수의 대체로 활용 가능
        fscanf(fp, "%x", &fetched_inst);
        */
        ////Convert16to2(Hex_array); //16진수 문자열을 2진수 형식의 문자열로 변경
        //정수형으로 명령어를 읽었다면 Convert16to2 함수의 활용은 필요 없음
        //// String index 사용했을 때
        /*
        for(i=0; i<=17; i++)
            main_tag[i]=binary[i];
        main_tag[i]='\0';
        for(i=18,j=0; i<=31; i++,j++){
            main_index[j]=binary[i];
        }
        main_index[j]='\0';
        Convert2to10();
        */
        ////
        if(int_command == 0 || int_command == 1) {
            int_address /= cache->block_size; //// block size 로 나눠 block 작업에 필요한 만큼만  사용하려고 한다.
            data_tag = int_address / cache->block_num; //// block 의 tag 를 사용한다.
            data_index = int_address % cache->block_num; /// block 의 address 를 사용한다.
            cache->acesss++;
            if (cache->blocks[data_index].valid == 'Y') { ////valid bit 가 Y 라면 tag 를 조회한다.
                if (cache->blocks[data_index].test_tag==data_tag) {
                    cache->hits++;
                } else {
                    cache->blocks[data_index].test_tag=data_tag; //// valid bit 가 Y 임에도 tag 가 다르다면 tag 를 바꿔준다.
                    cache->misses++;
                }
            } else { //// 아래는 처음부터 miss 가 발생하는 경우이다.
                //strcpy(cache->blocks[two_to_ten % cache->block_num].index, main_index);
                cache->blocks[data_index].valid = 'Y';
                cache->blocks[data_index].test_tag=data_tag;
                cache->misses++;
            }
        }
        if(int_command == 2){ //// 위와 같은 코드, command , Label 만 다르다.
            int_address /= Icache->block_size; //offset을 없애준다. (Block offset + Byte offset)
            int_tag = int_address / Icache->block_num; //Instruction의 TAG를 구한다.
            int_index = int_address % Icache->block_num; //Instruction INDEX를 구한다.Icache->acesss++;
            Icache->acesss++;
            if(Icache->blocks[int_index].valid=='Y'){
                if(Icache->blocks[int_index].test_tag==int_tag){
                    Icache->hits++;
                }
                else{
                    //strcpy(Icache->blocks[two_to_ten%Icache->block_num].index,main_index);
                    Icache->blocks[int_index].test_tag=int_tag;
                    Icache->misses++;
                }
            }
            else{
                //strcpy(Icache->blocks[two_to_ten%Icache->block_num].index,main_index);
                Icache->blocks[int_index].test_tag=int_tag;
                Icache->blocks[int_index].valid='Y';
                Icache->misses++;
            }
        }
        /*
        if(operand == 0 || operand == 1)		//data cahce 동작 구현
        {
            printf("%d  - data %d    %s    %s\n",two_to_ten,operand, Hex_array, binary);
            printf("test index binary : %s\n",main_index);
        }
        if(operand == 2)				//inst cache 동작 구현
        {
            printf("%d  - inst %d    %s    %s\n",two_to_ten,operand, Hex_array, binary);
            printf("test binary : %s\n",main_index);
        }*/
    }

    //Data Cache
    printf("\n\nInstruction cache\n");
    printf("Access : %d \n",Icache->acesss );
    //printf("Hits : %d \n",Icache->hits );
    printf("Miss : %d \n",Icache->misses);
    float dest = Icache->misses;
    float source = Icache->acesss;
    float ratio = dest/source;
    printf("Miss ratio : %.6f",ratio);

    //Data Cache
    printf("\n\nData cache");
    printf("Access : %d \n",cache->acesss );
    //printf("Hits : %d \n",cache->hits );
    printf("Miss : %d \n",cache->misses);
    dest = cache->misses;
    source = cache->acesss;
    ratio = dest/source;
    printf("Miss ratio : %.6f",ratio);

    return 0;
}

//2진수 10진수 변환 함수
int Convert2to10() {
   int mul=0;
   int sum=0;
   for(int i=0; i< 15; i++){
       if(main_index[i]=='1') {
           if(i==0)
               sum=sum+1;
           sum = sum + 1 * mul;
       }
       if(i==0)
           mul=1;
       mul*=2;
   }
   two_to_ten=sum;
}
//16진수 2진수 변환함수.
void Convert16to2(char *Hex_array)
{
    int res=0;
    char temp;
    unsigned long Hex = strtoul(Hex_array, NULL, 16);

    for (int i = 0; i < 32; i++)
    {
        res = Hex & (0x01 << (31 - i));
        if (res == 0)
        {
            temp = '0';
            binary[i] = temp;
        }
        else
        {
            temp = '1';
            binary[i] = temp;
        }
    }
}
