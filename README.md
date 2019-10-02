# Compacta
A fast contig clustering tool

[![DOI](https://zenodo.org/badge/212251636.svg)](https://zenodo.org/badge/latestdoi/212251636)

Compacta is a software designed to reduce the number of contigs given by the assembler to a smaller set of representative sequences, preserving the information about relative expression given by the reads. Compacta output can then be employed in downstream analyses for contig identification and differential gene expression.

Compacta is provided in form of and executable binary for Linux, and as compilable source, the easiest way to use Compacta is to download the precompiled files and just run it as is or add it to your PATH variable.


PREREQUISITES.

The zlib compression and decompression library, in ubuntu you can installing with apt (sudo apt-get install zlib1g-dev)

samtools 1.0 (librares and header files) either installed and defined in your path or specify samtools path at installation time. you can download version 1.0 from github (https://github.com/samtools/samtools/archive/1.0.tar.gz), this file needs to be decompressed (tar -xvf 1.0.tar.gz) and compiled with ./configure and make. please be sure to install zlib first as is one of samtools prerequisites.

A c++ compiler, we suggest using gcc version 7.4, compilation older versions of gcc and with OSx clang gcc compiler might fail. 
 
 
INSTALLATION.

Download Compacta's tarball, decompress it with tar -xvf 

enter Compacta decompressed folder and run configure

./configure

if you don`t have samtools on your path you need to run configure with the following option:

--with-sam_tools=<full path to your samtools 1.0 directory>

if configure ran with no errors you can compile Compacta by running  
make

and you can install it to your bin folder by running

make install


