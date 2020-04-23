
# VOS

## BUILD

### PREREQUISITES ON UBUNTU

```shell
sudo apt install -y bochs bochs-x nasm
```

### COMPILE

```shell
make
```

## RUN & DEBUG

### bochs

```shell
make run-bochs
```

### qemu

```shell
make run-qemu
``` 

### qemu(Debug with C language)

```shell
make run-qemu-debug
``` 

```shell
gdb --init-command=.gdbinit
```

## REFERENCES

- [Multiboot2 Specification version 2.0](https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html)

- [VirtualBox](https://www.virtualbox.org/)

- [HyperPlatform](https://github.com/tandasat/HyperPlatform)

- 《[一个64位操作系统的设计与实现](https://www.ituring.com.cn/book/2450)》
