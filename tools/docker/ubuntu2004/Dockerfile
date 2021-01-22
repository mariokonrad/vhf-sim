FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

USER root
RUN apt-get update \
	&& apt-get install -y apt-utils \
	&& apt-get install -y curl make build-essential g++ \
	&& apt-get install -y qtbase5-dev qtmultimedia5-dev qttools5-dev-tools qttools5-dev \
	&& rm -fr /var/lib/apt/lists/*

RUN useradd --groups users -M --uid 1000 user

RUN mkdir -p /opt \
	&& chown -R user:users /opt

USER user

RUN curl -o /tmp/cmake.tar.gz https://cmake.org/files/v3.13/cmake-3.13.4-Linux-x86_64.tar.gz \
	&& mkdir -p /opt/local/cmake \
	&& tar -C /opt/local/cmake --strip-components=1 -xf /tmp/cmake.tar.gz \
	&& rm -f /tmp/cmake.tar.gz

ENV PATH /opt/local/cmake/bin:$PATH
