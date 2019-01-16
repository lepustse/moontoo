qemu-system-arm -M vexpress-a9 -cpu cortex-a15 -m 512M -nographic -kernel start.elf -monitor pty  -serial stdio -serial telnet:localhost:1235,server
