kmsg_log kernel logger
======================

Description
-----------

kmsg_log is a simple Linux kernel module that creates a procfs file named
/proc/kmsg_log. This file is write-only by root, and dumps any data written
to it to the kernel ring buffer (kmsg/dmesg). The only manipulation performed
is that empty lines are not sent to the buffer.

The purpose for this module is for cases where you wish to use the kernel ring
buffer as your system log. Specifically this may be the case in an initramfs,
where it would be useful to dump logging messages into kmsg for reading later,
as the initramfs filesystem becomes unavailable after boot.

Compiling
---------

    make


Usage
-----

    # insert module into your kernel
    insmod kmsg_log.ko

    # write to log
    echo "My Log Message" > /proc/kmsg_log

    # optionally remove module when done
    rmmod kmsg_log
