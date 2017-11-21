# Maintenance

## Sync with github.com

### Setup hg-git

```bash
pip install mercurial hg-git
```

#### ~/.hgrc

```
[extensions]
hggit =
```

### mbed.org to github.com

```bash
hg clone https://developer.mbed.org/teams/SAKURA-Internet/code/SakuraIO_Standard/ SakuraIO_Standard-sync
cd SakuraIOmbed-sync
hg push git@github.com:sakuraio/sakuraio-mbed-standard.git
```

### github.com to mbed.org

```bash
hg pull git@github.com:sakuraio/sakuraio-mbed-standard.git
hg outgoing https://developer.mbed.org/teams/SAKURA-Internet/code/SakuraIO_Standard/ # check diff
hg push https://developer.mbed.org/teams/SAKURA-Internet/code/SakuraIO_Standard/
```
