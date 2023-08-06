# 32位下x86架构简单操作系统内核udOS

## 简介

在32位x86环境下，完成了一个简单的操作系统内核，实现了如下功能：

1.boot：实现GRUB引导的的系统启动

2.printf：基本的屏幕打印函数

3.GDT：全局描述符表设置

4.IDT：中断向量表的设置与中断的注册

5.pmm：物理内存的基本管理

6.vmm：虚拟内存的基本管理

7.heap：堆管理 / malloc()与free()的实现

8.thread：内核线程的设置与切换


## 开发工具
运行环境：qemu虚拟机

开发语言：C，佐以极少量AT&T风格汇编

编译器： gcc, nasm

链接器：ld

调试器：gdb

构建工具：Makefile

## 如何运行
在终端输入：

git clone https://github.com/Wsw6484/udOS.git

cd udOS/

make 

make qemu

若需调试，请在终端输入：

make debug

