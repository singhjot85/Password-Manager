FROM alpine:3.20 AS builder

RUN apk add --no-cache g++ make postgresql-dev

WORKDIR /src
COPY . .
RUN make build USE_POSTGRES=1

FROM alpine:3.20 AS runtime

RUN apk add --no-cache libstdc++ libpq \
    && addgroup -S app \
    && adduser -S app -G app \
    && mkdir -p /data \
    && chown app:app /data

COPY --from=builder /src/bin/password-manager /usr/local/bin/password-manager

USER app
WORKDIR /data
VOLUME ["/data"]

ENTRYPOINT ["password-manager"]
