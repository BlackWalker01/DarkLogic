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

# run Tests
RUN cd DarkLogic/Lib && \
    chmod +x ../CI/build/runTests.sh && \
    ../CI/build/runTests.sh

# expose port
EXPOSE 5000
