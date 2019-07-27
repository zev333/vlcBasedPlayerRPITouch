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

#define MUSIC_DIR "/home/zev/Music"

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
    //add media list release
    //add media release

     libvlc_media_list_player_set_media_player(m_vlcListPlayer, m_vlcPlayer);
     libvlc_media_list_player_release(m_vlcListPlayer);
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

    //QPushButton *fsBut = new QPushButton("Fullscreen");
    //QObject::connect(fsBut, SIGNAL(clicked()), this, SLOT(fullscreen()));

    QPushButton *nextBut = new QPushButton("Next");
    QObject::connect(nextBut, SIGNAL(clicked()), this, SLOT(next()));

    QPushButton *previousBut = new QPushButton("Previous");
    QObject::connect(previousBut, SIGNAL(clicked()), this, SLOT(previous()));

    QPushButton *jumpForwardBut = new QPushButton("Jump forward");
    QObject::connect(jumpForwardBut, SIGNAL(clicked()), this, SLOT(jumpForward()));

    QPushButton *jumpBackwardBut = new QPushButton("Jump backward");
    QObject::connect(jumpBackwardBut, SIGNAL(clicked()), this, SLOT(jumpBackward()));

    QPushButton *toStartBut = new QPushButton("To start");
    QObject::connect(toStartBut, SIGNAL(clicked()), this, SLOT(toStart()));

    QPushButton *normalizeBut = new QPushButton("Normalize");
    QObject::connect(normalizeBut, SIGNAL(clicked()), this, SLOT(normalize()));

    /* A timer to update the sliders */
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateInterface()));
    timer->start(100);

    /* Central Widgets */
    QWidget* centralWidget = new QWidget;
    /* Put all in layouts */
    QGridLayout *layout = new QGridLayout;
    layout->setMargin(0);
    layout->addWidget(m_playBut, 0, 0);
    layout->addWidget(stopBut, 0, 1);
    layout->addWidget(toStartBut, 0, 2);
    layout->addWidget(normalizeBut, 0, 3);
    layout->addWidget(slowerBut, 1, 0);
    layout->addWidget(fasterBut, 1, 1);
    layout->addWidget(volDownBut, 1, 2);
    layout->addWidget(volUpBut, 1, 3);
    layout->addWidget(previousBut, 2, 0);
    layout->addWidget(nextBut, 2, 1);
    layout->addWidget(jumpBackwardBut, 2, 2);
    layout->addWidget(jumpForwardBut, 2, 3);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize( 600, 400);
    openFiles();
}

void Mwindow::openFiles() {
    libvlc_media_list_t *mediaList = libvlc_media_list_new (m_vlcInstance);

    DIR *dir = opendir(MUSIC_DIR);
    dirent *entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;
        std::string str;
        str.append(MUSIC_DIR);
        str.append("/");
        str.append(entry->d_name);
        libvlc_media_list_add_media (mediaList, libvlc_media_new_path(m_vlcInstance, str.c_str()));
    }
    closedir(dir);

    m_vlcListPlayer = libvlc_media_list_player_new (m_vlcInstance);
    libvlc_media_list_player_set_media_list (m_vlcListPlayer, mediaList);
    libvlc_media_list_player_set_playback_mode(m_vlcListPlayer, libvlc_playback_mode_loop);
    libvlc_media_list_player_play(m_vlcListPlayer);
    m_vlcPlayer = libvlc_media_list_player_get_media_player(m_vlcListPlayer);

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
    libvlc_media_list_player_next(m_vlcListPlayer);
}

void Mwindow::previous()
{
    libvlc_media_list_player_previous(m_vlcListPlayer);
}

void Mwindow::jumpForward()
{
    libvlc_media_player_set_position(m_vlcPlayer, libvlc_media_player_get_position(m_vlcPlayer)+0.1f);
}

void Mwindow::toStart()
{
    libvlc_media_player_set_position(m_vlcPlayer, 0);
}

void Mwindow::normalize()
{
    libvlc_media_player_set_rate(m_vlcPlayer, 1);
}

void Mwindow::jumpBackward()
{
    libvlc_media_player_set_position(m_vlcPlayer, libvlc_media_player_get_position(m_vlcPlayer)-0.1f);
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
