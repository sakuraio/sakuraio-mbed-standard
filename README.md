# sakuraio-mbed-standard

Hello World example code for sakura.io with Mbed.


## Getting started


### Install mbed-cli

Please install [mbed-cli](https://github.com/ARMmbed/mbed-cli) .

```bash
pyenv install 2.7.11
pyenv virtualenv mbed-cli 2.7.11
pyenv activate mbed-cli
pip install mbed-cli
```

### Import and Compile

```bash
mbed import https://github.com/sakuraio/sakuraio-mbed-standard
cd sakuraio-mbed-standard
mbed compile -m NUCLEO_F401RE -t GCC_ARM
```
