# web-to-webm [![Build Status](https://travis-ci.org/solusipse/web-to-webm.svg?branch=master)](https://travis-ci.org/solusipse/web-to-webm)

web-to-webm is simple, cross-platform gui tool for extracting clips from any of Youtube videos (and from many other supported services) and converting them to webm format.

## Download
For downloads, visit *Releases* page: [https://github.com/solusipse/web-to-webm/releases/latest](https://github.com/solusipse/web-to-webm/releases/latest). There are Windows (64 and 32 bit) and OS X (64 bit) releases.

If you are on Linux, follow these steps:

1. Build web-to-webm (see `Building` section).
2. Install `youtube-dl` ([download page](http://rg3.github.io/youtube-dl/download.html)):

    - `sudo curl https://yt-dl.org/downloads/2015.02.10.3/youtube-dl -o /usr/local/bin/youtube-dl`
    - `sudo chmod a+x /usr/local/bin/youtube-dl`
    
3. Install `ffmpeg` via your package manager.

Release page: [https://github.com/solusipse/web-to-webm/releases](https://github.com/solusipse/web-to-webm/releases).

## Usage
### Time format
Format is derived from `ffmpeg`. You have to use colons as separators. There are three options:
- just seconds (`ss - ss`, e.g. `5 - 35`),
- minutes and seconds (`mm:ss - mm:ss`, e.g. `15:00 - 15:25`),
- hours, minutes and seconds (`hh:mm:ss - hh:mm:ss`, e.g. `02:05:00 - 02:05:30`).

You can mix these standards (e.g. `55 - 01:15` which is equivalent of `00:55 - 01:15` or `55 - 75`).

## Interface
![web-to-webm](http://solusipse.net/misc/webtoweb-0.7.0-screen.png)

## Building
You need `QT5` to build this software. To build web-to-webm, type these commands:

1. `qmake-qt5 web-to-webm.pro`,
2. `make`.

On `Windows` and `Mac OS X` it will be probably easier to use QT Creator (open .pro file to start).

## Supported sites
Since web-to-webm downloading mechanism is based on `yotube-dl` it supports same services. For full list see: http://rg3.github.io/youtube-dl/supportedsites.html.

## Changelog

### v.0.7.2
- lightweight mode (no player)

### v.0.7.1
- asynchronous loading of video details

### v.0.7.0
- higher accuracy of conversion percentage bar
- fixed downloading and conversion of clips downloaded from other services than youtube
- minor interface and binary detecting bugfixes
- now it's possible to set custom ffmpeg parameters
- autoupdate
- installer for Windows
- add widget for setting quality and estimating size of output video

### v.0.6.0
- added support for other websites than Youtube
- added to log information about output video duration
- added menu option for setting youtube-dl and ffmpeg paths
- added menu option for resetting settings

## License
web-to-webm is MIT licensed. Icons are from Font Awesome ([license](http://fortawesome.github.io/Font-Awesome/license/)).
