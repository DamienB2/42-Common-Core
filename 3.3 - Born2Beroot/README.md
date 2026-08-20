*This project has been created as part of the 42 curriculum by dbaise.*

# Born2beRoot

## Description

Born2beRoot is an introductory project to system administration and virtualization. The goal is to manually configure a secure, minimal server running inside a virtual machine, following strict rules, without any graphical interface.

This repository only contains `signature.txt` (the SHA1 fingerprint of the virtual disk) and this `README.md`, as required by the subject — the VM itself is never versioned.

### OS choice: Debian

I chose **Debian** (latest stable version) over Rocky Linux, mainly because this was my first experience with system administration and the subject explicitly recommends it in that case.

**Debian vs Rocky Linux**

| | Debian | Rocky Linux |
|---|---|---|
| Origin | Independent community distribution | Fork of RHEL (Red Hat Enterprise Linux) |
| Package manager | APT / dpkg (`.deb`) | DNF/YUM (`.rpm`) |
| MAC (access control) | AppArmor | SELinux |
| Default firewall | UFW (front-end for iptables/nftables) | firewalld |
| Philosophy | Stability, simplicity, widely used | Enterprise compatibility with RHEL, production-oriented |
| Learning curve | More beginner-friendly | More complex configuration (especially SELinux) |

**AppArmor vs SELinux**
- **AppArmor** (Debian): mandatory access control (MAC) based on **file paths**. Profiles are simpler to write and read, but less granular.
- **SELinux** (Rocky): MAC based on **labels/contexts** applied to files and processes. Much more powerful and precise, but noticeably harder to configure and debug.

**UFW vs firewalld**
- **UFW** (*Uncomplicated Firewall*, Debian): a simple front-end for iptables/nftables, designed to be easy to use from the command line (`ufw allow 4242`).
- **firewalld** (Rocky): manages rules through "zones" and supports hot-reloading without dropping existing connections; more flexible for production servers but somewhat more abstract to learn.

**VirtualBox vs UTM**
- **VirtualBox** (Oracle): a cross-platform type-2 hypervisor (Windows/Linux/Intel macOS), free and open-source, very well documented.
- **UTM**: based on QEMU, mainly used on Apple Silicon Macs (M1/M2/M3...) since VirtualBox isn't natively available there; relies on Apple's hardware virtualization (Hypervisor.framework).

I used **VirtualBox**.

### Main technical choices

- **Hostname**: `dbaise42`
- **Partitioning**: a 12G disk, with an unencrypted `/boot` partition (791M) and a LUKS-encrypted partition (`sda5`, 11.2G) containing an LVM volume group (`dbaise42--vg`) split into:
  - `root` (7.1G) → `/`
  - `swap` (620M)
  - `home` (3.4G) → `/home`
- **Encryption**: LUKS on the extended partition, with LVM on top to manage the logical volumes.
- **SSH**: running on **port 4242**, root login disabled (`PermitRootLogin no`).
- **Firewall**: UFW active at startup, only port 4242 open.
- **Users and groups**: besides `root`, the user `dbaise` belongs to the `user42` and `sudo` groups.
- **sudo**: authentication limited to 3 attempts, custom error message `"Incorrect Password"`, full logging (inputs and outputs) to `/var/log/sudo/`, TTY mode enabled, restricted `secure_path`.
- **Password policy** (via `libpam-pwquality` / `login.defs` / `chage`):
  - expires every 30 days (`PASS_MAX_DAYS 30`)
  - minimum 2 days between password changes (`PASS_MIN_DAYS 2`)
  - warning 7 days before expiration (`PASS_WARN_AGE 7`)
  - minimum 10 characters, must include an uppercase letter, a lowercase letter, and a digit
  - no more than 3 consecutive identical characters
  - the password must not contain the username
  - at least 7 characters different from the previous password (does not apply to root)
- **AppArmor**: active at startup (`aa-status`).
- **monitoring.sh**: a bash script broadcast to all terminals (via `wall`) every 10 minutes through a **cron** job, displaying architecture/kernel, number of physical/virtual CPUs, RAM and disk usage, CPU load, last reboot date, LVM status, active TCP connections, number of logged-in users, IP/MAC address, and number of commands executed with sudo.

## Instructions

1. Create a VirtualBox VM with the Debian stable ISO (text-mode / non-graphical install), following the manual installation prompts to set up the encrypted LVM partitioning described above.
2. Once the system is installed, log in as `root` and configure, in order:
   - the hostname (`hostnamectl set-hostname dbaise42`)
   - creation of the `dbaise` user and adding it to the `user42` and `sudo` groups
   - installation and configuration of `sudo` (`/etc/sudoers.d/` file)
   - the password policy (`libpam-pwquality`, `login.defs`, `chage`)
   - `openssh-server` on port 4242, with `PermitRootLogin no`
   - `ufw`, allowing only port 4242
   - AppArmor (verify it is active at startup)
3. Place the `monitoring.sh` script (e.g. in `/usr/local/bin/`), make it executable, then set up:
   - a cron job (`*/10 * * * * /usr/local/bin/monitoring.sh`)
   - a startup call (e.g. via `/etc/rc.local` or a systemd service) for the initial display
4. **Change all passwords** (including root) once the whole configuration is complete, so they comply with the password policy.
5. To generate `signature.txt`: compute the SHA1 hash of the VM's `.vdi` file (`sha1sum dbaise42.vdi` on Linux) and paste the result into `signature.txt` at the root of the repository, **with the VM shut down** and without restarting it afterward (otherwise the signature changes).

## Resources

- [Official Debian documentation](https://www.debian.org/doc/)
- [LVM documentation (Debian wiki)](https://wiki.debian.org/LVM)
- [ArchWiki — dm-crypt/LUKS](https://wiki.archlinux.org/title/Dm-crypt/Encrypting_an_entire_system) (general LUKS reference, applicable to Debian)
- [sudoers documentation (man sudoers)](https://www.sudo.ws/docs/man/sudoers.man/)
- [UFW — Ubuntu/Debian community help](https://help.ubuntu.com/community/UFW)
- Born2beRoot guide followed step by step for the overall server setup

**AI usage**: I did not use AI for the design, configuration, or `monitoring.sh` script of this project — the server installation and hardening were done by following a Born2beRoot guide and the official documentation. The only use of an AI assistant happened **at the very end**, to help structure and write this `README.md` based on my actual choices (OS, partitioning, services) once the project was finished.