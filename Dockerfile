FROM gitpod/workspace-full:latest
USER root
RUN apt-get update && \
    apt-get install -y nasm xorriso quilt minicom && \
    apt-get install -y qemu qemu-kvm && \
    apt-get install -y grub-pc grub-pc-bin grub-rescue-pc
