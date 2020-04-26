
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
make bochs
```

### qemu

```shell
make qemu
``` 

### qemu(Debug)

```shell
make debug-qemu
``` 

```shell
make debug-qemu-amd
```

```shell
gdb --init-command=.gdbinit
```

## REFERENCES

- [Bochs](http://bochs.sourceforge.net/)

- [Intel Manuals](https://software.intel.com/en-us/articles/intel-sdm)

- [AMD Manuals](https://developer.amd.com/resources/developer-guides-manuals/)

- [Multiboot2 Specification version 2.0](https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html)

- [VirtualBox](https://www.virtualbox.org/)

- [HyperPlatform](https://github.com/tandasat/HyperPlatform)

- [SimpleSvm](https://github.com/tandasat/SimpleSvm)

- 《[一个64位操作系统的设计与实现](https://www.ituring.com.cn/book/2450)》
