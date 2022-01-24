# bingo-datalogger-backend
weather, vibration and tilt angle data logger for telescopic bingo antenna

## Installing Node-RED v2.x with Node.js v14.x on ubuntu

### Instaling dependecies

First of all, you need to install Node.js. A simple way to do that is to get an installer script using cURL.

If you don't have cURL yet, install it with: 

```bash
sudo snap install curl
```

Then, download the script with:

```bash
curl -fsSL https://deb.nodesource.com/setup_14.x | sudo -E bash -
```

and install Node.js:

```bash
sudo apt-get install -y nodejs
```
### Instaling Node-red

Install the Node-red with npm:

```bash
sudo npm install -g --unsafe-perm node-red
```

### Running

To start up the Node-red, just open a new terminal and run:

```bash
node-red
```

get the IP shown in the prompt and open up with a web browser.

##  New Node-RED project

Instead of all Node-red flows using the same directory, you can stores flows and new modules in a separate folder by using *user directories*.

This could be done by editing the settings file, but a easier way is running the command bellow on the folder that you want to create a new project:

```bash
node-red -u project1
```

This causes node-red to create a folder called project1 and copy a settings.js file into it. In addition any nodes we install are placed into this folder under a node_modules folder.

In addiction, you can have access to multiple projects at the same time using a different port:

```bash
node-red -u project1 -p 1881
```

## Reopen a project

When you create a new Node-red project anywhere else the default location, you have to go back to the project folder and start Node-red from there using the same command used to create the project.

## Reference

* [IoT project with Esp32, IBM Watson and Node-RED](https://www.youtube.com/watch?v=T-Xg15Iokhg     )
* [Node-red getting started](https://nodered.org/docs/getting-started/local)
* [Node.js docs](https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions)
* [Node-red locally](https://nodered.org/docs/getting-started/local)
* [Node-red first flow](https://nodered.org/docs/tutorials/first-flow)
* [Securing Node-red](https://nodered.org/docs/user-guide/runtime/securing-node-red)
* [Node-red dashboard lib](https://flows.nodered.org/node/node-red-dashboard)
* [Node-red projects](https://stevesnoderedguide.com/managing-node-red-projects)
* [MQTT](https://mqtt.org/)
* [MQTT and esp32 project](https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/)
* [local only issue](https://stackoverflow.com/questions/65278648/mosquitto-starting-in-local-only-mode)
* [pio tutorial](https://www.filipeflop.com/blog/como-programar-esp32-com-vs-code-e-platformio/)