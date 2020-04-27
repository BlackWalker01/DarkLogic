FROM ubuntu:latest

#install git
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y git

#install gcc-9
RUN apt-get install -y gcc g++

#install gcc-10
RUN git clone https://github.com/gcc-mirror/gcc.git
RUN apt-get install -y curl
RUN apt-get install -y make
RUN apt-get install -y flex
RUN cd gcc && \
     ./contrib/download_prerequisites && \
    ./configure --enable-multilib && \
    make && \
    make install



#install cmake
RUN apt install -y libprotobuf-dev protobuf-compiler
RUN DEBIAN_FRONTEND="noninteractive" apt-get -y install tzdata
RUN apt-get install -y cmake

#copy project
RUN git clone https://github.com/BlackWalker01/DarkLogic.git
RUN cd DarkLogic && \
    ./configure && \
    cd build && \
    make

# expose port
EXPOSE 5000
