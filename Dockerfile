FROM ubuntu:latest
RUN apt-get update &&\
    apt-get install -y build-essential make libboost-dev git