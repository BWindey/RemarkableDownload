# RemarkableDownload
Bash script to download an entire folder from your Remarkable 2 at once.
This being bash, makes it so this will only work on UNIX-systems, or on WSL if you're on Windows.


## How to use
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
This script is a quick proof of concept, and definitly can use some more work. 
Especially handling multiple folders with the same name, downloading folders recursively, more options like making a full backup, ...
And maybe this bash-script can be made into a Python-script to improve portability. Altough I don't know how to do the ssh-part yet.

So if you're someone who knows some Bash or Python, or knows how to improve this in any way, please feel free to open pull requests!
