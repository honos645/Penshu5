FROM ubuntu:18.04

RUN apt-get update && apt-get install -y gcc gdb make libpq-dev less curl vim emacs language-pack-ja-base language-pack-ja

RUN curl https://getmic.ro | bash

RUN localedef -i ja_JP -c -f UTF-8 -A /usr/share/locale/locale.alias ja_JP.UTF-8


ENV LANG=ja_JP.utf8


RUN mkdir libpq_program

RUN echo "alias ls='ls --color=auto'" >> /root/.bashrc

RUN echo "alias micro='/micro'" >> /root/.bashrc

WORKDIR /libpq_program
