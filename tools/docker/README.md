# Short manual about EVAA Docker container

## Requirements

* Installed Docker :)

On Ubuntu you can just type:

```sh
sudo apt update
sudo apt install -y docker.io
```

## Prepared scripts

You can find there (`evaa/tools/docker`) scripts prepared to ease EVAA Docker
container usage. You can use them or not, as you wish. They are only to
make Docker usage easier.

### Build container image

To build prepared Docker container launch `build-docker.sh` script in
directory with `Dockerfile` (`evaa/tools/docker`):

```sh
./build-docker.sh
```

Built container will be named `evaa-docker`.

### Run built container

To start EVAA Docker container execute `run-docker.sh` script:

```sh
./run-docker.sh
```

If you launch it in `evaa/tools/docker` directory, the whole EVAA repository
will be mounted in the started container. In other cases only current directory
will be mounted.

#### Enable flash option

If you want to have access to USB devices in the EVAA Docker container run
`./run-docker.sh` with ` build-n-flash` suffix:

```sh
./run-docker.sh build-n-flash
```

USB devices will be mounted in the container. This is not done by default,
because it causes `--privileged -v /dev:/dev` usage. This reduces the level of
isolation. So if you don't need it, better don't use it.
