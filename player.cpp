/******************************
 * Qt player using libVLC     *
 * By protonux                *
 *                            *
 * Under WTFPL                *
 ******************************/

#include "player.h"
#include <vlc/vlc.h>
#include <iostream>
using namespace std;

#define qtu( i ) ((i).toUtf8().constData())

#include <QtGui>

Mwindow::Mwindow() {
    m_vlcPlayer = NULL;

    /* Initialize libVLC */
    m_vlcInstance = libvlc_new(0, NULL);

    /* Complain in case of broken installation */
    if (m_vlcInstance == NULL) {
        QMessageBox::critical(this, "Qt libVLC player", "Could not init libVLC");
        exit(1);
    }

    /* Interface initialization */
    initUI();
}

Mwindow::~Mwindow() {
    /* Release libVLC instance on quit */
    if (m_vlcInstance)
        libvlc_release(m_vlcInstance);
}

void Mwindow::initUI() {

    /* Buttons for the UI */
    m_playBut = new QPushButton("Play");
    QObject::connect(m_playBut, SIGNAL(clicked()), this, SLOT(play()));

    QPushButton *stopBut = new QPushButton("Stop");
    QObject::connect(stopBut, SIGNAL(clicked()), this, SLOT(stop()));

    QPushButton *fasterBut = new QPushButton("Faster");
    QObject::connect(fasterBut, SIGNAL(clicked()), this, SLOT(faster()));

    QPushButton *slowerBut = new QPushButton("Slower");
    QObject::connect(slowerBut, SIGNAL(clicked()), this, SLOT(slower()));

    QPushButton *volUpBut = new QPushButton("Vol up");
    QObject::connect(volUpBut, SIGNAL(clicked()), this, SLOT(louder()));

    QPushButton *volDownBut = new QPushButton("Vol down");
    QObject::connect(volDownBut, SIGNAL(clicked()), this, SLOT(quite()));

    QPushButton *fsBut = new QPushButton("Fullscreen");
    QObject::connect(fsBut, SIGNAL(clicked()), this, SLOT(fullscreen()));

    /* A timer to update the sliders */
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateInterface()));
    timer->start(100);

    /* Central Widgets */
    QWidget* centralWidget = new QWidget;
    /* Put all in layouts */
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_playBut);
    layout->addWidget(stopBut);
    layout->addWidget(fasterBut);
    layout->addWidget(slowerBut);
    layout->addWidget(volUpBut);
    layout->addWidget(volDownBut);
    layout->addWidget(fsBut);
 /*
    layout->addWidget(volumeSlider);

    QVBoxLayout *layout2 = new QVBoxLayout;
    layout2->setMargin(0);
    layout2->addWidget(videoWidget);
    layout2->addWidget(slider);
    layout2->addLayout(layout);
    */
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize( 600, 400);
    openFile();
}

void Mwindow::openFile() {

    //libvlc_instance_t *vlc = libvlc_new (NULL, 0);
    //libvlc_media_list_t *ml = libvlc_media_list_new (vlc);
    //libvlc_media_t *md = libvlc_media_new_path (vlc, file);

    /* The basic file-select box */
    QString fileOpen = QFileDialog::getOpenFileName(this, tr("Load a file"), "~");

    /* Stop if something is playing */
    if (m_vlcPlayer && libvlc_media_player_is_playing(m_vlcPlayer))
        stop();

    /* Create a new Media */
    libvlc_media_t *vlcMedia = libvlc_media_new_path(m_vlcInstance, qtu(fileOpen));
    if (!vlcMedia)
        return;

    /* Create a new libvlc player */
    m_vlcPlayer = libvlc_media_player_new_from_media (vlcMedia);

    /* Release the media */
    libvlc_media_release(vlcMedia);

    /* And start playback */
    libvlc_media_player_play (m_vlcPlayer);

    /* Update playback button */
    m_playBut->setText("Pause");
}
/*
PLAY/PAUSE – одна кнопка

Медленнее/быстрее – две кнопки

Громче/тише – две кнопки

Следующий трек – одна кнопка

Предыдущий трек – одна кнопка

В начало текущего трека (это СТОП или сильная перемотка назад) – одна кнопка

Короткий прыжок назад – одна кнопка

Короткий прыжок вперед – одна кнопка
*/


void Mwindow::faster()
{
    if (!m_vlcPlayer)
        return;
    m_rate = 0.3f + m_rate;
    libvlc_media_player_set_rate(m_vlcPlayer, m_rate);
}

void Mwindow::slower()
{
    if (!m_vlcPlayer)
        return;
    m_rate-=0.3f;
    libvlc_media_player_set_rate(m_vlcPlayer, m_rate);
}

void Mwindow::louder()
{
    if (!m_vlcPlayer)
        return;
    m_volume+=10;
    changeVolume(m_volume);
}

void Mwindow::quite()
{
    if (!m_vlcPlayer)
        return;
    m_volume-=10;
    changeVolume(m_volume);
}

void Mwindow::next()
{

}

void Mwindow::previous()
{

}

void Mwindow::jumpForward()
{

}

void Mwindow::jumpBackward()
{

}

void Mwindow::play() {
    if (!m_vlcPlayer)
        return;

    if (libvlc_media_player_is_playing(m_vlcPlayer))
    {
        /* Pause */
        libvlc_media_player_pause(m_vlcPlayer);
        m_playBut->setText("Play");
    }
    else
    {
        /* Play again */
        libvlc_media_player_play(m_vlcPlayer);
        m_playBut->setText("Pause");
    }
}

int Mwindow::changeVolume(int vol) { /* Called on volume slider change */

    if (m_vlcPlayer)
        return libvlc_audio_set_volume (m_vlcPlayer,vol);

    return 0;
}

void Mwindow::updateInterface() { //Update interface and check if song is finished

    if (!m_vlcPlayer)
        return;

    /* Stop the media */
    if (libvlc_media_player_get_state(m_vlcPlayer) == libvlc_Ended)
        this->stop();
}

void Mwindow::stop() {
    if(m_vlcPlayer) {
        /* stop the media player */
        libvlc_media_player_stop(m_vlcPlayer);

        /* release the media player */
        //libvlc_media_player_release(vlcPlayer);

        /* Reset application values */
        //vlcPlayer = NULL;
        m_playBut->setText("Play");
    }
}

void Mwindow::about()
{
    QMessageBox::about(this, "Qt libVLC player demo", QString::fromUtf8(libvlc_get_version()) );
}

void Mwindow::fullscreen()
{
   if (isFullScreen()) {
       showNormal();
       menuWidget()->show();
   }
   else {
       showFullScreen();
       menuWidget()->hide();
   }
}

void Mwindow::closeEvent(QCloseEvent *event) {
    stop();
    event->accept();
}
