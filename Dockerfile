FROM neubauergroup/centos-build-base:latest

COPY build-requirements.txt build-requirements.txt

RUN yum update -y -q && \
    yum install -y -q \
      git && \
    yum clean all && yum autoremove -y && \
    python3 -m pip install --no-cache-dir -r build-requirements.txt

CMD ["bash"]
