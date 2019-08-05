#include "banshee.h"
#include "ui_banshee.h"
#include <QFileDialog>
#include <QTreeView>
#include <QDebug>
#include <QDir>
#include <QStatusBar>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QUrl>
#include <QtCore>
#include <QDesktopServices>
#include <QDirIterator>
#include <QMovie>
#include <iostream>

QString Name , Version , Description;

banshee::banshee(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::banshee)
{
    ui->setupUi(this);
    system ("mount /dev/mmcblk0p5 /mnt");
    QString strKeyLocalVersion("File System Configuration/");
    QSettings * config = 0;
    config = new QSettings("/mnt/fsdefs.ini", QSettings::IniFormat );
    ui->FileSystemNameLabel->setText(config->value( strKeyLocalVersion + "Name", "r").toString());
    ui->FileSystemVersionLabel->setText(config->value( strKeyLocalVersion + "Version", "r").toString());
    ui->FileSystemDescriptionLabel->setText(config->value( strKeyLocalVersion + "Description", "r").toString());
    system ("umount /mnt");
    this->setCursor(Qt::ArrowCursor);
}

banshee::~banshee()
{
    delete ui;
}
#define NUMOPS 29
/* bin home mnt run sys boot lib opt sbin tmp dev proc selinux usr etc media root srv var */
char ops[NUMOPS][512] = {
    "umount /src /dest_rootfs ; mkdir /src /dest_rootfs; mount /dev/mmcblk0p5 /src ;\
    sgdisk -Z /dev/mmcblk2; \
    partprobe /dev/mmcblk2 ; \
    dd if=/dev/zero of=/dev/mmcblk2 bs=16M count=1 ; \
    dd if=/src/eMMC_Boot/idbloader.img of=/dev/mmcblk2 seek=64 conv=notrunc,sync,fsync; \
    dd if=/src/eMMC_Boot/uboot.img     of=/dev/mmcblk2 seek=$(((0x002000 + 0x2000))) ibs=1M conv=sync,fsync; \
    dd if=/src/eMMC_Boot/trust.img     of=/dev/mmcblk2 seek=$(((0x004000 + 0x2000))) ibs=1M conv=sync,fsync ; sync",
    "sgdisk -n 1:16M: -c 1:rootfs /dev/mmcblk2 ;  yes | mkfs.ext4 /dev/mmcblk2p1 ; mount /dev/mmcblk2p1 /dest_rootfs ; sleep 1 ; sync",
    "cp -a /src/bin /dest_rootfs/.",
    "cp -a /src/boot /dest_rootfs/.",
    "cp -a /src/dev /dest_rootfs/.",
    "cp -a /src/etc /dest_rootfs/.",
    "cp -a /src/home /dest_rootfs/.",
    "cp -a /src/lib /dest_rootfs/.",
    "cp -a /src/media /dest_rootfs/.",
    "cp -a /src/mnt /dest_rootfs/.",
    "cp -a /src/opt /dest_rootfs/.",
    "cp -a /src/proc /dest_rootfs/.",
    "cp -a /src/root /dest_rootfs/.",
    "cp -a /src/run /dest_rootfs/.",
    "cp -a /src/sbin /dest_rootfs/.",
    "cp -a /src/selinux /dest_rootfs/.",
    "cp -a /src/srv /dest_rootfs/.",
    "cp -a /src/sys /dest_rootfs/.",
    "cp -a /src/tmp /dest_rootfs/.",
    "mkdir /dest_rootfs/usr ; cp -a /src/usr/bin /dest_rootfs/usr/.",
    "cp -a /src/usr/games /dest_rootfs/usr/.",
    "cp -a /src/usr/include /dest_rootfs/usr/.",
    "cp -a /src/usr/lib /dest_rootfs/usr/.",
    "cp -a /src/usr/local /dest_rootfs/usr/.",
    "cp -a /src/usr/sbin /dest_rootfs/usr/.",
    "cp -a /src/usr/share /dest_rootfs/usr/.",
    "cp -a /src/usr/src /dest_rootfs/usr/.",
    "cp -a /src/var /dest_rootfs/.",
    "cp /src/eMMC_Boot/NOVAsomParams /dest_rootfs/boot/. ; cp /src/eMMC_Boot/dtb.dtb /dest_rootfs/boot/. ; \
    cd /dest_rootfs/home/novasomindustries ; \
    chown -R 1000 * ; \
    chgrp -R 1000 * ; \
    chown -R 1000 .* ; \
    chgrp -R 1000 .* ; cd ; \
    umount /src /dest_rootfs ; \
    sync"
};
char visible_text[NUMOPS][512] = {
    "Partitioning eMMC",
    "Formatting eMMC and writing boot files",
    "Copying /bin",
    "Copying /boot",
    "Copying /dev",
    "Copying /etc",
    "Copying /home",
    "Copying /lib",
    "Copying /media",
    "Copying /mnt",
    "Copying /opt",
    "Copying /proc",
    "Copying /root",
    "Copying /run",
    "Copying /sbin",
    "Copying /selinux",
    "Copying /srv",
    "Copying /sys",
    "Copying /tmp",
    "Copying /usr/bin",
    "Copying /usr/games",
    "Copying /usr/include",
    "Copying /usr/lib",
    "Copying /usr/local",
    "Copying /usr/sbin",
    "Copying /usr/share ( this is a long process, and depends on uSD speed and eMMC size. Please wait )",
    "Copying /usr/src",
    "Copying /var",
    "Customizing image",
};


void banshee::on_write_pushButton_clicked()
{
    int i=0 , interval , statbarval;
    interval = 100 / NUMOPS;
    statbarval = 0;
    this->setCursor(Qt::WaitCursor);

    for(i=0;i<NUMOPS;i++)
    {
        ui->writeop_label->setText(visible_text[i]);
        ui->writeop_label->repaint();
        qApp->processEvents();

        std::cout << "ops : " << ops[i] << "\n" << std::flush;
        system(ops[i]);
        statbarval += interval;
        ui->progressBar->setValue(statbarval);
        ui->progressBar->repaint();
        qApp->processEvents();

    }
    ui->writeop_label->setText("Finished! Power off your target and remove uSD");
    ui->progressBar->setValue(100);
    this->setCursor(Qt::ArrowCursor);
}
