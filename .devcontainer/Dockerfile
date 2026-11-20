FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# Install build tools and toolchain install script dependencies.
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        cmake \
        ninja-build \
        wget \
        xz-utils \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Run toolchain installers at image build time.
COPY motorlib/scripts/install_gcc.sh /tmp/install_gcc.sh
COPY motorlib/scripts/install_llvm.sh /tmp/install_llvm.sh
RUN chmod +x /tmp/install_gcc.sh /tmp/install_llvm.sh \
    && /tmp/install_gcc.sh \
    && /tmp/install_llvm.sh \
    && rm -f /tmp/install_gcc.sh /tmp/install_llvm.sh

ENV PATH="/gcc/bin:/llvm/bin:${PATH}"
