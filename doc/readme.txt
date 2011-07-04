                            qOS, The Real-Time Kernel
                               DISTRIBUTION
                                
                               Version 0.01

                               READ ME FILE

    qOS，是一个非常小的非抢先式嵌入式操作系统。其设计理念如下：
1   简单。
    该系统的设计，尽量体现简单的原则，举几例为证：
    用静态数组作为task control table（TCB）。
    只需要一个定时器中断(前台)，作为任务切换时间源，其他代码，均运行于后台。
    因为任务都运行于后台，所以除了和中断之间交互数据，任务之间交互数据，无需考虑函数的可重入、互斥、同步、死锁等问题。
    简单设计的结果，使系统容易理解，更容易调试和移植，系统更轻巧、快速。
2   可裁剪。
3   透明的任务时间约束。
    可以给每个任务提供明确的时间约束条件，例如：设置某个任务必须10ms调用一次，如果超出10ms，报错。
    这样，当系统失效时，能够立即知道原因。
4   注重调试。哪里出错，哪里报错。
    分为调试状态和工作状态。
    调试状态，速度稍慢，但进行详尽的错误测试。
    工作状态时，做较少的错误检测，运行速度加快。
    当然，你也可以发布调试状态，这样，当系统运行时，消息队列满了，可以通过代码出错记录看到，
    而不是消息丢失后，通过系统表现异常,来猜测出错原因。
5   注重用函数名、变量名来说明，而不是注释。
    
    我希望qOS能给大家下面的感觉。
1   less意外，more直观。
2   less变化，more固定。
3   less动态，more静态。
4   less间接，more直接。
5   less模糊，more透明。

LICESING TERMS--------------------------------------



TABLE OF CONTENTS-----------------------------------
1. Reporting problems
2. Software Upgrades
3. Release Notes


Reporting problems-----------------------------------
    If you find a problem (i.e. bug) in qOS, do not hesitate to report
the problem to either:
1) Through the INTERNET:
    My INTERNET address is: 
2) By writing to me at:
    My email is: husthl@gmail.com



Software upgrades-----------------------------------



Release notes-----------------------------------




------------------------------------------------------------------------------
readme.txt,v1.0 2009/08/29 18:46:00


