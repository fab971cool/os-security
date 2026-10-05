
# Security OS

- Mettre en place une image linux avec NFS / from scratch (avec alpine)
- Ajouter des appels systèmes directement
    - Dans le noyau (In tree)
    - En tant que module (Out-of-tree)
- Mettre en place un rootkit
- Mettre en place un EDR


## Image Linux NFS


1. Récupérer le noyau linux sur [kernel.org](https://kernel.org)

Traditionnellement le noyau se trouve à : 
```sh 
cp ~/Téléchargement/ linux-xxx /usr/src/linux-xxx
```

2. Activer les options pour NFS *Network File System*
```shell
make nconfig
 
 "Networking support" --> "Networking options" --> "IP:kernel level autoconfiguration".

"File system" --> "Network File System" --> "Root file system on NFS(NEW)" 
```

> Sortie : $KERNEL_DIR/linux-xxx/arch/x86/boot/bzImage


3. Ecrire le programme "my_init.c" permettant de lancer un shell à la fin du démarrage (séquence du bootloader enregistrée dans le MBR de l'UEFI / BIOS)

```
(UEFI) MBR <-- (GRUB) Bootloader "insert linux kernel" + "insert initrd process /  daemon"
```

4. Récupérer busybox
```sh
git clone git://git.busybox.net/busybox
cd busybox

make defconfig
make
make install
```
Troubleshoot : "error: « TCA_CBQ_MAX » non déclaré"
```sh
make menuconfig
```
- Décodher :  Network Utilities --> [ ] tc

