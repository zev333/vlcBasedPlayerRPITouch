/******************************
 * Qt player using libVLC     *
 * By zev                     *
 *                            *
 * Under WTFPL                *
 ******************************/

#ifndef PLAYER
#define PLAYER

#include <QtGui>
#include <vlc/vlc.h>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <dirent.h>

class Mwindow : public QMainWindow {

    Q_OBJECT

        public:
               Mwindow();
               virtual ~Mwindow();

        private slots:
               void openFiles();
               void play();
               void stop();
               void fullscreen();
               void louder();
               void quite();
               void next();
               void previous();
               void jumpForward();
               void jumpBackward();
               void faster();
               void slower();
               void toStart();
               void normalize();
               void changeLoop();

               int changeVolume(int);
               void updateInterface();

        protected:
               virtual void closeEvent(QCloseEvent*);

        private:
               QPushButton *m_playBut;
               QPushButton *m_loopBut;
               QPushButton *m_normalizeBut;
               libvlc_instance_t *m_vlcInstance;
               libvlc_media_list_player_t * m_vlcListPlayer;
               libvlc_media_player_t * m_vlcPlayer;

               int m_volume = 100;
               float m_rate = 1;
               bool m_loopFullList = true;

               void initUI();
};


#endif
