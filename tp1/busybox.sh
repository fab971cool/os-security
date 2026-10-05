set -xe


export BUILDS=$PWD
export INITRAMFS_BUILD=$BUILDS/initramfs
export BUSYBOX_BUILD=$BUILDS/busybox

# Construction du répertoire contenant notre systeme de fichiers racine
mkdir -p $INITRAMFS_BUILD
cd $INITRAMFS_BUILD
mkdir -p bin sbin etc proc sys dev usr/bin usr/sbin

# Retour dans notre répertoire et copie de busybox
cd $BUILDS
cp -a $BUSYBOX_BUILD/_install/* $BUILDS/initramfs

# Compilation du programme init_loop qui boucle et spawn un shell
gcc $BUILDS/my_init_loop/main.c -o $INITRAMFS_BUILD/init_loop


# Copie de la bibliothèque C depuis le système hôte
mkdir -p $INITRAMFS_BUILD/lib/x86_64-linux-gnu/
mkdir -p $INITRAMFS_BUILD/lib64

# copie des bibliothèques C standards dans notre systeme de fichiers racine
cp /usr/lib/libc.so.6 $INITRAMFS_BUILD/lib/x86_64-linux-gnu/

cp /usr/lib/libm.so.6 $INITRAMFS_BUILD/lib/x86_64-linux-gnu/

cp /usr/lib/libresolv.so.2 $INITRAMFS_BUILD/lib/x86_64-linux-gnu/

cp /lib64/ld-linux-x86-64.so.2 $INITRAMFS_BUILD/lib64


# Script init lancé par le noyau au démarrage
cat > $INITRAMFS_BUILD/init <<'END'
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
mount -t devtmpfs none /dev
cat <<!
Boot took $(cut -d' ' -f1 /proc/uptime) seconds
___________ .__ ________ _______________ _______
\_ _____/ ____ ____ | | ____ \_____ \/ _____/\ _ \ \ _ \
| __)__/ ___\/ _ \| | _/ __ \ / ____/ __ \ / /_\ \/ /_\ \
| \ \__( <_> ) |_\ ___/ / \ |__\ \\ \_/ \ \_/ \
/_______ /\___ >____/|____/\___ > \_______ \_____ / \_____ /\_____ /
\/ \/ \/ \/ \/ \/ \/
Welcome to "Ecole 2600 linux"
!
/init_loop
END

chmod +x $INITRAMFS_BUILD/init

# construction de l'image initramfs GPIO gzippée
cd $BUILDS/initramfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > $BUILDS/initramfs.cpio.gz