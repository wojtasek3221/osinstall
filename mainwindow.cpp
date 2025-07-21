#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isLsblkAvailable();
    on_pushButton_2_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // Open file browser and get the selected path
    QString filePath = QFileDialog::getOpenFileName(
        this,                       // Parent window
        "Select a File",            // Dialog title
        QDir::homePath(),           // Default directory
        "Raw/ISO Images (*.iso *.img)"      // File filter
        );
    //checks if file pati is emtpy, if not save it as a variable and apply to line edit
    if (!filePath.isEmpty()) { //if not empty
        qDebug() << "Selected file:" << filePath; //do file stuff
        ui->lineEdit->setText(filePath);
    }
}



void MainWindow::on_pushButton_2_clicked()
{
    ui->comboBox_2->clear(); //clear combobox
    QProcess lsblk; //run lsblk
    lsblk.start("lsblk", {"-n", "-o", "NAME,MOUNTPOINT"}); //declare lsblk flags
    lsblk.waitForFinished(); //wait till finish

    QString output = lsblk.readAllStandardOutput(); //read lsblk output
    QStringList lines = output.split('\n', Qt::SkipEmptyParts); //skip empty parts

    for (const QString &line : lines) {//ok you will get a warning here but it works fine
        QStringList cols = line.trimmed().split(' '); //this will split the output to each row
        if (cols.size() >= 2 && !cols[1].isEmpty()) {
        ui->comboBox_2->addItem("/dev/" + cols[0], "/dev/" + cols[0]); //place text in syntax "/dev/sda1"
        }
    }
}


bool MainWindow::isLsblkAvailable() {
    QProcess checkProcess;
    checkProcess.start("which", {"lsblk"});  // Linux/macOS
    // Alternative for Windows (though lsblk is Linux-only):
    // checkProcess.start("where", {"lsblk"});

    checkProcess.waitForFinished();

    if (checkProcess.exitCode() != 0) {
        QMessageBox::critical(
            this,
            "Missing Dependency",
            "The 'lsblk' command is required but not found"
            );
        return false;
    }
    return true;

}

void MainWindow::on_pushButton_3_clicked()
{
    if (!ui->lineEdit->text().endsWith(".iso", Qt::CaseInsensitive) &&
        !ui->lineEdit->text().endsWith(".img", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Invalid File", "Please select a .iso or .img file!");
        ui->lineEdit->setText("");
        return;
    }
    ui->lineEdit->setText("");  // Update UI
}

