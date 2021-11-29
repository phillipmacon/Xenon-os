<h1 align="center">Xenon</h1>

<p align="center">
	<a href="https://discord.gg/t4C349U">
		<img alt="Discord" src="https://discordapp.com/api/guilds/678966940743761990/widget.png?style=shield">
	</a>
	<a href="https://www.gnu.org/licenses/gpl-3.0.en.html">
		<img alt="GitHub" src="https://img.shields.io/github/license/elementalOS/xenon?style=flat">
	</a>
</p>

Xenon aims to be a lightweight, secure, cross-architecture, monolithic kernel written in C++

## Features
| Architecture |  Status   |
|--------------|-----------|
| x86_64       | Supported |
| aarch64      | Planned   |

**Support for other architectures may be added in the future**

<br>

## Building
### Linux
To build Xenon, you will need to have `meson`, `ninja`, `xorriso`, and `clang` installed on your system.<br>
Xenon can be built by running the `build.sh` script in the project root, resulting in a bootable ISO file in the `build` directory 

Xenon can also be manually built by running
```
meson build --cross-file=arch/x86_64.ini && cd build
ninja
```
Do note however that this only produces an ELF, not an ISO

### Windows
Xenon can be built on Windows 10 by using WSL and following the <b>Linux</b> guide.

<br>

## Running
**Real hardware**<br>
Xenon can be run on real hardware by burning the `xenon.iso` image file to a CD or a USB-stick using a tool like rufus on Windows, or the `dd` command on Unix systems

**QEMU**<br>
Simply run the `run.sh` script, or execute `make qemu` in the `build` directory

**VirtualBox / VMWare**<br>
Xenon can be run like any other virtual machine by selecting the `Other (64-bit)` operating system when prompted, and inserting `xenon.iso`

<br>

## Chat
Come hang out on our Discord server!

[![](https://discordapp.com/api/guilds/678966940743761990/widget.png?style=banner2)](https://discord.gg/t4C349U)
