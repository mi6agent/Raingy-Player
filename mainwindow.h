#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
class QMediaPlayer;
class QMediaPlaylist;
class QMouseEvent;
class QSystemTrayIcon;
class QAction;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;

    QMediaPlayer *player;
    QMediaPlaylist *playList;
    int play_flag;
    int menu_visable_flag;

private slots:



    void playTime_slot(qint64 finishTime);

    void currentTime_slot(qint64 position);

    void on_horizontalSlider_sliderMoved(int position);

    void volumeChanged_slot(int volume);

    void on_horizontalSlider_2_sliderMoved(int position);


    //bofang
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();




    //上一首
    void on_pushButton_6_clicked();

    //下一首
    void on_pushButton_5_clicked();

    void on_FILE_clicked();

    void on_pushButton_8_clicked();

    void CurrentItemOnce_slot();

    void CurrentItemInLoop_slot();

    void Loop_slot();

    void Random_slot();

    void on_pushButton_7_clicked();
    void updateSongList(int i);

    void on_pushButton_9_clicked();
};

#endif // MAINWINDOW_H
