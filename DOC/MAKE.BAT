@echo off
makedoc -ascii cview.txt -htm cview.htm -texinfo cview.txi cview._tx
makeinfo cview.txi
copy cview.txt ..
