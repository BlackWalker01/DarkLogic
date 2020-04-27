FROM blackwalker01/dl_environment:latest
MAINTAINER blackwalker01

#install project
COPY ./ .
RUN tar -zxvf DarkLogic.tar.gz
RUN cd DarkLogic && \
    ./configure && \
    cd build && \
    make && \
    make install

# expose port
EXPOSE 5000
