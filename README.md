# ytwebm
ytwebm is simple, cross-platform gui tool for extracting clips from any of Youtube videos (and from many other supported services) and converting them to webm format.

## Download
- Windows: [Windows 32 bit binary](https://github.com/solusipse/ytwebm/releases/download/0.5.0/ytwebm-windows-32-rc1-0.5.0.7z)
- Mac OS X: [Mac OS X 64 bit binary](https://github.com/solusipse/ytwebm/releases/download/0.5.0/ytwebm-osx-64-rc1-0.5.0.dmg)

If you are on Linux, follow these steps:

1. Build ytwebm (see `Building` section).
2. Install `youtube-dl` ([download page](http://rg3.github.io/youtube-dl/download.html)):

    - `sudo curl https://yt-dl.org/downloads/2015.02.10.3/youtube-dl -o /usr/local/bin/youtube-dl`
    - `sudo chmod a+x /usr/local/bin/youtube-dl`
    
3. Install `ffmpeg` via your package manager.

Release page: [https://github.com/solusipse/ytwebm/releases](https://github.com/solusipse/ytwebm/releases).

## Usage
### Time format
Format is derived from `ffmpeg`. You have to use colons as separators. There are three options:
- just seconds (`ss - ss`, e.g. `5 - 35`),
- minutes and seconds (`mm:ss - mm:ss`, e.g. `15:00 - 15:25`),
- hours, minutes and seconds (`hh:mm:ss - hh:mm:ss`, e.g. `02:05:00 - 02:05:30`).

You can mix these standards (e.g. `55 - 01:15` which is equivalent of `00:55 - 01:15` or `55 - 75`).

## Interface
![ytwebm](http://solusipse.net/misc/ytwebm.png)

## Building
You need `QT5` to build this software. To build ytwebm, type these commands:

1. `qmake-qt5 ytwebm.pro`,
2. `make`.

On `Windows` and `Mac OS X` it will be probably easier to use QT Creator (open .pro file to start).

## Supported sites
Since ytwebm downloading mechanism is based on `yotube-dl` it supports same services. For full list see: http://rg3.github.io/youtube-dl/supportedsites.html.

## License
See `License`.
