# RemarkableDownload
My own tool to easily download files from the Remarkable tablet over USB. 
First I made a bash-script, which worked, but then I wanted to do recursive downloading. 
That's not something I wanted to do in bash anymore, so I started on a C++ project. 

If you want a simple bash-script that downloads all the files under a single folder, by typing its name, 
use the bash-script. If you want more, then you'll need to use the C++ program.

For now, I only have instructions for the bash-script, as the C++ one is still under construction.

- Collections of other Remarkable projects: https://github.com/reHackable/awesome-reMarkable
- Comparable project in Python: https://github.com/cherti/remarkable-cli-tooling


## How to install (bash-script)
There are 2 options for now, either you clone this repo on your system, and make a sym-link for rmDown.sh to somewhere in your path (like `~/.local/bin`), 
or you only download the rmDown.sh script, and put it somewhere in your path.


## How to use (bash-script)
Connect your tablet via USB to your computer, and type
```bash
rmDown <folder>
```
This will create a new folder called `<folder>_rm/`, and it will put all documents that are in the `<folder>` on your Remarkable, inside that new folder, all converted to pdf's.

> [!WARNING]
> When there are multiple folders with the name you're downloading, then all the files will end up in the same directory (should be fixed later)


## How it works
Apparently you can just make an HTTP request to your remarkable: `http://<ip>/download/<file-UUID>/placeholder`, and it will send a pdf. 
For some more smartiness, this script also ssh's in your remarkable to know which UUIDs it should download, using grep.


## Todo
This script is a quick proof of concept, and definitely can use some more work. 
Especially handling multiple folders with the same name, downloading folders recursively, more options like making a full backup, ...
And maybe this bash-script can be made into a Python-script to improve portability. Although I don't know how to do the ssh-part yet.

So if you're someone who knows some Bash or Python, or knows how to improve this in any way, please feel free to open pull requests!
