FROM debian:trixie-slim

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update && apt-get -y upgrade && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /sim

# copy everything
COPY . /sim

# build fanuc simulator
RUN make -C fanuc/src

EXPOSE 8193

CMD ["./fanuc/src/fanuc-sim", "0.0.0.0", "8193"]
