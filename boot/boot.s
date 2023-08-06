;根据 GRUB Multiboot 规范的一些定义


MBOOT_HEADER_MAGIC	equ	0x1BADB002	;MBOOT魔数
MBOOT_PAGE_ALIGN	equ	1		;引导模块按4KB对齐
MBOOT_MEM_INFO		equ	2		;空间信息包含在Multiboot信息结构中
MBOOT_HEADER_FLAGS	equ	3		;定义Multiboot标记

MBOOT_CHECKSUM		equ	-(MBOOT_HEADER_MAGIC+MBOOT_HEADER_FLAGS) ;校验和+魔数+flag=0

; 符合Multiboot规范的 OS 映象需要这样一个 magic Multiboot 头
; Multiboot 头的分布必须如下表所示：
; ----------------------------------------------------------
; 偏移量    类型  	域名    	         备注
;
;   0     u32   magic       必需
;   4     u32   flags       必需 
;   8     u32   checksum    必需 
;

;-----------------------------------------------------------
;-----------------------------------------------------------------------------
[BITS 32]	;32位编译

section .init.text	;为了将内核转移到3G处，创建初始用代码段

dd MBOOT_HEADER_MAGIC 	;加载三者到GRUB 
dd MBOOT_HEADER_FLAGS    
dd MBOOT_CHECKSUM 	

[GLOBAL START]		;内核从START开始，将其声明以提供给ld链接器
[GLOBAL mboot_ptr_tmp]  
[GLOBAL glb_mboot_ptr] 
[EXTERN kern_entry]	;声明内核C代码入口函数

START:
	cli				;关中断
	mov [mboot_ptr_tmp],ebx		;将ebx存储的指针存入glb_mboot_ptr
	mov esp,STACK_TOP		;创建内核栈
	and esp,0x0FFFFFFF0		;栈按16字节对齐
	mov ebp,0			;帧指针为0
	call kern_entry			;调用入口函数
	
;-----------------------------------------------------------------------------
section .init.data			;开启分页前的临时数据段
stack:	times 1024 db 0			;临时内核栈
STACK_TOP equ $-stack-1			;内核栈顶
mboot_ptr_tmp equ 0 			
glb_mboot_ptr equ 0

