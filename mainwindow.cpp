#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMoveEvent>
#include <QCloseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTime>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QMenu>
#include <QString>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    play_flag = 0;
    menu_visable_flag = 0;
    ui->setupUi(this);

    ui->tableWidget->hide();


    this->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(showMinimized()));

    ui->horizontalSlider_2->setRange(0,100);//音量滑块设置范围0-100

    player=new QMediaPlayer(this);
    playList=new QMediaPlaylist(this);
    player->setPlaylist(playList);

    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(playTime_slot(qint64)));//最终播放时间变化
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(currentTime_slot(qint64)));//当前播放时间变化

    connect(player,SIGNAL(volumeChanged(int)),this,SLOT(volumeChanged_slot(int)));//音量变化
    player->setVolume(50);//设置初始音量
    playList->setPlaybackMode(QMediaPlaylist::Loop);//设置初始播放模式：循环播放
    ui->pushButton_8->setText(tr("循环\n播放"));

    connect(playList, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSongList(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ReleaseCapture())
      SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}

void MainWindow::playTime_slot(qint64 finishTime)
{
    QTime playFinishTime(0,finishTime/60000,finishTime/1000%60);
    ui->label_2->setText(playFinishTime.toString(tr("mm:ss")));
    ui->horizontalSlider->setMaximum(finishTime);
}
void MainWindow::currentTime_slot(qint64 position)
{
    QTime playCurrentTime(0,position/60000,position/1000%60);
    ui->label->setText(playCurrentTime.toString(tr("mm:ss")));
    ui->horizontalSlider->setValue(position);
}
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    QTime time(0,position/60000,position/1000%60);
    ui->label->setText(time.toString(tr("mm:ss")));
    player->setPosition(position);
}
//sasdasdasffsdfsdfds
void MainWindow::volumeChanged_slot(int volume)
{
    player->setVolume(volume);
    ui->horizontalSlider_2->setValue(volume);
}
void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    player->setVolume(position);
}

//bofang
void MainWindow::on_pushButton_3_clicked()
{
    if(!playList->isEmpty()){
        ui->pushButton_3->hide();
        play_flag = 1;
        player->play();
    }

}
void MainWindow::on_pushButton_4_clicked()
{
        ui->pushButton_3->show();
        play_flag = 0;
        player->pause();
}



//上一首
void MainWindow::on_pushButton_6_clicked()
{
    if(!playList->isEmpty())
        ui->pushButton_3->hide();
    int currentIndex=playList->currentIndex();
    if(--currentIndex<0) currentIndex=0;
    playList->setCurrentIndex(currentIndex);
    player->play();
}
//下一首
void MainWindow::on_pushButton_5_clicked()
{
    if(!playList->isEmpty())
        ui->pushButton_3->hide();
    int currentIndex=playList->currentIndex();
    if(++currentIndex==playList->mediaCount()) currentIndex=0;
    playList->setCurrentIndex(currentIndex);
    player->play();
}

void MainWindow::on_FILE_clicked()
{
     QStringList fileName=QFileDialog::getOpenFileNames(this,tr("打开你需要播放的音乐"),QDir::homePath(),tr("音乐(*.mp3)"));
     if(fileName.count()>0)
     {
        foreach(QString fileItem,fileName)
        {

            if(!fileItem.isEmpty())
            {
                playList->addMedia(QUrl::fromLocalFile(fileItem));
                QString table_file_name = fileItem.split("/").last();
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row,0,new QTableWidgetItem(table_file_name.split(".").front()));
            }
        }
        playList->setCurrentIndex(0);
     }
}

void MainWindow::on_pushButton_8_clicked()
{
    QMenu menu;
    menu.addAction(tr("单曲播放"),this,SLOT(CurrentItemOnce_slot()));
    menu.addAction(tr("单曲循环"),this,SLOT(CurrentItemInLoop_slot()));
    menu.addAction(tr("循环播放"),this,SLOT(Loop_slot()));
    menu.addAction(tr("随机播放"),this,SLOT(Random_slot()));
    menu.exec(QCursor::pos());
}
void MainWindow::CurrentItemOnce_slot()
{
    playList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    ui->pushButton_8->setText(tr("单曲\n播放"));
}
void MainWindow::CurrentItemInLoop_slot()
{
    playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    ui->pushButton_8->setText(tr("单曲\n循环"));
}

void MainWindow::Loop_slot()
{
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    ui->pushButton_8->setText(tr("循环\n播放"));
}
void MainWindow::Random_slot()
{
    playList->setPlaybackMode(QMediaPlaylist::Random);
    ui->pushButton_8->setText(tr("随机\n播放"));
}

//控制播放菜单消失与出现
void MainWindow::on_pushButton_7_clicked()
{
    if(menu_visable_flag == 0){
        ui->tableWidget->show();
        menu_visable_flag = 1;
    }else{
        ui->tableWidget->hide();
        menu_visable_flag = 0;
    }
}
void MainWindow::updateSongList(int i)
{
    ui->tableWidget->selectRow(i);

    ui->label_3->setText(tr("正在播放: %1").arg(ui->tableWidget->item(i, 0)->text()));

}


void MainWindow::on_pushButton_9_clicked()
{
    QMessageBox::about(this, tr("Raingy Player"), \
                       tr("Raingy Player，感谢您的使用！\n"
                          "欢迎访问我的GitHub\n"
                          "https://github.com/wangxiaozhi123"));
}
