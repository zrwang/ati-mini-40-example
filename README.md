Compile and install atift-0.1 library:
```bash
chmod +x configure
./configure --prefix=/usr/local
make && sudo make install
```

Install comedi library:
```bash
sudo apt-get install libcomedi-dev
```

```bash
gcc main.c atinano.c keyboard_hit.c -o atitest -lcomedi -latift-0.1 -lm
```
