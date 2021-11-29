[cohae]: <> (Change fat xenon banner to square/transparent logo?)

<p align="center">
	<img alt="Xenon logo" height="60%" width="60%" src="https://repository-images.githubusercontent.com/244417195/c2270680-69c6-11ea-85b3-e425e4d1df24">
</p>
<h1 align="center">Xenon</h1>

<p align="center">
	<a href="https://discord.gg/t4C349U">
		<img alt="Discord" src="https://discordapp.com/api/guilds/678966940743761990/widget.png?style=shield">
	</a>
	<a href="https://www.gnu.org/licenses/gpl-3.0.en.html">
		<img alt="GitHub" src="https://img.shields.io/github/license/elementalOS/xenon?style=flat">
	</a>
</p>

Xenon aims to be a lightweight, secure, monolithic kernel written in C++

## Features
| Architecture |  Status   |
|--------------|-----------|
| x86_64       | Supported |
| aarch64      | Planned   |

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
Note that this only produces an ELF file, not an ISO

### Windows
Xenon can be built on Windows by using WSL and following the [Linux](#Linux) guide.

<br>

## Running
**Real hardware**<br>
Xenon can be run on real hardware by burning the `xenon.iso` image file to a CD or a USB-stick using a tool like rufus on Windows, or the `dd` command on Unix systems

```diff
-- DISCLAIMER --
We are not responsible for damage done to your hardware
Xenon is in-dev software, and has not been widely tested on real hardware yet
```

[cohae]: <> (Add raspberry pi guide using UBoot)

**QEMU**<br>
Simply run the `run.sh` script, or execute `make qemu` in the `build` directory

**VirtualBox / VMWare**<br>
Xenon can be run like any other virtual machine by selecting the `Other (64-bit)` operating system when prompted, and inserting `xenon.iso`

<br>

## Chat
Come hang out on our Discord server!

[![](https://discordapp.com/api/guilds/678966940743761990/widget.png?style=banner2)](https://discord.gg/t4C349U)
