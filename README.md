# EOSDemo

Short demo using EOS:

- How to run a dedicated server on specific Map and GameMode.
- How to Login and Travel from MainMenu map to the dedicated server session.
- How to build and run Linux dedicated server.

## How to setup EOS

Please follow this instruction on [EOS Plugin Configuration](https://dev.epicgames.com/community/learning/courses/1px/unreal-engine-the-eos-online-subsystem-oss-plugin/mMoq/unreal-engine-plugin-configuration).

For demo purposes please use `Client` and `Server` as artifact names.

## Run dedicated server with `UnrealEditor`

Navigate to `tools/Server.bat`. Open it with any note editor.

Modify `ExePath` with your local `UnrealEditor`, make sure this editor is used to run your project.

Modify `ProjectPath` with the location of `*.uproject` file.

Double-click to run, you should see this if everything is set up correctly.

```sh
[2024.06.23-16.16.41:821][ 53]LogEOSSDK: LogEOSAnalytics: Start Session (User: ...)
[2024.06.23-16.16.42:185][ 64]LogEOSSDK: LogEOS: SDK Config Product Update Request Successful, Time: 10.484439
[2024.06.23-16.16.42:188][ 64]LogEOSSDK: LogEOS: SDK Config Data - Watermark: 947546383
[2024.06.23-16.16.42:188][ 64]LogEOSSDK: LogEOS: ScheduleNextSDKConfigDataUpdate - Time: 10.484439, Update Interval: 323.480347
[2024.06.23-16.16.43:219][ 95]LogTemp: Session: SessionName Created!
```

Additionally, you can navigate to [Dev Portal](https://dev.epicgames.com/portal/en-US/), then move to `Product -> Game Services -> Matchmaking`, use the attribute key as `GROUP` and value as `EOSSession`, or use `GAMEMAP` and `GAMEMODE` as key and value as set in `Server.bat` file.

## Run client with `UnrealEditor`

Navigate to `tools/Client.bat`. Open it with any note editor.

Modify `ExePath` with your local `UnrealEditor`, make sure this editor is used to run your project.

Modify `ProjectPath` with the location of `*.uproject` file.

Open `DevAuthTool`, please refer to [EOS Signing In](https://dev.epicgames.com/community/learning/courses/1px/unreal-engine-the-eos-online-subsystem-oss-plugin/9VyP/unreal-engine-signing-in), log in and use the same name as `-AUTH_PASSWORD`.

You can do the same for `tools/Client2.bat` but with a different account.

### In-game Menu

- `Login` - this will log you in with an Epic Games account.
- `Refresh Session` - after logging in, this will be available, when you press refresh it will show you the Session run by the dedicated server above.
- Press on 1 of the servers in the list, and you will navigate to the session run by that server.

## Build Linux Server

### Environment Set up

#### 1-Install Linux Cross-Compile Toolchain

- Navigate to [Cross-Compile Toolchain Page](https://dev.epicgames.com/documentation/en-us/unreal-engine/linux-development-requirements-for-unreal-engine?application_version=5.4), and find the version that you compile your source code Unreal Editor.

- Install the tool.

#### 2-Modify Source Engine Editor to build EOS plugin with Linux

- Open `\Engine\Plugins\Online\OnlineSubsystemEOS\OnlineSubsystemEOS.uplugin`. And add `Linux` to `PlatformAllowList`:

```sh
"PlatformAllowList": [
	"Win64",
	"Mac",
	"Android",
	"Linux"
]
```

- Navigate to `\Engine\Plugins\Online\OnlineSubsystemEOS\Private`. Duplicate `Mac` folder and rename the folder to `Linux`. Then inside that folder rename `MacEOSHelpers.h` to `LinuxEOSHelpers.h`
- Build the `UnrealEditor` again (not require Rebuild).

#### 3-Install Docker

Follow the instructions here [Docker Window Install](https://docs.docker.com/desktop/install/windows-install/)

This might require installing WSL2 in the process.

#### 4-Create Github Token to pull Unreal Engine runtime image

Follow the instructions here [Authenticating with GitHub Container Registry](https://dev.epicgames.com/documentation/en-us/unreal-engine/quick-start-guide-for-using-container-images-in-unreal-engine?application_version=5.4)

After getting your access token, open `Command Prompt` or `PowerShell` and run this

```sh
echo ACCESS_TOKEN | docker login ghcr.io -u USERNAME --password-stdin
```

Try to pull runtime image for Linux, it might take a while. Make sure `Docker` is running.

```sh
docker pull ghcr.io/epicgames/unreal-engine:runtime
```

### Build and Run Linux server with docker image

#### 1-Package project as a Linux Server

Open Project and Go to Packaging project. Please choose `Development` for binary configuration for debugging purposes.

![Pack Linux Server](https://raw.githubusercontent.com/DuncanDam/EOSDemo/main/pack-linux.png)

Select `Build` folder, or any folder you like but make sure to update that with docker build.

#### 2-Build Docker image

Open `Command Prompt` or `PowerShell`, and navigate to the project folder.

```sh
cd PROJECT_DIRECTORY
```

```sh
docker build -t eos-demo -f ./tools/linux.Dockerfile .
```

#### 3-Mount the docker image and run the dedicated server

Start image and name it `unreal`

```sh
docker run -dit --name unreal -p 7777:7777/udp eos-demo
```

Start and attach to the image process

```sh
docker start unreal
docker attach unreal
```

Now you should see this and can start a new linux server
```sh
ue4@ee030e556770:~/project$./linux-server-start.sh -m Map_1 -g Default
```

To remove the image run
```sh
docker remove unreal
```

**NOTE**: An alternative way to run is using Docker environment variable, you can uncomment these from `tools/linux.Dockerfile`

```sh
# To run the docker with env variable instead
# Example: docker run -dit --name unreal -p 7777:7777/udp -e DOCKER_MAPNAME=Map_1 -e DOCKER_GAMEMODE=Default [IMAGE_NAME]
ENV SERVER_MAPNAME ""
ENV SERVER_GAMEMODE "Default"
CMD ["/home/ue4/project/linux-server-start.sh", "-m", "${SERVER_MAPNAME}", "-g", "${SERVER_GAMEMODE}"]
```

And to run

```sh
docker run -dit --name unreal -p 7777:7777/udp -e DOCKER_MAPNAME=Map_1 -e DOCKER_GAMEMODE=Default eos-demo
```

Now you can run the client and connect to the local Linux Dedicated Server.
