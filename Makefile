default: build-image-debug

base_image=neubauergroup/centos-build-base:3.8.10

build-image-debug:
	docker pull $(base_image)
	docker build . \
		-f Dockerfile \
		-t awkward-arrow-cmake-pybind11:debug-local

debug:
	docker run --rm -ti \
		-v $(shell pwd):$(shell pwd) \
		-w $(shell pwd) \
		awkward-arrow-cmake-pybind11:debug-local

