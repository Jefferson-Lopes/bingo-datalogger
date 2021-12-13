# Installing Node-red v14.x on ubuntu

## Instaling dependecies

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
## Instaling Node-red

Install the Node-red with npm:

```bash
sudo npm install -g --unsafe-perm node-red
```

## Running

To start up the Node-red, just open a new terminal and run:

```bash
node-red
```

get the IP shown in the prompt and open up with a web browser.

## Reference

* [Node-red getting started](https://nodered.org/docs/getting-started/local)
* [Node.js docs](https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions)