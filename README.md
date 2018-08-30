# PHD
A minimalistic digital watchface for pebble with date

## Development setup

1. Install Pebble SDK
For Windows 10, follow these instructions: https://gist.github.com/thomasstoeckert/98567adaca27de20d4487d989a4ac5b0.

2. Now the Pebble Developer is dead, download the SDK from here: https://cdn.discordapp.com/attachments/264746316477759489/470048827391475742/sdk-core-4.3.tar.bz2  
or here: https://github.com/aveao/PebbleArchive/raw/master/SDKCores/sdk-core-4.3.tar.bz2  
And run `pebble sdk install /path/to/sdk-core-4.3.tar.bz2`

3. Run `npm install`
This requires node and npm to be installed

4. Run `bower install`
This requires bower to be installed, `sudo npm install -g bower`

5. Create `keys.json` file in project root for secrets
```
{
    "googleServerKey": "xx",
    "openWeatherMapAppId": "xx"
}
```

## To run in emulator

This will compile all C and JavaScript and run both C and JavaScript tests.

```
pebble clean && pebble build
pebble install --emulator basalt --logs
```

## To deploy to a device

```
pebble install --phone <ip_address>
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
