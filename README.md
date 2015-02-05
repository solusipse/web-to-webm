# ytwebm
ytwebm is simple, cross-platform gui tool for extracting clips from any of Youtube videos (and from many other supported services) and converting them to webm format.

## Download
The only available now option to obtain ytwebm is to build it on your own. See `Building` section. Binaries will be provided soon.

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
Build it like any other `QT5` application. To run it, place `youtube-dl` and `ffmpeg` binaries in main directory. Software was tested on Windows, Linux and Mac OS X.

## Supported sites
Since ytwebm downloading mechanism is based on `yotube-dl` it supports same services. For full list see: http://rg3.github.io/youtube-dl/supportedsites.html.

## License
See `License`.
