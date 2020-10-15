#!/bin/bash
cd ~
# install tools required to build the guest additions
sudo apt install gcc make git
git clone https://github.com/dsandy12/simulavr.git
cd simulavr
sudo apt-get install build-essential g++ libtool-bin binutils-dev texinfo autoconf swig
./bootstrap
./configure --disable-doxygen-doc --enable-dependency-tracking
make
# patch and build gdb for avr
sudo apt-get --purge remove gdb-avr
wget ftp.gnu.org/gnu/gdb/gdb-8.1.1.tar.xz
tar xf gdb-8.1.1.tar.xz
cd gdb-8.1.1
perl -i -0pe 's/  ULONGEST addr = unpack_long \(type, buf\);\R\R  return avr_make_saddr \(addr\);\R/  ULONGEST addr = unpack_long (type, buf);\n\n  if (TYPE_DATA_SPACE (type))\n    return avr_make_saddr (addr);\n  else\n    return avr_make_iaddr (addr);\n/' gdb/avr-tdep.c
./configure --prefix=/usr --target=avr
make
sudo make install
cd ~
# this will build simulavr for your installation.  Note that there may be
# a few errors during the build process.  These are okay.
sudo apt install codeblocks
sudo add-apt-repository ppa:damien-moore/codeblocks-stable
sudo apt update
sudo apt install codeblocks codeblocks-contrib
cd ~
