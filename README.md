<h1 align="center">Xenon</h1>

<p align="center">
	<a href="https://discord.gg/t4C349U">
		<img alt="Discord" src="https://img.shields.io/discord/678966940743761990?color=7289da&style=flat-square">
	</a>
	<a href="https://www.gnu.org/licenses/gpl-3.0.en.html">
		<img alt="GitHub" src="https://img.shields.io/github/license/xenon-kernel/xenon?style=flat-square">
	</a>
</p>

Xenon aims to be a lightweight, secure, cross-architecture, monolithic kernel written in C++

## Features
| Architecture |  Status   |
|--------------|-----------|
| x86_64       | Supported |
| armv8        | Planned   |
| mips64       | Planned   |

## Building
### Linux
To build Xenon, you will need to have `cmake`, `make`, `grub2`, `xorriso`, and a working C++ compiler (we recommend `clang`) installed on your system.<br>
Xenon can be built by either running the `build.sh` script in the project root, or building it manually by running
```
mkdir build && cd build
cmake ..
make -j xenon.iso
```
After building, the resulting disk image will be located in the `build` directory

### Windows
Xenon can be built on Windows 10 by using WSL and following the <b>Linux</b> guide.

## Running
<b>Real hardware</b><br>
Xenon can be run on real hardware by burning the `xenon.iso` image file to a CD or a USB-stick using a tool like rufus on Windows, or the `dd` command on Unix systems

<b>QEMU</b><br>
Simply run the `run.sh` script, or execute `make qemu` in the `build` directory

<b>VirtualBox / VMWare</b><br>
Xenon can be run like any other virtual machine by selecting the `Other (64-bit)` operating system when prompted, and inserting `xenon.iso`

## Support
You can get support by joining our [Discord Server](https://discord.gg/t4C349U)