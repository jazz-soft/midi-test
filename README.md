# midi-test

[![npm](https://img.shields.io/npm/v/midi-test.svg)](https://www.npmjs.com/package/midi-test)
[![npm](https://img.shields.io/npm/dt/midi-test.svg)](https://www.npmjs.com/package/midi-test)
[![build](https://github.com/jazz-soft/midi-test/actions/workflows/build.yml/badge.svg)](https://github.com/jazz-soft/midi-test/actions)

## Virtual MIDI ports for testing MIDI applications

See also: [**web-midi-test**](https://github.com/jazz-soft/web-midi-test), [**test-midi-files**](https://github.com/jazz-soft/test-midi-files)

Install: `npm install midi-test --save-dev`

Manual rebuild (if needed): `node-gyp rebuild`

## API
### MIDI Source (Virtual MIDI-In)

```js
var MT = require('midi-test');
var port = new MT.MidiSrc('VIRTUAL MIDI-In');
port.connect();
port.emit([0x90, 0x40, 0x7f]);
//...
port.disconnect();
```

### MIDI Destination (Virtual MIDI-Out)

```js
var MT = require('midi-test');
var port = new MT.MidiDst('VIRTUAL MIDI-Out');
port.receive = function(msg) { console.log('received:', msg); };
port.connect();
//...
port.disconnect();
```

### TypeScript

```ts
import * as MT from 'midi-test'
//...
```

## Developer notes

**midi-test** currently works in **Linux** and **MacOS**.  
**Windows** version may appear some day in the future, but it is not in the immediate plans.

This package requires **Node.js v10.6.0 or later**,
and the full stack of [**node-gyp**](https://www.npmjs.com/package/node-gyp) build tools.  
Linux build also requires the ALSA libraries: `apt-get install libasound2-dev`

Since the Linux version needs special permissions to access the computer's sound system,  
it may not work properly in virtual Linux environments such as the one at [**Travis-CI**](https://travis-ci.org)
or [**GitHub Actions**](https://docs.github.com/en/actions).

To make your test work with **Travis-CI** or **GitHub Actions**, run it on **MacOS**.  
in `.travis.yml`:
```
os:
  - osx
...
```
in `.github/workflows/*.yml`:
```
jobs:
  build-macos:
    runs-on: macos-latest
  ...
```

If you are getting a build error related to Python 3.12, you may want to add this build step
(see the [build.yml](https://github.com/jazz-soft/midi-test/blob/master/.github/workflows/build.yml) file):
```
- run: python -m pip install --upgrade packaging pip setuptools
```
