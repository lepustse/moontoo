# nanopi-m1-plus

## 代码下载
修改代码，make编译，通过uboot下载
- 插入sd卡
- 启动minicom
- 板子reset，在终端任意敲一个按键，进入uboot命令行
- 输入`loady 0x40000000; go 0x40000000`
	> 此时为等待下载状态
- 以此按下`ctrl-a, s`，出现对话框
	- 选择`zmodem`，按下enter键
	- 双击空格，进入目录，去到`start.elf.bin`所在目录
	- 光标跳到`start.elf.bin`
	- 按下空格键，选项反白，按下回车
	- 等待完成，按任意键
- 板子reset

> 注意：设置启动命令，重启后就直接进入等待下载状态
- `setenv bootcmd loady 0x40000000; go 0x40000000`
- `saveenv`

