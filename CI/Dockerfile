FROM ubuntu:latest
MAINTAINER blackwalker01

#basic dev tools
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y git
RUN apt-get install -y curl
RUN apt-get install -y make
RUN apt-get install -y flex
RUN apt install -y libprotobuf-dev protobuf-compiler
RUN DEBIAN_FRONTEND="noninteractive" apt-get -y install tzdata
RUN apt-get install -y cmake
RUN apt-get install -y wget
RUN apt-get install -y unzip

#install gcc-9
RUN apt-get install -y gcc g++ gcc-multilib g++-multilib

#install gcc-10
RUN wget -c https://github.com/BlackWalker01/gcc/releases/download/releases%2Fgcc-10-27.04-experimental/gcc-10.zip
RUN unzip gcc-10.zip

RUN cd gcc-10 && \
    chmod +x install.sh && \
    ./install.sh

#install python
RUN apt install -y software-properties-common
RUN apt-get install -y python

#install clang
RUN wget -c https://github.com/BlackWalker01/llvm-project/releases/download/llvm-11-experimental/clang-11.zip
RUN unzip clang-11.zip
RUN cd clang-11 && \
    chmod +x install.sh && \
    ./install.sh 

# expose port
EXPOSE 5000
