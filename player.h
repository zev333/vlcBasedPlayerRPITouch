/******************************
 * Qt player using libVLC     *
 * By protonux                *
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

class Mwindow : public QMainWindow {

    Q_OBJECT

        public:
               Mwindow();
               virtual ~Mwindow();

        private slots:
               void openFile();
               void play();
               void stop();
               void about();
               void fullscreen();
               void louder();
               void quite();
               void next();
               void previous();
               void jumpForward();
               void jumpBackward();
               void faster();
               void slower();

               int changeVolume(int);
               void updateInterface();

        protected:
               virtual void closeEvent(QCloseEvent*);

        private:
               QPushButton *m_playBut;
               libvlc_instance_t *m_vlcInstance;
               libvlc_media_player_t *m_vlcPlayer;
               libvlc_media_list_player_t *m_mediaList;

               int m_volume = 100;
               float m_rate = 1;

               void initUI();
};


#endif
