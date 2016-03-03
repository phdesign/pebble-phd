# PHD
A minimalistic digital watchface for pebble with date

## To run in emulator

This will compile all C and JavaScript and run both C and JavaScript tests.

```
pebble clean && pebble build
pebble install --emulator basalt --logs
```

## To run tests

To run just the tests and JavaScript build again:

```
grunt build
```

Or just the C tests, cwd to the /test/c/ folder:

```
make
./test
```
