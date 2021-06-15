ARG BASE_IMAGE=neubauergroup/centos-build-base:3.8.10
FROM ${BASE_IMAGE} as base

USER root
WORKDIR /

SHELL [ "/bin/bash", "-c" ]

COPY build-requirements.txt /tmp/build-requirements.txt
RUN python -m pip --no-cache-dir install --upgrade pip setuptools wheel && \
    python -m pip --no-cache-dir install --requirement /tmp/build-requirements.txt && \
    python -m pip list
