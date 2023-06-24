# 编译原理

## 介绍
重庆大学2020级计科编译原理词法、语法平时作业以及四次课程实验

如果你觉得有用的话，点个星星不过分吧，表达一下支持！！！



## 注意点

- 实验一语法分析树构建采用的是不带回溯的递归下降方法，最简单并且符合提供的框架，是在win环境中跑的，实验一58点全过。

- 实验二是在docker linux环境中跑的，实验二过了57个点，最后一个点是浮点数相关的处理，实现有点麻烦，发现只有最后一个点与浮点数相关，而且该测试点有非常多的浮点数相关内容，放弃！！！但是代码里面已经写了很多处理浮点数的逻辑，有兴趣可以补充。

- 实验三是中间表示IR转换为RISC-V汇编，采用了最简单的实现方式，即只使用`t0`和`t1`两个寄存器作为`rs1`和`rs2`，`t2`作为`rd`，不需要实现寄存器的分配与回收策略，运行速度稍慢，但是不影响正确性，没有性能跑分的计分项，所以这样实现最快。

    其中，要查看GCC生成的RISC-V汇编参考，可以先将.sy后缀改成.c，然后在docker中使用如下命令：

    ```
    riscv32-unknown-linux-gnu-gcc test.c -S
    ```

    这个生成的只是参考，GCC进行了一定程度的优化，并且它生成的没有伪指令，理解起来较为困难，但实际上不需要按照生成的来，灵活运用RISC-V的伪指令可以减少很多工作。

    期末考试结束后继续实验三debug，实在提不起兴趣，头晕眼花，故停留在通过56个点结束，反正满绩了...

- 顺带一提，助教提供的框架前两个实验比较好用，实验三不好用，根据自己的思路，完全可以大改，保持基本框架，只要功能实现即可。

- docker运行代码报错cmake最低版本要3.22，但是docker里面只有3.10，只需要将cmakelist.txt文件中的cmake版本改成3.10即可。



## 关于参考代码

- compiler_wxr	实验一、二、三的完成度都较高，写法较符合框架，容易理解，抛弃了浮点数的点（推荐）
- compiler_zc       实验一、二、三都完成，但写法完全脱离框架，难以读懂，代码有些冗余
- Compiler_hr_incomplete    只得到部分代码，不完整，可参考部分实现方式
- compiler_qbh    实验一、二都完成，可参考前两个实验
