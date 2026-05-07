FROM alpine:3.20 AS builder

RUN apk add --no-cache g++ make

WORKDIR /src
COPY . .
RUN make build

FROM alpine:3.20 AS runtime

RUN apk add --no-cache libstdc++ \
    && addgroup -S app \
    && adduser -S app -G app \
    && mkdir -p /data \
    && chown app:app /data

COPY --from=builder /src/bin/password-manager /usr/local/bin/password-manager

USER app
WORKDIR /data
VOLUME ["/data"]

ENTRYPOINT ["password-manager"]
