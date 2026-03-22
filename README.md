# Lab for CSAPP

## datalab

### 3.19

 make btest 时报错
 ```bash
 usr/include/limits.h:26:10: fatal error: bits/libc-header-start.h: 没有那个文件或目录
   26 | #include <bits/libc-header-start.h>
      |          ^~~~~~~~~~~~~~~~~~~~~~~~~~
 ```

询问 AI 得知因为在64位系统上编译32位程序时缺少32位的glibc文件，使用
```bash
sudo apt install gcc-multilib
```
安装解决。

rating 1,2 的部分比较简单，思政课写到 isLessOrEqual 卡住了，第一次直接算 $-x$ 但是对于 $TMin$ 解决不了，又想了若干异或之后判断最高位的方案都因为爆 int 范围解决不了。

晚上发现直接比 $TMin$ 有问题，那先 $>>1$ 做减法，然后对比最后一位，两段拼接因为等号处理了些细节。

logicalNeg 核心想法是判断二进制表示存不存在 $1$ ，第一次想的 $8$ 位一段与上掩码，但细想发现掩码是多余的，直接每次右移一半位数再或自己，若干次之后最低位就是所有位的或了。

### 3.22

howManyBits 感觉不如后面的简单先跳了。

float 部分放开限制随便写写就出来了，但最后一个 floatPower2 不知道为啥测到 $x=127$ 就会卡死，但是单独测试 $127$ 又没问题，怀疑是电脑问题，晚上虚拟机跑一下。

回来看 howManyBits 发现就是如果最高位是 $1$ ，那前缀的 $1$ 就可以压缩成一个，先左移后右移相同位数判断是否相等就行，但是操作数爆了，倍增一下就好了。

用 ./dlc 检查发现 isTmax 用了不改用的符号，该了一下。