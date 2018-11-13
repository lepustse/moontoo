# a处理器

## -a 模式

### 异常模式
- IRQ
- FIQ
- SVC

#### 非法指令
- ABT
- UDF

### 系统模式
- SYS
	- 用于读取USR模式的 lr & sp 寄存器

### 用户模式
- USR
	- 权限低，无法直接切换异常模式

### 异常模式切换
- 各种模式有自己的栈，因此需要初始化stack
- 返回原来的模式，因此需要设置lr


## 异常处理

### 异常向量表
- 这表是固定的
- 引导cpu找到这表
```
.align 5
arm_vector_table:
	ldr pc, =arm_reset_exception
	...
arm_reset_exception:
	b.
```

