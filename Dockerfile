# Build stage
FROM alpine:3.20 AS build
RUN apk add --no-cache build-base
WORKDIR /app
COPY Makefile ./
COPY src ./src
RUN make

# Runtime stage: just the binary
FROM alpine:3.20
COPY --from=build /app/build/nasweep /usr/local/bin/nasweep
# Mount the NAS share you want to scan at /data (read-only is fine).
VOLUME ["/data"]
ENTRYPOINT ["nasweep"]
CMD ["/data"]
