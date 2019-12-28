FROM gitpod/workspace-full:latest
USER root
RUN apt-get update && \
    apt-get install -y nasm xorriso quilt minicom && \
    apt-get install qemu qemu-kvm && \
    apt-get install grub-pc grub-pc-bin grub-rescue-pc
