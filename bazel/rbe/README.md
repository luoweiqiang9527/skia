RBE configurations
==================

Some subdirectories of this folder are generated. For example, `gce_linux` was generated by running
`make generate_linux_config`. Those generated files describe the C++ and Java toolchain that are
in the RBE Docker image; these toolchains are required to run Bazel, but are **not** the toolchains
that we use to compile our code.

We build our own, bare-bones, Docker image to use on RBE. We intend to use a hermetic toolchain
(see //toolchain) that specifies everything necessary to compile and link Skia. Use of the
hermetic toolchain on and off RBE makes the build reproducible and consistent across machines,
and not require internet access (assuming the toolchain has been cached at least once). This setup
has the desirable property of not needing to change and upload RBE Docker images if we need to
change a small detail of our toolchain.

The only requirement we have of our Docker image (beyond the minimum requirements to run Bazel)
are that it have sufficient runtime libraries to run our toolchain. For example, this means that
the Linux RBE image has at least glibc 2.32, which is the current minimum requirement of the
Linux binaries in our toolchain. This is the same requirement of any developer who tries to
build Skia using Bazel locally.

Getting rbe_configs_gen
-----------------------
It is suggested to download a prebuilt binary from
[GitHub](https://github.com/bazelbuild/bazel-toolchains/releases/tag/v5.1.1) and put that onto
your PATH.

Creating/Updating the RBE image
-------------------------------
In accordance with SLSA level 1, we want to be able to have a scripted way of building our image
and specify exactly what artifacts are in it. To accommodate this, we specify the exact sha256
hash of the base Docker image we use and the exact versions of the packages we install on top of
that. If we need to add a package or update things, it is best build the image without these
qualifiers to see what was actually used, and then respecify them so if someone were to build
the docker image again, they are likely to get the same image.

This process is:
 1) Modify the appropriate Dockerfile (e.g. gce_linux_container/Dockerfile) to not have the
    version or hash qualifiers. Also increment the appropriate VERSION variable in `Makefile`.
 2) Add any new packages or make any changes.
 3) Run `make build_linux_container` to build the image locally. One may verify it works by running
    something like `docker run -it gcr.io/skia-public/rbe_linux:v3 /bin/bash`.
 4) Note the versions and base image hash that were used. Modify the Dockerfile to use these.
    1) `docker pull debian:bookworm-slim` is the easiest way to see the sha256 and get the latest.
    2) Versions can be found looking for logs like:
       `Get:89 http://deb.debian.org/debian bookworm/main amd64 clang amd64 1:14.0-55.2+b1 [9976 B]`
 5) Run `make push_linux_container` to rebuild the container and push it to GCS where it can
    be used by our RBE workers. Note the sha256 hash of this created container
 6) Modify the appropriate generate step in `Makefile` (e.g. `generate_linux_config`) to refer
    to the correct toolchain_container. Then, run that step.
 7) Modify the RBE platform in `//platform/BUILD.bazel` to refer to the new `container_image`.

We chose not to use Bazel rules for this container step, as that could be difficult to bootstrap
without Bazel already setup. Additionally, Make is a simple and sufficient way to script the steps
for SLSA purposes.

Defining our own Bazel RBE platforms
------------------------------------
While the generated files *do* have a platform we can use (e.g.
`//bazel/rbe/gce_linux/config:platform`), we do not use it because we cannot easily customize it
without a risk that the changes will be lost when we update the image. Thankfully, we can specify
our own platforms, which we do in `//bazel/platform`, which is where we put the exec_properties
needed to use our RBE instances.

More details
------------
https://docs.google.com/document/d/14xMZCKews69SSTfULhE8HDUzT5XvPwZ4CvRufEvcZ74/edit

RBE Metrics
-----------
http://go/skia-rbe-metrics
