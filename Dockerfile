ARG BASE_IMAGE=neubauergroup/centos-build-base:3.8.10
FROM ${BASE_IMAGE} as base

USER root
WORKDIR /

SHELL [ "/bin/bash", "-c" ]

COPY build-requirements.txt /tmp/build-requirements.txt
RUN yum update -y -q && \
    yum install -y -q \
      epel-release \
      https://apache.jfrog.io/artifactory/arrow/centos/7/apache-arrow-release-latest.rpm && \
    yum install -y -q --enablerepo=epel \
      arrow-devel \
      arrow-glib-devel \
      arrow-dataset-devel \
      parquet-devel \
      parquet-glib-devel && \
    yum clean all && \
    yum autoremove -y && \
    python -m pip --no-cache-dir install --upgrade pip setuptools wheel && \
    python -m pip --no-cache-dir install --requirement /tmp/build-requirements.txt && \
    python -m pip list
