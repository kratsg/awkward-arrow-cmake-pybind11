FROM neubauergroup/centos-build-base:latest

COPY build-requirements.txt build-requirements.txt

RUN yum update -y -q && \
    yum install -y -q \
      git \
      epel-release \
      https://apache.jfrog.io/artifactory/arrow/centos/7/apache-arrow-release-latest.rpm && \
      yum install -y -q --enablerepo=epel \
      arrow-devel \
      arrow-glib-devel \
      arrow-dataset-devel \
      parquet-devel \
      parquet-glib-devel && \
    yum clean all && yum autoremove -y && \
    python3 -m pip install --no-cache-dir -r build-requirements.txt

CMD ["bash"]
