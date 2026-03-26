# Solution

## phase_1

objdump -d 搞出来了汇编，阅读了一下发现主函数把读进来的字符串地址放在了 %rdi ，然后调用了 phase_1 函数，函数里把一个神秘地址 0x402400 放在了 %rsi ，然后是如果 strings_not_equal 返回值为 0 那 phase_1 就解决了。

于是去看 strings_not_equal 写的啥，从名字看是比较字符串是否相等，大概看了一下发现在比 %rdi 和 %rsi 里的字符串是不是一样，那 phase_1 的答案就在 0x402400 里了， gdb 读一下解决了。

## phase_2

把栈指针存到了 rsi 里，然后调用了个 read_six_numbers ，看名字应该是读 $6$ 个数，然后返回值不能是 $1$ 能跳过第一个爆炸， read_six_numbers 里一通操作然后调用了 __isoc99_sscanf@plt ，且需要返回值 $>4$ ，但是去看 sscanf 发现好像跳到外部库了？然后就没搞懂了。