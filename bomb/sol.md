# Solution

## phase_1

objdump -d 搞出来了汇编，阅读了一下发现主函数把读进来的字符串地址放在了 %rdi ，然后调用了 phase_1 函数，函数里把一个神秘地址 0x402400 放在了 %rsi ，然后是如果 strings_not_equal 返回值为 0 那 phase_1 就解决了。

于是去看 strings_not_equal 写的啥，从名字看是比较字符串是否相等，大概看了一下发现在比 %rdi 和 %rsi 里的字符串是不是一样，那 phase_1 的答案就在 0x402400 里了， gdb 读一下解决了。

### answer

Border relations with Canada have never been better.



## phase_2

把栈指针存到了 rsi 里，然后调用了个 read_six_numbers ，看名字应该是读 $6$ 个数，然后返回值不能是 $1$ 能跳过第一个爆炸， read_six_numbers 里一通操作然后调用了 __isoc99_sscanf@plt ，且需要返回值 $>5$ ，但是去看 sscanf 发现好像跳到外部库了？然后就没搞懂了。

尝试反汇编了 __isoc99_sscanf@plt ，发现出现了更多外部函数调用，看不懂，那先跳过这部分，就当是他函数名字的功能读入 $6$ 个数字，接着看 phase_2 ，调用完 read_six_numbers 之后是栈顶的数跟 $1$ 比较，那读进来的六个数应该依次存在栈里，尝试了一下输入格式发现是用空格隔开，读了下后面的发现是在判断这 $6$ 个数是不是等差数列。

等做完查一下 read_six_numbers 里面到底在干啥。

### answer

1 2 4 8 16 32



## phase_3

阅读 phase_3 ，发现又在调用 __isoc99_sscanf@plt ，又读了一下 read_six_numbers ，好像 sscanf 大概是把读入的字符串提取出数字，放到指定的寄存器存的位置里， phase_3 指定了 $2$ 个寄存器 %rcx , %rdx ，读入两个数分别放在 %rsp+12 和 %rsp+8 里。

继续看，不爆炸的要求是读入数 $>1$ 个，然后 %rsp+8 上的数 $\le 7$ ，接着中间会跳转到 0x402470+8*%rax 位置上存的数据那一行，x/wx 一下 0x402470 上存的是 0x00400f7c ，发现跳到那个位置是可以的，且要求 %rsp+12 上的数是 0xcf=207 ，把 %rsp+8 上的数设为 $0$ 就可以了

### answer

0 207



## phase_4

依旧先把读入字符串提取 $2$ 个数，存在 %rsp+8 和 %rsp+12 里，首先需要 %rsp+8 的数 $\le 14$ ，然后设置了一些初始参数调用了一个叫 fun4 的函数，认真阅读了 fun4 ，写成 c++ 大概长这样：

```cpp
// res 是存在 %rsp+8 里的数
// x=14 y=0
int res;

int fun4(int x,int y){
    int val=floor((x-y)/2.0)+y;
    if(val>res)return 2*fun4(x-1,y);
    else if(val==res)return 0;
    else return 2*fun4(x,val+1)+1;
}
```

然后要求是这个函数返回值是 $0$ ，那让 $res=7$ 即可，最后判断 %rsp+12 上存的数是不是 $0$ 

### answer

7 0



## phase_5

先输入了一个字符串，然后要求字符串长度为 $6$ ，然后每次根据这个字符串构造一个新的字符串，然后要求跟一个存在 0x4025e 上的长度为 $6$ 的目标字符串相同，具体的：

1. 假设输入的字符串为 $s$ ，构造出来的字符串为 $t$ ，存在 0x4024b0 地址上的字符串为 $base$ ，那么 $t_i=base_{s_i\& 15}$ 
2. 用 gdb 得到目标字符串 $6$ 个位置与 $base$ 中下标的对应关系：

```
目标字符串             base
0x66                 0x9
0x6c                 0xf
0x79                 0xe
0x65				 0x5
0x72				 0x6
0x73				 0x7
```

构造一个 $16$ 进制低位分别为这些的字符串即可

### answer

ionefg



## phase_6

汇编写了很长，认真研究了一下，实际在干的事情很简单：

1. 内存里提前存好了一个 $6$ 个元素的链表，具体的:

```cpp
struct list{
	int ind;
    int val;
    struct *next;
};
```

2. 输入一个长度为 $6$ 的排列，把排列第 $i$ 个位置的值 $a_i$ 换成 $7-a_i$ ，然后将这个链表按照输入的排列的顺序重排
3. 要求重新排好的链表的 val 单调不增

gdb 得到 $6$ 个 val 值分别为 $0x14c,0xa8,0x39c,0x2b3,0x1dd,0x1bb$ 

### answer

4 3 2 1 5 6



## secret_phase

按理 $6$ 个 phase 解决了他也 congratulation 了，但是看汇编发现还有一个 secret_phase 和 fun7 ，发现是在 phase_defused 里进入的，且只有 $6$ 个 phase 全部解决了才有可能进入，去网上查了 sscanf 到底在干啥，发现是把输入格式存在了 %rsi 里，然后进入 secret_phase 的条件是 phase_4 的输入在末尾多输入一个 DrEvil ，然后观察 fun7 ，首先输入一个数不得大于 0x3e7 ，然后 fun7 大概长这样：

```cpp
int fun7(int *x){
    if(x==nullptr)return 0xfffffff;
    int val=*x;
    if(val<res)return 2*fun7(x+16)+1;
    else if(val==res)return 0;
    else return 2*fun7(x+8);
}
```

其中 res=%rax ，初始传入的 *x 为 0x6030f ，要求返回值为 $2$ ，那说明进去一定是 $>,<,若干个>,=$ 的顺序递归， gdb 查看一下对应内存的地址， res=0x16 是合法的。 

### answer

22
