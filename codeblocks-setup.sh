# install libs
sudo apt install codeblocks-dev libwxgtk3.0-0v5 libwxgtk3.0-dev
# clone simulavr-toolbar
git -C ../ clone https://github.com/dsandy12/simulavr-toolbar.git
# change directory
cd ~/simulavr-toolbar
# run codeblocks
codeblocks /na /nd --nosplashscreen -no-batch-window-close --rebuild ./toolbar.cbp --target="default"
echo configure codeblocks
